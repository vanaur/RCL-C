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
/*#include <ctype.h>
#include <Arithmetic>
#include <..\RawCode\RawCode>
#include <..\Stack\Stack>
#include <..\Stack\Combinators\Basics>
#include <..\Browse\Browse>

void handleOps(Stack * stack, Value value, BResult * bresult)
{
    switch (value.kind)
    {
    case RCL_Value_Quotation ... RCL_Value_String:
        __DO(push(stack, value));
    case RCL_Value_Combinator:
        __DO(doComb(stack, value.u.comb_));
    case RCL_Value_LiteralOperation:
        __DO(doArithmetic(stack, value.u.litOperation_, bresult));
    }
}

void computeParallel_strict(Stack * runtimeStack, Value *f1, Value *f2, size_t f1_args, size_t f2_args, BResult * bresult)
{
    if (runtimeStack->used < f1_args + f2_args)
    {
        fprintf(stderr, "There are not enough elements on the Stack (need %d and there are only %d).\n", f1_args + f2_args, runtimeStack->used);
        perror("");
        return;
    }

    Stack tmp1, tmp2;
    init_stack(&tmp1, f1_args + 1);
    init_stack(&tmp2, f2_args + 1);

    while (f1_args--)
        push(&tmp1, drop(runtimeStack));
    while (f2_args--)
        push(&tmp2, drop(runtimeStack));

    // Reverse stacks => To optimize ==> direct take from well place in array

    for (Iterator i = 0; i < tmp1.used / 2; i++)
    {
        Value t = tmp1.array[i];
        tmp1.array[i] = tmp1.array[tmp1.used - i - 1];
        tmp1.array[tmp1.used - i - 1] = t;
    }

    for (Iterator i = 0; i < tmp2.used / 2; i++)
    {
        Value t = tmp2.array[i];
        tmp2.array[i] = tmp2.array[tmp2.used - i - 1];
        tmp2.array[tmp2.used - i - 1] = t;
    }

    // That it, well done man

    handleOps(&tmp1, *f1, bresult);
    handleOps(&tmp2, *f2, bresult);

    for (Iterator i = 0; i < tmp1.used; i++)
        push(runtimeStack, tmp1.array[i]);
    for (Iterator i = 0; i < tmp2.used; i++)
        push(runtimeStack, tmp2.array[i]);
}
 */