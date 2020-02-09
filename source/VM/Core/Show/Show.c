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

static String show_array(RCL_Value_Array_t array)
{
    String result;
    if (array.length > 30)
    {
        rcl_asprintf(&result, "'Array{of %lld %ss}", array.length, showKind(array.array->array[0].kind));
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
    String result;
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
    case RCL_Value_Word:
    {
        if (!strcmp(value.u.word_.word_str, RCL_NIL_WRD))
            return "<null>";
        else
            rcl_asprintf(&result, "%s", value.u.word_.word_str);
        break;
    }
    case RCL_Value_Quotation:
    {
        rcl_asprintf(&result, "%s", showQuote(*value.u.quote_));
        break;
    }
    case RCL_Value_EndLamScope:
    {
        rcl_asprintf(&result, "%s$", value.u.endLamScope_);
        break;
    }
    case RCL_Value_Lambda:
    {
        rcl_asprintf(&result, "%s\\", value.u.lam_);
        break;
    }
    case RCL_Value_Combinator:
    {
        rcl_asprintf(&result, "%s", showComb(value.u.comb_));
        break;
    }
    case RCL_Value_Empty:
        return ""; // 'empty-stack
    case RCL_Value_LiteralOperation:
    {
        rcl_asprintf(&result, showLo(value.u.litOperation_));
        break;
    }
    case RCL_Value_Array:
        return show_array(value.u.array_);

    case RCL_Value_DataStruct:
    {
        rcl_asprintf(&result, "'Structure<:%s>", value.u.dataStruct_.template->name);
        break;
    }
    case RCL_Value_Field:
    {
        rcl_asprintf(&result, ".<>");
        break;
    }
    case RCL_Value_Parallel:
    {
        rcl_asprintf(&result, "(%s # %s)", show_value(*value.u.parallel_.v1), show_value(*value.u.parallel_.v2));
        break;
    }
    case RCL_Value_Concatenation:
        return show_concatenation(value.u.concatenation_);

    default:
        return "'Unknown";
    }
    return result;
}

String showComb(Combinator comb)
{
    return (String)Combinators[(int)comb];
}

String showLo(LiteralOperation lo)
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

String showStack(Stack stack)
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

String showStack_nobraces(Stack stack)
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

String showQuote(RawCode rcode)
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