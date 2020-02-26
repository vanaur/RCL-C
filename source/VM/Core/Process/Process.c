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

#include <crtdefs.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <VM\Core\Process\Process.h>

#include <Tools\Utils\Utils.h>
#include <Tools\console-color\console-color.h>
#include <VM\Core\Browse\Browse.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Syntax\Parser.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Optimize\Optimizer.h>

String hasArgv(const BResult bresult)
{
    if (bresult.wordico.lambdas.used == 0)
        return "no";
    String result;
    rcl_asprintf(
        &result,
        bresult.wordico.lambdas.array[0].kind == is_Argv ? "yes (%s)" : "no",
        bresult.wordico.lambdas.array[0].name);
    return result;
}

inline size_t refsize(char *s[], size_t nbr)
{
    size_t result = 0;
    while (nbr--)
        if (strlen(s[nbr]) > result)
            result = strlen(s[nbr]);
    return result;
}

inline void concatWordico(BResult * dest, const BResult source)
{
    for (Iterator i = 0; i < source.wordico.externs.used; i++)
        vec_add_externs_data(&dest->wordico.externs, source.wordico.externs.array[i]);

    for (Iterator i = 0; i < source.wordico.functions.used; i++)
        vec_add_function_data(&dest->wordico.functions, source.wordico.functions.array[i]);

    for (Iterator i = 0; i < source.wordico.structures.used; i++)
        vec_add_structure_data(&dest->wordico.structures, source.wordico.structures.array[i]);
}

inline void concatBResult(BResult * dest, const BResult source)
{
    concatWordico(dest, source);

    for (Iterator i = 0; i < source.state.used; i++)
        state_put(&dest->state, source.state.array[i]);

    for (Iterator i = 0; i < source.wordico.lambdas.used; i++)
        vec_add_lambdas(&dest->wordico.lambdas, source.wordico.lambdas.array[i]);

    for (Iterator i = 0; i < source.psdata.rcode.used; i++)
        push_rcode(&dest->psdata.rcode, source.psdata.rcode.array[i]);
}

inline BResult processFile(String filename)
{
    FILE *input = fopen(filename, "r");
    Program parsed = pProgram(input);
    if (!parsed)
    {
        printf("Can't continue with parsing error.\n\n");
        exit(1);
    }
    const BResult res = browseAbsyn(parsed, filename);
    free(parsed);
    return res;
}

BResult processFiles(String filenames[], size_t nbr, Exec exec)
{

    size_t nbr2 = nbr,
           counter = 1,
           szrf = refsize(filenames, nbr),
           j = 0;

    if (nbr >= 10)
        szrf++;

    if (nbr >= 100)
        szrf++;

    BResult result;
    result.exec_infos = exec;

    state_init(&result.state);
    init_wordico_nbr(&result.wordico, 1);
    init_rcode(&result.psdata.rcode, 1);
    init_imports__(&result.psdata.imports, 1);
    init_imports__(&result.psdata.includes, 1);

    // Avant, c'était   while(nbr--) ... Avec nbr comme indexe
    for (int i = 0; i < nbr; i++)
    {
        if (startsWith(filenames[i], "--"))
            continue;

        if (i == 9)
        {
            szrf--;
        }
        if (i == 99)
        {
            szrf--;
        }

        if (exec.noise_level == Noise)
        {
            printf("> Process %I64d of %I64d  --  (%s) ", counter, nbr2, filenames[i]);
        }

        concatBResult(&result, processFile(filenames[i]));

        optimize(&result);

        if (exec.noise_level == Noise)
        {
            for (j = strlen(filenames[i]); j <= szrf; j++)
                putchar(' ');

            if (!result.state.hasError)
            {
                printf(" [");
                cc_fprintf(CC_FG_GREEN, stdout, "ok");
                printf("]\n");
            }
            else
            {
                printf(" [");
                cc_fprintf(CC_FG_DARK_YELLOW, stdout, "fail");
                printf("]\n");
            }
        }

        prettyPrint_state(result.state);
        state_init(&result.state);

        if (exec.kir)
        {
            // TODO :)
        }

        counter++;
    }

    return result;
}
