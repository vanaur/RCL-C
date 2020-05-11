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
#include <stdbool.h>
#include <Tools\Internal\Internal.h>
#include <Tools\console-color\console-color.h>

#define InitVector(name, Size, T)           \
    name->array = malloc(Size * sizeof(T)); \
    name->used = 0;                         \
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

// The vector is typedefed by default
#define Vector(name, T)                                                                  \
    struct name                                                                          \
    {                                                                                    \
        T *array;                                                                        \
        size_t used;                                                                     \
        size_t size;                                                                     \
    };                                                                                   \
    typedef struct name name;                                                            \
    inline void init_##name(struct name *vec_ptr, size_t size)                           \
    {                                                                                    \
        InitVector(vec_ptr, size, T);                                                    \
    }                                                                                    \
    inline struct name new_##name(const size_t nbr_items)                                \
    {                                                                                    \
        return (struct name){malloc(nbr_items * sizeof(T)), 0, nbr_items};               \
    }                                                                                    \
    inline void push_##name(struct name *vec_ptr, T item)                                \
    {                                                                                    \
        if (!vec_ptr->size)                                                              \
        {                                                                                \
            _interr("Uninitialized vector `%s`", #name);                                 \
            exit(0);                                                                     \
        }                                                                                \
        PushToVector(vec_ptr, T, item);                                                  \
    }                                                                                    \
    inline struct name singleton_##name(T item)                                          \
    {                                                                                    \
        struct name res = new_##name(1);                                                 \
        push_##name(&res, item);                                                         \
        return res;                                                                      \
    }                                                                                    \
    inline T head_##name(const struct name *vec_ptr)                                     \
    {                                                                                    \
        return vec_ptr->array[0];                                                        \
    }                                                                                    \
    inline T last_##name(const struct name *vec_ptr)                                     \
    {                                                                                    \
        return vec_ptr->array[vec_ptr->used - 1];                                        \
    }                                                                                    \
    inline struct name tail_##name(const struct name *vec_ptr)                           \
    {                                                                                    \
        struct name res = new_##name(vec_ptr->used - 1);                                 \
        res.array = vec_ptr->array - 1;                                                  \
        return res;                                                                      \
    }                                                                                    \
    inline struct name front_##name(const struct name *vec_ptr)                          \
    {                                                                                    \
        struct name res = new_##name(vec_ptr->used - 1);                                 \
        for (int i = 0; i < vec_ptr->used - 1; i++)                                      \
            push_##name(&res, vec_ptr->array[i]);                                        \
        return res;                                                                      \
    }                                                                                    \
    inline struct name rreverse_##name(const struct name *vec_ptr)                       \
    {                                                                                    \
        struct name res = *vec_ptr;                                                      \
        for (int i = 0; i < res.used / 2; i++)                                           \
        {                                                                                \
            T t = res.array[i];                                                          \
            res.array[i] = res.array[res.used - i - 1];                                  \
            res.array[res.used - i - 1] = t;                                             \
        }                                                                                \
        return res;                                                                      \
    }                                                                                    \
    inline void pop_##name(struct name *vec_ptr)                                         \
    {                                                                                    \
        PopVector(vec_ptr);                                                              \
    }                                                                                    \
    inline T ctop_##name(struct name vec)                                                \
    {                                                                                    \
        if (!vec.size)                                                                   \
        {                                                                                \
            _interr("Uninitialized vector `%s`", #name);                                 \
            exit(0);                                                                     \
        }                                                                                \
        return vec.array[vec.used - 1];                                                  \
    }                                                                                    \
    inline T *top_ptr_##name(struct name *vec_ptr)                                       \
    {                                                                                    \
        if (!vec_ptr->size)                                                              \
        {                                                                                \
            _interr("Uninitialized vector `%s`", #name);                                 \
            exit(0);                                                                     \
        }                                                                                \
        return &vec_ptr->array[vec_ptr->used - 1];                                       \
    }                                                                                    \
    inline void extend_size_##name(struct name *vec_ptr, const size_t add_size)          \
    {                                                                                    \
        vec_ptr->size += add_size;                                                       \
        vec_ptr->array = (T *)realloc(vec_ptr->array, vec_ptr->size * sizeof(T));        \
        if (vec_ptr->array == NULL)                                                      \
            perror("Out of vector memory");                                              \
    }                                                                                    \
    inline void concat_##name(struct name *vec_ptr, const struct name to_copy)           \
    {                                                                                    \
        if (to_copy.used == 0)                                                           \
            return;                                                                      \
        if ((vec_ptr->used - vec_ptr->size) < to_copy.used)                              \
            extend_size_##name(vec_ptr, to_copy.used);                                   \
        memcpy(&vec_ptr->array[vec_ptr->used], to_copy.array, to_copy.used * sizeof(T)); \
        vec_ptr->used += to_copy.used;                                                   \
    }
