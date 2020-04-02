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
#include <Tools\Utils\Utils.h>
#include <Tools\Hash\djb2.h>

/*
    The "scopes" make it possible to define a hierarchical level of accessibility for data, functions, classes, ...
    For example, a namespace is a scope encompassing functions (globally),
    but a function also has its own scope from which variables can be created:

    o namespace -------------------- scope depth=0
        o class -------------------- scope depth=1
            o private -------------- scope depth=2
                o method ----------- scope depth=3
                    o variables ---- scope depth=4
            o public --------------- scope depth=2
                o constructor ------ scope depth=3
                    o variables ---- scope depth=4
        o function ----------------- scope depth=1
    o function --------------------- scope depth=0

    With this module you can define scopes.
*/

typedef struct rcl_scope_t *rcl_scope_env_t;

struct rcl_scope_t
{
    // The name of the scope, this is the name of the module / class / function / ...
    // If it is defined as NULL or contains only spaces, then the scope will be defined as anonymous.
    String scope_name;
    hash_t scope_hash;
    bool is_anonymous;
    // The depth of the current scope in the given environment of scopes.
    unsigned int depth;
    // All the children scope of this one.
    // It cans be NULL if the current scope is the last.
    rcl_scope_env_t env;
};

// Define a head scope, this is the first scope of all the scope environment
rcl_scope_env_t rcl_head_scope(const String scope_name);
// Append a children scope to the given environment
void rcl_new_scope(rcl_scope_env_t *env, const String scope_name);
// Get the number of the scope being in the given environment
size_t rcl_count_scopes(const rcl_scope_env_t env);
// Get the main scope (= the head/first) of the given environment
struct rcl_scope_t rcl_main_scope(const rcl_scope_env_t env);
// Get the current scope (= the last one) of the given environment
struct rcl_scope_t rcl_current_scope(const rcl_scope_env_t env);
// This just free the given environment when it is no longer used
void rcl_free_scope_env(rcl_scope_env_t env);

// Shortcut for anonymous new env
#define rcl_anonym_new_scope(env) rcl_new_scope(env, NULL)
// Shortcut for anonymous head env
#define rcl_anonym_head_scope() rcl_head_scope(NULL)
