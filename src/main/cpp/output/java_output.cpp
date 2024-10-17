/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2020, Raspberry Pi (Trading) Ltd.
 *
 * file_output.cpp - Write output to file.
 */

#include "java_output.hpp"

JavaOutput::JavaOutput(VideoOptions const *options)
        : Output(options), fp_(nullptr), count_(0), file_start_time_ms_(0)
{
}

JavaOutput::~JavaOutput()
{

}

void JavaOutput::setCallback(void (*f)(void *, size_t))
{
        javaCallback = f;
}

void JavaOutput::outputBuffer(void *mem, size_t size, int64_t timestamp_us, uint32_t flags)
{
        // This method will callback into Java to send the current output buffer.

        LOG(2, "JavaOutput: output buffer " << mem << " size " << size);
        if (size)
        {
                (*javaCallback)(mem, size);
        }
}
