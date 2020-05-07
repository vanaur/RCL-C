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

#include <ffi.h>
#include <stdlib.h>
#include <stdio.h>

#include <VM\Core\FFI\C\ctypes.h>
#include <VM\Core\Syntax\Absyn.h>
#include <Tools\Internal\Internal.h>

ffi_type *ffi_ctype_to_real_ctype_primitive(FFI_Type_Signature rawty)
{
    switch (rawty->u.tprimitive_.ffi_type_signaturet_->kind)
    {
    case is_TVoid:
        return &ffi_type_void;

    case is_TUint8:
        return &ffi_type_uint8;

    case is_TSint8:
        return &ffi_type_sint8;

    case is_TUint16:
        return &ffi_type_uint16;

    case is_TSint16:
        return &ffi_type_sint16;

    case is_TUint32:
        return &ffi_type_uint32;

    case is_TSint32:
        return &ffi_type_sint32;

    case is_TUint64:
        return &ffi_type_uint64;

    case is_TSint64:
        return &ffi_type_sint64;

    case is_TFloat:
        return &ffi_type_float;

    case is_TDouble:
        return &ffi_type_double;

    case is_TUchar:
        return &ffi_type_uchar;

    case is_TSchar:
        return &ffi_type_schar;

    case is_TUshort:
        return &ffi_type_ushort;

    case is_TSshort:
        return &ffi_type_sshort;

    case is_TUint:
        return &ffi_type_uint;

    case is_TSint:
        return &ffi_type_sint;

    case is_TUlong:
        return &ffi_type_ulong;

    case is_TSlong:
        return &ffi_type_slong;

    case is_TLongDouble:
        return &ffi_type_longdouble;

    case is_TAny:
        return &ffi_type_pointer;

    case is_Tint:
        return &ffi_type_sint;
    }
}

static ffi_type *ffi_ctype_struct_to_real_ctype_struct(FFI_Type_Signature rawty)
{
    const ListFFI_Type_Signature ts = rawty->u.tstruct_.listffi_type_signature_;
    const size_t fnbr = count_ffi_types(ts);
    ffi_type *result = malloc(sizeof *result);
    ffi_type **fields = malloc((fnbr + 1) * sizeof *fields);
    result->type = FFI_TYPE_STRUCT;
    result->size = result->alignment = 0;
    result->elements = fields;
    ffi_ctype_to_real_ctypes(fields, ts);
    fields[fnbr] = NULL;
    return result;
}

ffi_type *ffi_ctype_to_real_ctype(const FFI_Type_Signature rawty)
{
    switch (rawty->kind)
    {
    case is_TPrimitive:
        return ffi_ctype_to_real_ctype_primitive(rawty);

    case is_TPtr:
        return &ffi_type_pointer;

    case is_TStruct:
        return ffi_ctype_struct_to_real_ctype_struct(rawty);

    default:
        _interr("Can't handle the FFI(\"C\") type of %d.", rawty->kind);
    }
}

void ffi_ctype_to_real_ctypes(ffi_type **result, const ListFFI_Type_Signature rawts)
{
    ListFFI_Type_Signature current = rawts;
    size_t i = 0;
    while (current != NULL)
    {
        result[i++] = ffi_ctype_to_real_ctype(current->ffi_type_signature_);
        current = current->listffi_type_signature_;
    }
}

static bool is_void(const ListFFI_Type_Signature type)
{
    if (type->ffi_type_signature_->kind == is_TPrimitive)
        if (type->ffi_type_signature_->u.tprimitive_.ffi_type_signaturet_->kind == is_TVoid)
            return true;
    return false;
}

size_t count_ffi_types(const ListFFI_Type_Signature ls)
{
    ListFFI_Type_Signature current = ls;
    size_t result = 0;
    while (current != NULL)
    {
        if (!is_void(current))
            result += 1;
        current = current->listffi_type_signature_;
    }
    return result;
}
