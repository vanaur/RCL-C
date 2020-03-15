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

#include <VM\Core\Checker\Checker.h>
#include <VM\Core\Types\Show.h>
#include <VM\Core\Checker\Typechecker\Typechecker.h>
#include <VM\Core\Types\Types.h>
#include <VM\Core\Types\Combinator.h>
#include <VM\Core\Types\Primitives.h>
#include <VM\Core\Types\Infer.h>

#include <VM\Core\Browse\BResult.h>
#include <VM\Core\State\State.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\rawcodecmp.h>
#include <VM\Core\Show\Show.h>

static Checked_out typecheck(BResult *bresult)
{
    const Env_map_t env = inferall(bresult, &bresult->state);
    const RCL_Type main_type = infer_type(&env, &bresult->psdata.rcode, NULL, &bresult->state);

    // printf("\n\n%s : %s\n", show_rcode(bresult->psdata.rcode), show_type(main_type));

/*     for (Iterator i = 0; i < bresult->wordico.functions.used; i++)
        printf("%s : %s\n", bresult->wordico.functions.array[i].name, show_type(get_val_Env(&env, bresult->wordico.functions.array[i].hash_code, &cmp_hashs)));
 */
    if (main_type.u.rcl_type_stack_.nbr > 1)
    {
        state_put_warn_ch("The return of the program is a concatenation (of type `%s`), which is not compilable.", show_type(main_type));
        state_put_info_ch("The return result of the `main' function will be ignored at compile time.", NULL);
    }

    for (Iterator i = 0; i < main_type.u.rcl_type_stack_.nbr; i++)
    {
        if (main_type.u.rcl_type_stack_.tstack[i]->kind == TYPE_ARROW)
        {
            const String current_type_str = show_type(*main_type.u.rcl_type_stack_.tstack[i]);
            state_put_err_ch("The return of the program contains an irreducible closure (of type `%s`), which is not evaluable.\n", current_type_str);
            return CHECKED_OUT_ERR(bresult->state);
        }
    }

    if (main_type.u.rcl_type_stack_.nbr == 1)
    {
        const RCL_Type mainT = *main_type.u.rcl_type_stack_.tstack[0];
        const RCL_Type expectedT = T_LITERAL(RCL_Value_Integer);

        if (!cmp_types(mainT, expectedT))
            state_put_warn_ch("The return type of the program is '%s`. Was expected an integer or nothing at all.", show_type(mainT));
    }

    return CHECKED_OUT_OK(bresult->state);
}

Checked_out check_program(BResult *bresult)
{
    Checked_out res;
    res.able = true;

    /**| TYPE CHECKING |**/
    if (bresult->exec_infos.type_check)
    {
        Checked_out r = typecheck(bresult);
        if (!r.able)
            res.able = false;
    }

    return res;
}
