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
#include <Tools\Utils\Utils.h>
#include <Tools\Internal\Internal.h>
#include <VM\Core\Types\Combinator.h>
#include <VM\Core\Types\Primitives.h>
#include <VM\Core\Types\Types.h>
#include <VM\Core\Types\Show.h>

static String show_type_value(RCL_Type type)
{
    switch (type.u.rcl_type_literal_.tlit)
    {
    case RCL_Value_Integer:
        return "Int";

    case RCL_Value_Number:
        return "Num";

    case RCL_Value_Float:
        return "Float";

    case RCL_Value_Char:
        return "Char";

    case RCL_Value_String:
        return "String";

    case RCL_Value_Qual:
        // show function type
        break;

    case RCL_Value_EndLamScope:
        return "Void";

    default:
        return "<?>";
    }
}

static String show_type_array(RCL_Type type)
{
    String result;
    rcl_asprintf(&result, "{%s}", show_type(*SIGMA_GETV_BYVAL(type, rcl_type_array, tarray)));
    return result;
}

static String show_type_quote(RCL_Type type)
{
    String result;
    rcl_asprintf(&result, "[%s]", show_type(*SIGMA_GETV_BYVAL(type, rcl_type_quote, tquote)));
    return result;
}

static String show_type_stack(RCL_Type type)
{
    if (type.u.rcl_type_stack_.nbr == 0)
        return "()"; // Empty stack

    String result = show_type(*type.u.rcl_type_stack_.tstack[0]);
    for (Iterator i = 1; i < type.u.rcl_type_stack_.nbr; i++)
    {
        const RCL_Type current = *SIGMA_GETV_BYVAL(type, rcl_type_stack, tstack)[i];
        rcl_asprintf(&result, "%s . %s", result, show_type(current));
    }
    return result;
}

static String show_arrow_type(const RCL_Type t)
{
    String result = "";
    if (t.u.rcl_type_arrow_.t2->kind == TYPE_ARROW)
        rcl_asprintf(
            &result, "%s -> (%s)",
            show_type(*SIGMA_GETV_BYVAL(t, rcl_type_arrow, t1)),
            show_type(*SIGMA_GETV_BYVAL(t, rcl_type_arrow, t2)));
    else
        rcl_asprintf(
            &result, "(%s -> %s)",
            show_type(*SIGMA_GETV_BYVAL(t, rcl_type_arrow, t1)),
            show_type(*SIGMA_GETV_BYVAL(t, rcl_type_arrow, t2)));
    return result;
}

String show_type(const RCL_Type type)
{
    switch (type.kind)
    {
    case TYPE_LITERAL:
        return show_type_value(type);

    case TYPE_ARRAY:
        return show_type_array(type);

    case TYPE_QUOTE:
        return show_type_quote(type);

    case TYPE_STACK:
        return show_type_stack(type);

    case TYPE_ARROW:
        return show_arrow_type(type);

    case TYPE_ANY:
        //return (char[2]){type.u.rcl_type_any_.tany, '\0'};
        {
            return rcl_sprintf_s("%c", type.u.rcl_type_any_.tany);
        }

    case TYPE_SVTA:
        return type.u.rcl_type_svta_.svta.visualize;

    case TYPE_VARIABLE:
        return SIGMA_GETV_BYVAL(type, rcl_type_variable, name);

    case TYPE_EMPTY:
        return "()";

    case TYPE_ERROR:
        return "#error#";

    default:
        _interr("Unknown type kind: %d.", type.kind);
        return "?";
    }
}
