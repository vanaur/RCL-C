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
#include <VM\Core\Checker\Typechecker\Types\Show.h>
#include <VM\Core\Checker\Typechecker\Typechecker.h>
#include <VM\Core\Checker\Typechecker\Types\Types.h>
#include <VM\Core\Checker\Typechecker\Types\Combinator.h>
#include <VM\Core\Checker\Typechecker\Types\Primitives.h>
#include <VM\Core\Checker\Typechecker\Types\Infer.h>

#include <VM\Core\Browse\BResult.h>
#include <VM\Core\State\State.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\rawcodecmp.h>
#include <VM\Core\Show\Show.h>

Checked_out check_program(BResult * bresult)
{
    Env_map_t env;
    init_Env(&env, bresult->psdata.rcode.used);
    RCL_Type main_type = infer_type(&env, &bresult->psdata.rcode, &bresult->state);
    printf("\n%s : %s\n", show_quote(bresult->psdata.rcode), show_type(main_type));
    return CHECKED_OUT_OK(bresult->state);
}

// https://semantic-domain.blogspot.com/2019/07/all-comonads-all-time.html

// RCL.exe test.clir --check-type
// option:  --check-all
// RCL.exe test.clir --ext-ptr
//                   --ext-ref
//                   --ext-...
