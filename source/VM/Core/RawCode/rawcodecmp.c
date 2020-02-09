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

#include <stdbool.h>

#include <VM\Core\RawCode\rawcodecmp.h>
#include <VM\Core\RawCode\cmpvalue.h>

bool rawcodecmp(const Value *str1, const Value *str2, size_t count)
{
    register const Value *s1 = (const Value *)str1;
    register const Value *s2 = (const Value *)str2;

    while (count-- > 0)
        if (!cmpvalue((Value)(*s1++), (Value)(*s2++)))
            return false;

    return true;
}

bool rawcodecmp_kind(const Value *str1, const Value *str2, size_t count)
{
    register const Value *s1 = (const Value *)str1;
    register const Value *s2 = (const Value *)str2;

    while (count-- > 0)
        if (((Value)(*s1++)).kind != ((Value)(*s2++)).kind)
            return false;

    return true;
}

bool rawcodecmp_kind2(const Value *str1, const enum Value_Kind *str2, size_t count)
{
    register const Value *s1 = (const Value *)str1;
    register const enum Value_Kind *s2 = (const enum Value_Kind *)str2;

    while (count-- > 0)
        if (((Value)(*s1++)).kind != (*s2++))
            return false;

    return true;
}
