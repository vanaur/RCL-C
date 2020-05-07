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
#include <stdint.h>
#include <ffi.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Atoms\FFI\C\ffi.h>
#include <VM\Core\FFI\C\cffi.h>
#include <VM\Core\FFI\C\ctypes.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\State\State.h>
#include <Tools\Map\Map.h>

static void *itov(const int d)
{
    void *ptr = malloc(sizeof(int));
    *((int *)ptr) = d;
    return ptr;
}

static void *ftov(const double d)
{
    void *ptr = malloc(sizeof(double));
    *((double *)ptr) = d;
    return ptr;
}

static void *ctov(const char c)
{
    void *ptr = malloc(sizeof(char));
    *((char *)ptr) = c;
    return ptr;
}

static void *stov(const String str)
{
    String *r = malloc(strlen(str));
    *r = strdup(str);
    return r;
}

static void *qtov(const RCL_Value_Quote_t quote)
{
    //TODO! Convertir une quotation en array si tout les éléments sont identiques, sinon en fonction...
    //! On peut convertir un quote en ARRAY avec un combinator => un array = array, une quote = fonction.
}

static void *atov(const RCL_Value_Array_t array)
{
    // ...
}

static void *etov(const RCL_Value_Field_t enum_field)
{
    // ...
}

static void *rcl_ffi_C_getarg(Stack *stack)
{
    const Value arg = drop(stack);
    switch (arg.kind)
    {
    case RCL_Value_Integer:
        return itov(mpz_get_d(arg.u.int_));

    case RCL_Value_Float:
        return ftov(mpf_get_d(arg.u.float_));

    case RCL_Value_Char:
        return ctov(arg.u.char_);

    case RCL_Value_String:
        return stov(arg.u.string_);

    case RCL_Value_Quotation:
        return qtov(arg.u.quote_);

    default:
        printf("Can't get arg!\n");
        break;
    }
    return NULL;
}

static void rcl_ffi_C_result_on_stack(
    Stack *stack,
    const void *rc,
    const ffi_type *rt,
    struct State *state,
    const String fname,
    const String libpath)
{
    if (rc == NULL)
        return;
    if (rt == &ffi_type_pointer) // void pointer on any type, could be array
        return push(stack, RCL_String(*(String *)rc));
    if (rt == &ffi_type_uint) // unsigned int
        return push(stack, RCL_Integer_I(*(unsigned int *)rc));
    if (rt == &ffi_type_sint) // signed int
        return push(stack, RCL_Integer_I(*(int *)rc));
    if (rt == &ffi_type_uint8) // unsigned int 8 bits
        return push(stack, RCL_Integer_I(*(uint8_t *)rc));
    if (rt == &ffi_type_sint8) // signed int 8 bits
        return push(stack, RCL_Integer_I(*(int8_t *)rc));
    if (rt == &ffi_type_uint16) // unsigned int 16 bits
        return push(stack, RCL_Integer_I(*(uint16_t *)rc));
    if (rt == &ffi_type_sint16) // signed int 16 bits
        return push(stack, RCL_Integer_I(*(int16_t *)rc));
    if (rt == &ffi_type_uint32) // unsigned int 32 bits
        return push(stack, RCL_Integer_I(*(uint32_t *)rc));
    if (rt == &ffi_type_sint32) // signed int 32 bits
        return push(stack, RCL_Integer_I(*(int32_t *)rc));
    if (rt == &ffi_type_uint64) // unsigned int 64 bits
        return push(stack, RCL_Integer_I(*(uint64_t *)rc));
    if (rt == &ffi_type_double        // double
        || rt == &ffi_type_float      // float
        || rt == &ffi_type_longdouble // long double
        || rt == &ffi_type_slong      // signed long
        || rt == &ffi_type_ulong)     // unsigned long
        return push(stack, RCL_Float_F(*(double *)rc));
    if (rt == &ffi_type_ushort) // unsigned short
        return push(stack, RCL_Integer_I(*(u_short *)rc));
    if (rt == &ffi_type_sshort) // signed short
        return push(stack, RCL_Integer_I(*(short *)rc));
    if (rt == &ffi_type_uchar) // unsigned char
        return push(stack, RCL_Char(*(u_char *)rc));
    if (rt == &ffi_type_schar) // signed char
        return push(stack, RCL_Char(*(char *)rc));

    state_put_err_it_cst(
        state, "Unable to push the returned value from foreign function `%s::%s` on the stack.",
        fname, libpath);
}

