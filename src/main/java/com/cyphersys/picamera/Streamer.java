package com.cyphersys.picamera;

import java.io.IOException;
import java.io.OutputStream;
import java.lang.foreign.Arena;
import java.lang.foreign.FunctionDescriptor;
import java.lang.foreign.Linker;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.SymbolLookup;
import java.lang.foreign.ValueLayout;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;

public class Streamer {
    private OutputStream stream;
    private MethodHandle frameCallback;

    /**
     * Constructor for the Streamer class
     * Accepts an OutputStream to which it will
     * write camera frames as they become ready
     * @param stream
     */

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
        /* This won't actually work yet. Need to spin up a thread for the underlying native
         * event loop
         * TODO Create thread
         */
        try (Arena arena = Arena.ofConfined()) {
            Linker linker = Linker.nativeLinker();
            SymbolLookup libRpiCam = SymbolLookup.libraryLookup("librpicam_java.so", arena);
            MemorySegment startStreamAddress = libRpiCam.find("start_stream").orElseThrow();
            FunctionDescriptor startStreamSignature = FunctionDescriptor.ofVoid(ValueLayout.ADDRESS);
            MethodHandle startStream = linker.downcallHandle(startStreamAddress, startStreamSignature);
            startStream.invoke(frameCallback);
        } catch (Throwable e) {
            // Issue with invoking native method
            // TODO Implement this properly
            e.printStackTrace();
        }
    }

    public void stop() {
        // Once threading implemented in the start function, this function will
        // shut down the thread, thereby unloading the native library
    }
    
    
    /**
     * Internal method that is passed as a function pointer to the underlying native library.
     * The native library will call this every time a new video frame is available from the
     * encoder. This method then copies the frame as a byte array to the OutputStream provided
     * in the constructor.
     * 
     * If any error occurs transferring the byte array to the OutputStream, this method will return
     * false. This is to allow the native library to be aware of the issue on the Java side and
     * so that it can shut down the streaming output.
     * 
     * @param voidPointer
     * @param size_t
     * @return Boolean
     */
    protected Boolean handleFrame(MemorySegment voidPointer, MemorySegment size_t) {
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