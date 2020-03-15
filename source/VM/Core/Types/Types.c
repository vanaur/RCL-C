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

#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\cmpvalue.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\State\State.h>

#include <VM\Core\Types\Show.h>
#include <VM\Core\Types\Combinator.h>
#include <VM\Core\Types\Primitives.h>
#include <VM\Core\Types\Types.h>
#include <VM\Core\Types\TypeError.h>
#include <VM\Core\Types\Infer.h>

RCL_Type make_RCL_Type_stack(size_t n, RCL_Type types[n])
{
    if (n == 0)
        return T_STACK(&T_EMPTY);
    RCL_Type **result = (RCL_Type **)malloc(n * sizeof(RCL_Type *));
    for (Iterator i = 0; i < n; i++)
        result[i] = &types[i];
    return SIGMA_FILL_CTOR(RCL_Type, TYPE_STACK, rcl_type_stack, result, n);
}

#define _sk_vof_array__byref(v, i) show_kind(v->u.array_.array->array[i].kind)

static RCL_Type type_of_array(const Value *value, String function_name, BResult *bresult)
{
    size_t len = value->u.array_.length;
    RCL_Type ref_type = type_of(&value->u.array_.array->array[0], function_name, bresult);
    while (len--)
    {
        const RCL_Type current_type = type_of(&value->u.array_.array->array[len], function_name, bresult);
        if (memcmp(&current_type, &ref_type, sizeof(RCL_Type)))
        {
            PUT_TYPE_ERROR__ARRAY;
            return T_ERR;
        }
    }
    return SIGMA_FILL_CTOR(RCL_Type, TYPE_ARRAY, rcl_type_array, &ref_type);
}

// Obtenir l'arité d'une concaténation (d'un RawCode)
// Type de la concaténation (RawCode)
// Fonction de substitution + d'unification
// Typechecking au même moment que la compilation, afin d'obtenir toutes les informations
//      dont nous avons besoin, dont l'arité et la taille des trucs, enfin on sais.
// Unify <=> type matchs... ? Num <=> Int   a <=> Int   ...

RCL_Type type_of_rcode(RawCode rcode)
{
}

RCL_Type type_of(const Value *value, const String function_name, const BResult *bresult)
{
    switch (value->kind)
    {
    case RCL_Value_Array:
        return type_of_array(value, function_name, bresult);

    case RCL_Value_Integer ... RCL_Value_String:
        return T_LITERAL(value->kind);

    case RCL_Value_Word:
        break;

    case RCL_Value_Lambda:
        // TODO: Add to environment
        return T_ARROW(T_ANY('a'), T_EMPTY);

    case RCL_Value_EndLamScope:
        return T_EMPTY;

    case RCL_Value_LiteralOperation:
        return primitive_types[value->u.litOperation_->kind];

    case RCL_Value_Combinator:
        return combinator_types[(short)value->u.comb_];

    case RCL_Value_Parallel:
        break;

    case RCL_Value_Field:
        break;

    case RCL_Value_DataStruct:
        break;

    case RCL_Value_Replicated: // Todo
        return type_of(value->u.repeat_.op, function_name, bresult);

    case RCL_Value_Empty:
        return T_EMPTY;

    case RCL_Value_All:
        return T_ANY('a');

    case RCL_Value_Quotation:
        break;

    default:
        printf("%d\n", value->kind);
        printf("TODO types: %s:%d\n", __FILE__, __LINE__);
    }
}

bool cmp_types(const RCL_Type t1, const RCL_Type t2)
{
    if (t1.kind != t2.kind)
        return false;

    switch (t1.kind)
    {
    case TYPE_ANY:
        return (SIGMA_GETV_BYVAL(t1, rcl_type_any, tany) == SIGMA_GETV_BYVAL(t2, rcl_type_any, tany));

    case TYPE_LITERAL:
        return (SIGMA_GETV_BYVAL(t1, rcl_type_literal, tlit) == SIGMA_GETV_BYVAL(t2, rcl_type_literal, tlit)) // t1 == t2
               || (SIGMA_GETV_BYVAL(t1, rcl_type_literal, tlit) == RCL_Value_Number                           // t1 == Num
                       && SIGMA_GETV_BYVAL(t2, rcl_type_literal, tlit) == RCL_Value_Integer                   // t2 == Int
                   || SIGMA_GETV_BYVAL(t2, rcl_type_literal, tlit) == RCL_Value_Float)                        // t2 == Float
               || (SIGMA_GETV_BYVAL(t2, rcl_type_literal, tlit) == RCL_Value_Number                           // t2 == Num
                       && SIGMA_GETV_BYVAL(t1, rcl_type_literal, tlit) == RCL_Value_Integer                   // t1 == Int
                   || SIGMA_GETV_BYVAL(t1, rcl_type_literal, tlit) == RCL_Value_Float);                       // t1 == FLoat

    case TYPE_EMPTY:
        return t2.kind == TYPE_EMPTY;

    case TYPE_ARRAY:
        return cmp_types(
            *SIGMA_GETV_BYVAL(t1, rcl_type_array, tarray),
            *SIGMA_GETV_BYVAL(t2, rcl_type_array, tarray));

    case TYPE_QUOTE:
        return cmp_types(
            *SIGMA_GETV_BYVAL(t1, rcl_type_quote, tquote),
            *SIGMA_GETV_BYVAL(t2, rcl_type_quote, tquote));

    case TYPE_ARROW:
        return cmp_types(*SIGMA_GETV_BYVAL(t1, rcl_type_arrow, t1), *SIGMA_GETV_BYVAL(t2, rcl_type_arrow, t1)) //
               && cmp_types(*SIGMA_GETV_BYVAL(t1, rcl_type_arrow, t2), *SIGMA_GETV_BYVAL(t2, rcl_type_arrow, t2));

    case TYPE_STACK:
        for (Iterator i = 0; i < t1.u.rcl_type_stack_.nbr; i++)
            if (!cmp_types(*t1.u.rcl_type_stack_.tstack[i], *t1.u.rcl_type_stack_.tstack[i]))
                return false;
        return true;

    case TYPE_VARIABLE: // Replace String by hash
        return (!strcmp(SIGMA_GETV_BYVAL(t1, rcl_type_variable, name), SIGMA_GETV_BYVAL(t2, rcl_type_variable, name)));
    
    case TYPE_ERROR:
        return false;
    }

    // May no reach
    return false;
}

size_t arity(RCL_Type t)
{
    if (t.kind == TYPE_STACK)
        return SIGMA_GETV_BYVAL(t, rcl_type_stack, nbr);

    if (t.kind == TYPE_ARROW)
    {
        size_t a = arity(*t.u.rcl_type_arrow_.t1);
        if (t.u.rcl_type_arrow_.t2->kind == TYPE_ARROW)
            return a + arity(*t.u.rcl_type_arrow_.t2->u.rcl_type_arrow_.t1);
        return a;
    }
    return 0;
}

size_t trace(RCL_Type t)
{
    if (t.kind == TYPE_STACK)
        return SIGMA_GETV_BYVAL(t, rcl_type_stack, nbr);

    if (t.kind == TYPE_ARROW)
        return arity(*t.u.rcl_type_arrow_.t2);

    return 0;
}
