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
#include <pthread.h>

#include <VM\Core\Optimize\Peephole\Peephole.h>
#include <VM\Core\Optimize\Inlining\Inline.h>
#include <VM\Core\Optimize\Precomputation\Precompute.h>
#include <VM\Core\Optimize\Optimizer.h>
#include <VM\Core\Show\Show.h>

#include <VM\Core\Exec.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\State\State.h>

static void optimize_O1(BResult * bresult)
{
    pthread_t th;
    pthread_create(&th, NULL, (void *(*)(void *))peephole_functions, (void *)bresult);
    peephole_main(&bresult->psdata.rcode);
    pthread_join(th, NULL);
}

static void optimize_O2(BResult * bresult)
{
    inline_optimization_function(bresult);
    inline_optimization_function(bresult);
    peephole_functions(bresult);
    inline_optimization_code(bresult);
    inline_optimization_code(bresult);
    peephole_main(&bresult->psdata.rcode);
}

static void optimize_O3(BResult * bresult)
{
    precompute_main(bresult);
}

void optimize(BResult * bresult)
{
    //printf("\n--> Before: %s\n", show_stack(*((Stack *)&bresult->psdata.rcode)));
    switch (bresult->exec_infos.optimize_level)
    {
    case O0:
        // There is no optimization to perform
        // with this optimization flag that is
        // applied by default
        break;

    case O1:
        optimize_O1(bresult);
        break;

    case O2:
        optimize_O2(bresult);
        break;

    case O3:
        optimize_O3(bresult);
        break;

    default:
        NewState_return(
            make_warning,
            Optimizer,
            "Unknown or unimplemented yet optimization flag: '%d'.",
            bresult->exec_infos.optimize_level);
    }
    //printf("--> After:  %s\n", show_stack(*((Stack *)&bresult->psdata.rcode)));
}
