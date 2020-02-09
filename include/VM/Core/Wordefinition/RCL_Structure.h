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
#include <ctype.h>
#include <Tools\Utils\Utils.h>
#include <Tools\Vector\Vector.h>
#include <Tools\Hash\djb2.h>
#include <VM\Core\Syntax\Absyn.h>

typedef enum Kind_Structure Kind_Structure;

struct RCL_Structure_field {
    String name;
    hash_t hash_code;
    enum Kind_Structure { _is_Enum, _is_Field, _is_Spec } kind;
    union {
        struct { } _enum;
        struct { } _field;
        struct { String struct_name; } _typed;
    } u;
} __attribute__((packed)) RCL_Structure_field;

struct RCL_Structure {
    String name;
    hash_t hash_code;
    struct RCL_Structure_field* fields;
    size_t field_alloc_size;
    size_t field_alloc_used;
} __attribute__((packed)) RCL_Structure;

// Vector of structures
Vector (VEC_Structures, struct RCL_Structure);

struct RCL_Structure_field make_enum_field(String);
struct RCL_Structure_field make_free_field(String);
struct RCL_Structure_field make_spec_field(String, String);

size_t sizeofFields(ListRCL_Structure_Field);
struct RCL_Structure make_rcl_structure(UIdent, ListRCL_Structure_Field);

signed int field_index(struct RCL_Structure, String);

void init_rcl_structure(struct RCL_Structure*, String, size_t);
void add_field(struct RCL_Structure*, struct RCL_Structure_field);

void vec_init_structures(struct VEC_Structures*, size_t);
void vec_add_structures(struct VEC_Structures*, Definition);

void vec_add_structure_data(struct VEC_Structures*, struct RCL_Structure);
