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
#include <VM\Core\Execution\Interpreter\Atoms\lambda.h>
#include <VM\Core\Execution\Interpreter\Atoms\operation.h>

void new_lambda(Stack *stack, BResult * bresult, const Value lamdop)
{
    struct RCL_Lambda *lam_yet_exists = getSpecific_lambda(&bresult->wordico, lamdop.u.lam_.hash_code);
    if (lam_yet_exists)
    {
        if (bresult->exec_infos.low)
            lam_yet_exists->u.On_Interpreted.i_lam.value = drop(stack);
        else
        {
            state_put_warn_it("The lambda `%s` is overwrote in function `%s`.", lam_yet_exists->name, S_CURRENTF);
            state_put_info_it("If you need lambda overwriting, use option `--low`.", NULL);
            state_put_info_it("Else, maybe you want unscope the lambda: `%s$`.", lam_yet_exists->name);
            return;
        }
    }
    vec_add_lambdas(&bresult->wordico.lambdas, make_rcl_Lambda(lamdop.u.lam_.word_str, bresult->argvs, drop(stack), *stack));
}

void unscope_lambda(BResult * bresult, const Value lamdop)
{
    if (getSpecific_lambda(&bresult->wordico, lamdop.u.endLamScope_.hash_code) == NULL)
    {
        state_put_warn_it("Trying to unscope an non-existing lambda (`%s').", lamdop.u.endLamScope_);
    }
    else
    {
        vec_remove_lambda(&bresult->wordico.lambdas, lamdop.u.lam_.hash_code);
    }
}

void eval_lambda_call(Stack * stack, struct RCL_Lambda * lambda, BResult * bresult)
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
