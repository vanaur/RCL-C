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

#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <VM\Core\FFI\C\cffi.h>
#include <Tools\Internal\Internal.h>

struct rcl_ffi_C_function_t make_rcl_ffi_C_function(const String fname, const struct rcl_ffi_C_attributes_t attribs)
{
    struct rcl_ffi_C_function_t result;
    result.fname = strdup(fname);
    result.attribs = attribs;
    return result;
}

static void rcl_ffi_C_error_loading_dlib(const String libpath, struct State *state)
{
    state_put_err_it_cst(state, "The dynamic library `%s' cannot be loaded.", libpath);
}

static void rcl_ffi_C_error_loading_dfun(const String fname, const String libpath, struct State *state)
{
    state_put_err_it_cst(state, "The function `%s' from the dynamic library `%s' cannot be loaded.", fname, libpath);
}

static rcl_ffi_C_fn_ptr_t rcl_ffi_C_load_dyn_f(const String fname, const String libpath, struct State *state)
{
    rcl_ffi_C_fn_ptr_t dynfptr;
#if defined(_WIN32) || defined(__MINGW32__)
    HINSTANCE hDll = LoadLibrary(libpath);
    if (!hDll)
    {
        rcl_ffi_C_error_loading_dlib(libpath, state);
        __fast_assign(dynfptr, NULL);
    }
    else
        dynfptr = (rcl_ffi_C_fn_ptr_t)GetProcAddress(hDll, fname);
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    void *pLib = dlopen(libpath, RTLD_LAZY);
    if (!pLib)
    {
        rcl_ffi_C_error_loading_dlib(libpath, state);
        __fast_assign(dynfptr, NULL);
    }
    else
        dynfptr = (rcl_ffi_C_fn_ptr_t)dlsym(pLib, fname);
#else
// This `#else` could use the `unix` version written above it a priori.
#error "The compiler does not seem to be willing to compile the FFI used by the RCL.\
If it is able to compile the functions 'dlopen' and 'dlsym', or 'LoadLibrary' and 'GetProcAddress',\
then this is a preprocessor error that needs to be fixed!"
#endif

    if (!dynfptr)
        rcl_ffi_C_error_loading_dfun(fname, libpath, state);

    return dynfptr;
}

struct rcl_ffi_C_attributes_t new_rcl_ffi_C_attributes(
    const String fname,   // The name of the function from the dynamic library
    const String libpath, // The path of the library
    const size_t nargs,   // The number of arguments
    ffi_type *tret,       // The C return type of the function
    ffi_type **targs,     // The C arguments' type
    struct State *state)  // The state
{
    struct rcl_ffi_C_attributes_t result;
    result.cif_already_initialized = false;
    result.properties.fn = rcl_ffi_C_load_dyn_f(fname, libpath, state);
    result.properties.usable = result.properties.fn != NULL;
    result.properties.targs = targs;
    result.properties.tret = tret;
    result.properties.nargs = nargs;
    return result;
}

void rcl_ffi_C_initialize_cif(struct rcl_ffi_C_attributes_t *ffi_attribs_ptr)
{
    rcl_assert(ffi_attribs_ptr != NULL);
    if (!ffi_attribs_ptr->cif_already_initialized)
        ffi_attribs_ptr->status = ffi_prep_cif(
            &ffi_attribs_ptr->cif,
            FFI_DEFAULT_ABI,
            ffi_attribs_ptr->properties.nargs,
            ffi_attribs_ptr->properties.tret,
            ffi_attribs_ptr->properties.targs);
    ffi_attribs_ptr->cif_already_initialized = true;
}

struct rcl_ffi_C_dynflib_t rcl_ffi_C_get_dynflib(const rcl_ffi_C_lib_map_t *lib_map_ptr, const size_t ilib, const size_t ifn)
{
    rcl_assert(lib_map_ptr != NULL);
    return (struct rcl_ffi_C_dynflib_t){&lib_map_ptr->array[ilib].val.libpath,
                                        &lib_map_ptr->array[ilib].val.functions.array[ifn].val};
}

struct rcl_ffi_C_lib_t make_uninitializedf_rcl_ffi_C_lib(const String libpath, const String libname)
{
    struct rcl_ffi_C_lib_t result;
    result.libname = libname;
    result.libpath = libpath;
    result.functions = new_rcl_ffi_C_function_map_t(0);
    return result;
}
