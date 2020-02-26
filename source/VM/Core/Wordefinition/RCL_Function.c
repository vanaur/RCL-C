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
#include <stdbool.h>

#include <Tools\Hash\djb2.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BrowsedAbsyn.h>
#include <VM\Core\Wordefinition\RCL_Function.h>

struct RCL_Function make_RCL_function(LIdent id, Operation op, BrowsedAbsyn *psdata)
{
    ListOperation lo = op->u.concatenation_.listoperation_;
    RawCode rcode;
    init_rcode(&rcode, count_operations(lo));
    while (lo != NULL)
    {
        push_rcode(&rcode, otov(lo->operation_));
        lo = lo->listoperation_;
    }
    return (struct RCL_Function){.name = id, .body = rcode, .hash_code = hash_djb2(id)};
}

void vec_init_function(struct VEC_Functions * rcl_f, size_t size)
{
    InitVector(rcl_f, size, struct RCL_Function);
}

void vec_add_function(struct VEC_Functions * rcl_f, BrowsedAbsyn *psdata, Definition def)
{
    PushToVector(rcl_f, struct RCL_Function, make_RCL_function(def->u.def_.lident_, def->u.def_.operation_, psdata));
}

void vec_add_function_data(struct VEC_Functions *vec, struct RCL_Function rcl_function)
{
    PushToVector(vec, struct RCL_Function, rcl_function);
}