static void *allocate_return(const ffi_type *ft_ret)
{
    if (ft_ret == &ffi_type_void)
        return NULL;
    if (ft_ret == &ffi_type_pointer) // void pointer on any type, could be array
        return malloc(sizeof(void *));
    if (ft_ret == &ffi_type_uint) // unsigned int
        return malloc(sizeof(unsigned int));
    if (ft_ret == &ffi_type_sint) // signed int
        return malloc(sizeof(signed int));
    if (ft_ret == &ffi_type_uint8) // unsigned int 8 bits
        return malloc(sizeof(uint8_t));
    if (ft_ret == &ffi_type_sint8) // signed int 8 bits
        return malloc(sizeof(int8_t));
    if (ft_ret == &ffi_type_uint16) // unsigned int 16 bits
        return malloc(sizeof(uint16_t));
    if (ft_ret == &ffi_type_sint16) // signed int 16 bits
        return malloc(sizeof(int16_t));
    if (ft_ret == &ffi_type_uint32) // unsigned int 32 bits
        return malloc(sizeof(uint32_t));
    if (ft_ret == &ffi_type_sint32) // signed int 32 bits
        return malloc(sizeof(int32_t));
    if (ft_ret == &ffi_type_uint64) // unsigned int 64 bits
        return malloc(sizeof(uint64_t));
    if (ft_ret == &ffi_type_double        // double
        || ft_ret == &ffi_type_float      // float
        || ft_ret == &ffi_type_longdouble // long double
        || ft_ret == &ffi_type_slong      // signed long
        || ft_ret == &ffi_type_ulong)     // unsigned long
        return malloc(sizeof(double));
    if (ft_ret == &ffi_type_ushort) // unsigned short
        return malloc(sizeof(u_short));
    if (ft_ret == &ffi_type_sshort) // signed short
        return malloc(sizeof(short));
    if (ft_ret == &ffi_type_uchar) // unsigned char
        return malloc(sizeof(u_char));
    if (ft_ret == &ffi_type_schar) // signed char
        return malloc(sizeof(char));
    return malloc(sizeof(ffi_arg));
}

static void free_args(void **args, size_t nbr)
{
    for (Iterator i = 0; i < nbr; i++)
        free(args[i]);
    free(args);
}

static void rcl_ffi_C_callup(
    Stack *stack,
    struct rcl_ffi_C_attributes_t *ffi_attribs_ptr,
    struct State *state,
    const String fname,
    const String libpath)
{
    const size_t nbr = ffi_attribs_ptr->properties.nargs;
    rcl_assert(stack->used >= nbr);
    void *_return = allocate_return(ffi_attribs_ptr->properties.tret);
    void **args = nbr > 0 ? malloc(nbr * sizeof *args) : NULL;
    for (Iterator i = 0; i < nbr; i++)
        args[i] = rcl_ffi_C_getarg(stack);
    ffi_call(&ffi_attribs_ptr->cif, ffi_attribs_ptr->properties.fn, _return, args);
    rcl_ffi_C_result_on_stack(stack, _return, ffi_attribs_ptr->properties.tret, state, fname, libpath);
    free_args(args, nbr);
    free(_return);
}

static void popx_psh0(Stack *stack, const size_t nbr)
{
    rcl_assert(stack->used >= nbr);
    stack->used -= nbr; //TODO! Apply `free`!
    push(stack, RCL_Word(RCL_NIL_WRD));
}

static void rcl_ffi_C_call(
    Stack *stack,
    const String fname,
    const String libpath,
    const struct rcl_ffi_C_attributes_t *ffi_attribs_ptr,
    struct State *state)
{
    if (!ffi_attribs_ptr->cif_already_initialized)
        rcl_ffi_C_initialize_cif(ffi_attribs_ptr);

    if (!ffi_attribs_ptr->properties.usable)
    {
        state_put_err_it_cst(state, "Unusable foreign function call of `%s::%s'.", libpath, fname);
        popx_psh0(stack, ffi_attribs_ptr->properties.nargs);
        return;
    }

    if (ffi_attribs_ptr->status == FFI_OK)
        return rcl_ffi_C_callup(stack, ffi_attribs_ptr, state, fname, libpath);

    state_put_err_it_cst(
        state, "Unable to invoke the function `%s' from the loaded library `%s'.\n       [FFI error code: %d].",
        fname, libpath, ffi_attribs_ptr->status);
}

void eval_external_call(Stack *stack, BResult *bresult, struct RCL_Extern *external)
{
    /*     struct rcl_ffi_C_attributes_t ffi_attribs =
        new_rcl_ffi_C_attributes(external->name, external->dll, external->nargs, external->tret, external->targs, &bresult->state);
    rcl_ffi_C_call(stack, external->name, external->dll, &ffi_attribs, &bresult->state); */

    rcl_ffi_C_call(stack, external->fun.fname, external->dll, &external->fun.attribs, &bresult->state);
}

void eval_C_call(Stack *stack, BResult *bresult, const struct rcl_ffi_C_dynflib_t dynflib)
{
    rcl_ffi_C_call(stack, dynflib.dynf->fname, *dynflib.libpath, &dynflib.dynf->attribs, &bresult->state);
}
