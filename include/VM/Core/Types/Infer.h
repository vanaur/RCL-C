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
#include <VM\Core\Types\Types.h>

// A table of functions that are non-typed again
Vector(fununtyped_table_t, hash_t);

bool cmp_hashs(const hash_t, const hash_t);

// Performs a type inference (more precisely, it determines the action on the stack,
// the scope of the elements and the type of the result) of the expression sent,
// taking into account an environment of variable types or not already existing.
RCL_Type infer_type(Env_map_t *, RawCode *, fununtyped_table_t *, struct State *);
Env_map_t inferall(BResult *, struct State *);
