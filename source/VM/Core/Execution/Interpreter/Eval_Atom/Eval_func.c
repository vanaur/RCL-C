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
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <Tools\Utils\Utils.h>
#include <Tools\CPS\Trampoline.h>
#include <VM\Core\State\State.h>
#include <VM\Core\Wordefinition\Wordico.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_op.h>

TDef_Trampoline_Params(
    Eval_function,
    Field(Stack *, stack); // The main stack
    Field(Stack *, rest);  // The rest of the function to execute after each trampoline call
    Field(Stack *, tmp);   // A temporar stack
    Field(size_t, nbr);
    Field(Stack *, result);                 // The resulting stack
    Field(struct RCL_Function *, func_ptr); // The function to run
    Field(BResult *, bresult));             // The dictionnary

static bool is_expected_function(Value value, String name)
{
    if (value.kind == RCL_Value_Word)
        if (!strcmp(value.u.word_.word_str, name))
            return true;
    return false;
}

static void tramp_Eval_function(Trampoline_t *data)
{

    Tramp_Update(Tramp_Eval_function_params, parameters, data);

    if (parameters->stack->overflow)
        Tramp_Stop(data);

    if (parameters->tmp->used > 0)
        *parameters->stack = *parameters->tmp;

    concatStack(parameters->tmp, parameters->result, parameters->nbr);
    concatStack(parameters->rest, parameters->result, 0);

    parameters->nbr = parameters->tmp->used;

    resetStack(parameters->tmp);
    resetStack(parameters->rest);

    Iterator i = 0;

    parameters->stack->in_rec = true;
    for (i = 0; i < parameters->func_ptr->body.used; i++)
        evalop(parameters->stack, &parameters->func_ptr->body.array[i], parameters->bresult);
    parameters->stack->in_rec = false;

    size_t stack_used = parameters->stack->used;

    for (i = 0; i < stack_used; i++)
    {
        if (is_expected_function(parameters->stack->array[i], parameters->func_ptr->name))
        {
            for (i += 1; i < stack_used; i++)
                push(parameters->rest, parameters->stack->array[i]);
            resetStack(parameters->stack); // C'était pas là avant ==> Permet aucune fuite de mémoire inutile
            return;
            //return concatStackUntil(parameters->rest, parameters->stack, i + 1, stack_used);
        }
        evalop(parameters->tmp, &parameters->stack->array[i], parameters->bresult);
    }

    concatStack(parameters->result, parameters->stack, parameters->result->used);

    resetStack(parameters->stack);

    for (i = 0; i < parameters->result->used; i++)
        evalop(parameters->stack, &parameters->result->array[i], parameters->bresult);

/*     printf("Stack:  %s\n", show_stack(*parameters->stack));
    printf("Tmp:    %s\n", show_stack(*parameters->tmp));
    printf("Rest:   %s\n", show_stack(*parameters->rest));
    printf("Result: %s\n\n", show_stack(*parameters->result)); */

    Tramp_Stop(data);
}

#define Orec_infoState_msg \
    "The recursion optimization is experimental (and slower) in the interpreter, \
and will not guarantee the success of the evaluation of the function '%s`."

void eval_function(Stack *restrict stack, struct RCL_Function *restrict function, BResult *restrict bresult)
{
    bresult->current_name = function->name;
    if (bresult->exec_infos.optimize_rec)
    {
        NewState_continue(
            make_info, Interpreter,
            Orec_infoState_msg,
            function->name);

        Stack rest, tmp, result;
        init_stack(&rest, 1);
        init_stack(&tmp, 1);
        init_stack(&result, 1);

        stack->fname = function->name;
        rest.fname = function->name;
        tmp.fname = function->name;
        result.fname = function->name;

        Tramp_Eval_function_params params =
            {stack, &rest, &tmp, tmp.used, &result, function, bresult};

        //Trampoline_t t = {&tramp_Eval_function, &params};

        Tramp_Def(t, tramp_Eval_function, params);

        trampoline(&t);

        if (stack->overflow)
        {
            NewState_return(make_warning, Interpreter, "The function `%s' produced an overflow by filling the allocated stack about %d.", function->name, (int)stack->size);
        }

        totalyFreeStack(&rest);
        totalyFreeStack(&tmp);
        totalyFreeStack(&result);

        Tramp_Update(Tramp_Eval_function_params, updated, (&t));
        stack = updated->stack;
        stack->fname = "\0";
    }
    else
    {
        extend_size_RawCode(((RawCode *)stack), function->body.used);
        for (Iterator i = 0; i < function->body.used; i++)
            evalop(stack, &function->body.array[i], bresult);
    }
    bresult->current_name = NULL;
}
