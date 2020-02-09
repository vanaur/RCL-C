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
#include <stdbool.h>

#include <Tools\Utils\Utils.h>
#include <Tools\Vector\Vector.h>
#include <Tools\Hash\djb2.h>
#include <VM\Core\Exec.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Argv.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Syntax\Absyn.h>

struct RCL_Lambda {
    String name;
    hash_t hash_code;
    enum LamKind { is_Argv, is_Lam } kind;
    union {
        union On_Interpreted {
            struct { RawCode rcl_args; } i_argv;
            struct { Value value; } i_lam;
        } On_Interpreted;
        union On_Compiled {
            struct { /* if compiled, args are send to the executable */ } c_argv;
            struct { Value value; } c_lam;
        } On_Compiled;
    } u;
} __attribute__((packed)) RCL_Lambda;

// Vector of lambdas
Vector (VEC_Lambdas, struct RCL_Lambda);

// name     → lambda name
// rcl_argv → information about execution
// value    → the value to assign to the lambda
// stack    → current execution stack
struct RCL_Lambda make_rcl_Lambda(String, struct RCL_Argv, Value, Stack);

void vec_init_lambdas(struct VEC_Lambdas*, size_t);
void vec_add_lambdas(struct VEC_Lambdas*, struct RCL_Lambda);
void vec_remove_lambda(struct VEC_Lambdas*, String);
