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
#include <assert.h>
#include <pthread.h>

#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\cmpvalue.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\RCLIB\builtin.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Execution\Interpreter\Builtin\Misc\Misc.h>

static inline void f_case(Stack *restrict stack, BResult *restrict bresult)
{
    // [to_case] {cases} {results} [default] case

    Value default_case = drop(stack);
    RCL_Value_Array_t results = drop(stack).u.array_;
    RCL_Value_Array_t cases = drop(stack).u.array_;
    Value to_case = drop(stack);

    assert(cases.length == results.length);

    Stack tmp;
    init_stack(&tmp, to_case.u.quote_->used);
    doUnquote(&tmp, &to_case, bresult);
    assert(tmp.used == 1);

    Stack case_array_eval;

    for (Iterator i = 0; i < cases.length; i++)
    {
        init_stack(&case_array_eval, cases.array->used);
        doUnquote(&case_array_eval, &cases.array->array[i], bresult);
        if (cmpvalue(tmp.array[0], case_array_eval.array[0]))
            return doUnquote(stack, &results.array->array[i], bresult);
    }

    return doUnquote(stack, &default_case, bresult);
}

static inline void f_ifthen(Stack *restrict stack, BResult *restrict bresult)
{
    // TODO == [x] [y] [id] ift
}

static inline void f_if(Stack *restrict stack, BResult *restrict bresult)
{
    Value _else = drop(stack);
    Value _then = drop(stack);
    Value _cond = drop(stack);

    Stack tmp;
    init_stack(&tmp, _cond.u.quote_->used);
    doUnquote(&tmp, &_cond, bresult);
    assert(tmp.used == 1);

    int condition = mpz_cmp_d(top_ptr(&tmp)->u.int_, false);
    condition ? doUnquote(stack, &_then, bresult) : doUnquote(stack, &_else, bresult);
}

struct unquote_t
{
    Stack *stack;
    Value *value;
    BResult *bresult;
};

static void thread_dual(struct unquote_t arg)
{
    doUnquote(arg.stack, arg.value, arg.bresult);
}

// To improve
static inline void f_dual(Stack *stack, BResult *bresult)
{
    Value _p1 = drop(stack);
    Value _p2 = drop(stack);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, (void *(*)(void *))thread_dual, (void *)(&(struct unquote_t){stack, &_p1, bresult}));
    pthread_create(&t2, NULL, (void *(*)(void *))thread_dual, (void *)(&(struct unquote_t){stack, &_p2, bresult}));

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

void perform_builtin_misc(Stack *restrict stack, struct Builtin builtin_infos, BResult *restrict bresult)
{
    switch (builtin_infos.u.misc_function)
    {
    case Case:
        return f_case(stack, bresult);

    case IfThen:
        return f_ifthen(stack, bresult);

    case IfThenElse:
    case If:
        return f_if(stack, bresult);

    case Dual:
        return f_dual(stack, bresult);
    }
}
