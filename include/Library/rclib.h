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

// ---
// This file is only the basic interface (not the kernel) of the library,
// the proposed functions are non-specific and simple.
// ---

#include <Tools\Utils\Utils.h>

#include <Library\Primitive\Primitive.h>
#include <Library\Primitive\Expression.h>

// Prints a simple message as `Hello, world!` with the version of the RCL in use.
// Allows user to checks if the linkage worked properly.
_export void rcl_hello_world();
// Returns the currently used version of the RCL as a string.
_export const String rcl_get_current_version_s();
// Returns the currently used version of the RCL
// as a structure containing the specific fields of the current version.
_export const Version rcl_get_current_version_v();

_export enum IResult_kind { SUCCESS,
                            FAILURE,
                            INCOMPLETE };
_export struct IResult
{
    enum IResult_kind program_res;
    double main_returned;
};

// Takes as argument an RCL IR bytecode program as string, supposedly correct, interprets it
// and returns the result of the main function:
//   `SUCCESS`    (0) if everything work well ;
//   `FAILURE`    (1) if there are one or more errors ;
//   `INCOMPLETE` (2) if there are still elements on the stack).
// The boolean argument displays the result at the end of the execution if `true`.
_export struct IResult rcl_simple_interpret_ir_s(const String, const bool);

// Takes as argument a compiled program and evaluate it (interpreter or JIT or compiler)
// taking into account all informations having being given.
// The boolean argument displays the result at the end of the execution if `true`.
_export struct IResult rcl_eval(const rcl_program_t, const bool);

// Interpret the given expression and returns the resulting
_export struct IResult rcl_evali_expr(const rcl_expr_t);
