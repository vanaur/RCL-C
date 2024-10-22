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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#include <VM\Core\Syntax\Absyn.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\Show\Show.h>

static String show_structure_whole(const struct RCL_Value_DataStruct rcl_struct);

static String show_array(RCL_Value_Array_t array)
{
    String result;
    if (array.length > 30)
    {
        rcl_asprintf(&result, "'Array{of %lld %ss}", array.length, show_kind(array.array->array[0].kind));
    }
    else
    {
        rcl_asprintf(&result, "{ %s", show_value(array.array->array[0]));
        for (Iterator i = 1; i < array.length; i++)
            rcl_asprintf(&result, "%s, %s", result, show_value(array.array->array[i]));
        rcl_asprintf(&result, "%s }", result);
    }
    return result;
}

static String show_concatenation(RCL_Value_Concatenation_t concatenation)
{
    String result;
    rcl_asprintf(&result, "%s", show_value(concatenation->array[0]));
    for (Iterator i = 1; i < concatenation->used; i++)
        rcl_asprintf(&result, "%s %s", result, show_value(concatenation->array[i]));
    return result;
}

String show_value(Value value)
{
    String result = "";
    switch (value.kind)
    {
    case RCL_Value_Integer:
    {
        result = mpz_get_str(NULL, 10, value.u.int_);
        break;
    }
    case RCL_Value_Float:
    {
        gmp_asprintf(&result, "%.Fg", value.u.float_);
        break;
    }
    case RCL_Value_Char:
    {
        rcl_asprintf(&result, "'%c'", value.u.char_);
        break;
    }
    case RCL_Value_String:
    {
        rcl_asprintf(&result, "\"%s\"", value.u.string_);
        break;
    }
    case RCL_Value_Qual:
    {
        result = show_qual(value.u.qual_);
        break;
    }
    case RCL_Value_Quotation:
    {
        rcl_asprintf(&result, "%s", show_quote(*value.u.quote_));
        break;
    }
    case RCL_Value_EndLamScope:
    {
        rcl_asprintf(&result, "%s$", value.u.endLamScope_.word_str);
        break;
    }
    case RCL_Value_Lambda:
    {
        rcl_asprintf(&result, "%s\\", value.u.lam_.word_str);
        break;
    }
    case RCL_Value_Combinator:
    {
        rcl_asprintf(&result, "%s", show_comb(value.u.comb_));
        break;
    }
    case RCL_Value_Empty:
        return ""; // 'empty-stack
    case RCL_Value_LiteralOperation:
    {
        rcl_asprintf(&result, show_arithop(value.u.litOperation_));
        break;
    }
    case RCL_Value_Array:
        return show_array(value.u.array_);

    case RCL_Value_DataStruct:
        return show_structure_whole(value.u.dataStruct_);

    case RCL_Value_Field:
    {
        switch (value.u.field_->kind)
        {
        case _is_Enum:
            rcl_asprintf(&result, ".%s", value.u.field_->u.enumfield_.lident_);
            break;
        case _is_Field:
            rcl_asprintf(&result, ".%s", value.u.field_->u.freefield_.lident_);
            break;
        case _is_Spec:
            rcl_asprintf(&result, ".%s", value.u.field_->u.specfield_.lident_);
            break;

        default:
            rcl_asprintf(&result, ".<>");
            break;
        }
        break;
    }
    case RCL_Value_Parallel:
    {
        rcl_asprintf(&result, "(%s # %s)", show_value(*value.u.parallel_.v1), show_value(*value.u.parallel_.v2));
        break;
    }
    case RCL_Value_Concatenation:
        return show_concatenation(value.u.concatenation_);

    case RCL_Value_Replicated:
        rcl_asprintf(&result, "~%d %s", value.u.repeat_.times, show_value(*value.u.repeat_.op));
        break;

    default:
        return "'Unknown";
    }
    return result;
}

String show_comb(Combinator comb)
{
    return (String)Combinators[(int)comb];
}

String show_arithop(LiteralOperation lo)
{
    switch (lo->kind)
    {
    case is_Add:
        return "+";
    case is_Sub:
        return "-";
    case is_Pow:
        return "**";
    case is_Mul:
        return "*";
    case is_Div:
        return "/";
    case is_Mod:
        return "%%";
    case is_Inc:
        return "++";
    case is_Dec:
        return "--";
    case is_BAnd:
        return "&";
    case is_BOr:
        return "|";
    case is_BXor:
        return "^";
    case is_BLeft:
        return "<<";
    case is_BRight:
        return ">>";
    }
}

