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

#include "REPL\REPL.h"
#include "RCL.h"
#include "Arguments.h"

#include <VM\Core\Browse\Browse.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Exec.h>
#include <VM\Core\Argv.h>
#include <VM\Core\Process\Process.h>
#include <VM\Core\Execution\Compiler\Assembler\Assemble.h>
#include <VM\Core\Execution\Compiler\Assembler\Show.h>
#include <VM\Core\Execution\Interpreter\Interpreter.h>
#include <VM\Core\Execution\JIT\RCL_JIT.h>
#include <VM\Core\Checker\Checker.h>
#include <VM\Core\Show\Show.h>
#include <Tools\Time_Measure\Time_Measure.h>

void atex()
{
    printf("\n----\n\n");
}

void clean_heap(BResult *bresult, const Exec exec)
{
    if (bresult->psdata.rcode.size > 0)
        free(bresult->psdata.rcode.array);
    if (bresult->psdata.imports.size > 0)
        free(bresult->psdata.imports.array);
    if (bresult->psdata.includes.size > 0)
        free(bresult->psdata.includes.array);
    if (bresult->argvs.u.interpreted.args.used > 0)
        free(bresult->argvs.u.interpreted.args.array);
    if (bresult->resulting.size > 0 && exec.ioc != Compiled)
        free(bresult->resulting.array);
    if (bresult->state.size > 0)
        free(bresult->state.array);
    if (bresult->wordico.functions.size > 0)
        free(bresult->wordico.functions.array);
    if (bresult->wordico.lambdas.size > 0)
        free(bresult->wordico.lambdas.array);
    if (bresult->wordico.structures.size > 0)
        free(bresult->wordico.structures.array);
    if (bresult->argvs.u.interpreted.args.size > 0)
        free(bresult->argvs.u.interpreted.args.array);
}

#define _display_info(x...)                    \
    {                                          \
        printf("\n [");                        \
        cc_fprintf(CC_FG_YELLOW, stdout, "i"); \
        printf("] ");                          \
        printf(x);                             \
    }
#define _display_newline printf("\n");

void make_noise(
    const BResult bresult,
    const Exec exec,
    const String files[],
    const size_t argc,
    const long double all_time,     // Time of all RCL
    const long double eval_time,    // Time of evaluation
    const long double process_time) // Time of processing program, without evaluation
{
    printf("\nInformation:");

    if (exec.ioc != Compiled)
    {
        if (exec.show_res)
            _display_info("Resulting stack: %s", show_stack(*((Stack *)&bresult.resulting))) else _display_newline;
    }
    else
        _display_newline;

    if (argc > 1)
    {
        _display_info("Programs composed: %I64d", argc);
        _display_info("Program composition:");

        for (size_t i = 0; i <= argc - 1; i++)
        {
            if (startsWith(files[i], "--"))
                continue;
            printf(" %s o", files[i]);
        }

        printf("\b ");
    }

    _display_info("Initial operations number: %I64d.", bresult.psdata.rcode.used);
    _display_info("Non-native words: %I64d.\n", /* bresult.wordico.externs.used + */ bresult.wordico.functions.used + bresult.wordico.structures.used);
    printf("     > %I64d functions.\n\
     > %I64d structures.\n\
     > %I64d external functions.\n",
           bresult.wordico.functions.used, bresult.wordico.structures.used, count_cffi_externs(&bresult.psdata.cffi_map));
    _display_info("Included DLLs: ....................... %I64d.", bresult.psdata.includes.used);
    _display_info("Imported files: ...................... %I64d.", bresult.psdata.imports.used);
    _display_info("Recursion optimization: .............. %s.", bresult.exec_infos.optimize_rec == true ? "yes" : "no");
    _display_info("Optimization level: .................. Opt-%d.", bresult.exec_infos.optimize_level);
    if (exec.ioc == Interpreted)
        _display_info("Usage of program parameters: ......... %s.", hasArgv(bresult));
    _display_info("Execution mode: ...................... %s.", exec.ioc == 0 ? "interpreted" : "compiled");
    _display_info("Evaluation mode: ..................... %s.", exec.sool == 0 ? "strict" : exec.sool == 1 ? "optimistic" : "lazy");
    _display_info("Time taken by all RCL: ............... %s.", show_time(all_time));
    _display_info("Browing, checking, optimizing time: .. %s.", show_time(process_time));
    if (exec.ioc == Interpreted)
        _display_info("Execution time of the program: ....... %s.", show_time(eval_time));
    printf("\n\nSuccessful end of program \\o/\n\n");
}

