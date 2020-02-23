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

// Interpreted or compiled
typedef enum
{
    Interpreted,
    Compiled,
    Jited
} IOC;

// Strict eval, optimistic eval or Lazy eval
typedef enum
{
    Strict,
    Optimistic,
    Lazy
} SOOL;

typedef enum
{
    Noise,
    Silent
} NoiseLevel;

typedef enum
{
    O0, // Does no apply optimization
    O1, // Apply peephole
    O2, // Apply O1 + inlining of non-recursive function
    O3  // Apply O2 + a pre-evaluation of the functions and program
} OptimizationLevel;

typedef enum
{
    P_no,   // Not to prehandle
    P_prog, // Prehandle as entier program
    P_lib,  // Prehandle as library
    P_fn    // Prehandle as function (the name of the function is the name of the file)
} Prehandle;

// Execution and evaluation mode
typedef struct
{
    IOC ioc;
    SOOL sool;
    NoiseLevel noise_level;
    OptimizationLevel optimize_level;
    Prehandle prehandle;
    bool optimize_rec;
    bool gc_free;
    bool osize;
    // Keep generated
    bool kir;
    bool kasm;
    bool show_steps;
    bool docasm;
    // Typecheck?
    bool type_check;
    bool show_res;
    // extentions
    bool ext_ptr;
    bool ext_bignum;
    bool low; // Lambda Over Write
    bool shw_inside_struct;
} __attribute__((packed)) Exec;
