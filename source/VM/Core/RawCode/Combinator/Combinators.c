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

#include <string.h>
#include <stdbool.h>

#include <VM\Core\RawCode\Combinator\Combinators.h>
#include <Tools\Utils\Utils.h>

Combinator str_to_comb(const String s)
{
    for (unsigned int i = 0; i < sizeof(Combinators) / sizeof(Combinators[0]); i++)
        if (!strcmp(Combinators[i], s))
            return (Combinator)i;
    return -1;
}

bool is_combinator(String c)
{
    for (unsigned int i = 0; i < sizeof(Combinators) / sizeof(Combinators[0]); i++)
        if (!strcmp(Combinators[i], c))
            return 1;
    return 0;
}
