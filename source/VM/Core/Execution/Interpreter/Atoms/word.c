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
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Execution\Interpreter\Atoms\operation.h>
#include <VM\Core\Execution\Interpreter\Atoms\word.h>
#include <VM\Core\Execution\Interpreter\Atoms\struct_builder.h>
#include <VM\Core\Execution\Interpreter\Atoms\lambda.h>
#include <VM\Core\Execution\Interpreter\Atoms\func.h>
#include <VM\Core\Execution\Interpreter\Atoms\field.h>
#include <VM\Core\Execution\Interpreter\Atoms\FFI\C\ffi.h>
#include <VM\Core\FFI\C\cffi.h>

inline void evalword(Stack *stack, BResult *bresult, RCL_Value_Word_t *word)
{

    struct RCL_Lambda *cpytmp_lambda = getSpecific_lambda(&bresult->wordico, word->hash_code);
    if (cpytmp_lambda != NULL)
    {
        return eval_lambda_call(stack, cpytmp_lambda, bresult);
    }

    struct RCL_Function *cpytmp_function = getSpecific_function(&bresult->wordico, word->hash_code);
    if (cpytmp_function != NULL)
    {
        return eval_function(stack, cpytmp_function, bresult);
    }

    const char c = word->word_str[0];
    word->word_str[0] = toupper(word->word_str[0]);
    struct RCL_Structure *cpytmp_structure = getSpecific_structure(&bresult->wordico, hash_djb2(word->word_str));
    if (cpytmp_structure != NULL)
    {
        return push(stack, make_RCL_Value_DataStruct(make_DataStruct_unfilled(cpytmp_structure)));
    }
    word->word_str[0] = c;

    /** "Words" that are not combinators, but constructors or initializers... **/

    if (word->hash_code == RCL_NEWF_HSH)
    {
        return eval_newf(stack, bresult);
    }

    if (word->hash_code == RCL_NEWA_HSH)
    {
        return eval_newa(stack, bresult);
    }

    if (word->hash_code == RCL_NEW_HSH)
    {
        return eval_new(stack, bresult);
    }

    if (word->hash_code == RCL_NIL_HSH)
    {
        // Used by data structures, this word simply means
        // that you do not want to fill in a field immediately
        return push(stack, RCL_Word(RCL_NIL_WRD));
    }

    /** Forgein functions (external, FFI) **/

    struct RCL_Extern *cpytmp_extern = getSpecific_extern(&bresult->wordico, word->hash_code);
    if (cpytmp_extern != NULL)
    {
        return eval_external_call(stack, bresult, cpytmp_extern);
    }

    // If the word is unrecognized, then put an error to the state

    state_put_err_it("Unrecognized word: `%s', in function `%s'.", word->word_str, S_CURRENTF);
    state_put_info_it("It could have been used as a lambda name that was unscoped.", NULL);
}
//TODO! Il faut revoir la manière de gérer les Words, pour accepter les quals !
