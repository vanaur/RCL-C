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
#include <stdlib.h>
#include <string.h>
#include <Tools\Internal\Internal.h>
#include <Tools\console-color\console-color.h>

#define InitVector(name, Size, T)                \
    name->array = (T *)malloc(Size * sizeof(T)); \
    name->used = 0;                              \
    name->size = Size;

#define PushToVector(name, T, Item)                                      \
    if (name->used == name->size)                                        \
    {                                                                    \
        name->size = 2 * name->used;                                     \
        name->array = (T *)realloc(name->array, name->size * sizeof(T)); \
        if (name->array == NULL)                                         \
            perror("Out of vector memory");                              \
    }                                                                    \
    name->array[name->used++] = Item;

#define PopVector(name) \
    name->array[name->used--];

#define Vector(name, T)                                                                                 \
    struct name                                                                                         \
    {                                                                                                   \
        T *array;                                                                                       \
        size_t used;                                                                                    \
        size_t size;                                                                                    \
    } __attribute__((packed)) name;                                                                     \
    inline void init_##name(struct name *vec_ptr, size_t size)                                          \
    {                                                                                                   \
        InitVector(vec_ptr, size, T);                                                                   \
    }                                                                                                   \
    inline struct name new_##name(const size_t nbr_items)                                               \
    {                                                                                                   \
        return (struct name){(T *)malloc(nbr_items * sizeof(T)), 0, nbr_items};                         \
    }                                                                                                   \
    inline void push_##name(struct name *vec_ptr, T item)                                               \
    {                                                                                                   \
        if (!vec_ptr->size)                                                                             \
        {                                                                                               \
            _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Uninitialized vector `%s`", #name); \
            exit(0);                                                                                    \
        }                                                                                               \
        PushToVector(vec_ptr, T, item);                                                                 \
    }                                                                                                   \
    inline void pop_##name(struct name *vec_ptr)                                                        \
    {                                                                                                   \
        PopVector(vec_ptr);                                                                             \
    }                                                                                                   \
    inline void extend_size_##name(struct name *vec_ptr, const size_t add_size)                         \
    {                                                                                                   \
        vec_ptr->size += add_size;                                                                      \
        vec_ptr->array = (T *)realloc(vec_ptr->array, vec_ptr->size * sizeof(T));                       \
        if (vec_ptr->array == NULL)                                                                     \
            perror("Out of vector memory");                                                             \
    }                                                                                                   \
    inline void concat_##name(struct name *vec_ptr, const struct name to_copy)                          \
    {                                                                                                   \
        if (to_copy.used == 0)                                                                          \
            return;                                                                                     \
        if ((vec_ptr->used - vec_ptr->size) < to_copy.used)                                             \
            extend_size_##name(vec_ptr, to_copy.used);                                                  \
        memcpy(&vec_ptr->array[vec_ptr->used], to_copy.array, to_copy.used * sizeof(T));                \
        vec_ptr->used += to_copy.used;                                                                  \
    }
