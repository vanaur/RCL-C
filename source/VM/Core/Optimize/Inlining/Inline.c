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
#include <string.h>

#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Function.h>

void inline_optimization_function(BResult * bresult)
{
/*     size_t n = bresult->wordico.functions.used;
    // The current function in process
    struct RCL_Function *current = NULL;
    // The found function to be inlined
    struct RCL_Function *fcpy = NULL;

    while (n--)
    {
        current = &bresult->wordico.functions.array[n];

        for (Iterator i = 0; i < current->body.used; i++)
        {
            if (current->body.array[i].kind != RCL_Value_Word)
                continue;

            fcpy = getSpecific_function(&bresult->wordico, current->body.array[i].u.word_.hash_code);
            if (fcpy == NULL)
                continue;

            seq_replace(
                &current->body,
                (const Value[]){make_RCL_Value_Word(current->name)}, 1,
                (const Value *)fcpy->body.array, fcpy->body.used);
            fcpy = NULL;
        }
    } */
}

// To improve
void inline_optimization_code(BResult * bresult)
{
/*     struct RCL_Function *fcpy = NULL;

    RawCode *rcode = &bresult->psdata.rcode;
    size_t itrmx = rcode->used;

    for (Iterator i = 0; i < itrmx; i++)
    {
        if (rcode->array[i].kind != RCL_Value_Word)
            continue;

        fcpy = getSpecific_function(&bresult->wordico, rcode->array[i].u.word_.hash_code);

        if (fcpy == NULL)
            continue;

        seq_replace(
            rcode,
            (const Value[]){make_RCL_Value_Word(rcode->array[i].u.word_.word_str)}, 1,
            (const Value *)fcpy->body.array, fcpy->body.used);
        fcpy = NULL;
    } */
}
