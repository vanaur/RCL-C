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

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <Tools\Utils\Utils.h>
#include <Tools\console-color\console-color.h>

#ifndef __FUNCTION_NAME__
#ifdef WIN32
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

#define _internal_error(file, line, fn, fmt, args...) \
    (void)cc_fprintf(CC_FG_RED, stdout, "  [Internal: %s, %d in '%s']\n     > " fmt "\n", file, line, fn, args);

#define _interr(fmt, args...) _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, fmt, args);

#define _interr_ext(fmt, args...) \
    {                             \
        _interr(fmt, args);       \
        exit(EXIT_FAILURE);       \
    }
