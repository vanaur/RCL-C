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

#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>
#include <VM\Core\Wordefinition\Sort_hash\Sort_hash.h>

static int hash_cmp_fn(const void *p1, const void *p2)
{
    int va = ((struct RCL_Function *)p1)->hash_code;
    int vb = ((struct RCL_Function *)p2)->hash_code;
    return (va > vb) - (va < vb);
}

static int hash_cmp_ex(const void *p1, const void *p2)
{
    int va = ((struct RCL_Extern *)p1)->hash_code;
    int vb = ((struct RCL_Extern *)p2)->hash_code;
    return (va > vb) - (va < vb);
}

static int hash_cmp_st(const void *p1, const void *p2)
{
    int va = ((struct RCL_Structure *)p1)->hash_code;
    int vb = ((struct RCL_Structure *)p2)->hash_code;
    return (va > vb) - (va < vb);
}

void *sort_functions(struct VEC_Functions *functions)
{
    if (!functions->used)
        return NULL;
    qsort(functions->array, functions->used, sizeof(struct RCL_Function), hash_cmp_fn);
    return NULL;
}

void *sort_externs(struct VEC_Externs *externs)
{
    if (!externs->used)
        return NULL;
    qsort(externs->array, externs->used, sizeof(struct RCL_Extern), hash_cmp_ex);
    return NULL;
}

void *sort_structures(struct VEC_Structures *structures)
{
    if (!structures->used)
        return NULL;
    qsort(structures->array, structures->used, sizeof(struct RCL_Structure), hash_cmp_st);
    return NULL;
}
