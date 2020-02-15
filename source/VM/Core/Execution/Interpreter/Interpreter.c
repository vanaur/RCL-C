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
#include <VM\Core\Execution\Interpreter\Interpreter.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_op.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>

inline RawCode seval(const RawCode rcode, BResult *bresult)
{
    Stack runtime_stack;
    init_stack(&runtime_stack, rcode.used);

    for (Iterator i = 0; i < rcode.used; i++)
        evalop(&runtime_stack, &rcode.array[i], bresult);

    return *((RawCode *)&runtime_stack);
}

inline void interpret(BResult *restrict bresult, const SOOL mode)
{
    bresult->current_name = NULL;
    switch (mode)
    {
    case Strict:
        return executeStrict(bresult);

    case Lazy:
        return executeLazy(bresult);

    case Optimistic:
        return executeOptimistic(bresult);

    default:
        return state_put(&bresult->state, make_error(Interpreter, "Invalid interpretation mode."));
    }
}

inline void executeStrict(BResult *restrict bresult)
{
    Stack runtimeStack;
    init_stack(&runtimeStack, bresult->psdata.rcode.used);

    for (Iterator i = 0; i < bresult->psdata.rcode.used; i++)
        evalop(&runtimeStack, &bresult->psdata.rcode.array[i], bresult);

    bresult->resulting.used = runtimeStack.used;
    bresult->resulting.array = runtimeStack.array;
}

inline void executeLazy(BResult *restrict bresult)
{
    Stack runtimeStack;
    Iterator itr = bresult->psdata.rcode.used;
    init_stack(&runtimeStack, bresult->psdata.rcode.used);
    // TODO
    bresult->resulting.used = runtimeStack.used;
    bresult->resulting.array = runtimeStack.array;
}

inline void executeOptimistic(BResult *restrict bresult) {}
