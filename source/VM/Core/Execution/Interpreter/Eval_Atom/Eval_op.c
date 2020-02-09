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

#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>
#include <VM\Core\Wordefinition\Wordico.h>

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Execution\Interpreter\Arithmetic.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_op.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_word.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_new__.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_lambda.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_func.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_field.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_FFI.h>

#include <VM\Core\Show\Show.h>

inline void evalop(Stack *restrict stack, Value *restrict op, BResult *restrict bresult)
{
    if (bresult->exec_infos.show_steps)
        printf("step>\t\t%s%s\n", showStack_nobraces(*stack), show_value(*op));

    switch (op->kind)
    {
    case RCL_Value_Word:
        /*  if (stack->in_rec)
        {
            if (!strcmp(stack->fname, op->u.word_))
                return push(stack, *op);
        }
        else
             */
        return evalword(stack, bresult, &op->u.word_);

    case RCL_Value_Array ... RCL_Value_String:
        return push(stack, *op);

    case RCL_Value_Combinator:
        return doComb(stack, op->u.comb_, bresult);

    case RCL_Value_LiteralOperation:
        return doArithmetic(stack, op->u.litOperation_, bresult);

    case RCL_Value_Parallel:
        printf("TODO");
        break;

    case RCL_Value_Replicated:
    {
        int rep = op->u.repeat_.times;
        while (rep--)
            evalop(stack, op->u.repeat_.op, bresult);
    }
    break;

    case RCL_Value_Lambda:
        return new_lambda(stack, bresult, *op);

    case RCL_Value_EndLamScope:
        return unscope_lambda(bresult, *op);

    case RCL_Value_Empty:
        return;

    case RCL_Value_Field:
        return eval_field(stack, bresult, op->u.field_);

    default:
        NewState_return(
            make_error,
            Interpreter,
            "Can't evaluate an element of the unknown kind '%d'.",
            op->kind);
    }
}