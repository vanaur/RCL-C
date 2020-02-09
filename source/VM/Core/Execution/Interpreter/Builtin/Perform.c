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

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\RCLIB\builtin.h>

#include <VM\Core\Execution\Interpreter\Builtin\Perform.h>
#include <VM\Core\Execution\Interpreter\Builtin\Math\Math.h>
#include <VM\Core\Execution\Interpreter\Builtin\Misc\Misc.h>

void perform_builtin(const struct Builtin *restrict builtin, Stack *restrict stack, BResult *restrict bresult, const String name)
{
    switch (builtin->module)
    {
    case _Math:
        return perform_builtin_math(stack, *builtin, bresult);

    case _Misc:
        return perform_builtin_misc(stack, *builtin, bresult);

    default:
        NewState_return(make_error,
                        Interpreter,
                        "Unknown builtin module `%d'.",
                        builtin->module);
    }
}
