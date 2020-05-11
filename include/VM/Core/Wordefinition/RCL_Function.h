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

#include <Tools\Utils\Utils.h>
#include <Tools\Vector\Vector.h>
#include <Tools\Hash\djb2.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BrowsedAbsyn.h>
#include <VM\Core\Syntax\Absyn.h>

struct __attribute__((packed)) RCL_Function {
    String name;
    RawCode body;
    hash_t hash_code;
};

struct RCL_Function make_RCL_function(LIdent, Operation, BrowsedAbsyn*);

// Vector of functions
Vector (VEC_Functions, struct RCL_Function);

void vec_init_function(struct VEC_Functions*, size_t);
void vec_add_function(struct VEC_Functions*, BrowsedAbsyn*, Definition);

void vec_add_function_data(struct VEC_Functions*, struct RCL_Function);
