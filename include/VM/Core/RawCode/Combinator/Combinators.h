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

#pragma once
#include <stdbool.h>
#include <Tools\Utils\Utils.h>

/*** Native RCL combinators
 * We can distinguish 2 types of native combinators:
 *  - those who exchange positions
 *  - those who interact with the quotations
 * As well as the combinator `pop` and `id`.
 */

typedef enum
    { VQ, POP, DUP, SWAP
    , FLIP, ID, DIP
    , QUOTE, UNQUOTE
    , QAP, CAT, REP
    , KAP, PAK, CONS, UNCONS
    , SAP, TAKE, TAKEX
    , COMB_MAP, REC
    , STEP, NPREC, PUTC, PUTS }
Combinator;

static const char Combinators[][8] =
    { "vq", "pop", "dup", "swap"
    , "flip", "id", "dip"
    , "quote", "unquote"
    , "qap", "cat", "rep"
    , "kap", "pak", "cons", "uncons"
    , "sap", "take", "takex"
    , "comb_map", "rec"
    , "step", "nprec", "putc", "puts" };

Combinator str_to_comb(const String);
bool is_combinator(String);
