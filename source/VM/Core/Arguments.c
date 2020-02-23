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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <Tools\console-color\console-color.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\Syntax\Parser.h>
#include <VM\Core\Browse\Browse.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Exec.h>
#include <VM\Core\Argv.h>

#define RCL_VERSION \
    "\tRCL (x86_64-unix/mingw -- designed by vanaur as the Runtime Core Library project) 0.0.1 {rcl-x86_64-0.0.1}\n\
\tUnder Apache-2.0 license (all kind of use + copyright notice and source), 2018, *MEGALITY*.\n\
\tFree software with NO warrenty.\n\n"

#define RCL_DEPENDENCIES \
    "\t- libffi (https://github.com/libffi/libffi)\n\
\t- GMP    (https://gmplib.org)\n\n"

void display_help()
{
    printf("Hey, welcome! You are in the help menu of the RCL, the runtime core library, in the version ");
    cc_fprintf(CC_FG_GREEN, stdout, "RCL %s", show_version(CURRENT_VERSION));
    printf("!\n\n");
    cc_fprintf(CC_FG_DARK_GRAY, stdout, " This help menu only lists flags that are useful for running or analyzing a program,\n if you are looking for detailed documentation, go to ");
    cc_fprintf(CC_FG_BLUE, stdout, "https://MEGTY/RCL/doc.org");
    cc_fprintf(CC_FG_DARK_GRAY, stdout, ".\n\n");
    printf("Here is the minimum list of the most common flags to use:\n\n");

    cc_fprintf(CC_FG_CYAN, stdout, "    RCL --c <file>\n");
    printf("        This will compile the file into a native executable on your platform,\n        without any external dependency on the RCL.\n\n");

    cc_fprintf(CC_FG_CYAN, stdout, "    RCL --jit <file>\n");
    printf("        This will run your program with the Just In Time method,\n        allowing for optimized execution during the evaluation.\n\n");

    cc_fprintf(CC_FG_CYAN, stdout, "    RCL <file> --O<k>\n");
    printf("        This will activate a level 'k' optimization, k being a number between 0 and 3.\n        Go to the link above for more information. (O1 is usually sufficient and relatively fast).\n        Note that RCL has in its philosophy that an optimization should never\n        be necessary for a program to run.\n\n");

    cc_fprintf(CC_FG_CYAN, stdout, "    RCL <file> --bench\n");
    printf("        This will make a level performance analysis for the given program.\n        You can use the ");
    cc_fprintf(CC_FG_CYAN, stdout, "--diagnostic");
    printf(" option for a more complete diagnostic.\n\n");

    cc_fprintf(CC_FG_CYAN, stdout, "    RCL <file> --check-<k>\n");
    printf("        This option will perform a 'k' check on your program, where 'k' can be:\n");
    printf("                                                                                + ");
    cc_fprintf(CC_FG_DARK_BLUE, stdout, "type\n");
    printf("                                                                                + ");
    cc_fprintf(CC_FG_DARK_BLUE, stdout, "overflow\n");
    printf("                                                                                + ");
    cc_fprintf(CC_FG_DARK_BLUE, stdout, "number\n");
    printf("                                                                                + ");
    cc_fprintf(CC_FG_DARK_BLUE, stdout, "loop\n\n");

    cc_fprintf(CC_FG_CYAN, stdout, "    RCL -repl\n");
    printf("        To simply enter the interactive environment of the RCL IR, and play with it, to better understand it.\n\n");

    printf("For the complete list of all options, type ");
    cc_fprintf(CC_FG_CYAN, stdout, "RCL -give-me-more");
    printf(". It will write all the options and their explanation in a file named `RCL options.md`.");

    printf("\nIf you encounter difficulties or bugs/defects,\ndon't hesitate to report them at ");
    cc_fprintf(CC_FG_BLUE, stdout, "https://github.com/vanaur/The-Runtime-Core-Library/issues");
    printf(" in order to fix them!");

    printf("\n\n");
}

void remove_element(String *argv, int total, int at)
{
    if (at != total - 1)
        argv[at] = argv[total - 1];
}

struct RCL_Argv get_lamargs(String *argv, int *argc)
{
    const int total = *argc;
    struct RCL_Argv *result = (struct RCL_Argv *)malloc(sizeof(*result));
    FILE *tmp;

    for (int i = 0; i < total; i++)
    {
        if (!strcmp(argv[i], "args=") || !strcmp(argv[i], "a="))
        {
            (*argc)--;
            remove_element(argv, total, i);
            tmp = fopen("ilamargs", "w");
            for (int j = ++i; j < total; j++)
            {
                fprintf(tmp, "%s ", argv[j]);
                (*argc)--;
                remove_element(argv, total, i);
            }
            fclose(tmp);
            break;
        }
    }

    tmp = fopen("ilamargs", "r");
    Program p = pProgram(tmp);
    fclose(tmp);
    remove("ilamargs");
    BResult btmp = browseAbsyn(p, "Interctive");
    init_rcode(&result->u.interpreted.args, btmp.psdata.rcode.used);
    result->u.interpreted.args = btmp.psdata.rcode;
    return *result;
}

