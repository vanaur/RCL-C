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

#include <gmp.h>
#include <assert.h>

#include <VM\Core\Execution\Interpreter\Arithmetic.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Show\Show.h>

enum abstract_number_kind
{
    _INTEGER,
    _FLOAT
};

typedef struct
{
    enum abstract_number_kind kind;
    mpz_t *_integer;
    mpf_t *_float;
} abstract_number_t;

static mpf_t *ztof(const mpz_t mpz)
{
    mpf_t *res = malloc(sizeof(mpf_t));
    mpf_init(*res);
    mpf_set_z(*res, mpz);
    return res;
}

abstract_number_t new_abstract_number(const Value value_num)
{
    assert(value_num.kind == RCL_Value_Integer || value_num.kind == RCL_Value_Float);
    if (value_num.kind == RCL_Value_Integer)
    {
        mpz_t *res_ptr = malloc(sizeof(mpz_t));
        mpz_init_set(*res_ptr, value_num.u.int_);
        return (abstract_number_t){_INTEGER, res_ptr, NULL};
    }
    mpf_t *res_ptr = malloc(sizeof(mpf_t));
    mpf_init_set(*res_ptr, value_num.u.float_);
    return (abstract_number_t){_FLOAT, NULL, res_ptr};
}

static void compute_binop(const abstract_number_t n1, const abstract_number_t n2, Stack *stack,
                          void (*fnz)(mpz_ptr, mpz_srcptr, mpz_srcptr),
                          void (*fnf)(mpf_ptr, mpf_srcptr, mpf_srcptr))
{
    if (n1.kind == _INTEGER && n2.kind == _INTEGER)
    {
        if (fnz == NULL)
            _interr_ext("Unsuccessful attempt to apply a `mpz_` function", NULL);
        if (fnz == &mpz_div)
        {
            mpf_t res;
            mpf_init(res);
            mpf_div(res, *ztof(*n1._integer), *ztof(*n2._integer));
            return push(stack, RCL_Float(res));
        }
        mpz_t res;
        mpz_init(res);
        fnz(res, *n1._integer, *n2._integer);
        return push(stack, RCL_Integer(res));
    }
    if (fnf == NULL)
        _interr_ext("Unsuccessful attempt to apply a `mpf_` function", NULL);
    if (n1.kind == _INTEGER && n2.kind == _FLOAT)
    {
        mpf_t res;
        mpf_init(res);
        fnf(res, *ztof(*n1._integer), *n2._float);
        return push(stack, RCL_Float(res));
    }
    if (n1.kind == _FLOAT && n2.kind == _INTEGER)
    {
        mpf_t res;
        mpf_init(res);
        fnf(res, *n1._float, *ztof(*n2._integer));
        return push(stack, RCL_Float(res));
    }
    if (n1.kind == _FLOAT && n2.kind == _FLOAT)
    {
        mpf_t res;
        mpf_init(res);
        fnf(res, *n1._float, *n2._float);
        return push(stack, RCL_Float(res));
    }
    _interr_ext("Reaching the impossible!", NULL);
}

//  a b <op>  =>  `b` <op> `a`
//  6 5 +      => 5 + 6
//  4 7 -      => 7 - 4

static void compute_arithmetic(Stack *stack, const LiteralOperation lo)
{
    if (lo->kind != is_Inc && lo->kind != is_Dec)
    {
        assert(stack->used >= 2);
    }
    else
    {
        assert(stack->used >= 1);
    }

    if (lo->kind == is_Inc || lo->kind == is_Dec)
    {
        const abstract_number_t n1 = new_abstract_number(drop(stack));
        const abstract_number_t n2 = new_abstract_number(RCL_Integer_I(1));
        if (lo->kind == is_Inc)
            return compute_binop(n1, n2, stack, &mpz_add, &mpf_add);
        return compute_binop(n1, n2, stack, &mpz_sub, &mpf_sub);
    }

    const abstract_number_t n1 = new_abstract_number(drop(stack));
    const abstract_number_t n2 = new_abstract_number(drop(stack));

    switch (lo->kind)
    {
    case is_Add:
        compute_binop(n1, n2, stack, &mpz_add, &mpf_add);
        break;

    case is_Mul:
        compute_binop(n1, n2, stack, &mpz_mul, &mpf_mul);
        break;

    case is_Sub:
        compute_binop(n1, n2, stack, &mpz_sub, &mpf_sub);
        break;

    case is_Div:
        compute_binop(n1, n2, stack, &mpz_div, &mpf_div);
        break;

    case is_Pow:
        // Only integer exponentiation for now
        {
            mpz_t result;
            mpz_init(result);
            mpz_pow_ui(result, *n1._integer, mpz_get_d(*n2._integer));
            push(stack, RCL_Integer(result));
            break;
        }

    case is_Mod:
        compute_binop(n1, n2, stack, &mpz_mod, NULL);
        break;

    case is_BAnd:
        compute_binop(n1, n2, stack, &mpz_and, NULL);
        break;

    case is_BOr:
        compute_binop(n1, n2, stack, &mpz_ior, NULL);
        break;

    case is_BXor:
        compute_binop(n1, n2, stack, &mpz_xor, NULL);
        break;

    case is_BLeft:
    {
        mpz_t result;
        mpz_init(result);
        mpz_mul_2exp(result, drop(stack).u.int_, mpz_get_d(drop(stack).u.int_));
        push(stack, RCL_Integer(result));
        break;
    }

    case is_BRight:
    {
        mpz_t result;
        mpz_init(result);
        mpz_div_2exp(result, drop(stack).u.int_, mpz_get_d(drop(stack).u.int_));
        push(stack, RCL_Integer(result));
        break;
    }

    default:
        _interr_ext("Reaching the impossible!", NULL);
        break;
    }
}

void do_arithmetic(Stack *stack, const LiteralOperation lo, BResult *bresult)
{
    compute_arithmetic(stack, lo);
}