String show_stack(Stack stack)
{
    String result;
    rcl_asprintf(&result, "[ ");
    for (Iterator i = 0; i < stack.used; i++)
    {
        if (stack.used >= 100 && i == 50)
        {
            i = stack.used - 1;
            rcl_asprintf(&result, "%s (...) ", result);
        }
        rcl_asprintf(&result, "%s%s ", result, show_value(stack.array[i]));
    }
    rcl_asprintf(&result, "%s]", result);
    return result;
}

String show_stack_nobraces(Stack stack)
{
    String result;
    rcl_asprintf(&result, "");
    for (Iterator i = 0; i < stack.used; i++)
    {
        if (stack.used >= 100 && i == 50)
        {
            i = stack.used - 1;
            rcl_asprintf(&result, "%s (...) ", result);
        }
        rcl_asprintf(&result, "%s%s ", result, show_value(stack.array[i]));
    }
    rcl_asprintf(&result, "%s", result);
    return result;
}

String show_rcode(RawCode rcode)
{
    if (rcode.used >= 50)
        return "(...)";
    if (rcode.used == 0)
        return "";
    String result;
    rcl_asprintf(&result, "(");
    for (Iterator i = 0; i < rcode.used; i++)
    {
        if (rcode.used >= 100 && i == 50)
        {
            i = rcode.used - 1;
            rcl_asprintf(&result, "%s (...) ", result);
        }
        rcl_asprintf(&result, "%s%s ", result, show_value(rcode.array[i]));
    }
    rcl_asprintf(&result, "%s\b)", result);
    return result;
}

String show_quote(RawCode rcode)
{
    if (rcode.used >= 50)
        return "[...]";
    if (rcode.used == 0)
        return "[]";
    String result;
    rcl_asprintf(&result, "[");
    for (Iterator i = 0; i < rcode.used; i++)
    {
        if (rcode.used >= 100 && i == 50)
        {
            i = rcode.used - 1;
            rcl_asprintf(&result, "%s (...) ", result);
        }
        rcl_asprintf(&result, "%s%s ", result, show_value(rcode.array[i]));
    }
    rcl_asprintf(&result, "%s\b]", result);
    return result;
}

static String show_indent(int n)
{
    String res = "";
    while (n--)
        rcl_asprintf(&res, "%s   ", res);
    return res;
}

static void indent(String *str, int n)
{
    rcl_asprintf(str, "%s%s", *str, show_indent(n));
}

static String show_structure(const struct RCL_Value_DataStruct, const int);

static void show_field(String *res_ptr, const struct RCL_Value_DataStruct_Field field, const String fs, const bool ln)
{
    rcl_asprintf(res_ptr, "%s  .%s:%s%s", *res_ptr, field.field_type_info->name, fs, ln ? "\n" : "");
}

static void show_fields(String *res_ptr, const struct RCL_Value_DataStruct_Field field, const int depth)
{
    if (field.field_value->kind == RCL_Value_DataStruct)
        return show_field(res_ptr, field, show_structure(field.field_value->u.dataStruct_, depth + 1), false);
    show_field(res_ptr, field, show_value(*field.field_value), true);
}

static String show_structure(const struct RCL_Value_DataStruct rcl_struct, const int depth)
{
    String res = rcl_sprintf_s("\n%s'Structure<:%s>\n", show_indent(depth), rcl_struct.template->name);
    const size_t fields_nbr = rcl_struct.template->field_alloc_used;

    for (Iterator i = 0; i < fields_nbr; i++)
    {
        if (rcl_struct.fields[i].field_value == NULL)
            return res;

        indent(&res, depth);
        show_fields(&res, rcl_struct.fields[i], depth);
    }
    return res;
}

String show_structure_whole(const struct RCL_Value_DataStruct rcl_struct)
{
    if (rcl_struct.fields == NULL)
        return rcl_sprintf_s("'RawStructure<:%s>", rcl_struct.template->name);
    String res = show_structure(rcl_struct, 2);
    rcl_asprintf(&res, "%s\n", res);
    return res;
}