Exec get_exec(String *argv, int *argc)
{
    const int total = *argc;
    int n = *argc;
    bool error = false;

    Exec result =
        { .ioc = Interpreted            // --i, --jit, --c
        , .sool = Strict                // --s, --o, --l
        , .noise_level = Noise          // --noise, --silent
        , .prehandle = P_no             // --pno, --pprog, --plib, --pfn
        , .optimize_rec = false         // --orec
        , .optimize_level = O0          // --O0, --O1, --O3
        , .gc_free = false              // --Ogc
        , .osize = false                // --Os, --Osize
        , .kasm = false                 // --kasm
        , .kir = false                  // --kir
        , .docasm = true                // --docasm, --dasm
        , .show_steps = false           // --st
        , .type_check = false           // --check-type
        , .show_res = true              // --show-res, --sr
        , .ext_bignum = false           // --ext-bignum
        , .ext_ptr = false              // --ext-ptr
        , .low = false                  // --low
        , .shw_inside_struct = false};  // --shw=insd-struct

    while (n--)
    {
        argv[n] = to_upper_s(argv[n]);
        if (!strcmp(argv[n], "--I"))
        {
            result.ioc = Interpreted;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--C"))
        {
            result.ioc = Compiled;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--JIT"))
        {
            result.ioc = Jited;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--S"))
        {
            result.sool = Strict;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--L"))
        {
            result.sool = Lazy;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--O"))
        {
            result.sool = Optimistic;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--SILENT"))
        {
            result.noise_level = Silent;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--NOISE"))
        {
            result.noise_level = Noise;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--SHW=INSD-STRUCT"))
        {
            result.shw_inside_struct = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--ST"))
        {
            result.show_steps = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--OREC"))
        {
            result.optimize_rec = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--OGC"))
        {
            result.gc_free = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--OSIZE") || !strcmp(argv[n], "--OS"))
        {
            result.osize = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--O0"))
        {
            result.optimize_level = O0;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--O1"))
        {
            result.optimize_level = O1;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--O2"))
        {
            result.optimize_level = O2;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--O3"))
        {
            result.optimize_level = O3;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--EXT-BIGNUM"))
        {
            result.ext_bignum = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--EXT-PTR"))
        {
            result.ext_ptr = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--LOW"))
        {
            result.low = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--PNO"))
        {
            result.prehandle = P_no;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--PP"))
        {
            result.prehandle = P_prog;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--PL"))
        {
            result.prehandle = P_lib;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--PF"))
        {
            result.prehandle = P_fn;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--KIR"))
        {
            result.kir = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--KASM"))
        {
            result.kasm = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--DOCASM") || !strcmp(argv[n], "--DASM"))
        {
            result.docasm = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--SHOW-RES") || !strcmp(argv[n], "--SR"))
        {
            result.show_res = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--NO-SHOW-RES") || !strcmp(argv[n], "--NSR"))
        {
            result.show_res = false;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "--CHECK-TYPE") || !strcmp(argv[n], "--CT"))
        {
            result.type_check = true;
            (*argc)--;
            remove_element(argv, total, n);
        }
        else if (!strcmp(argv[n], "-VERSION"))
        {
            printf("\nUsed version of the RCL:\n\n%s", RCL_VERSION);
            exit(EXIT_SUCCESS);
        }
        else if (!strcmp(argv[n], "-DEPENDENCIES"))
        {
            printf("\n%s", RCL_DEPENDENCIES);
            exit(EXIT_SUCCESS);
        }
        else if (!strcmp(argv[n], "-HELP") || !strcmp(argv[n], "-?"))
        {
            display_help();
            exit(EXIT_SUCCESS);
        }
        else if (fopen(argv[n], "r") == NULL) // In conflict with lambda arguments! To rework
        {
            String msg;
            printf("\nRCL:");
            rcl_asprintf(&msg, "Unrecognized flag or unexisting file '%s'.", argv[n]);
            cc_fprintf(CC_FG_DARK_RED, stdout, " %s\n", msg);
            free(msg);
            (*argc)--;
            remove_element(argv, total, n);
            error = true;
        }
    }

    if (error)
    {
        cc_fprintf(CC_FG_DARK_GREEN, stdout, "\nType ");
        cc_fprintf(CC_FG_DARK_CYAN, stdout, "-help / -?");
        cc_fprintf(CC_FG_DARK_GREEN, stdout, " for help, or go to ");
        cc_fprintf(CC_FG_BLUE, stdout, "https://github.com/vanaur/RCL\n\n");
        exit(EXIT_FAILURE);
    }

    // We have to shift items after removing someone...
    String value = argv[0];
    for (int k = 0; k < *argc - 1; k++)
        argv[k] = argv[k + 1];
    argv[*argc - 1] = value;

    return result;
}
