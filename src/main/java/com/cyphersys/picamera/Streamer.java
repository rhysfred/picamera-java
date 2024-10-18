package com.cyphersys.picamera;

import java.io.IOException;
import java.io.OutputStream;
import java.lang.foreign.FunctionDescriptor;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;

public class Streamer {
    private OutputStream stream;
    private MethodHandle frameCallback;

    public Streamer(OutputStream stream) {
        this.stream = stream;
        FunctionDescriptor frameHandlerDescription = FunctionDescriptor.of(
                ValueLayout.JAVA_BOOLEAN,
                ValueLayout.ADDRESS,
                ValueLayout.JAVA_LONG);

        try {
            frameCallback = MethodHandles.lookup().findVirtual(
                    this.getClass(),
                    "handleFrame",
                    frameHandlerDescription.toMethodType());
        } catch (Exception e) {
            throw new AssertionError(
                    "Problem creating method handle compareHandle", e);
        }
    }

    public void start() {

    }

    public void stop() {

    }
    
    public Boolean handleFrame(MemorySegment voidPointer, MemorySegment size_t) {
        Long size = size_t.get(ValueLayout.JAVA_LONG, 0);
        MemorySegment buffer = voidPointer.get(ValueLayout.ADDRESS, 0).reinterpret(size);
        try {
            stream.write(buffer.toArray(ValueLayout.JAVA_BYTE));
            return true;
        } catch (IOException e) {
            return false;
        }
    }
}