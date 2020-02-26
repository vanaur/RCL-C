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

#include <math.h>
#include <gmp.h>

#include <VM\Core\Execution\Interpreter\Arithmetic.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Syntax\Absyn.h>

// To improve: 10 5 / 2 + => 0.5 2 + => 2.5 ==> To reach
static void integer_arithmetic(Stack * stack, const LiteralOperation lo)
{
    mpz_t result;
    mpz_init(result);

    if (lo->kind != is_Inc && lo->kind != is_Dec)
        doComb(stack, SWAP, NULL);

    switch (lo->kind)
    {
    case is_Add:
        mpz_add(result, drop(stack).u.int_, drop(stack).u.int_);
        break;

    case is_Mul:
        mpz_mul(result, drop(stack).u.int_, drop(stack).u.int_);
        break;

    case is_Sub:
        mpz_sub(result, drop(stack).u.int_, drop(stack).u.int_);
        break;

    case is_Div:
    {
        mpf_t div_result;
        mpf_t x1, y1;
        mpz_t x2, y2;
        mpz_init_set(x2, drop(stack).u.int_);
        mpz_init_set(y2, drop(stack).u.int_);
        mpf_init(x1);
        mpf_init(y1);
        mpf_set_z(x1, x2);
        mpf_set_z(y1, y2);
        mpf_init(div_result);
        mpf_div(div_result, x1, y1);
        push(stack, make_RCL_Value_Float(div_result));
        return (void)(NULL);
    }

    case is_Pow:
        mpz_pow_ui(result, drop(stack).u.int_, mpz_get_ui(drop(stack).u.int_));
        break;

    case is_Mod:
        mpz_mod(result, drop(stack).u.int_, drop(stack).u.int_);
        break;

    case is_Inc:
        mpz_add_ui(result, drop(stack).u.int_, 1);
        break;

    case is_Dec:
        mpz_sub_ui(result, drop(stack).u.int_, 1);
        break;

    case is_BAnd:
        mpz_and(result, drop(stack).u.int_, drop(stack).u.int_);
        break;

    case is_BOr:
        mpz_ior(result, drop(stack).u.int_, drop(stack).u.int_);
        break;

    case is_BXor:
        mpz_xor(result, drop(stack).u.int_, drop(stack).u.int_);
        break;

    case is_BLeft:
        mpz_mul_2exp(result, drop(stack).u.int_, mpz_get_d(drop(stack).u.int_));
        break;

    case is_BRight:
        mpz_div_2exp(result, drop(stack).u.int_, mpz_get_d(drop(stack).u.int_));
        break;
    }

    push(stack, make_RCL_Value_Integer(result));
}

/* static void integer_arithmetic(Stack * stack, const LiteralOperation lo)
{
    if (lo->kind == lo_inc)
    {
        if (top(stack).kind == RCL_Value_Integer)
            mpz_add_ui(top_ptr(stack)->u.int_, top_ptr(stack)->u.int_, 1);

        if (top(stack).kind == RCL_Value_Float)
            mpf_add_ui(top_ptr(stack)->u.float_, top_ptr(stack)->u.float_, 1);

        return;
    }

    const Value a = drop(stack);
    const Value b = drop(stack);

    if (a.kind == RCL_Value_Integer && b.kind == RCL_Value_Integer);
    if (a.kind == RCL_Value_Integer && b.kind == RCL_Value_Float);
    if (a.kind == RCL_Value_Float && b.kind == RCL_Value_Integer);
    if (a.kind == RCL_Value_Float && b.kind == RCL_Value_Float);

} */

void doArithmetic(Stack * stack, const LiteralOperation lo, BResult * bresult)
{
    if (top_ptr(stack)->kind == RCL_Value_Integer)
        integer_arithmetic(stack, lo);
}
