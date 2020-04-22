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
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <Tools\Utils\Utils.h>
#include <Tools\CPS\Trampoline.h>
#include <VM\Core\State\State.h>
#include <VM\Core\Wordefinition\Wordico.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\Execution\Interpreter\Atoms\operation.h>

/*

    Recursive function evaluation optimizations are supported when the `--Orec` option is enabled.
    The recursion optimizations provided focus on recursive tail calls,
    all recursive tail call cases are supported in principle.
    The RCL library (RCL::Library) tries to generate recursive tail calls most of the time,
    so most recursion cases are optimized. Some contrary cases exist, however, and are not all managed (for now),
    although some non-recursive call recursion schemes are worked on.

*/

#define FOREVER(instr) \
    while (true)       \
    {                  \
        instr;         \
    }

inline void simple_eval_rcode(Stack *stack, const RawCode rcode, BResult *bresult)
{
    for (Iterator i = 0; i < rcode.used; i++)
        evalop(stack, &rcode.array[i], bresult);
}

struct th_simple_eval_rcode_args_t
{
    Stack *stack;
    const RawCode rcode;
    BResult *bresult;
};

static void th_simple_eval_rcode(struct th_simple_eval_rcode_args_t args)
{
    FOREVER(simple_eval_rcode(args.stack, args.rcode, args.bresult));
}

static bool value_is_fname(const Value value, hash_t f_hash)
{
    if (value.kind == RCL_Value_Word)
        if (value.u.word_.hash_code == f_hash)
            return true;
    return false;
}

// Linear recursive call (LRC) ==> infinite loop
static bool optimized_eval_func__linear_reccall(Stack *stack, struct RCL_Function *function, BResult *bresult)
{
    const RawCode body = function->body;
    Iterator i = body.used;

    if (value_is_fname(body.array[0], function->hash_code))
        FOREVER(); // When there is no instruction to evaluate
                   // for example: `undefined = undefiend`
                   // or again: `foo = foo 3 2 +`
                   // => When the recursive function is call in first, the rest is dead code

    for (i; i > 0; i--)
        if (value_is_fname(body.array[i], function->hash_code))
            break;

    if (i > 0)
    {
        const int j = body.used - i;
        const RawCode undead_real_body = fast_rcode_subv(body, 0, body.used - j);
        struct th_simple_eval_rcode_args_t th_args = {stack, undead_real_body, bresult};
        pthread_t th_fn_eval;
        pthread_create(&th_fn_eval, NULL, (void *(*)(void *))th_simple_eval_rcode, (void *)&th_args);
        pthread_join(th_fn_eval, NULL);
        return true;
    }

    return false;
}

static bool optimized_eval_func__combinator_reccal_ifte(Stack *stack, struct RCL_Function *function, BResult *bresult)
{
    const RawCode body = function->body;

    const RawCode q_else = *(topx_ptr(RCODE_TO_STACK_PTR(&body), 2)->u.quote_);
    const RawCode q_then = *topx_ptr(RCODE_TO_STACK_PTR(&body), 3)->u.quote_;
    const RawCode q_cond = fast_rcode_subv(body, 0, body.used - 3);

    bool rec_in_else = false,
         rec_in_then = false;

    if (value_is_fname(top_rcode(&q_else), function->hash_code))
    {
        rec_in_else = true;
        pop_rcode(&q_else);
    }
    if (value_is_fname(top_rcode(&q_then), function->hash_code))
    {
        rec_in_then = true;
        pop_rcode(&q_then);
    }

    // --- Handle when recursion is in ELSE part of an IFTE expression --- //

    if (rec_in_else && !rec_in_then)
    {
    else__eval_cond:

        simple_eval_rcode(stack, q_cond, bresult);

        Value s_cond = drop(stack);

        if (mpz_cmp_d(s_cond.u.quote_->array[0].u.int_, false))
        {
            simple_eval_rcode(stack, q_then, bresult);
        }
        else
        {
            simple_eval_rcode(stack, q_else, bresult);
            goto else__eval_cond;
        }
    }

    // --- Handle when recursion is in THEN part of an IFTE expression --- //

    if (rec_in_then && !rec_in_else)
    {
    then__eval_cond:

        simple_eval_rcode(stack, q_cond, bresult);

        Value s_cond = drop(stack);

        if (mpz_cmp_d(s_cond.u.quote_->array[0].u.int_, false))
        {
            simple_eval_rcode(stack, q_then, bresult);
            goto then__eval_cond;
        }
        else
        {
            simple_eval_rcode(stack, q_else, bresult);
        }
    }

    return true;
}

// Combinator recursive call (CRC) => case-by-case basis
// Applies to => IFTE, DIP, UNQUOTE, REP, KAP, PAK, UNCONS, SAP, REC
static bool optimized_eval_func__combinator_reccall(Stack *stack, struct RCL_Function *function, BResult *bresult)
{
    if (top_rcode(&function->body).kind != RCL_Value_Combinator)
        return false;
    switch (top_rcode(&function->body).u.comb_)
    {
    case IFTE:
        return optimized_eval_func__combinator_reccal_ifte(stack, function, bresult);

    case DIP:
    case UNQUOTE:
    case REP:
    case KAP:
    case PAK:
    case UNCONS:
    case SAP:
    case REC:
    default:
        return false;
    }
    return true;
}

static void optimized_eval_func(Stack *stack, struct RCL_Function *function, BResult *bresult)
{
    if (optimized_eval_func__linear_reccall(stack, function, bresult))
        return;

    if (optimized_eval_func__combinator_reccall(stack, function, bresult))
        return;

    simple_eval_rcode(stack, function->body, bresult);
}

void eval_function(Stack *stack, struct RCL_Function *function, BResult *bresult)
{
    bresult->current_name = function->name;

    if (bresult->exec_infos.optimize_rec)
        optimized_eval_func(stack, function, bresult);
    else
        simple_eval_rcode(stack, function->body, bresult);
    bresult->current_name = NULL;
}
