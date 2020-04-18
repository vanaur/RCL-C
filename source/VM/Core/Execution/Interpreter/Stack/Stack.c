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
#include <string.h>

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>

static const Value emptyStackValue = {.kind = RCL_Value_Empty};

inline void init_stack(Stack *stack, const size_t initSize)
{
    stack->array = (Value *)malloc(initSize * sizeof(Value));
    stack->used = 0;
    stack->size = initSize;
    stack->in_rec = false;
    stack->overflow = false;
}

inline void multinit_stack(Stack *stacks[], size_t nbr_stck)
{
    while (nbr_stck--)
        init_stack(stacks[nbr_stck], 1);
}

inline void resetStack(Stack *stack)
{
    memset(stack->array, stack->used = 0, stack->size);
    stack->array = (Value *)malloc((stack->size = 1) * sizeof(Value));
    stack->in_rec = false;
    //stack->fname = "\0";
}

inline void totalyFreeStack(Stack *stack)
{
    free(stack->array);
    stack->array = NULL;
    stack->used = stack->size = 0;
    stack->in_rec = false;
    stack->fname = "\0";
}

inline void concatStack(Stack *dest_ptr, Stack *src_ptr, size_t from)
{
    assert(src_ptr->used >= from);

    memcpy(
        dest_ptr->array + (dest_ptr->used - from) * sizeof(Value),
        src_ptr->array + from,
        src_ptr->used * sizeof(Value));

    dest_ptr->used += src_ptr->used - from;

    if (dest_ptr->size <= dest_ptr->used)
        dest_ptr->size = 2 * dest_ptr->used;
}

inline void concatStackUntil(Stack *dest_ptr, Stack *src_ptr, size_t from, size_t until)
{
    if (from >= until)
        return;

    assert(src_ptr->used >= from);
    assert(src_ptr->used >= until);

    memcpy(
        dest_ptr->array + (dest_ptr->used - (from - until)) * sizeof(Value),
        src_ptr->array + from - until,
        src_ptr->used * sizeof(Value));

    dest_ptr->used += src_ptr->used - (from - until);

    if (dest_ptr->size <= dest_ptr->used)
        dest_ptr->size = 2 * dest_ptr->used;
}

inline void push(Stack *stack, const Value item)
{
    if (stack->used == stack->size)
    {
        stack->size = 2 * stack->used;
        stack->array = (Value *)realloc(stack->array, stack->size * sizeof(Value));
        assert(stack->array != NULL);
    }
    stack->array[stack->used++] = item;
}

inline void pop(Stack *stack)
{
    stack->array[stack->used == 0 ? 0 : stack->used--];
}

Value drop(Stack *stack)
{
    if (!stack->used)
        return emptyStackValue;
    Value last = *top_ptr(stack);
    pop(stack);
    return last;
}

inline Value top(const Stack *stack)
{
    return (
        stack->used == 0 ? emptyStackValue : stack->array[stack->used - 1]);
}

inline Value *top_ptr(Stack *stack)
{
    rcl_assert(stack->used > 0);
    return &stack->array[stack->used - 1];
}

inline Value *topx_ptr(Stack *stack, const size_t x)
{
    assert(stack->used > 0);
    return &stack->array[stack->used - x];
}
