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

#include <stdio.h>
#include <ffi.h>
#include <stdbool.h>

#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>

typedef struct Stack
{
    Value *array;
    size_t used;
    size_t size;
    bool in_rec;
    bool overflow;
    String fname;
} __attribute__((packed)) Stack;

void init_stack(Stack *, size_t);
void multinit_stack(Stack *restrict stacks[], size_t);
void resetStack(Stack *);
void totalyFreeStack(Stack *);
void concatStack(Stack *, Stack *, size_t);
void concatStackUntil(Stack *, Stack *, size_t, size_t);

// Adds an item to the top of the stack
void push(Stack *, Value);
// Removes the most recently added item
void pop(Stack *);
// Removes the most recently added item and returns it
Value drop(Stack *);
// Gets the last added item => the top of the stack
Value top(const Stack *);

Value *top_ptr(Stack *);
Value *topx_ptr(Stack *, const size_t);
