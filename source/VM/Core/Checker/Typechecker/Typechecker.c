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
// https://github.com/cdiggins/type-inference
// https://en.wikipedia.org/wiki/Hindley%E2%80%93Milner_type_system
// https://www.google.be/search?newwindow=1&sxsrf=ACYBGNSg0vH7Z96hK3tXWuvRj2tYgInWIQ%3A1577038719249&ei=f7P_Xd7uDtvhgAbZlYjoAw&q=hindley+milner+type+inference+in+c&oq=hindley+milner+type+inference+in+c&gs_l=psy-ab.3..33i160l2j33i21l2.1160871.1167693..1167965...2.0..0.121.1281.12j3......0....1..gws-wiz.......35i39j0i273j0i131j0i67j0j0i10j0i20i263j0i333.dPM0Bfz8eMo&ved=0ahUKEwieqqGF78nmAhXbMMAKHdkKAj0Q4dUDCAs&uact=5

#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Tools\Utils\Utils.h>
#include <Tools\Map\Map.h>
#include <VM\Core\State\State.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\cmpvalue.h>
#include <VM\Core\Browse\BResult.h>

#include <VM\Core\Types\Combinator.h>
#include <VM\Core\Types\Primitives.h>
#include <VM\Core\Types\Types.h>

