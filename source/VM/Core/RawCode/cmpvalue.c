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
#include <string.h>
#include <stdbool.h>
#include <gmp.h>

#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\cmpvalue.h>
#include <VM\Core\RawCode\rawcodecmp.h>

static inline bool cmpvalue_int(const Value v1, const Value v2)
{
    return (bool)(mpz_cmp(v1.u.int_, v2.u.int_) == 0 ? true : false);
}

static inline bool cmpvalue_float(const Value v1, const Value v2)
{
    return (bool)(mpf_cmp(v1.u.float_, v2.u.float_) == 0 ? true : false);
}

static inline bool cmpvalue_char(const Value v1, const Value v2)
{
    return (bool)(v1.u.char_ == v2.u.char_);
}

static inline bool cmpvalue_string(const Value v1, const Value v2)
{
    return (bool)(strcmp(v1.u.string_, v2.u.string_));
}

static inline bool cmpvalue_word(const Value v1, const Value v2)
{
    return (bool)(v1.u.word_.hash_code == v2.u.word_.hash_code);
}

static inline bool cmpvalue_comb(const Value v1, const Value v2)
{
    return (bool)(v1.u.comb_ == v2.u.comb_);
}

static inline bool cmpvalue_lo(const Value v1, const Value v2)
{
    return (bool)(v1.u.litOperation_->kind == v2.u.litOperation_->kind);
}

static inline bool cmpvalue_quote(const Value v1, const Value v2)
{
    if (v1.u.quote_->used != v2.u.quote_->used)
        return false;
    return rawcodecmp(v1.u.quote_->array, v2.u.quote_->array, v1.u.quote_->used);
}

inline bool cmpvalue(const Value v1, const Value v2)
{
    if (v1.kind == RCL_Value_Lambda         //
        || v1.kind == RCL_Value_EndLamScope //
        || v1.kind == RCL_Value_Empty       //
        || v1.kind == RCL_Value_All         //
        || v2.kind == RCL_Value_All         //
                                            // || (v1.kind == RCL_Value_Number && (v2.kind == RCL_Value_Integer || v2.kind == RCL_Value_Float))
                                            // || (v2.kind == RCL_Value_Number && (v1.kind == RCL_Value_Integer || v1.kind == RCL_Value_Float))
    )
        return true;

    if (v1.kind != v2.kind)
        return false;

    switch (v1.kind)
    {
    case RCL_Value_Integer:
        return cmpvalue_int(v1, v2);

    case RCL_Value_Float:
        return cmpvalue_float(v1, v2);

    case RCL_Value_Char:
        return cmpvalue_char(v1, v2);

    case RCL_Value_String:
        return cmpvalue_string(v1, v2);

    case RCL_Value_Word:
        return cmpvalue_word(v1, v2);

    case RCL_Value_Combinator:
        return cmpvalue_comb(v1, v2);

    case RCL_Value_LiteralOperation:
        return cmpvalue_lo(v1, v2);

    case RCL_Value_Quotation:
        return cmpvalue_quote(v1, v2);

    default:
        printf("%d\n", v1.kind);
        perror("TODO cmpval");
    }
}

/*    RCL_Value_Array,
    RCL_Value_Quotation,
    RCL_Value_Integer,
    RCL_Value_Float,
    RCL_Value_Char,
    RCL_Value_String,
    RCL_Value_Word,
    RCL_Value_Lambda,
    RCL_Value_EndLamScope,
    RCL_Value_LiteralOperation,
    RCL_Value_Combinator,
    RCL_Value_Parallel,
    RCL_Value_Field,
    RCL_Value_DataStruct,
    RCL_Value_Replicated,
    RCL_Value_Empty,
    RCL_Value_Number,
    RCL_Value_All*/
