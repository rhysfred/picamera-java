/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2020, Raspberry Pi (Trading) Ltd.
 *
 * file_output.cpp - Write output to file.
 */

#include "java_output.hpp"

boolean (*javaCallback)(void *, size_t)

JavaOutput::JavaOutput(VideoOptions const *options, boolean (*f)(void *, size_t))
        : Output(options), fp_(nullptr), count_(0), file_start_time_ms_(0)
{
        javaCallback = f;
}

JavaOutput::~JavaOutput()
{

}


void JavaOutput::outputBuffer(void *mem, size_t size, int64_t timestamp_us, uint32_t flags)
{
        // This method will callback into Java to send the current output buffer.

        LOG(2, "JavaOutput: output buffer " << mem << " size " << size);
        if (size)
        {
                if(!(*javaCallback)(mem, size)) {
                        // Some failure has occured on the Java side, probably stream has been remotely
                        // closed.
                        
                        return;
                }
        }
}
