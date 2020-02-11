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
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_lambda.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_op.h>

void new_lambda(Stack *stack, BResult *restrict bresult, const Value lamdop)
{
    if (getSpecific_lambda(&bresult->wordico, hash_djb2(lamdop.u.lam_)))
    {
        NewState_return(
            make_error,
            Interpreter,
            "A lambda declaration is done over another else that wasn't unscoped (`%s') but having the same name.",
            lamdop.u.lam_);
    }

    vec_add_lambdas(&bresult->wordico.lambdas, make_rcl_Lambda(lamdop.u.lam_, bresult->argvs, drop(stack), *stack));
}

void unscope_lambda(BResult *restrict bresult, const Value lamdop)
{
    if (getSpecific_lambda(&bresult->wordico, hash_djb2(lamdop.u.endLamScope_)) == NULL)
    {
        NewState_return(
            make_warning,
            Interpreter,
            "Trying to unscope an non-existing lambda (`%s').",
            lamdop.u.endLamScope_);
    }

    vec_remove_lambda(&bresult->wordico.lambdas, lamdop.u.lam_);
}

void eval_lambda_call(Stack *restrict stack, struct RCL_Lambda *restrict lambda, BResult *restrict bresult)
{
    switch (lambda->kind)
    {
    case is_Lam:
        evalop(stack, &lambda->u.On_Interpreted.i_lam.value, bresult);
        break;

    case is_Argv:
        for (Iterator i = 0; i < lambda->u.On_Interpreted.i_argv.rcl_args.used; i++)
            evalop(stack, &lambda->u.On_Interpreted.i_argv.rcl_args.array[i], bresult);
        break;
    }
}
