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

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_new__.h>

void eval_new(Stack *restrict stack, BResult *restrict bresult)
{
    if (top_ptr(stack)->kind != RCL_Value_DataStruct)
    {
        NewState_return(
            make_error,
            Interpreter,
            "The `new__` operator need to work with a data structure, but the top of the stack is a %s.",
            show_kind(top_ptr(stack)->kind));
    }

    struct RCL_Value_DataStruct _struct = drop(stack).u.dataStruct_;

    if (stack->used < _struct.template->field_alloc_used)
    {
        NewState_return(
            make_error,
            Interpreter,
            "The data structure `%s' has %d fields to fill with the `new` operator, but the stack contains %d values.",
            _struct.template->name,
            (int)_struct.template->field_alloc_used,
            (int)stack->used);
    }

    Value *dropped;

    // TODO selon les champs

    for (Iterator i = 0; i < _struct.template->field_alloc_used; i++)
    {
        _struct.fields[i].field_type_info = &_struct.template->fields[i];
        dropped = malloc(sizeof(Value));
        *dropped = drop(stack);
        // Check structure field kind

        switch (_struct.template->fields[i].kind)
        {
        case _is_Enum:
        {
            // An enumeration field doesn't take any value,
            // because it is just an enumeration value/index
            push(stack, *dropped);
            break;
        }
        case _is_Spec:
        {
            // TODO
        }
        case _is_Field:
        {
            // A free field take anything as value
            // There is no constraint
            _struct.fields[i].field_value = dropped;
            break;
        }
        }

        // ***
        //_struct.fields[i].field_value = dropped;
    }

    push(stack, make_RCL_Value_DataStruct(_struct));
}
