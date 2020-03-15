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

#include <Tools\Utils\Utils.h>
#include <VM\Core\Types\Types.h>
#include <VM\Core\Types\Primitives.h>

// In order: is_Add, is_Sub, is_Pow, is_Mul, is_Div, is_Mod, is_Inc, is_Dec, is_BAnd, is_BOr, is_BXor, is_BLeft, is_BRight

const RCL_Type primitive_types[] =
    {
        // add
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number))),
        // sub
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number))),
        // pow
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number))),
        // mul
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number))),
        // div
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Float))),
        // mod
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number))),
        // inc
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number)),                               T_STACK(&T_LITERAL(RCL_Value_Number))),
        // dec
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number)),                               T_STACK(&T_LITERAL(RCL_Value_Number))),
        // and
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number))),
        // or
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number))),
        // xor
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number))),
        // left
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number))),
        // right
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Number), &T_LITERAL(RCL_Value_Number)), T_STACK(&T_LITERAL(RCL_Value_Number)))

};
