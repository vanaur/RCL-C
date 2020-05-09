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
#include <VM\Core\Execution\Interpreter\Atoms\qual.h>
#include <VM\Core\Execution\Interpreter\Atoms\struct_builder.h>
#include <VM\Core\Execution\Interpreter\Atoms\lambda.h>
#include <VM\Core\Execution\Interpreter\Atoms\func.h>
#include <VM\Core\Execution\Interpreter\Atoms\field.h>
#include <VM\Core\Execution\Interpreter\Atoms\FFI\C\ffi.h>
#include <VM\Core\FFI\C\cffi.h>

/**************************************************************************
*!  We consider everything to be in a single file FOR THE MOMENT,         *
*!  until we have an implementation of the import file management system. *
***************************************************************************/

static bool evaluated_word_aside(Stack *stack, BResult *bresult, const RCL_Value_Word_t *word)
{
    switch (word->hash_code)
    {
    // `newf': allows to fill in a data structure according to the given fields' name.
    case RCL_NEWF_HSH:
        eval_newf(stack, bresult);
        return true;

    // `newa': fill each fields of a data structure with the given value ("kinf of `memset`").
    case RCL_NEWA_HSH:
        eval_newa(stack, bresult);
        return true;

    // `new': fill each fields of a data structure with the values in the stack.
    case RCL_NEW_HSH:
        eval_new(stack, bresult);
        return true;

    // `nil' kind of "NULL value", allow to not fill a field of structure with a specific value.
    case RCL_NIL_HSH:
        push(stack, UNIQUE_QUAL_FROM_STR(RCL_NIL_WRD));
        return true;
    }
    return false;
}

static void eval_final_word(Stack *stack, BResult *bresult, const RCL_Value_Word_t *word)
{
    const struct RCL_Lambda *tmp_lambda_ptr = getSpecific_lambda(&bresult->wordico, word->hash_code);
    if (tmp_lambda_ptr != NULL)
        return eval_lambda_call(stack, tmp_lambda_ptr, bresult);

    const struct RCL_Function *tmp_function_ptr = getSpecific_function(&bresult->wordico, word->hash_code);
    if (tmp_function_ptr != NULL)
        return eval_function(stack, tmp_function_ptr, bresult);

    // In waiting for a parsing fix...:
    const char c = word->word_str[0];
    word->word_str[0] = toupper(word->word_str[0]);
    const struct RCL_Structure *tmp_structure_ptr = getSpecific_structure(&bresult->wordico, hash_djb2(word->word_str));
    if (tmp_structure_ptr != NULL)
        return push(stack, make_RCL_Value_DataStruct(make_DataStruct_unfilled(tmp_structure_ptr)));
    word->word_str[0] = c;

    if (evaluated_word_aside(stack, bresult, word))
        return; // Evaluation of word has been done in the above function

    /* If word is not recognized */

    state_put_err_it("Unrecognized word: `%s' in function `%s'.", word->word_str, S_CURRENTF);
}

void eval_qual(Stack *stack, BResult *bresult, const RCL_Value_Qual_t *qual)
{

    if (IS_NO_QUALIFIED(qual)) // When there is no qualification
        return eval_final_word(stack, bresult, &qual->quals[0]);

    //* The qualification of a name can come either from an imported RCL file,
    //* or from an external function (where the qualification would be the name given to the library),
    //* it can also be a VM native word, for example `RCL.some_func`.

    //? First of all, since the file import system is not yet ready,
    //? we will only consider here the reference qualifications to external functions (ex: DynLib.function):

    const int found_lib = key_find_rcl_ffi_C_lib(
        &bresult->psdata.cffi_map,
        qual->quals[0].hash_code,
        &cmp_f_hash);

    if (found_lib == map_unfound)
    {
        state_put_err_it("The qualification `%s' from the term `%s' isn't recognized.", qual->quals[0].word_str, show_qual(*qual));
        return;
    }

    const int found_fun =
        key_find_rcl_ffi_C_function(
            &bresult->psdata.cffi_map.array[found_lib].val.functions,
            qual->quals[1].hash_code,
            &cmp_f_hash);

    if (found_fun == map_unfound)
    {
        state_put_err_it("The word `%s' from the term `%s' isn't recognized.", qual->quals[1].word_str, show_qual(*qual));
        return;
    }

    const struct rcl_ffi_C_dynflib_t dynflib = rcl_ffi_C_get_dynflib(&bresult->psdata.cffi_map, found_lib, found_fun);
    eval_C_call(stack, bresult, dynflib);
}