int noarg()
{
    printf("\nRCL: ");
    cc_fprintf(CC_FG_DARK_YELLOW, stdout, "There is no file to process.\n");
    cc_fprintf(CC_FG_DARK_GREEN, stdout, "Type ");
    cc_fprintf(CC_FG_DARK_CYAN, stdout, "-help");
    cc_fprintf(CC_FG_DARK_GREEN, stdout, " for help, or go to ");
    cc_fprintf(CC_FG_BLUE, stdout, "%s\n\n", RCL_GITHUB_LINK);
    return 0;
}

/*** Entry point ***/
int main(int argc, String argv[])
{
    /*     const rcl_fun_params_t f1params = rcl_fun_params(2, (const String[]){"x", "y"});
    const rcl_fun_t fn1 = rcl_fun_define("addition", f1params, rcl_expr__add(rcl_expr__var("x"), rcl_expr__var("y")));

    const rcl_fun_args_t f1args = rcl_fun_args(2, (const rcl_expr_t[]){rcl_expr__cst_int(6), rcl_expr__cst_int(21)});
    const rcl_fun_t fn2 = rcl_fun_define("example", rcl_fun_params_0, rcl_expr__mul(rcl_fun_call("addition", f1args), rcl_expr__cst_int(2)));
 */
    // If the RCL is launch without argument
    if (argc == 1)
        return noarg();

    // Remove the program name from `argv`
    remove_element(argv, argc--, 0);
    Exec exec = get_exec(argv, &argc);

    if (exec.repl && argc == 0)
    {
        BResult tmp = new_bresult();
        run_repl(&tmp);
        return 0;
    }

    // Don't display anything if this is the flag 'noise'
    if (exec.noise_level == Noise)
    {
        atexit(atex);
        putchar('\n');
        PRINT_RCL_TEXT;
    }

    // If there are any lambda arguments, get them
    struct RCL_Argv rcl_argv = get_lamargs(argv, &argc);
    rcl_argv.ioc = exec.ioc;

    // Set timers
    struct timespec start1, end1, start2, end2, start3, end3;
    long double all_time, eval_time, process_time;

    String files[argc];
    *files = *argv;

    gettime(&start1);
    gettime(&start3);

    // Process the given file(s) by building the IR
    BResult bresult = processFiles(files, argc, exec);
    bresult.argvs = rcl_argv;
    bresult.exec_infos = exec;

    /*     foo(&bresult);
    prettyPrint_state(bresult.state);
    return 0; */

    Checked_out checked = check_program(&bresult);

    if (!checked.able)
    {
        prettyPrint_state(bresult.state);
        cc_fprintf(CC_FG_WHITE, stdout, "Can't evaluate the program due to error(s) :(\n\n");
        return EXIT_FAILURE;
    }

    gettime(&end3);
    gettime(&start2);

    // Execute or compile the program

    if (exec.ioc == Interpreted)
    {
        interpret(&bresult, exec.sool);
    }
    else if (exec.ioc == Compiled)
    {
        Assembled_Program assembled_asm = assemble(&bresult);
        String assembled_asm_str = show_assembled_program(assembled_asm, bresult);
        if (exec.kasm)
        {
            FILE *file = fopen("output_asm.asm", "w");
            fputs(((const char *)(assembled_asm_str)), file);
            fclose(file);
        }
        printf("\n%s\n", assembled_asm_str);
    }
    else if (exec.ioc == Jited)
    {
        jit(&bresult);
    }

    gettime(&end2);
    gettime(&end1);

    all_time = timesdiff(start1, end1);
    eval_time = timesdiff(start2, end2);
    process_time = timesdiff(start3, end3);

    // Display errors, warnigns and infos
    prettyPrint_state(bresult.state);

    // If we want more informations about execution or compilation
    if (exec.noise_level == Noise && exec.ioc == Interpreted)
        make_noise(bresult, exec, files, argc, all_time, eval_time, process_time);
    /*     else
    {
        printf("\n[i] All time taken: %s.\n", show_time(all_time));
        printf("[i] Compilation time taken: %s.\n", show_time(process_time));
    } */

    if (bresult.exec_infos.repl)
        run_repl(&bresult);

    // Free the allocated memory areas
    clean_heap(&bresult, exec);

    return EXIT_SUCCESS;
}
