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
#include <VM\Core\Types\Combinator.h>

/*
    { VQ, POP, DUP, SWAP
    , FLIP, ID, DIP, GIS
    , QUOTE, UNQUOTE
    , QAP, CAT, REP
    , KAP, PAK, CONS, UNCONS
    , SAP, TAKE, TAKEX
    , COMB_MAP, REC, STEQ
    , STEP, NPREC, PUTC, PUTS, NVSHOW
    , IFTE, CASE, SELECT
    , EQ, LW, LEQ, GR, GEQ, NEQ, NOT
    , NEG
    , FTOI, ITOF, ITOC, CTOI, ITOB
    , QLEN, ALEN
    , WTDO, HALT
    , GENREC, LINREC, BINREC, TAILREC, PRIMREC }
*/

// (Δ -> Δ a)  ==  (Δ a)

const RCL_Type combinator_types[] =
    {
        // VQ : Δ []
        T_QUOTE(T_EMPTY),
        // POP : Δ α -> Δ
        T_ARROW(T_STACK(&T_ANY('a')), T_EMPTY),
        // DUP : Δ α -> Δ α α
        T_ARROW(T_STACK(&T_ANY('a')), T_STACK(&T_ANY('a'), &T_ANY('a'))),
        // SWAP : Δ α β -> Δ β α
        T_ARROW(T_STACK(&T_ANY('a'), &T_ANY('b')), T_STACK(&T_ANY('b'), &T_ANY('a'))),
        // FLIP : Δ α β γ -> Δ γ β α
        T_ARROW(T_STACK(&T_ANY('a'), &T_ANY('b'), &T_ANY('c')), T_STACK(&T_ANY('c'), &T_ANY('b'), &T_ANY('a'))),
        // ID : Δ α -> Δ α
        T_ARROW(T_STACK(&T_ANY('a')), T_STACK(&T_ANY('a'))),
        // DIP : Δ α [β] -> Δ β α
        T_ARROW(T_STACK(&T_ANY('a'), &T_QUOTE(T_ANY('b'))), T_STACK(&T_ANY('b'), &T_ANY('a'))),
        // GIS : Δ Int Int -> Δ {Int}
        T_ARROW(T_STACK(&T_LITERAL(RCL_Value_Integer), &T_LITERAL(RCL_Value_Integer)), T_STACK(&T_ARRAY(T_LITERAL(RCL_Value_Integer)))),
        // QUOTE : Δ α -> [α]
        T_ARROW(T_STACK(&T_ANY('a')), T_STACK(&T_QUOTE(T_ANY('a')))),
        // UQUOTE : Δ [α] -> α
        T_ARROW(T_STACK(&T_QUOTE(T_ANY('a'))), T_STACK(&T_ANY('a'))),
        // UNCONS : Δ [α β] -> Δ [α] β
};
