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
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define _export __declspec(dllexport)

#define NUMARGS(t, ...) (sizeof((t[]){__VA_ARGS__}) / sizeof(t))
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N
#define COUNT_VARARGS(...) _GET_NTH_ARG("ignored", ##__VA_ARGS__, 4, 3, 2, 1, 0)

typedef char *String;
typedef void *GenericT;
typedef long long int Iterator;

#define S_ITR "%I64d"

typedef struct
{
    size_t large;
    size_t spec;
    size_t aspect;
} Version;

#define SET_VERSION(x, y, z) \
    (Version) { .large = x, .spec = y, .aspect = z }

#define CURRENT_VERSION SET_VERSION(0, 0, 1)

typedef int thread_ret_t;
#define THREAD_SUCCESS_RET return EXIT_SUCCESS;
#define THREAD_FAILURE_RET return EXIT_FAILURE;

#define SIGMA_CTOR(name, args...) \
    struct name##_                \
    {                             \
        args;                     \
    } name##_;
#define SIGMA_GETV_BYREF(sigvar_name, ctor_name, p_name) sigvar_name->u.ctor_name##_.p_name
#define SIGMA_GETV_BYVAL(sigvar_name, ctor_name, p_name) sigvar_name.u.ctor_name##_.p_name
#define SIGMA_GETC_BYREF(sigvar_name, ctor_name) sigvar_name->u.ctor_name##_
#define SIGMA_GETC_BYVAL(sigvar_name, ctor_name) sigvar_name.u.ctor_name##_
#define SIGMA_FILL_CTOR(sigma_t, ctor_kind, ctor_name, content...) \
    (sigma_t)                                                      \
    {                                                              \
        .kind = ctor_kind, .u.ctor_name##_ = { content }           \
    }

// Sigma type is great, don't judge me.

#define SIGMA_TYPE(name, ctors_kind, ctors_arg) \
    struct name                                 \
    {                                           \
        ctors_kind kind;                        \
        union {                                 \
            ctors_arg;                          \
        } u;                                    \
    } __attribute__((packed)) name;

#define LAMBDA(c_) ({ c_ _; })

String show_version(Version);
bool startsWith(const String, const String);
String timWs(String);
size_t levenshtein(String, size_t, String, size_t);
String indexToWord(size_t);
String getline(void);
void *array_concat(const void *, size_t, const void *, size_t, size_t);
size_t allocated_size(void *);
String to_upper_s(String);
String to_lower_s(String);
int first_digit(int);
int rcl_vasprintf(String *, const String, va_list);
int rcl_asprintf(String *, const String, ...);
String rcl_sprintf_s(const String, ...);
