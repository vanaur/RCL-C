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
#include <Library\rclib.h>
#include <Tools\Utils\Utils.h>
#include <Tools\console-color\console-color.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Syntax\Parser.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Browse\Browse.h>
#include <VM\Core\Execution\Interpreter\Interpreter.h>

void rcl__hello_world()
{
    cc_fprintf(CC_FG_DARK_CYAN, stdout, "Welcome and hello world in RCL v");
    cc_fprintf(CC_FG_CYAN, stdout, "%s", show_version(CURRENT_VERSION));
    cc_fprintf(CC_FG_DARK_CYAN, stdout, "!\n");
}

const String rcl__get_current_version_s()
{
    return show_version(CURRENT_VERSION);
}

const Version rcl__get_current_version_v()
{
    return CURRENT_VERSION;
}

_export struct IResult rcl__simple_interpret_ir_s(const String p_s, const bool show_res)
{
    FILE *file_tmp = fopen("tmp_file.clir", "w");
    fprintf(file_tmp, "%s", p_s);
    fclose(file_tmp);
    file_tmp = fopen("tmp_file.clir", "r");
    BResult bresult = browseAbsyn(pProgram(file_tmp), "the given program");
    fclose(file_tmp);
    interpret(&bresult, Strict);
    remove("tmp_file.clir");
    struct IResult res;
    if (bresult.resulting.used == 1)
    {
        if (bresult.resulting.array[0].kind == RCL_Value_Integer)
        {
            res.main_returned = mpz_get_d(bresult.resulting.array[0].u.int_);
            if (!bresult.state.hasError)
                res.program_res = SUCCESS;
            else
                res.program_res = FAILURE;
        }
        else
        {
            if (!bresult.state.hasError)
            {
                res.main_returned = EXIT_SUCCESS;
                res.program_res = SUCCESS;
            }
            else
            {
                res.main_returned = EXIT_FAILURE;
                res.program_res = FAILURE;
            }
        }
    }
    else
    {
        if (!bresult.state.hasError)
            res.main_returned = EXIT_SUCCESS;
        else
            res.main_returned = EXIT_FAILURE;
        res.program_res = INCOMPLETE;
    }
    if (show_res)
        printf("Resulting: %s\n", showQuote(bresult.resulting));
    return res;
}

struct IResult rcl__evali_expr(const rcl__expr_t expr)
{
    struct IResult res;
    const struct BResult bres = new_bresult();
    const RawCode evaluated = seval(expr, &bres);

    if (evaluated.used > 1)
    {
        res.program_res = INCOMPLETE;
        res.main_returned = EXIT_FAILURE;
    }
    else
    {
        res.program_res = SUCCESS;
        if (top_rcode(&evaluated).kind == RCL_Value_Integer)
            res.main_returned = mpz_get_d(top_rcode(&evaluated).u.int_);
        else if (top_rcode(&evaluated).kind == RCL_Value_Float)
            res.main_returned = (int)mpf_get_d(top_rcode(&evaluated).u.float_);
        else
            res.main_returned = 0;
    }
    
    return res;
}
