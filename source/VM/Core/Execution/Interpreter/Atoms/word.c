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

#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>
#include <VM\Core\Wordefinition\Wordico.h>
#include <VM\Core\RCLIB\builtin.h>
#include <VM\Core\RawCode\RawCode.h>

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Execution\Interpreter\Builtin\Perform.h>
#include <VM\Core\Execution\Interpreter\Atoms\operation.h>
#include <VM\Core\Execution\Interpreter\Atoms\word.h>
#include <VM\Core\Execution\Interpreter\Atoms\new__.h>
#include <VM\Core\Execution\Interpreter\Atoms\lambda.h>
#include <VM\Core\Execution\Interpreter\Atoms\func.h>
#include <VM\Core\Execution\Interpreter\Atoms\field.h>
#include <VM\Core\Execution\Interpreter\Atoms\FFI.h>

inline void evalword(Stack *restrict stack, BResult *restrict bresult, RCL_Value_Word_t *restrict word)
{
    struct RCL_Function *cpytmp_function = getSpecific_function(&bresult->wordico, word->hash_code);
    if (cpytmp_function != NULL)
    {
        return eval_function(stack, cpytmp_function, bresult);
    }

    if (is_builtin(word->hash_code))
    {
        const struct Builtin builtin = make_builtin(word->hash_code);
        return perform_builtin(&builtin, stack, bresult, word->word_str);
    }

    struct RCL_Extern *cpytmp_extern = getSpecific_extern(&bresult->wordico, word->hash_code);
    if (cpytmp_extern != NULL)
    {
        return eval_external_call(stack, bresult, cpytmp_extern);
    }

    struct RCL_Lambda *cpytmp_lambda = getSpecific_lambda(&bresult->wordico, word->hash_code);
    if (cpytmp_lambda != NULL)
    {
        return eval_lambda_call(stack, cpytmp_lambda, bresult);
    }

    if (!strcmp(word->word_str, "new__"))
    {
        return eval_new(stack, bresult);
    }

    if (!strcmp(word->word_str, RCL_NIL_WRD))
    {
        // Used by data structures, this word simply means
        // that you do not want to fill in a field immediately
        return push(stack, RCL_Word(RCL_NIL_WRD));
    }

    String upped;
    rcl_asprintf(&upped, "%s", word->word_str);
    upped[0] = toupper(upped[0]);

    struct RCL_Structure *cpytmp_structure = getSpecific_structure(&bresult->wordico, hash_djb2(upped));

    if (cpytmp_structure != NULL)
    {
        return push(stack, make_RCL_Value_DataStruct(make_DataStruct_unfilled(cpytmp_structure)));
    }

    // If the word is unrecognized, then put an error to the state

    NewState_continue(
        make_error,
        Interpreter,
        "Unrecognized word: `%s'.",
        word->word_str);

    state_put(&bresult->state, make_info(Interpreter, "It could have been used as a lambda name that was unscoped."));
}
