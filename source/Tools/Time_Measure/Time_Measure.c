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

#include <time.h>
#include <math.h>
#include <Tools\Time_Measure\Time_Measure.h>
#include <Tools\Utils\Utils.h>

bool gettime(struct timespec *ts)
{
    return !(clock_gettime(CLOCK_REALTIME, ts));
}

double timesdiff(const struct timespec t1, const struct timespec t2)
{
    return (double)(t2.tv_sec - t1.tv_sec) + ((double)(t2.tv_nsec - t1.tv_nsec) / 1e9L);
}

String show_time(const double t)
{
    return rcl_sprintf_s("%.3lf seconds", t);
}
