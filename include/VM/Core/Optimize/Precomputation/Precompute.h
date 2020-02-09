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

// Pre-calculation is a simple optimization, which, as its name suggests,
// already evaluates what can be calculated in code, thus reducing the workload for program execution.
// The algorithm recognizes certain signatures in a sequence, extracts it, evaluates it and replaces this sequence with the result.

// Basic example:   `x y 3 2 + z` → `x y 5 z`
// Other example:
//      fac = dup 1 Math.lowerEq [pop 1] [dup -- fac *] Misc.ifThenElse
//      `x y 4 fac z` → `x y 24 z`

// The algorithm works by recognizing sequences of predefined values,
// for example  - [number number arithmetic_operation]

#include <stddef.h>
#include <VM\Core\Browse\BResult.h>

void precompute_main(BResult *);
void precompute_defs(BResult *);
