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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include <Tools\Utils\Utils.h>
#include <Tools\console-color\console-color.h>
#include <VM\Core\State\State.h>

struct StateKind make_error(enum Origin origin, const String msg)
{
    return (struct StateKind){.kind = Error, .u = {.error = {.msg = msg}}};
}

struct StateKind make_warning(enum Origin origin, const String msg)
{
    return (struct StateKind){.kind = Warning, .u = {.warning = {.msg = msg}}};
}

struct StateKind make_info(enum Origin origin, const String msg)
{
    return (struct StateKind){.kind = Info, .u = {.info = {.msg = msg}}};
}

void state_init(struct State *restrict state)
{
    state->hasError = false;
    InitVector(state, 1, struct StateKind);
}

void state_put(struct State *restrict state, struct StateKind statekind)
{
    if (statekind.kind == Error)
        state->hasError = true;
    PushToVector(state, struct StateKind, statekind);
}

size_t count_errors(const struct State state)
{
    size_t result = 0;
    for (Iterator i = 0; i < state.used; i++)
        if (state.array[i].kind == Error)
            result++;
    return result;
}

size_t count_warnigns(const struct State state)
{
    size_t result = 0;
    for (Iterator i = 0; i < state.used; i++)
        if (state.array[i].kind == Warning)
            result++;
    return result;
}

size_t count_infos(const struct State state)
{
    size_t result = 0;
    for (Iterator i = 0; i < state.used; i++)
        if (state.array[i].kind == Info)
            result++;
    return result;
}

void prettyPrint_state(struct State state)
{
    for (Iterator i = 0; i < state.used; i++)
    {
        if (i >= 25 && i > 25)
        {
            i = state.used;
            cc_fprintf(
                CC_FG_MAGENTA,
                stdout, "\n... %d errors in all (+ %d warnings and %d information)\n",
                count_errors(state), count_warnigns(state), count_infos(state));
            return;
        }
        switch (state.array[i].kind)
        {
        case Error:
            cc_fprintf(CC_FG_RED, stdout, "    Error: ");
            cc_fprintf(CC_FG_DARK_RED, stdout, "%s\n", state.array[i].u.error.msg);
            break;

        case Warning:
            cc_fprintf(CC_FG_YELLOW, stdout, "    Warning: ");
            cc_fprintf(CC_FG_DARK_YELLOW, stdout, "%s\n", state.array[i].u.warning.msg);
            break;

        case Info:
            cc_fprintf(CC_FG_CYAN, stdout, "    Info: ");
            cc_fprintf(CC_FG_DARK_CYAN, stdout, "%s\n", state.array[i].u.info.msg);
            break;
        }
    }
}
