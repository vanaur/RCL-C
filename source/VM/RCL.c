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
#include <time.h>
#include <stdint.h>

#include <VM\Core\State\State.h>
#include <VM\Core\Exec.h>
#include <VM\Core\Arguments.h>
#include <VM\Core\Argv.h>
#include <VM\Core\Process\Process.h>
#include <VM\Core\Argv.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\Checker\Checker.h>
#include <VM\Core\RCLIB\builtin.h>
#include <VM\Core\RCL.h>
#include <VM\Core\Execution\Interpreter\Interpreter.h>
#include <VM\Core\Execution\JIT\RCL_JIT.h>
#include <VM\Core\Execution\Compiler\Compiler.h>
#include <VM\Core\Execution\Compiler\Assembler\Assemble.h>
#include <VM\Core\Execution\Compiler\Assembler\Show.h>
#include <Tools\Map\Map.h>
#include <Tools\Utils\Utils.h>
#include <Tools\console-color\console-color.h>
#include <Tools\Time_Measure\Time_Measure.h>
#include <Library\Primitive\Primitive.h>
#include <Library\Primitive\Expression.h>
#include <Library\rclib.h>

void atex()
{
    printf("----\n\n");
}

int main(int argc, String argv[])
{
    atexit(atex);

/*     rcl__expr_t cst_n1 = rcl_expr__cst_int(10);
    rcl__expr_t cst_n2 = rcl_expr__cst_int(5);
    rcl__expr_t my_exp = rcl_expr__div(cst_n1, cst_n2);

    printf("%s\n", rcl__show_ir_expr(my_exp));
    struct IResult res = rcl__evali_expr(my_exp);
    printf("Resulting: %d\n", res.main_returned); */

    // If the RCL is launch without argument
    if (argc == 1)
        return noarg();

    //printf("%lld\n", hash_djb2("Math.rand"));

    // Remove the program name from `argv`
    remove_element(argv, argc--, 0);
    Exec exec = get_exec(argv, &argc);

    // if O3 then activate "all" the optimisations

    // Don't display anything if this is the flag 'noise'
    if (exec.noise_level == Noise)
    {
        putchar('\n');
        PRINT_RCL_TEXT;
    }

    // If there are any lambda arguments, get them
    struct RCL_Argv rcl_argv = get_lamargs(argv, &argc);
    rcl_argv.ioc = exec.ioc;

    // Initialize timers
    uint64_t start1, end1, start2, end2;

    String files[argc];
    *files = *argv;

    time_measure(&start1);

    // Process the given file(s) by building the IR
    BResult bresult = processFiles(files, argc, exec);
    bresult.argvs = rcl_argv;
    bresult.exec_infos = exec;

    // Check the program
    if (exec.type_check)
    {
        Checked_out checked = check_program(&bresult);

        if (checked.infos.used)
        {
            prettyPrint_state(checked.infos);
        }
        if (!checked.able)
        {
            if (exec.ioc == Interpreted)
                cc_fprintf(CC_FG_RED, stdout, "\n Can't interpret the program due to error(s).\n\n");
            else
                cc_fprintf(CC_FG_RED, stdout, "\n Can't compile the program due to error(s).\n\n");
            return EXIT_FAILURE;
        }
    }

    time_measure(&start2);

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
            fputs(((const char *restrict)(assembled_asm_str)), file);
            fclose(file);
        }
        printf("\n%s\n", assembled_asm_str);
    }
    else if (exec.ioc == Jited)
    {
        jit(&bresult);
    }

    time_measure(&end2);
    time_measure(&end1);

    // Display compilation or interpretation errors
    if (bresult.state.hasError)
        cc_fprintf(CC_FG_MAGENTA, stdout, "\n ---  Runtime interpreter errors  ---\n\n");
    // Display errors, warnigns and infos
    prettyPrint_state(bresult.state);

    // If we want more informations about execution or compilation
    if (exec.noise_level == Noise && exec.ioc == Interpreted)
        make_noise(bresult, exec, files, argc, ns_to_ms(times_diff(start1, end1)), ns_to_ms(times_diff(start2, end2)), start1, start2, end1, end2);
    else
    {
        printf("\n[i] All time taken: %s.\n", show_time(start1, end1));
        printf("[i] Compilation time taken: %s.\n", show_time(start2, end2));
    }

    // Free the allocated memory areas
    clean_heap(&bresult, exec);

    return EXIT_SUCCESS;
}

void clean_heap(BResult *bresult, const Exec exec)
{
    if (bresult->psdata.rcode.size > 0)
        free(bresult->psdata.rcode.array);
    // TODO BETTER
    if (bresult->psdata.imports.size > 0)
        free(bresult->psdata.imports.array);
    if (bresult->psdata.includes.size > 0)
        free(bresult->psdata.includes.array);
    // ------
    if (bresult->argvs.u.interpreted.args.used > 0)
        free(bresult->argvs.u.interpreted.args.array);
    if (bresult->resulting.size > 0 && exec.ioc != Compiled)
        free(bresult->resulting.array);
    if (bresult->state.size > 0)
        free(bresult->state.array);
    if (bresult->wordico.externs.size > 0)
        free(bresult->wordico.externs.array);
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

void make_noise(const BResult bresult, const Exec exec, const String files[], const size_t argc, const uint64_t all_time, const uint64_t eval_time, const uint64_t t1, const uint64_t t2, const uint64_t e1, const uint64_t e2)
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

    _display_info("Initial operations number: %I64d", bresult.psdata.rcode.used);

    _display_info("Non-native words: %I64d\n", bresult.wordico.externs.used + bresult.wordico.functions.used + bresult.wordico.structures.used);

    printf("     > %I64d functions\n\
     > %I64d structures\n\
     > %I64d externals",
           bresult.wordico.functions.used, bresult.wordico.structures.used, bresult.wordico.externs.used);

    _display_info("Included DLLs: %I64d", bresult.psdata.includes.used);

    _display_info("Imported files: %I64d", bresult.psdata.imports.used);

    _display_info("Recursion optimization: %s", bresult.exec_infos.optimize_rec == true ? "yes" : "no");

    _display_info("Optimization level: Opt-%d", bresult.exec_infos.optimize_level);

    if (exec.ioc == Interpreted)
        _display_info("Usage of program parameters: %s", hasArgv(bresult));

    _display_info("Execution mode: %s", exec.ioc == 0 ? "interpreted" : "compiled");

    _display_info("Evaluation mode: %s", exec.sool == 0 ? "strict" : exec.sool == 1 ? "optimistic" : "lazy");

    _display_info("Time taken by all RCL: %I64d ms", all_time);
    _display_info("Browing, checking, optimizing time: %I64d ms", times_diff(eval_time, all_time));

    /*     if (exec.ioc == Interpreted)
        _display_info("Execution time of the program: %I64d ms", eval_time) else _display_info("Compilation time of the program: %I64d ms", eval_time);
 */

    if (exec.ioc == Interpreted)
        _display_info("Execution time of the program: %s", show_time(t2, e2));

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
