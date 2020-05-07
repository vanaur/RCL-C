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
#include <ffi.h>

#include <Tools\Map\Map.h>
#include <VM\Core\State\State.h>

// Type of function adress to be called
typedef void (*rcl_ffi_C_fn_ptr_t)(void);

// These are the properties of the foreign function, which allows us to invoke it.
struct rcl_ffi_C_properties_t
{
    // Fields indicating whether the function is usable or not.
    // For example, if the function in question cannot be found in the dynamic library, this field will be `FALSE`.
    bool usable;
    // The address of the function to be called. It is obtained when loading the dynamic library.
    rcl_ffi_C_fn_ptr_t fn;
    // The number of arguments the function takes.
    size_t nargs;
    // The type of arguments.
    ffi_type **targs;
    // The return type.
    ffi_type *tret;
};

// These are the attributes of the foreign function to be called via the interface.
struct rcl_ffi_C_attributes_t
{
    // Describes whether the CIF (Call InterFace) is initialized.
    // If it is not, the function cannot be called, so if this field is `FALSE', the caller initializes the CIF at runtime.
    bool cif_already_initialized;
    // The call interface used by libffi.
    ffi_cif cif;
    // The call status of the function, it describes whether the call parameters are correct (ABI correct and types correct).
    // If this parameter is not valid, the function cannot be called. This parameter cannot be corrected at runtime.
    ffi_status status;
    // The properties of the function to be called.
    struct rcl_ffi_C_properties_t properties;
};

// A tuple: (function name, attributes)
struct rcl_ffi_C_function_t
{
    String fname;
    struct rcl_ffi_C_attributes_t attribs;
};

// Make a tuple of function/attributes
struct rcl_ffi_C_function_t make_rcl_ffi_C_function(const String, const struct rcl_ffi_C_attributes_t);

Map(rcl_ffi_C_function, hash_t, struct rcl_ffi_C_function_t);

// A tuple: (dynamic library name, its functions)
struct rcl_ffi_C_lib_t
{
    String libpath;                     // The path to find the library
    String libname;                     // The name given to the included library
    rcl_ffi_C_function_map_t functions; // The functions used from the library
};

// Build a 'rcl_ffi_C_lib_t' without any functions
struct rcl_ffi_C_lib_t make_uninitializedf_rcl_ffi_C_lib(const String, const String);

Map(rcl_ffi_C_lib, hash_t, struct rcl_ffi_C_lib_t);

// Defines the attributes of an external function.
struct rcl_ffi_C_attributes_t new_rcl_ffi_C_attributes(
    const String fname,   // The name of the function from the dynamic library
    const String libpath, // The path of the library
    const size_t nargs,   // The number of arguments
    ffi_type *tret,       // The C return type of the function
    ffi_type **targs,     // The C arguments' type
    struct State *state); // The state

// Initialize the given CIF (Call InterFace)
void rcl_ffi_C_initialize_cif(struct rcl_ffi_C_attributes_t *ffi_attribs_ptr);

// This is a simple tuple that makes it easy to use a function by keeping the name of its original library.
struct rcl_ffi_C_dynflib_t
{
    const String *libpath;
    const struct rcl_ffi_C_function_t *dynf;
};

// Get the tuple of function and library
struct rcl_ffi_C_dynflib_t rcl_ffi_C_get_dynflib(
    const rcl_ffi_C_lib_map_t *, // This is the libraries dictionnary
    const size_t,                // This is the index of the library in the dictionnary
    const size_t);               // This is the index of the function in the dictionnary
