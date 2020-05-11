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
#include <Tools\Utils\Utils.h>
#include <Tools\Vector\Vector.h>
#include <Tools\Hash\djb2.h>

#define CMP_SIMP_F(t1, t2) LAMBDA(bool _(t1 x, t2 y) { return x == y; })
#define CMP_F_STR LAMBDA(bool _(String x, String y) { return !strcmp(x, y); })

inline bool cmp_f_str(const String s1, const String s2)
{
    return CMP_F_STR(s1, s2); // If your editor shows you an error here, your are unlucky,
                              // because it is wrong (this underlines it for me under VScode).
}

inline bool cmp_f_hash(const hash_t h1, const hash_t h2)
{
    return h1 == h2;
}

#define map_unfound -1

// Will generate some useful types and functions working around the Map container (dictionnary)
// The type name is `<name>_map_t` and is typedefed
#define Map(name, key_t, val_t)                                                                             \
    struct name##_entry_s                                                                                   \
    {                                                                                                       \
        key_t key;                                                                                          \
        val_t val;                                                                                          \
    };                                                                                                      \
    Vector(name##_map_t, struct name##_entry_s);                                                    \
    inline void init_##name(name##_map_t *map_ptr, size_t size)                                             \
    {                                                                                                       \
        InitVector(map_ptr, size, struct name##_entry_s);                                                   \
    }                                                                                                       \
    inline void add_##name(name##_map_t *map_ptr, const key_t key, const val_t val)                         \
    {                                                                                                       \
        register const struct name##_entry_s new_item = (struct name##_entry_s){key, val};                  \
        PushToVector(map_ptr, struct name##_entry_s, new_item);                                             \
    }                                                                                                       \
    inline int key_find_##name(const name##_map_t *map_ptr, const key_t key, bool (*cmp_f)(key_t, key_t))   \
    {                                                                                                       \
        for (Iterator i = 0; i < map_ptr->used; i++)                                                        \
            if (cmp_f(key, map_ptr->array[i].key))                                                          \
                return i;                                                                                   \
        return map_unfound;                                                                                 \
    }                                                                                                       \
    inline bool key_exist_##name(const name##_map_t *map_ptr, const key_t key, bool (*cmp_f)(key_t, key_t)) \
    {                                                                                                       \
        return key_find_##name(map_ptr, key, cmp_f) == map_unfound ? false : true;                          \
    }                                                                                                       \
    inline val_t get_val_##name(const name##_map_t *map_ptr, const key_t key, bool (*cmp_f)(key_t, key_t))  \
    {                                                                                                       \
        int index = key_find_##name(map_ptr, key, cmp_f);                                                   \
        if (index == map_unfound)                                                                           \
            printf("Unexisting key for the map %s.\n", #name);                                              \
        return map_ptr->array[index].val;                                                                   \
    }                                                                                                       \
    inline void free_##name(name##_map_t *map_ptr)                                                          \
    {                                                                                                       \
        free(map_ptr->array);                                                                               \
    }                                                                                                       \
    inline void update_val_##name(name##_map_t *map_ptr, size_t index, val_t new_val)                       \
    {                                                                                                       \
        map_ptr->array[index].val = new_val;                                                                \
    }                                                                                                       \
    inline key_t get_last_key_##name(const name##_map_t *map_ptr)                                           \
    {                                                                                                       \
        return map_ptr->array[map_ptr->used - 1].key;                                                       \
    }                                                                                                       \
    inline val_t get_last_val_##name(const name##_map_t *map_ptr)                                           \
    {                                                                                                       \
        return map_ptr->array[map_ptr->used - 1].val;                                                       \
    }                                                                                                       \
    inline void pop_##name(name##_map_t *map_ptr)                                                           \
    {                                                                                                       \
        PopVector(map_ptr);                                                                                 \
    }                                                                                                       \
    inline size_t length_##name(const name##_map_t *map_ptr)                                                \
    {                                                                                                       \
        return map_ptr->used;                                                                               \
    }                                                                                                       \
    inline bool empty_##name(const name##_map_t *map_ptr)                                                   \
    {                                                                                                       \
        return !((bool)map_ptr->used);                                                                      \
    }
