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

#include <VM\Core\Optimize\Precomputation\Precompute.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\rawcodecmp.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Execution\Interpreter\Interpreter.h>

//#define NUMARGS(...) (sizeof((const Value[]){__VA_ARGS__}) / sizeof(const Value))
#define NUMARGS_KIND(...) (sizeof((const enum Value_Kind[]){__VA_ARGS__}) / sizeof(const enum Value_Kind))
#define SEQ_FREP(vals...) (const Value[]){vals}, NUMARGS(const Value, vals)
#define SEQ_KIND(kinds...) (const enum Value_Kind[]){kinds}, NUMARGS_KIND(kinds)
#define SEQ_VOID SEQ_FREP()

static void seq_replace_fval_kind(BResult *restrict bresult, RawCode *restrict rcode, const enum Value_Kind seq_kind[], size_t size1)
{
    Value *p = rcode->array, *end = rcode->array + rcode->used;

    while (p < end)
    {
        if (p + size1 <= end && rawcodecmp_kind2(p, seq_kind, size1))
        {
            size_t size2 = 1;           // -->  TODO BETTER  <--
            BResult tmp = *bresult;
            memmove(p + size2, p + size1, (end - p - size1) * sizeof(*p));
            tmp.psdata.rcode.array = p;
            interpret(&tmp, bresult->exec_infos.sool);
            memcpy(p, tmp.resulting.array, size2 * sizeof(*p));
            rcode->used = rcode->used - size1 + size2;
            end = rcode->array + rcode->used;
        }
        else
        {
            p++;
        }
    }
}

static void precompute_table(BResult *restrict bresult)
{
    // Replace an arithmetic computation by its result
    seq_replace_fval_kind(bresult, &bresult->psdata.rcode, SEQ_KIND(RCL_Value_Integer, RCL_Value_Integer, RCL_Value_LiteralOperation));
    seq_replace_fval_kind(bresult, &bresult->psdata.rcode, SEQ_KIND(RCL_Value_Float, RCL_Value_Float, RCL_Value_LiteralOperation));
}

void precompute_main(BResult *restrict bresult)
{
    precompute_table(bresult);
    printf("\n%s", showQuote(bresult->psdata.rcode));
}

void precompute_defs(BResult *restrict bresult)
{
}
