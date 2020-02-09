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
#include <ctype.h>
#include <ffi.h>

#include <Tools\Utils\Utils.h>
#include <Tools\Vector\Vector.h>
#include <Tools\Hash\djb2.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Syntax\Absyn.h>

struct RCL_Extern {
    String name;                // Extern function name
    String dll;                 // Extern function DLL
    size_t nargs;               // Number of arguments
    ffi_type** targs;           // Type of arguments (array of pointer)
    ffi_type* tret;             // Return type (pointer)
    hash_t hash_code;    // Hash code of the external function
} __attribute__((packed)) RCL_Extern;

// Vector of extern functions
Vector(VEC_Externs, struct RCL_Extern);

struct RCL_Extern make_RCL_extern(String, String, Definition);
size_t count_args(Definition);

void vec_init_externs(struct VEC_Externs*, size_t);
void vec_add_externs(struct VEC_Externs*, Definition);

void vec_add_externs_data(struct VEC_Externs*, struct RCL_Extern);
