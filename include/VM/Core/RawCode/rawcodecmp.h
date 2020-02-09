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

#pragma once
#include <stdlib.h>
#include <stdbool.h>

#include <VM\Core\RawCode\RawCode.h>

/* rawcodecmp  --   Compares two arrays of `Value`.

Checks if the content of each value in each array is equal respectively.

If the two data to be compared are either
    - a lambda          (" x\ ")
    - an end of scope   (" x$ ")
then only the Kind is checked.

Everything else is checked for strict equality at all levels.

*/

bool rawcodecmp(const Value *, const Value *, size_t);
bool rawcodecmp_kind(const Value *, const Value *, size_t);
bool rawcodecmp_kind2(const Value *, const enum Value_Kind *, size_t);
