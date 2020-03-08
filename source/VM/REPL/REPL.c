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

#include <VM\REPL\REPL.h>
#include <VM\REPL\CmdParser.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\Checker\Typechecker\Types\Infer.h>
#include <VM\Core\Checker\Typechecker\Types\Show.h>
#include <VM\Core\Execution\Interpreter\Interpreter.h>
#include <VM\Core\Syntax\Parser.h>
#include <VM\Core\Browse\Browse.h>
#include <VM\Core\Arguments.h>
#include <VM\Core\Wordefinition\Wordico.h>
#include <Tools\Utils\Utils.h>
#include <Tools\Time_Measure\Time_Measure.h>

static const String repl_help =
"\
";

static RawCode parse_expr(const String str)
{
    FILE *tmp = fopen("repl_tmp", "w");
    fprintf(tmp, "%s", str);
    fclose(tmp);
    tmp = fopen("repl_tmp", "r");
    BResult b = browseAbsyn(pProgram(tmp), "REPL");
    fclose(tmp);
    remove("repl_tmp");
    return b.psdata.rcode;
}

static void handle_cmd(const REPL_AST cmd, BResult *bresult_ptr)
{
    switch (cmd.kind)
    {
    case REPL_STACK:
        cc_fprintf(CC_FG_WHITE, stdout, "%s\n", show_rcode(bresult_ptr->resulting));
        break;

    case REPL_TYPEOF:
    {
        RawCode exp = parse_expr(cmd.u.repl_typeof_.expr);
        cc_fprintf(CC_FG_WHITE, stdout, "%s", show_rcode(exp));
        printf(" : ");
        cc_fprintf(CC_FG_CYAN, stdout, "%s\n", show_type(infer_type(NULL, &exp, &bresult_ptr->state)));
        break;
    }

    case REPL_EXEC:
    {
        RawCode exp = parse_expr(cmd.u.repl_exec_.expr);
        RawCode res = seval(exp, bresult_ptr);
        if (res.used)
            cc_fprintf(CC_FG_WHITE, stdout, "%s\n", show_rcode(res));
        break;
    }

    case REPL_EXECT:
    {
        RawCode exp = parse_expr(cmd.u.repl_exec_.expr);
        struct timespec t1, t2;
        gettime(&t1);
        RawCode res = seval(exp, bresult_ptr);
        gettime(&t2);
        double time = timesdiff(t1, t2);
        if (res.used)
            cc_fprintf(CC_FG_WHITE, stdout, "%s\n", show_rcode(res));
        printf("[%s]\n", show_time(time));
        break;
    }

    case REPL_STEP:
    {
        RawCode exp = parse_expr(cmd.u.repl_exec_.expr);
        bresult_ptr->exec_infos.show_steps = true;
        RawCode res = seval(exp, bresult_ptr);
        if (res.used)
            cc_fprintf(CC_FG_WHITE, stdout, "%s\n", show_rcode(res));
        bresult_ptr->exec_infos.show_steps = false;
        break;
    }

    case REPL_SHOW:
    {
        struct RCL_Function *f_ptr = getSpecific_function(&bresult_ptr->wordico, hash_djb2(cmd.u.repl_show_.fname));
        if (f_ptr == NULL)
            cc_fprintf(CC_FG_RED, stdout, "Unknown function `%s'\n", cmd.u.repl_show_.fname);
        else
            cc_fprintf(CC_FG_WHITE, stdout, "%s = %s\n", f_ptr->name, show_rcode(f_ptr->body));
        break;
    }

    case REPL_SET:
    {
        int nargs = 1;
        Exec nex_ex = get_exec((String[]){cmd.u.repl_set_.option}, &nargs);
        bresult_ptr->exec_infos = nex_ex;
        break;
    }

    case REPL_QUIT:
        printf("Bye bye.\n");
        exit(EXIT_SUCCESS);
        break;

    case CMDERR:
        cc_fprintf(CC_FG_RED, stdout, "%s\n", cmd.u.cmderr_);
        break;

    default:
        cc_fprintf(CC_FG_DARK_YELLOW, stdout, "REPL KIND NOT EVALUABLE: %d.\n", cmd.kind);
        break;
    }
}

void run_repl(BResult *bresult_ptr)
{
    printf("--- REPL ---\n\n");
    while (true)
    {
        printf(": ");
        String cmd = getline();
        if (strcmp(trim(cmd), ""))
            handle_cmd(repl_cmd_parse(cmd), bresult_ptr);
    }
}
