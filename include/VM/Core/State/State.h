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
#include <stdarg.h>
#include <stdbool.h>
#include <stdarg.h>

#include <Tools\Utils\Utils.h>
#include <Tools\Vector\Vector.h>

#define NewState_return(fn, origin, fmt, ...)        \
    {                                                \
        String msg;                                  \
        rcl_asprintf(&msg, fmt, __VA_ARGS__);        \
        state_put(&bresult->state, fn(origin, msg)); \
        free(msg);                                   \
        return;                                      \
    }

#define NewState_continue(fn, origin, fmt, ...)      \
    {                                                \
        String msg;                                  \
        rcl_asprintf(&msg, fmt, __VA_ARGS__);        \
        state_put(&bresult->state, fn(origin, msg)); \
        free(msg);                                   \
    }

enum Origin { Browser, Interpreter, Compiler, Optimizer, Checker };

struct StateKind {
    enum { Error, Warning, Info } kind;
    union {
        struct { String msg; } error;
        struct { String msg; } warning;
        struct { String msg; } info;
    } u;
    enum Origin origin;
} __attribute__((packed)) StateKind;

struct State {
    struct StateKind* array;
    size_t size;
    size_t used;
    bool hasError;
} __attribute__((packed)) State;


struct StateKind make_error(enum Origin, String);
struct StateKind make_warning(enum Origin, String);
struct StateKind make_info(enum Origin, String);

void state_init(struct State*);
void state_put(struct State*, struct StateKind);
void prettyPrint_state(struct State);

size_t count_errors(struct State);
size_t count_warnigns(struct State);
size_t count_infos(struct State);
