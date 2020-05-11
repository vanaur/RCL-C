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

#include "CmdParser.h"

#include <Tools\Utils\Utils.h>
#include <Tools\console-color\console-color.h>
#include <string.h>

const String commands[] =
    {"stack", "free", "quit", "help", "typeof",
     "step", "exec", "exect", "show", "oof",
     "funs", "externs", "structs",
     "opt", "set"};

#define NBR_CMDS (sizeof(commands) / sizeof(commands[0]))

static bool askarg(const String s)
{
    return (!strcmp(s, "typeof"))   //
           || (!strcmp(s, "step"))  //
           || (!strcmp(s, "exec"))  //
           || (!strcmp(s, "exect")) //
           || (!strcmp(s, "show"))  //
           || (!strcmp(s, "oof"))   //
           || (!strcmp(s, "opt"))   //
           || (!strcmp(s, "set"));  //
}

void help_if_unknown(const String str)
{
    const String cmd = strtok(trim(strdup(str)), " ");
    const size_t len = strlen(cmd);

    String rest = strdup(str);
    int l = len + 1;
    while (l--)
        rest++;

    int levens[NBR_CMDS];
    for (Iterator i = 0; i < NBR_CMDS; i++)
        levens[i] = levenshtein(cmd, len, commands[i], strlen(commands[i]));

    for (Iterator i = 0; i < NBR_CMDS; i++)
    {
        if (!strcmp(cmd, commands[i]))
        {
            if (askarg(cmd))
                cc_fprintf(CC_FG_YELLOW, stdout, "Lacking arguments for `%s`.\n", commands[i]);
            else
                cc_fprintf(CC_FG_YELLOW, stdout, "Unexpected arguments for `%s`.\n", commands[i]);
            return;
        }
    }

    for (Iterator i = 0; i < NBR_CMDS; i++)
    {
        if (levens[i] == 1 || levens[i] == 2)
        {
            cc_fprintf(CC_FG_YELLOW, stdout, "Try: ");
            cc_fprintf(CC_FG_CYAN, stdout, "%s %s\n", commands[i], rest);
        }
    }
}

REPL_AST repl_cmd_parse(const String cmd)
{
    String trimed_cpy = trim(cmd);

    if (!strcmp(trimed_cpy, "stack"))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_STACK, repl_stack);
    if (!strcmp(trimed_cpy, "funs") || !strcmp(trimed_cpy, "fs"))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_FUNS, repl_funs);
    if (!strcmp(trimed_cpy, "externs") || !strcmp(trimed_cpy, "es"))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_EXTERNS, repl_externs);
    if (!strcmp(trimed_cpy, "structs") || !strcmp(trimed_cpy, "ss"))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_STRUCTS, repl_structs);
    if (!strcmp(trimed_cpy, "free") || !strcmp(trimed_cpy, "f"))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_FREE, repl_free);
    if (!strcmp(trimed_cpy, "quit") || !strcmp(trimed_cpy, "q"))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_QUIT, repl_quit);
    if (!strcmp(trimed_cpy, "help") || !strcmp(trimed_cpy, "?"))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_HELP, repl_help);
    if (startsWith(trimed_cpy, "typeof ") || !strcmp(trimed_cpy, "t "))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_TYPEOF, repl_typeof, trim(trimed_cpy += 7));
    if (startsWith(trimed_cpy, "step ") || !strcmp(trimed_cpy, "st "))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_STEP, repl_step, trim(trimed_cpy += 5));
    if (startsWith(trimed_cpy, "exec "))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_EXEC, repl_exec, trim(trimed_cpy += 5));
    if (startsWith(trimed_cpy, "exect "))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_EXECT, repl_exect, trim(trimed_cpy += 6));
    if (startsWith(trimed_cpy, "show "))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_SHOW, repl_show, trim(trimed_cpy += 5));
    if (startsWith(trimed_cpy, "oof "))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_OOF, repl_oof, trim(trimed_cpy += 4));
    if (startsWith(trimed_cpy, "opt "))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_OPT, repl_opt, trim(trimed_cpy += 4));
    if (startsWith(trimed_cpy, "set "))
        return SIGMA_FILL_CTOR(REPL_AST, REPL_SET, repl_set, trim(trimed_cpy += 4));

    help_if_unknown(cmd);

    return SIGMA_FILL_CTOR(REPL_AST, CMDERR, cmderr, rcl_sprintf_s("Unknown command: `%s'.", trimed_cpy));
}
