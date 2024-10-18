package com.cyphersys.picamera;

import java.lang.foreign.FunctionDescriptor;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;

public class Streamer {
    
    public Streamer() {
        FunctionDescriptor frameHandlerDescription =
        FunctionDescriptor.of(
            ValueLayout.JAVA_BOOLEAN,
            ValueLayout.ADDRESS.withTargetLayout(ValueLayout.JAVA_INT),
            ValueLayout.JAVA_INT);
    }

    public Boolean handleFrame(MemorySegment buffer, MemorySegment size) {
        Integer iSize = size.get(ValueLayout.JAVA_INT, 0);

        
        return true;
    }
}