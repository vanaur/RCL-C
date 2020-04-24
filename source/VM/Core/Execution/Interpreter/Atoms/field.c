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

#include <stdbool.h>
#include <stdlib.h>

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Wordefinition\Wordico.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\State\State.h>
#include <VM\Core\Execution\Interpreter\Atoms\field.h>

void eval_field(Stack *stack, BResult *bresult, RCL_Structure_Field field)
{
    rcl_assert(top_ptr(stack)->kind == RCL_Value_DataStruct);

    String field_name = NULL;
    hash_t hash_code = (hash_t)0;

    switch (field->kind)
    {
    case is_FreeField:
        __fast_assign(hash_code, field->u.freefield_.hash_code);
        __fast_assign(field_name, field->u.freefield_.lident_);
        break;

    case is_EnumField:
        __fast_assign(hash_code, field->u.enumfield_.hash_code);
        __fast_assign(field_name, field->u.enumfield_.lident_);
        break;

    case is_SpecField:
        __fast_assign(hash_code, field->u.specfield_.hash_code);
        __fast_assign(field_name, field->u.specfield_.lident_);
        break;
    }

    struct RCL_Value_DataStruct _struct = drop(stack).u.dataStruct_;

    if (field->kind == is_EnumField)
    {
        int index = field_index(*_struct.template, hash_code);

        if (index == -1)
            goto unknown_field;
        rcl_assert(_struct.template->fields[index].kind == _is_Enum);
        return push(stack, RCL_Integer_I(index));
    }

    struct RCL_Value_DataStruct_Field *field_ptr = getSpecific_DataStruct_field(_struct, hash_code);

    if (field_ptr == NULL)
        goto unknown_field;

    return push(stack, *field_ptr->field_value);

unknown_field:
    NewState_return(
        make_error,
        Interpreter,
        "The field `%s' does not exist for structure `%s'.",
        field_name,
        _struct.template->name);
}
