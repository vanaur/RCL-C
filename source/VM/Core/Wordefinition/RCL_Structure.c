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
#include <string.h>
#include <stdio.h>

#include <Tools\Hash\djb2.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Syntax\Identifier\Functions.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BrowsedAbsyn.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>

struct RCL_Structure_field make_enum_field(String name)
{
    return (struct RCL_Structure_field){.kind = _is_Enum, .name = name, .u._enum = {}};
}

struct RCL_Structure_field make_free_field(String name)
{
    return (struct RCL_Structure_field){.kind = _is_Field, .name = name, .u._field = {}};
}

struct RCL_Structure_field make_spec_field(String name, String typename)
{
    return (struct RCL_Structure_field){.kind = _is_Spec, .name = name, .u._typed = typename};
}

void init_rcl_structure(struct RCL_Structure * rcl_struct, String name, size_t fields_alloc_size)
{
    rcl_struct->name = name;
    rcl_struct->field_alloc_size = fields_alloc_size;
    rcl_struct->field_alloc_used = 0;
    rcl_struct->fields = malloc(fields_alloc_size * sizeof(struct RCL_Structure_field));
}

void add_field(struct RCL_Structure * rcl_struct, struct RCL_Structure_field rcl_field)
{
    if (rcl_struct->field_alloc_used == rcl_struct->field_alloc_size)
    {
        rcl_struct->field_alloc_size *= 2;
        rcl_struct->fields = realloc(rcl_struct->fields, rcl_struct->field_alloc_size * sizeof(struct RCL_Structure_field));
        if (rcl_struct->fields == NULL)
            perror("Out of RCL structure fields memory");
    }
    String field_hash_src;
    rcl_asprintf(&field_hash_src, "%s::%s", rcl_struct->name, rcl_field.name);
    rcl_field.hash_code = hash_djb2(field_hash_src);
    rcl_struct->fields[rcl_struct->field_alloc_used++] = rcl_field;
}

size_t count_fields(ListRCL_Structure_Field fields)
{
    ListRCL_Structure_Field cpy = fields;
    size_t result = 0;
    while (cpy--)
        result++;
    return result;
}

struct RCL_Structure make_rcl_structure(UIdent name, ListRCL_Structure_Field fields)
{
    struct RCL_Structure result = {.name = name, .hash_code = hash_djb2(name)};

    const size_t fields_size = count_fields(fields);

    result.fields = (struct RCL_Structure_field *)malloc(fields_size * sizeof(struct RCL_Structure_field));
    result.field_alloc_used = 0;
    result.field_alloc_size = fields_size;

    while (fields != NULL)
    {
        switch (fields->rcl_structure_field_->kind)
        {
        case is_FreeField:
            add_field(&result, make_free_field(fields->rcl_structure_field_->u.freefield_.lident_));
            break;

        case is_EnumField:
            add_field(&result, make_enum_field(fields->rcl_structure_field_->u.enumfield_.lident_));
            break;

        case is_SpecField:
            add_field(&result, make_spec_field(fields->rcl_structure_field_->u.specfield_.lident_,
                                               show_ast_identifier(fields->rcl_structure_field_->u.specfield_.identifier_)));
            break;
        }
        fields = fields->listrcl_structure_field_;
    }

    return result;
}

signed int field_index(struct RCL_Structure rcl_structure, String name)
{
    for (Iterator i = 0; i < rcl_structure.field_alloc_used; i++)
        if (!strcmp(rcl_structure.fields[i].name, name))
            return i;
    return -1;
}

void vec_init_structures(struct VEC_Structures * rcl_structure, size_t size)
{
    InitVector(rcl_structure, size, struct RCL_Structure);
}

void vec_add_structures(struct VEC_Structures * rcl_structure, Definition def)
{
    vec_add_structure_data(
        rcl_structure,
        make_rcl_structure(def->u.structure_.uident_, def->u.structure_.listrcl_structure_field_));
}

void vec_add_structure_data(struct VEC_Structures * vec, struct RCL_Structure rcl_structure)
{
    PushToVector(vec, struct RCL_Structure, rcl_structure);
}
