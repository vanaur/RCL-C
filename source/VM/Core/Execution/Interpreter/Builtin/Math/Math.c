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
#include <gmp.h>
#include <time.h>
#include <math.h>

#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\RCLIB\builtin.h>
#include <Tools\Random\MersenneTwister.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Execution\Interpreter\Builtin\Math\Math.h>

static void f_min(Stack *restrict stack)
{

    int result = mpz_cmp(drop(stack).u.int_, drop(stack).u.int_);
    mpz_t v;
    if (result <= 0)
    {
        mpz_init_set_d(v, (double)false);
        push(stack, RCL_Integer(v));
    }
    else
    {
        mpz_init_set_d(v, (double)true);
        push(stack, RCL_Integer(v));
    }
}

static void f_lowereq_int(Stack *restrict stack)
{
    push(stack, RCL_Integer_I(mpz_cmp(drop(stack).u.int_, drop(stack).u.int_) <= 0 ? false : true));
}

static void f_lower_int(Stack *restrict stack)
{

    int result = mpz_cmp(drop(stack).u.int_, drop(stack).u.int_);
    mpz_t v;
    if (result < 0)
    {
        mpz_init_set_d(v, (double)false);
        push(stack, RCL_Integer(v));
    }
    else
    {
        mpz_init_set_d(v, (double)true);
        push(stack, RCL_Integer(v));
    }
}

static void f_lowereq_float(Stack *restrict stack)
{

    int result = mpf_cmp(drop(stack).u.float_, drop(stack).u.float_);
    mpz_t v;
    if (result <= 0)
    {
        mpz_init_set_d(v, (double)false);
        push(stack, RCL_Integer(v));
    }
    else
    {
        mpz_init_set_d(v, (double)true);
        push(stack, RCL_Integer(v));
    }
}

static void f_sin(Stack *restrict stack)
{
    if (top_ptr(stack)->kind == RCL_Value_Float)
    {
        double res = mpf_get_d(drop(stack).u.float_);
        mpf_t m;
        mpf_init_set_d(m, sin(res));
        push(stack, RCL_Float(m));
    }
    else
    {
        double res = mpz_get_d(drop(stack).u.int_);
        mpz_t m;
        mpz_init_set_d(m, sin(res));
        push(stack, RCL_Integer(m));
    }
}

static void f_cos(Stack *restrict stack)
{
    if (top_ptr(stack)->kind == RCL_Value_Float)
    {
        double res = mpf_get_d(drop(stack).u.float_);
        mpf_t m;
        mpf_init_set_d(m, cos(res));
        push(stack, RCL_Float(m));
    }
    else
    {
        double res = mpz_get_d(drop(stack).u.int_);
        mpz_t m;
        mpz_init_set_d(m, cos(res));
        push(stack, RCL_Integer(m));
    }
}

static void f_tan(Stack *restrict stack)
{
    if (top_ptr(stack)->kind == RCL_Value_Float)
    {
        double res = mpf_get_d(drop(stack).u.float_);
        mpf_t m;
        mpf_init_set_d(m, tan(res));
        push(stack, RCL_Float(m));
    }
    else
    {
        double res = mpz_get_d(drop(stack).u.int_);
        mpz_t m;
        mpz_init_set_d(m, tan(res));
        push(stack, RCL_Integer(m));
    }
}

static void f_atan(Stack *restrict stack)
{
    if (top_ptr(stack)->kind == RCL_Value_Float)
    {
        double res = mpf_get_d(drop(stack).u.float_);
        mpf_t m;
        mpf_init_set_d(m, atan(res));
        push(stack, RCL_Float(m));
    }
    else
    {
        double res = mpz_get_d(drop(stack).u.int_);
        mpz_t m;
        mpz_init_set_d(m, atan(res));
        push(stack, RCL_Integer(m));
    }
}

static void f_asin(Stack *restrict stack)
{
    if (top_ptr(stack)->kind == RCL_Value_Float)
    {
        double res = mpf_get_d(drop(stack).u.float_);
        mpf_t m;
        mpf_init_set_d(m, asin(res));
        push(stack, RCL_Float(m));
    }
    else
    {
        double res = mpz_get_d(drop(stack).u.int_);
        mpz_t m;
        mpz_init_set_d(m, asin(res));
        push(stack, RCL_Integer(m));
    }
}
static void f_acos(Stack *restrict stack)
{
    if (top_ptr(stack)->kind == RCL_Value_Float)
    {
        double res = mpf_get_d(drop(stack).u.float_);
        mpf_t m;
        mpf_init_set_d(m, acos(res));
        push(stack, RCL_Float(m));
    }
    else
    {
        double res = mpz_get_d(drop(stack).u.int_);
        mpz_t m;
        mpz_init_set_d(m, acos(res));
        push(stack, RCL_Integer(m));
    }
}

int rand_lim(int limit)
{
    int divisor = RAND_MAX / (limit + 1);
    int retval;

    do
    {
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

static void f_rand(Stack *restrict stack)
{
    struct timeval tv;
    srand(time(NULL));
#if defined __MINGW32__
    mingw_gettimeofday(&tv, NULL);
#elif defined __GNUC__
    gettimeofday(&tv, NULL);
#endif
    push(stack, make_RCL_Value_Integer_i(abs(rand() * (tv.tv_sec + tv.tv_usec)) / stack->size + 1));
}

void perform_builtin_math(Stack *restrict stack, struct Builtin builtin_infos, BResult *restrict bresult)
{
    switch (builtin_infos.u.math_function)
    {
    case Rand:
        return f_rand(stack);

    case Sin:
        return f_sin(stack);

    case Cos:
        return f_cos(stack);

    case Tan:
        return f_tan(stack);

    case Asin:
        return f_asin(stack);

    case Acos:
        return f_acos(stack);

    case Atan:
        return f_atan(stack);

    case LowerEq:
        return f_lowereq_int(stack);

    case Lower:
        return f_lower_int(stack);

    default:
        printf("TODO: %s:%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}
