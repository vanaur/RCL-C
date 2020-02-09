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
#include <stdbool.h>

#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\State\State.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Fcallneed.h>

static inline bool is_number(const Value value)
{
    return value.kind == RCL_Value_Integer    // Int is number
           || value.kind == RCL_Value_Float   // Float is number
           || value.kind == RCL_Value_Number; // Number is number
}

inline bool is_able_to_process(
    Stack *restrict stack,
    BResult *restrict bresult,
    const size_t nbr_of_arg,
    const enum Value_Kind needed[nbr_of_arg],
    const bool use_state,
    const String process_name)
{ return true;
    if (stack->used < nbr_of_arg)
    {
        if (use_state)
        {
            NewState_continue(make_error,
                              Interpreter,
                              "Insufficient arguments on the stack when '%s` calls.",
                              process_name);
        }
        return false;
    }

    for (Iterator i = 0; i < nbr_of_arg; i++)
    {
        if ((needed[i] == RCL_Value_Number && is_number(stack->array[i])) || needed[i] == RCL_Value_All)
            continue;
        else if (stack->array[i].kind != needed[i])
        {
            if (use_state)
            {
                NewState_continue(make_error,
                                  Interpreter,
                                  "Bad value passed as the %s argument of the function `%s'.",
                                  indexToWord(nbr_of_arg - i),
                                  process_name);
                NewState_continue(make_info,
                                  Interpreter,
                                  "Expected a %s but got a %s (`%s`).",
                                  showKind(needed[i]),
                                  showKind(stack->array[i].kind),
                                  show_value(stack->array[i]));
            }
            return false;
        }
    }

    return true;
}
