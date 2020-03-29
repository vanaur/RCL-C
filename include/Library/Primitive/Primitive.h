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
_export typedef struct RawCode rcl_expr_t;
typedef rcl_expr_t *rcl_expr_ptr_t;

// Returns the string representation of the given expression
_export String rcl_show_ir_expr(const rcl_expr_t);

// This corresponds to the description of a function (+TODO: Maybe Type),
// with a body (a composition of terms), a name and a hash code.
// For example: `int add_one(const int n) { return n + 1; }` can be compiled
// as the following RCL IR function: `add_one = + 1`.
_export typedef struct RCL_Function rcl_fun_t;
typedef rcl_fun_t *rcl_fun_ptr_t;

// Returns the string representation of the given function
_export String rcl_show_ir_fun(const rcl_fun_t);

// This corresponds to a function to be called from the FFI, for exemple
// if you want to use the function `qsort` from the C standard library.
_export typedef struct RCL_Extern rcl_extern_t;
typedef rcl_extern_t *rcl_extern_ptr_t;

// Returns the string representation of the given external function
_export String rcl_show_ir_extern(const rcl_extern_t);

// This refers to the IR structures of the VM, which are C-like structures
// but can also behave as an enumeration of constants.
_export typedef struct RCL_Structure rcl_struct_t;
typedef rcl_struct_t *rcl_struct_ptr_t;

// Returns the string representation of the given structure
_export String rcl_show_ir_struct(const rcl_struct_t);

// This corresponds to constant table.
_export typedef RCL_Value_Array_t rcl_const_array_t;
typedef rcl_const_array_t *rcl_const_array_ptr_t;

// Set of functions
_export typedef Vector(rcl_fun_vec, rcl_fun_t);
// Set of external functions
_export typedef Vector(rcl_extern_vec, rcl_extern_t);
// Set of structures
_export typedef Vector(rcl_struct_vec, rcl_struct_t);
// Set of constant arrays
_export typedef Vector(rcl_const_array_vec, rcl_const_array_t);
// This is the entry point code
_export typedef rcl_expr_t rcl_entry_point;

// This refers to the execution information to follow when evaluating the code
// (optimisations, diagnostics, benchmarks, compilation, interpretation, checking, ...)
_export typedef Exec rcl_exec_infos_t;

// This structure gathers the constituent elements of a program executable on the VM,
// it contains all the functions, the FFI, the structures, the constant tables, the entry point
// as well as the evaluation information to follow.
_export typedef struct
{
    // The name of the program, will be used as file name if compiled
    String program_name;
    // The source language using RCL as backend (C -> RCL, for example)
    String src_language;
    rcl_fun_vec pfunctions;
    rcl_extern_vec pexternals;
    rcl_struct_vec pstructures;
    rcl_const_array_vec pconstarrays;
    rcl_expr_t pmain;
    rcl_exec_infos_t pexecinfos;
} rcl_program_t;

// Returns an initialized rcl_program data to be used all along the library by filling it.
_export rcl_program_t new_rcl_program(const String, const String, const rcl_exec_infos_t);
