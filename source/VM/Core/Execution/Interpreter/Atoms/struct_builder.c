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
#include <stdio.h>

#include <Tools\Map\Map.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>
#include <VM\Core\Execution\Interpreter\Atoms\struct_builder.h>
#include <VM\Core\Show\Show.h>

void eval_new(Stack *stack, BResult *bresult)
{
    // data S new
    // ex: 3 8 MyStruct new

    rcl_assert(top_ptr(stack)->kind == RCL_Value_DataStruct);

    struct RCL_Value_DataStruct _struct = drop(stack).u.dataStruct_;
    Value *value = NULL;

    for (Iterator i = 0; i < _struct.template->field_alloc_used; i++)
    {
        _struct.fields[i].field_type_info = &_struct.template->fields[i];
        value = malloc(sizeof *value);
        *value = drop(stack);
        _struct.fields[i].field_value = value;
    }

    push(stack, make_RCL_Value_DataStruct(_struct));
}

Map(fival_newX, hash_t, Value *);

void eval_newf(Stack *stack, BResult *bresult)
{
    // data {C} A S newf
    // ex: 4 6 {.six, .four} nil MyStruct newf

    rcl_assert(stack->used >= 3);

    struct RCL_Value_DataStruct _struct = drop(stack).u.dataStruct_;
    Value *other_fields_value = malloc(sizeof *other_fields_value);
    *other_fields_value = drop(stack);
    const RCL_Value_Array_t array_fields = drop(stack).u.array_;

    const size_t len = array_fields.length;
    RCL_Value_Field_t *fields = malloc(len * sizeof *fields);
    for (Iterator i = 0; i < len; i++)
        fields[i] = RCL_ARRAY_CONTENT_CASE_FROM_U(array_fields, i)->array[0].u.field_;

    rcl_assert(stack->used >= len);

    Value *values = malloc(len * sizeof *values);
    for (Iterator i = 0; i < len; i++)
        values[i] = drop(stack);

    fival_newX_map_t fival = new_fival_newX_map_t(len);
    for (Iterator i = 0; i < len; i++)
        add_fival_newX(&fival, fields[i]->u.freefield_.hash_code, &values[i]);

    for (Iterator i = 0; i < _struct.template->field_alloc_used; i++)
    {
        const int found = key_find_fival_newX(&fival, _struct.template->fields[i].hash_code, &cmp_f_hash);
        _struct.fields[i].field_type_info = &_struct.template->fields[i];
        _struct.fields[i].field_value = found == map_unfound ? other_fields_value : fival.array[found].val;
    }

    push(stack, make_RCL_Value_DataStruct(_struct));

    free_fival_newX(&fival);
}

void eval_newa(Stack *stack, BResult *bresult)
{
    // A S newa
    // ex: 0 MyStruct newa

    rcl_assert(stack->used >= 2);

    struct RCL_Value_DataStruct _struct = drop(stack).u.dataStruct_;
    Value *general_value = malloc(sizeof *general_value);
    *general_value = drop(stack);

    for (Iterator i = 0; i < _struct.template->field_alloc_used; i++)
    {
        _struct.fields[i].field_type_info = &_struct.template->fields[i];
        _struct.fields[i].field_value = general_value;
    }

    push(stack, make_RCL_Value_DataStruct(_struct));
}
