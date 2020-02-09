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

inline rcl__expr_t rcl_expr__cst_int(const int n)
{
    rcl__expr_t res = new_RawCode(1);
    push_RawCode(&res, RCL_Integer_I(n));
    return res;
}

inline rcl__expr_t rcl_expr__cst_bigint(const mpz_t n)
{
    rcl__expr_t res = new_RawCode(1);
    push_rcode(&res, RCL_Integer(n));
    return res;
}

inline rcl__expr_t rcl_expr__cst_float(const double n)
{
    rcl__expr_t res = new_RawCode(1);
    push_rcode(&res, RCL_Float_F(n));
    return res;
}

inline rcl__expr_t rcl_expr__cst_bigfloat(const mpf_t n)
{
    rcl__expr_t res = new_RawCode(1);
    push_rcode(&res, RCL_Float(n));
    return res;
}

inline rcl__expr_t rcl_expr__cst_char(const char c)
{
    rcl__expr_t res = new_RawCode(1);
    push_rcode(&res, RCL_Char(c));
    return res;
}

inline rcl__expr_t rcl_expr__cst_string(const String s)
{
    rcl__expr_t res = new_RawCode(1);
    push_rcode(&res, RCL_String(s));
    return res;
}

inline rcl__expr_t rcl_expr__cst_bool(const bool x)
{
    rcl__expr_t res = new_RawCode(1);
    push_rcode(&res, RCL_Integer_I((int)x));
    return res;
}

static rcl__expr_t rcl_expr__BOP(const rcl__expr_t e1, const rcl__expr_t e2, const size_t opk)
{
    rcl__expr_t res;
    init_rcode(&res, e1.used + e2.used + 1);
    concat_rcode(&res, &e2);
    concat_rcode(&res, &e1);
    push_rcode(&res, RCL_LiteralOperation_kind(opk));
    return res;
}

static rcl__expr_t rcl_expr__UOP(const rcl__expr_t e, const size_t opk)
{
    rcl__expr_t res;
    init_rcode(&res, e.used + 1);
    concat_rcode(&res, &e);
    push_rcode(&res, RCL_LiteralOperation_kind(opk));
    return res;
}

inline rcl__expr_t rcl_expr__add(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_add);
}

inline rcl__expr_t rcl_expr__sub(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_sub);
}

inline rcl__expr_t rcl_expr__pow(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_pow);
}

inline rcl__expr_t rcl_expr__mul(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_mul);
}

inline rcl__expr_t rcl_expr__div(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_div);
}

inline rcl__expr_t rcl_expr__mod(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_mod);
}

inline rcl__expr_t rcl_expr__inc(const rcl__expr_t e)
{
    return rcl_expr__UOP(e, lo_inc);
}

inline rcl__expr_t rcl_expr__dec(const rcl__expr_t e)
{
    return rcl_expr__UOP(e, lo_dec);
}

inline rcl__expr_t rcl_expr__band(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_band);
}

inline rcl__expr_t rcl_expr__bor(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_bor);
}

inline rcl__expr_t rcl_expr__xor(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_bxor);
}

inline rcl__expr_t rcl_expr__shl(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_bleft);
}

inline rcl__expr_t rcl_expr__shr(const rcl__expr_t e1, const rcl__expr_t e2)
{
    return rcl_expr__BOP(e1, e2, lo_bright);
}
