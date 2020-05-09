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

#include <stdlib.h>
#include <string.h>
#include <Tools\Utils\Utils.h>
#include <Library\Prefabs\Namespace\Scope.h>

static bool is_anonymous(const String scope_name)
{
    if (scope_name == NULL)
        return true;
    return !strcmp(trim(scope_name), "");
}

static void fill_scope(rcl_scope_env_t dest, const String scope_name, const size_t depth)
{
    dest->is_anonymous = is_anonymous(scope_name);
    dest->scope_name = scope_name;
    dest->scope_hash = hash_djb2(scope_name);
    dest->depth = depth;
    dest->env = NULL;
}

rcl_scope_env_t rcl_head_scope(const String scope_name)
{
    rcl_scope_env_t head = malloc(sizeof *head);
    fill_scope(head, scope_name, 0);
    head->env = head;
    return head;
}

void rcl_new_scope(rcl_scope_env_t *env, const String scope_name)
{
    if (*env == NULL)
    {
        *env = rcl_head_scope(scope_name);
        return;
    }
    rcl_scope_env_t tail = *env;
    rcl_scope_env_t new_node = malloc(sizeof *new_node);
    fill_scope(new_node, scope_name, (*env)->depth + 1);
    new_node->env = tail->env;
    tail->env = new_node;
    *env = new_node;
}

struct rcl_scope_t rcl_main_scope(const rcl_scope_env_t env)
{
    if (env == 0)
        return (struct rcl_scope_t){NULL, (hash_t)(0), false, 0, NULL};
    const rcl_scope_env_t tail = env;
    return *tail->env;
}

struct rcl_scope_t rcl_current_scope(const rcl_scope_env_t env)
{
    if (env == NULL)
        return rcl_main_scope(env);
    rcl_scope_env_t current = env;
    return *current;
}

size_t rcl_count_scopes(const rcl_scope_env_t env)
{
    return rcl_current_scope(env).depth + 1;
}

void rcl_free_scope_env(rcl_scope_env_t env)
{
    rcl_scope_env_t current = env;
    while (current != NULL)
    {
        current = env;
        env = env->env;
        free(current);
    }
    env = NULL;
}
