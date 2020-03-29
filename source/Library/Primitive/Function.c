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

#include <stdbool.h>
#include <stdlib.h>
#include <gmp.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>
#include <VM\Core\Wordefinition\Wordico.h>
#include <VM\Core\Show\Show.h>
#include <Library\Primitive\Primitive.h>
#include <Library\Primitive\Expression.h>
#include <Library\Primitive\Function.h>

inline rcl_fun_params_t rcl_fun_params(const size_t nbr, const String params_name[nbr])
{
    if (nbr == 0)
        return (rcl_fun_params_t){0, NULL};
    return (rcl_fun_params_t){nbr, params_name};
}

inline rcl_fun_args_t rcl_fun_args(const size_t nbr, const rcl_expr_t args[nbr])
{
    if (nbr == 0)
        return (rcl_fun_args_t){0, NULL};
    return (rcl_fun_args_t){nbr, args};
}

inline rcl_fun_t rcl_fun_define(const String name, const rcl_fun_params_t params, const rcl_expr_t body)
{
    RawCode res_body = new_RawCode(body.used + params.nbr * 2);

    RawCode params_start = new_RawCode(params.nbr);
    RawCode params_end = new_RawCode(params.nbr);

    // for (Iterator i = params.nbr - 1; i >= 0; i--)
    for (Iterator i = 0; i < params.nbr; i++)
    {
        push_RawCode(&params_start, RCL_Lambda(params.params_name[i]));
        push_RawCode(&params_end, RCL_EndLamScope(params.params_name[i]));
    }

    concat_RawCode(&res_body, params_start);
    concat_RawCode(&res_body, body);
    concat_RawCode(&res_body, params_end);

    return (rcl_fun_t){name, res_body, hash_djb2(name)};
}

inline rcl_expr_t rcl_fun_call(const String name, const rcl_fun_args_t args)
{
    RawCode result = new_RawCode(args.nbr + 1);
    for (Iterator i = args.nbr - 1; i >= 0; i--)
        concat_RawCode(&result, args.args[i]);
    push_RawCode(&result, RCL_Word(name));
    return result;
}
