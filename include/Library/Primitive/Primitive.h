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

#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>
#include <VM\Core\Wordefinition\Wordico.h>

// This represents a simple composition of terms, being an expression in the IR.
// For example, `x + 7` is an expression that can be compiled as the composition of
// each operations: `x 7 +`.
_export typedef struct RawCode rcl__expr_t;
typedef rcl__expr_t *rcl__expr_ptr_t;

// Returns the string representation of the given expression
_export String rcl__show_ir_expr(const rcl__expr_t);

// This corresponds to the description of a function (+TODO: Maybe Type),
// with a body (a composition of terms), a name and a hash code.
// For example: `int add_one(const int n) { return n + 1; }` can be compiled
// as the following RCL IR function: `add_one = + 1`.
_export typedef struct RCL_Function rcl__fun_t;
typedef rcl__fun_t *rcl__fun_ptr_t;

// Returns the string representation of the given function
_export String rcl__show_ir_fun(const rcl__fun_t);

// This corresponds to a function to be called from the FFI, for exemple
// if you want to use the function `qsort` from the C standard library.
_export typedef struct RCL_Extern rcl__extern_t;
typedef rcl__extern_t *rcl__extern_ptr_t;

// Returns the string representation of the given external function
_export String rcl__show_ir_extern(const rcl__extern_t);

// This refers to the IR structures of the VM, which are C-like structures
// but can also behave as an enumeration of constants.
_export typedef struct RCL_Structure rcl__struct_t;
typedef rcl__struct_t *rcl__struct_ptr_t;

// Returns the string representation of the given structure
_export String rcl__show_ir_struct(const rcl__struct_t);

// This corresponds to constant table.
_export typedef RCL_Value_Array_t rcl__const_array_t;
typedef rcl__const_array_t *rcl__const_array_ptr_t;

// Set of functions
_export typedef Vector(rcl__fun_vec, rcl__fun_t);
// Set of external functions
_export typedef Vector(rcl__extern_vec, rcl__extern_t);
// Set of structures
_export typedef Vector(rcl__struct_vec, rcl__struct_t);
// Set of constant arrays
_export typedef Vector(rcl__const_array_vec, rcl__const_array_t);
// This is the entry point code
_export typedef struct RawCode rcl__entry_point;

// This refers to the execution information to follow when evaluating the code
// (optimisations, diagnostics, benchmarks, compilation, interpretation, checking, ...)
_export typedef Exec rcl__exec_infos_t;

// This structure gathers the constituent elements of a program executable on the VM,
// it contains all the functions, the FFI, the structures, the constant tables, the entry point
// as well as the evaluation information to follow.
_export typedef struct
{
    // The name of the program, will be used as file name if compiled
    String program_name;
    // The source language using RCL as backend (C -> RCL, for example)
    String src_language;
    rcl__fun_vec pfunctions;
    rcl__extern_vec pexternals;
    rcl__struct_vec pstructures;
    rcl__const_array_vec pconstarrays;
    rcl__expr_t pmain;
    rcl__exec_infos_t pexecinfos;
} rcl__program_t;

// Returns an initialized rcl__program data to be used all along the library by filling it.
_export rcl__program_t new_rcl_program(const String, const String, const rcl__exec_infos_t);
