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
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>

#include <Tools\Utils\Utils.h>
#include <Tools\Hash\djb2.h>
#include <Tools\Map\Map.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Function.h>

// There are, for the moment, 8 types
enum Type_kind
{
    TYPE_LITERAL,   // Strings, integers, ...
    TYPE_ARRAY,     // {x, .., z}
    TYPE_QUOTE,     // [x y z]
    TYPE_ARROW,     // a -> b
    TYPE_ANY,       // a, b, c, ... => Variable type / undefined
    TYPE_SVTA,      // for type inference
    TYPE_EMPTY,     // Empty stack (ex: [])
    TYPE_STACK,     // Type of a stack
    TYPE_VARIABLE,  // Type of a variable (function, structure, lambda, ...)
    TYPE_ERROR
};

typedef enum Value_Kind Value_type;
typedef enum Type_kind Type_kind;

// + Creatre a type for pointers



// --- Type variable --- //

typedef unsigned short sva_t;
typedef char tvar_t;
typedef struct RCL_Type* RCL_Type_ptr;

// Single Variable Type Assignment
typedef struct
{
    // To view the SVTA variable type
    String visualize;
    // The tvar name (as char at the moment for debug)
    tvar_t tvar;
    // single tvar assignment
    sva_t sva;
    // The concrete corresponding type
    // The pointer just means that this field is optional (because not all variable types have a real match)
    RCL_Type_ptr concrete_type;
} __attribute__((packed)) svta_t;

typedef svta_t* svta_ptr_t;

typedef char TVar_t;

typedef SIGMA_TYPE(
    RCL_Type, Type_kind,
    SIGMA_CTOR(rcl_type_literal,  Value_type tlit);
    SIGMA_CTOR(rcl_type_any,      TVar_t tany);
    SIGMA_CTOR(rcl_type_svta,     svta_t svta);
    SIGMA_CTOR(rcl_type_array,    struct RCL_Type *tarray);
    SIGMA_CTOR(rcl_type_quote,    struct RCL_Type *tquote);
    SIGMA_CTOR(rcl_type_arrow,    struct RCL_Type *t1; struct RCL_Type *t2);
    SIGMA_CTOR(rcl_type_stack,    struct RCL_Type **tstack; size_t nbr);
    SIGMA_CTOR(rcl_type_variable, String name);
    SIGMA_CTOR(rcl_type_error));

RCL_Type make_RCL_Type_stack(size_t n, RCL_Type[n]);

// When there is an error:
#define T_ERR                 (RCL_Type){.kind = TYPE_ERROR}
#define T_EMPTY               (RCL_Type){.kind = TYPE_EMPTY}
#define T_ANY(vt)             (SIGMA_FILL_CTOR(RCL_Type, TYPE_ANY, rcl_type_any, vt))
#define T_SVTA(vt)            (SIGMA_FILL_CTOR(RCL_Type, TYPE_SVTA, rcl_type_svta, vt))
#define T_LITERAL(tk)         (SIGMA_FILL_CTOR(RCL_Type, TYPE_LITERAL, rcl_type_literal, tk))
#define T_ARRAY(tk)           (SIGMA_FILL_CTOR(RCL_Type, TYPE_ARRAY, rcl_type_array, &tk))
#define T_ARRAY_PTR(tk)       (SIGMA_FILL_CTOR(RCL_Type, TYPE_ARRAY, rcl_type_array, tk))
#define T_QUOTE(tk)           (SIGMA_FILL_CTOR(RCL_Type, TYPE_QUOTE, rcl_type_quote, &tk))
#define T_QUOTE_PTR(tk)       (SIGMA_FILL_CTOR(RCL_Type, TYPE_QUOTE, rcl_type_quote, tk))
#define T_ARROW(tk1, tk2)     (SIGMA_FILL_CTOR(RCL_Type, TYPE_ARROW, rcl_type_arrow, &tk1, &tk2))
#define T_ARROW_PTR(tk1, tk2) (SIGMA_FILL_CTOR(RCL_Type, TYPE_ARROW, rcl_type_arrow, tk1, tk2))
#define T_STACK(ts...)        (RCL_Type){ .kind = TYPE_STACK, .u.rcl_type_stack_ = (RCL_Type *[]){ts}, COUNT_VARARGS(ts)}
#define T_VARIABLE(name)      (SIGMA_FILL_CTOR(RCL_Type, TYPE_VARIABLE, rcl_type_variable, name))

RCL_Type type_of(const Value *, const String, const BResult *);
bool cmp_types(RCL_Type, RCL_Type, bool strict_lit);
size_t arity(RCL_Type);
size_t trace(RCL_Type);

// This environment groups the type of each function
Map(Env, hash_t, RCL_Type);
