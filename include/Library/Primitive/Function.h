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

#include <stdbool.h>
#include <stdlib.h>
#include <gmp.h>
#include <Tools\Utils\Utils.h>
#include <Library\Primitive\Primitive.h>
#include <Library\Primitive\Expression.h>

typedef struct
{
    // The number of parameters
    size_t nbr;
    // The parameters' name
    String *params_name;
} rcl_fun_params_t;

#define rcl_fun_params_0 rcl_fun_params(0, NULL)
_export rcl_fun_params_t rcl_fun_params(const size_t, const String[]);

typedef struct
{
    // The number of arguments
    size_t nbr;
    // The arguments
    rcl_expr_t *args;
} rcl_fun_args_t;

#define rcl_fun_args_0() rcl_fun_args(0, NULL)
_export rcl_fun_args_t rcl_fun_args(const size_t, const rcl_expr_t[]);

// Very elementary and simple function definitions
_export rcl_fun_t rcl_fun_define(const String, const rcl_fun_params_t, const rcl_expr_t);
// Function call
_export rcl_expr_t rcl_fun_call(const String, const rcl_fun_args_t);
