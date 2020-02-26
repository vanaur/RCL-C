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
#include <ffi.h>

#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\FFI\Types.h>
#include <VM\Core\Execution\Interpreter\Atoms\FFI.h>

inline void eval_external_call(Stack * stack, BResult * bresult, struct RCL_Extern * external)
{

    //printf("%s\n", bresult->psdata.includes.used);

    printf("TODO");

    // bresult->psdata.includes.array[0].asid
    // TODO:
    // Match external.dll with bresult->psdata.includes.array[k].asid
    //  => Obtains URL
    //  => Check if function external.name exists
    //  => Apply it onto the stack
}
