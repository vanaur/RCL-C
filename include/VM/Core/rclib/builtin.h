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
#include <string.h>

#include <Tools\Utils\Utils.h>

struct Builtin {
    enum { _Math, _IO, _File, _Env, _Stack, _Array, _Boolean, _Misc } module;
    union {
        enum { Cos, Sin, Tan, Cotan, Acos, Asin, Atan, Acotan, Min, Max, Itof, Ftoi, Lower, Greater, LowerEq, GreaterEq, Rand } math_function;
        enum { Write, Read, Stdin, Stdout, Stderr } io_function;
        enum { Exists, Open, ModeR, ModeW, ModeA, ModeRW, ModeRA, Remove } file_function;
        enum { Find, Get, Length, Concat, Replace } array_function;
        enum { Eq, Neq, And, Or } boolean_function;
        enum { Case, IfThenElse, IfThen, If, Dual } misc_function;
    } u;
} __attribute__((packed)) Builtin;


struct Builtin make_builtin(hash_t);
bool is_builtin(hash_t);
