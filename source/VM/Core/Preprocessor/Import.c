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
#include <stdbool.h>
#include <ctype.h>

#include <VM\Core\Preprocessor\Import.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Syntax\Identifier\Functions.h>
#include <VM\Core\Syntax\Parser.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Process\Process.h>
#include <VM\Core\State\State.h>

bool can_imports(const String path)
{
    return fopen(path, "r") != NULL;
}

void imports(BResult *restrict bresult, Preprocessor prec)
{
    if (!can_imports(prec->u.import_.string_))
        NewState_return(make_error, Browser, "Can't impote file: `%s'.", prec->u.import_.string_);

    FILE *file = fopen(prec->u.import_.string_, "r");
    Program ptmp = pProgram(file);
    BResult btmp = browseAbsyn(ptmp, prec->u.import_.string_);
    concatWordico(bresult, btmp);
    if (btmp.state.used > 0)
    {
        printf("\n  > From \"%s\" imported:\n", prec->u.import_.string_);
        prettyPrint_state(btmp.state);
    }
    fclose(file);
}
