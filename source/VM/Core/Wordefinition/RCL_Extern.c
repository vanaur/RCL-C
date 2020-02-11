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
#include <VM\Core\FFI\Types.h>
#include <VM\Core\FFI\RCL_File.h>

size_t count_args(Definition define)
{
    Definition tmp = define;
    size_t result = 0;
    while (tmp->u.extern_.listffi_type_signature_ != NULL)
    {
        result++;
        tmp->u.extern_.listffi_type_signature_ = tmp->u.extern_.listffi_type_signature_->listffi_type_signature_;
    }
    return result;
}

struct RCL_Extern make_RCL_extern(String name, String dll, Definition define)
{
    struct RCL_Extern result = {.name = name, .dll = dll, .hash_code = hash_djb2(name)};
    size_t nargs = count_args(define);
    result.nargs = nargs;
    result.targs = malloc(nargs * sizeof(ffi_type *));
    convertRCLT_FFITS(result.targs, define->u.extern_.listffi_type_signature_);
    return result;
}

void vec_init_externs(struct VEC_Externs *vec, size_t i)
{
    InitVector(vec, i, struct RCL_Extern);
}

void vec_add_externs(struct VEC_Externs *vec, Definition define)
{
    PushToVector(vec, struct RCL_Extern, make_RCL_extern(get_last_qual(define->u.extern_.identifier_), get_ast_DLL_extern(define), define));
}

void vec_add_externs_data(struct VEC_Externs *vec, struct RCL_Extern rcl_extern)
{
    PushToVector(vec, struct RCL_Extern, rcl_extern);
}
