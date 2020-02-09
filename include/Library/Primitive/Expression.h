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
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>
#include <VM\Core\Wordefinition\Wordico.h>

#include <Library\Primitive\Primitive.h>
#include <Library\Primitive\Expression.h>

_export rcl__expr_t rcl_expr__cst_int(const int);
_export rcl__expr_t rcl_expr__cst_bigint(const mpz_t);
_export rcl__expr_t rcl_expr__cst_float(const double);
_export rcl__expr_t rcl_expr__cst_bigfloat(const mpf_t);
_export rcl__expr_t rcl_expr__cst_char(const char);
_export rcl__expr_t rcl_expr__cst_string(const String);
_export rcl__expr_t rcl_expr__cst_bool(const bool);

_export rcl__expr_t rcl_expr__add(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__sub(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__pow(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__mul(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__div(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__mod(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__inc(const rcl__expr_t);
_export rcl__expr_t rcl_expr__dec(const rcl__expr_t);
_export rcl__expr_t rcl_expr__band(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__bor(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__xor(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__shl(const rcl__expr_t, const rcl__expr_t);
_export rcl__expr_t rcl_expr__shr(const rcl__expr_t, const rcl__expr_t);
