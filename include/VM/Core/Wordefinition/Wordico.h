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
#include <ctype.h>
#include <stdbool.h>

#include <Tools\Utils\Utils.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Browse\BrowsedAbsyn.h>
#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>

struct Wordico
{
    // List of functions
    struct VEC_Functions functions;
    // List of lambdas
    struct VEC_Lambdas lambdas;
    // List of structures
    struct VEC_Structures structures;
} __attribute__((packed)) Wordico;

/* Note about lambdas:
    Unlike the rest of the nominal data that is defined in the `.define` section,
    lambdas are defined and used within a stack (whether it is the main one, in a function, or in a quotation).
    Additions and revisions are therefore made to the runtime for interpretation, and adapted for compilation.
*/

void set_wordico(struct Wordico *, BrowsedAbsyn *, ListDefinition, struct State *);
void init_wordico(struct Wordico *, ListDefinition);
void init_wordico_nbr(struct Wordico *, size_t);

size_t count_functions(ListDefinition);
size_t count_externs(ListDefinition);
size_t count_structures(ListDefinition);

struct RCL_Function *getSpecific_function(struct Wordico *, hash_t);
struct RCL_Structure *getSpecific_structure(struct Wordico *, hash_t);
struct RCL_Lambda *getSpecific_lambda(struct Wordico *, hash_t);
struct RCL_Structure_field *getSpecific_field(struct Wordico *, hash_t);
