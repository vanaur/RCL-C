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
#include <stdbool.h>

#include <VM\Core\Browse\BResult.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>

#define Able_Kinds(kinds...) \
    (sizeof((const enum Value_Kind[]){kinds}) / sizeof(const enum Value_Kind)), (const enum Value_Kind[]) { kinds }

bool is_able_to_process(Stack *restrict stack,
                        BResult *restrict bresult,
                        const size_t nbr_of_arg,
                        const enum Value_Kind needed[nbr_of_arg],
                        const bool use_state,
                        const String process_name);
