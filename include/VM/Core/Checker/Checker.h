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
#include <stddef.h>
#include <stdbool.h>

#include <VM\Core\Browse\BResult.h>
#include <VM\Core\State\State.h>

typedef struct
{
    bool able;
} Checked_out;

#define DOT_CODE_NAME "main"
#define CHECKED_OUT(a)     (Checked_out) { .able = a }
#define CHECKED_OUT_OK(i)  (CHECKED_OUT(true))
#define CHECKED_OUT_ERR(i) (CHECKED_OUT(false))

// Performs an analysis of the program sent.
Checked_out check_program(BResult *);
