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
#include <stdbool.h>

typedef struct Trampoline_t
{
    void (*callback)(struct Trampoline_t *);
    void *parameters;
    bool stop_jump; // keep the callback but stop the calls
} __attribute__((packed)) Trampoline_t;

#define Tramp_Def(chosen_name, fn, params) \
    Trampoline_t chosen_name = {&fn, &params, false};

void trampoline(Trampoline_t *);
void trampoline_jump(Trampoline_t *);

// Define a structure type for the parameters to use in a trampoline function.
#define TDef_Trampoline_Params(Tname, params...) \
    typedef struct                               \
    {                                            \
        params                                   \
    } __attribute__((packed)) Tramp_##Tname##_params;

// Set a field syntax
#define Field(T, Name) T Name;

// Update the parameters and the callback function.
// Define a variable representing the actual parameters, they will been send to the next recursion.
// ##chosen_params_name## (= the parameters variable) can be modyfied.
#define Tramp_Update(tramp_params_type, chosen_params_name, tramp_data_name) \
    tramp_params_type *chosen_params_name = (tramp_params_type *)tramp_data_name->parameters;

// Stop the recursive calling by setting the callback function pointer to NULL
#define Tramp_Stop(tramp_data_name) \
    tramp_data_name->callback = NULL;

#define Tramp_StopJump(tramp_data_name) \
    tramp_data_name->stop_jump = true;
