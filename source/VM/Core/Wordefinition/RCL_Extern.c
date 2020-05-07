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

#include <stdlib.h>
#include <ctype.h>
#include <ffi.h>

#include <Tools\Hash\djb2.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Syntax\Identifier\Functions.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BrowsedAbsyn.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\FFI\C\ctypes.h>
#include <VM\Core\FFI\RCL_File.h>

size_t count_args(Definition define)
{
    ListFFI_Type_Signature cpy = define->u.extern_.listffi_type_signature_;
    size_t result = 0;
    while (cpy != NULL)
    {
        result += 1;
        cpy = cpy->listffi_type_signature_;
    }
    return result;
}

#include <VM\Core\State\State.h>

struct RCL_Extern make_RCL_extern(String name, String dll, Definition define)
{
    struct State state; //! State!
    state_init(&state);
    size_t nargs = count_ffi_types(define->u.extern_.listffi_type_signature_);
    ffi_type *tret = ffi_ctype_to_real_ctype(define->u.extern_.ffi_type_signature_);
    ffi_type **targs = malloc(nargs * sizeof *targs);
    ffi_ctype_to_real_ctypes(targs, define->u.extern_.listffi_type_signature_);
    struct RCL_Extern result;
    result.dll = dll;
    result.hash_code = hash_djb2(rcl_sprintf_s("%s.%s", dll, name));
    struct rcl_ffi_C_function_t fn = make_rcl_ffi_C_function(name, new_rcl_ffi_C_attributes(name, dll, nargs, tret, targs, &state));
    result.fun = fn;
    return result;

    /*     const String hsh_str_code = rcl_sprintf_s("%s.%s", dll, name);
    struct RCL_Extern result = {.name = name, .dll = dll, .hash_code = hash_djb2(hsh_str_code)};
    size_t nargs = count_ffi_types(define->u.extern_.listffi_type_signature_);
    result.nargs = nargs;
    result.targs = malloc(nargs * sizeof(ffi_type *));
    ffi_ctype_to_real_ctypes(result.targs, define->u.extern_.listffi_type_signature_);
    result.tret = ffi_ctype_to_real_ctype(define->u.extern_.ffi_type_signature_);

struct State my_state;
state_init(&my_state);

    struct rcl_ffi_C_function_t fn = make_rcl_ffi_C_function(name, new_rcl_ffi_C_attributes(name, dll, nargs, result.tret, result.targs, &my_state));
    result.fun = fn;

prettyPrint_state(my_state);

    return result; */
}

void vec_init_externs(struct VEC_Externs *vec, size_t i)
{
    InitVector(vec, i, struct RCL_Extern);
}

void vec_add_externs(struct VEC_Externs *vec, Definition define, rcl_ffi_C_lib_map_t *cffilibmap, struct State *state)
{
    const String fname = get_last_qual(define->u.extern_.identifier_);
    const String lname = define->u.extern_.identifier_->u.qualname_.uident_;

    const hash_t extern_from_lib_hash_code = hash_djb2(lname);
    const int found = key_find_rcl_ffi_C_lib(cffilibmap, extern_from_lib_hash_code, &cmp_f_hash);
    if (found == map_unfound)
    {
        state_put_err_br_cst(state, "Attempt to define the external function `%s' from an undefined library identifier: `%s'.", fname, lname);
        return;
    }

    size_t nargs = count_ffi_types(define->u.extern_.listffi_type_signature_);
    ffi_type *tret = ffi_ctype_to_real_ctype(define->u.extern_.ffi_type_signature_);
    ffi_type **targs = malloc(nargs * sizeof *targs);
    ffi_ctype_to_real_ctypes(targs, define->u.extern_.listffi_type_signature_);
    struct rcl_ffi_C_function_t fn = make_rcl_ffi_C_function(fname, new_rcl_ffi_C_attributes(fname, lname, nargs, tret, targs, state));

    if (cffilibmap->array[found].val.functions.size == 0)
        cffilibmap->array[found].val.functions = new_rcl_ffi_C_function_map_t(1);

    add_rcl_ffi_C_function(&cffilibmap->array[found].val.functions, hash_djb2(fname), fn);

    PushToVector(vec, struct RCL_Extern, make_RCL_extern(fname, get_ast_DLL_extern(define), define));
}

void vec_add_externs_data(struct VEC_Externs *vec, struct RCL_Extern rcl_extern)
{
    PushToVector(vec, struct RCL_Extern, rcl_extern);
}
