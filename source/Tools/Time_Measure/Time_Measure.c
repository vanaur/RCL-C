/*
 * Copyright (c) 2019-2020 vanaur.
 *
 * This file is part of RCL
 * (see https://github.com/vanaur/The-Runtime-Core-Library).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdio.h>
#include <stdlib.h>

#include <Tools\Time_Measure\Time_Measure.h>
#include <Tools\Utils\Utils.h>

#include <stdint.h>
#if defined(__linux)
#define HAVE_POSIX_TIMER
#include <time.h>
#ifdef CLOCK_MONOTONIC
#define CLOCKID CLOCK_MONOTONIC
#else
#define CLOCKID CLOCK_REALTIME
#endif
#elif defined(__APPLE__)
#define HAVE_MACH_TIMER
#include <mach/mach_time.h>
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
static uint64_t ns()
{
    static uint64_t is_init = 0;
#if defined(__APPLE__)
    static mach_timebase_info_data_t info;
    if (0 == is_init)
    {
        mach_timebase_info(&info);
        is_init = 1;
    }
    uint64_t now;
    now = mach_absolute_time();
    now *= info.numer;
    now /= info.denom;
    return now;
#elif defined(__linux)
    static struct timespec linux_rate;
    if (0 == is_init)
    {
        clock_getres(CLOCKID, &linux_rate);
        is_init = 1;
    }
    uint64_t now;
    struct timespec spec;
    clock_gettime(CLOCKID, &spec);
    now = spec.tv_sec * 1.0e9 + spec.tv_nsec;
    return now;
#elif defined(_WIN32)
    static LARGE_INTEGER win_frequency;
    if (0 == is_init)
    {
        QueryPerformanceFrequency(&win_frequency);
        is_init = 1;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (uint64_t)((1e9 * now.QuadPart) / win_frequency.QuadPart);
#endif
}

inline void time_measure(uint64_t *time_ptr)
{
    *time_ptr = ns();
}

inline uint64_t times_diff(uint64_t x, uint64_t y)
{
    return y - x;
}

inline uint64_t ns_to_ms(uint64_t x)
{
    return x / 1e6;
}

inline uint64_t ns_to_s(uint64_t x)
{
    return x / 1e9;
}

inline String show_time(uint64_t t1, uint64_t t2)
{
    String result = NULL;
    uint64_t ns = times_diff(t1, t2);
    uint64_t ms = ns_to_ms(ns);
    uint64_t s = ns_to_s(ns);
    if (s > 0)
        rcl_asprintf(&result, "%I64lld s", s);
    else if (ms > 0)
        rcl_asprintf(&result, "%I64lld ms", ms);
    else if (ns > 0)
        rcl_asprintf(&result, "%d ns", first_digit((int)ns));
    else if (result == NULL)
        return "unspecified time";
    return result;
}
