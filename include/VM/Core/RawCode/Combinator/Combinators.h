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


// Note: the order of enumeration is important, as it allows you to directly target the element
// in an array which references the combinators in the stack, as is the case below for `Combinators[][]`.


typedef enum
    { VQ, POP, DUP, SWAP
    , FLIP, ID, DIP, GIS
    , QUOTE, UNQUOTE
    , QAP, CAT, REP
    , KAP, PAK, CONS, UNCONS, PICK
    , SAP, TAKE, TAKEX
    , COMB_MAP, REC, STEQ
    , STEP, NPREC, PUTC, PUTS, NVSHOW
    , IFTE, CASE, SELECT, CRAVE
    , EQ, LW, LEQ, GR, GEQ, NEQ, NOT
    , NEG
    , FTOI, ITOF, ITOC, CTOI, ITOB, STOI, ITOS
    , QLEN, ALEN, REVERSE
    , WTDO, HALT
    , GENREC, LINREC, BINREC, TAILREC, PRIMREC }
Combinator;

static const char Combinators[][10] =
    { "vq", "pop", "dup", "swap"
    , "flip", "id", "dip", "gis"
    , "quote", "unquote"
    , "qap", "cat", "rep"
    , "kap", "pak", "cons", "uncons", "pick"
    , "sap", "take", "takex"
    , "comb_map", "rec", "steq"
    , "step", "nprec", "putc", "puts", "nvshow"
    , "ifte", "case", "select", "crave"
    , "eq", "lw", "leq", "gr", "geq", "neq", "not"
    , "neg"
    , "ftoi", "itof", "itoc", "ctoi", "itob", "stoi", "itos"
    , "qlen", "alen", "reverse"
    , "wtdo", "halt"
    , "genrec", "linrec", "binrec", "tailrec", "primrec" };

Combinator str_to_comb(const String);
bool is_combinator(String);
