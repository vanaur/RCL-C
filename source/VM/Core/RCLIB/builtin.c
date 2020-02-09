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

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include <Tools\Utils\Utils.h>
#include <Tools\Hash\djb2.h>
#include <VM\Core\RCLIB\builtin.h>

struct Hash_builtin
{
    hash_t hash;
    struct Builtin builtin;
};

#define BUILTIN(mod, mod_fn, fn) \
    (struct Builtin) { .module = mod, .u.mod_fn = fn }

#define BUILTIN_MATH(fn)    BUILTIN(_Math, math_function, fn)
#define BUILTIN_IO(fn)      BUILTIN(_IO, io_function, fn)
#define BUILTIN_FILE(fn)    BUILTIN(_File, file_function, fn)
#define BUILTIN_ARRAY(fn)   BUILTIN(_Array, array_function, fn)
#define BUILTIN_BOOL(fn)    BUILTIN(_Bool, boolean_function, fn)
#define BUILTIN_MISC(fn)    BUILTIN(_Misc, misc_function, fn)

static struct Hash_builtin hash_builtins[] =
    {
        {74614674,     BUILTIN_IO(Stdout)},
        {74625665,     BUILTIN_IO(Stderr)},
        {258039789,     BUILTIN_IO(Stdin)},
        {860363069,     BUILTIN_MISC(IfThen)},
        {675916430,     BUILTIN_MISC(If)},
        {161178382,     BUILTIN_ARRAY(Replace)},
        {300456915,     BUILTIN_MATH(Acotan)},
        {866333887,    BUILTIN_MATH(Min)},
        {866334141,    BUILTIN_MATH(Max)},
        {785941107,     BUILTIN_FILE(Exists)},
        {312341230,    BUILTIN_ARRAY(Get)},
        {272308161,     BUILTIN_FILE(Remove)},
        {262717398,     BUILTIN_IO(Write)},
        {466458949,     BUILTIN_FILE(Open)},
        {866344574,    BUILTIN_MATH(Cos)},
        {866327353,    BUILTIN_MATH(Sin)},
        {866326528,    BUILTIN_MATH(Tan)},
        {1443568434,    BUILTIN_MATH(Cotan)},
        {1475315171,    BUILTIN_MATH(Acos)},
        {1475332392,    BUILTIN_MATH(Asin)},
        {1475333217,    BUILTIN_MATH(Atan)},
        {1475621167,    BUILTIN_MATH(Itof)},
        {1475513359,    BUILTIN_MATH(Ftoi)},
        {1454245126,    BUILTIN_MATH(Lower)},
        {1058526215,    BUILTIN_MATH(Greater)},
        {1169987620,   BUILTIN_MATH(LowerEq)},
        {1683815197,    BUILTIN_MATH(GreaterEq)},
        {1475923874,    BUILTIN_MATH(Rand)},
        {1293738233,   BUILTIN_IO(Read)},
        {1789132950,   BUILTIN_FILE(ModeR)},
        {1789132945,   BUILTIN_FILE(ModeW)},
        {1789132967,   BUILTIN_FILE(ModeA)},
        {1088154881,    BUILTIN_FILE(ModeRW)},
        {1088154859,    BUILTIN_FILE(ModeRA)},
        {1717357677,   BUILTIN_ARRAY(Find)},
        {1661660620,   BUILTIN_ARRAY(Length)},
        {2002024918,   BUILTIN_ARRAY(Concat)},
        {1633367483,    BUILTIN_MISC(Case)},
        {1612980358,    BUILTIN_MISC(IfThenElse)},
        {1633424613,    BUILTIN_MISC(Dual)}};

#define SIZE sizeof(hash_builtins) / sizeof(hash_builtins[0])

struct Builtin make_builtin(const hash_t word_hash)
{
    for (Iterator i = 0; i < SIZE; i++)
        if (word_hash == hash_builtins[i].hash)
            return hash_builtins[i].builtin;
}

bool is_builtin(const hash_t word_hash)
{
    for (Iterator i = 0; i < SIZE; i++)
        if (word_hash == hash_builtins[i].hash)
            return true;
    return false;
}
