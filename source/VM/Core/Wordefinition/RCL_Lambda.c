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

#include <Tools\Hash\djb2.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BrowsedAbsyn.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>

struct RCL_Lambda make_rcl_Lambda(String name, struct RCL_Argv rcl_argv, Value value, Stack stack)
{
    struct RCL_Lambda result =
        {.name = name, .hash_code = hash_djb2(name), .kind = (stack.used == 0 && value.kind == RCL_Value_Empty) ? is_Argv : is_Lam};

    if (result.kind == is_Argv && rcl_argv.ioc == Interpreted)
        result.u.On_Interpreted.i_argv.rcl_args = rcl_argv.u.interpreted.args;

    if (result.kind == is_Lam)
    {
        switch (rcl_argv.ioc)
        {
        case Interpreted:
            __fast_assign(result.u.On_Interpreted.i_lam.value, value);
            break;

        case Compiled:
            __fast_assign(result.u.On_Compiled.c_lam.value, value);
            break;

        case Jited:
            break;
        }
    }
    return result;
}

void vec_init_lambdas(struct VEC_Lambdas *rcl_lam, size_t size)
{
    InitVector(rcl_lam, size, struct RCL_Lambda);
}

void vec_add_lambdas(struct VEC_Lambdas *rcl_lam, struct RCL_Lambda lam)
{
    PushToVector(rcl_lam, struct RCL_Lambda, lam);
}

void vec_remove_lambda(struct VEC_Lambdas *rcl_lam, hash_t hash_code)
{
    int index = 0;
    for (index = 0; index < rcl_lam->used; index++)
        if (rcl_lam->array[index].hash_code == hash_code)
            break;

    rcl_lam->array[index] = rcl_lam->array[rcl_lam->used - 1];
    rcl_lam->array[rcl_lam->used == 0 ? 0 : rcl_lam->used--];
}
