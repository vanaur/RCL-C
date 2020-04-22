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

#include <string.h>
#include <stdlib.h>

#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\cpyvalue.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\Combinator\Combinators.h>

static Value quotecpy(const RCL_Value_Quote_t src)
{
    RawCode *rcode = malloc(sizeof *rcode);
    *rcode = new_RawCode(src->used);
    for (Iterator i = 0; i < src->used; i++)
        valcpy(&rcode->array[i], &src->array[i]);
    rcode->used = src->used;
    return make_RCL_Value_Quotation(rcode);
}

void valcpy(Value *dest, const Value *src)
{
    rcl_assert(dest != NULL);
    rcl_assert(src != NULL);

    switch (src->kind)
    {
    case RCL_Value_Quotation:
        __fast_assign(*dest, quotecpy(src->u.quote_));
        break;

    case RCL_Value_String:
        __fast_assign(*dest, RCL_String(strdup(src->u.string_)));
        break;

    default:
        memcpy(dest, src, sizeof(Value));
    }
}
