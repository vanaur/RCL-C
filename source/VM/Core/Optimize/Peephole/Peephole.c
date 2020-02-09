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
#include <string.h>

#include <VM\Core\Show\Show.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\cmpvalue.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Optimize\Peephole\Peephole.h>
#include <VM\Core\Syntax\Absyn.h>

//       --- RCL IR peephole table ---       //
/**********************************************
    
    dup dup flip			    => dup dup
    swap flip swap flip swap	=> flip
    swap swap			        => <nothing>
    quote qap unquote		    => <nothing>
    qap unquote                 => unquote
    flip flip			        => <nothing>
    quote unquote			    => <nothing>
    dup pop				        => <nothing>
    dup swap			        => dup
    1 +				            => ++
    1 -				            => --
    2 *				            => dup +
    1 *				            => <nothing>
    0 *				            => pop 0
    0 +				            => <nothing>
    0 -				            => <nothing>
    4 *                         => 2 <<
    + flip +			        => + # +
    dup flip + swap pop         => +
    dup flip - swap pop         => -
    vq take unquote             => id
    vq take take unquote        => swap
    vq take take take unquote   => flip

********************************************/

#define SEQ_FREP(vals...) (const Value[]){vals}, NUMARGS(const Value, vals)
#define SEQ_VOID SEQ_FREP()

static void peepholise(RawCode *restrict rcode, const hash_t curr_fhash);
static void peephole_quote(RawCode *restrict rcode, const hash_t curr_fhash);
static void peephole_table(RawCode *restrict rcode, const hash_t curr_fhash);

static void peephole_table(RawCode *restrict rcode, const hash_t curr_fhash)
{
    // dup dup flip => dup dup
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(DUP), RCL_Combinator(DUP), RCL_Combinator(FLIP)),
        SEQ_FREP(RCL_Combinator(DUP), RCL_Combinator(DUP)));
    // swap flip swap flip swap => flip
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(SWAP), RCL_Combinator(FLIP), RCL_Combinator(SWAP), RCL_Combinator(FLIP), RCL_Combinator(SWAP)),
        SEQ_FREP(RCL_Combinator(FLIP)));
    // swap swap => id
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(SWAP), RCL_Combinator(SWAP)),
        SEQ_VOID);
    // quote qap unquote => id
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(QUOTE), RCL_Combinator(QAP), RCL_Combinator(UNQUOTE)),
        SEQ_VOID);
    // qap unquote => unquote
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(QAP), RCL_Combinator(UNQUOTE)),
        SEQ_FREP(RCL_Combinator(UNQUOTE)));
    // flip flip => id
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(FLIP), RCL_Combinator(FLIP)),
        SEQ_VOID);
    // quote unquote => id
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(QUOTE), RCL_Combinator(UNQUOTE)),
        SEQ_VOID);
    // dup pop => id
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(DUP), RCL_Combinator(POP)),
        SEQ_VOID);
    // dup swap => dup
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(DUP), RCL_Combinator(SWAP)),
        SEQ_FREP(RCL_Combinator(DUP)));
    // 1 + => ++
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Integer_I(1), RCL_LiteralOperation_kind(lo_add)),
        SEQ_FREP(RCL_LiteralOperation_kind(lo_inc)));
    // 1 - => --
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Integer_I(1), RCL_LiteralOperation_kind(lo_sub)),
        SEQ_FREP(RCL_LiteralOperation_kind(lo_dec)));
    // 2 * => dup +
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Integer_I(2), RCL_LiteralOperation_kind(lo_mul)),
        SEQ_FREP(RCL_Combinator(DUP), RCL_LiteralOperation_kind(lo_add)));
    // 1 * => id
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Integer_I(1), RCL_LiteralOperation_kind(lo_mul)),
        SEQ_VOID);
    // 0 * => 0
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Integer_I(0), RCL_LiteralOperation_kind(lo_mul)),
        SEQ_FREP(RCL_Combinator(POP), RCL_Integer_I(0)));
    // 0 + => id
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Integer_I(0), RCL_LiteralOperation_kind(lo_add)),
        SEQ_VOID);
    // 0 - => id
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Integer_I(0), RCL_LiteralOperation_kind(lo_sub)),
        SEQ_VOID);
    // 4 * => 2 <<
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Integer_I(4), RCL_LiteralOperation_kind(lo_mul)),
        SEQ_FREP(RCL_Integer_I(2), RCL_LiteralOperation_kind(lo_bleft)));
    // + flip + => + # +
    seq_replace(
        rcode,
        SEQ_FREP(RCL_LiteralOperation_kind(lo_add), RCL_Combinator(FLIP), RCL_LiteralOperation_kind(lo_add)),
        SEQ_FREP(RCL_Parallel(RCL_LiteralOperation_kind(lo_add), RCL_LiteralOperation_kind(lo_add))));
    // dup flip + swap pop => +
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(DUP), RCL_Combinator(FLIP), RCL_LiteralOperation_kind(lo_add), RCL_Combinator(SWAP), RCL_Combinator(POP)),
        SEQ_FREP(RCL_LiteralOperation_kind(lo_add)));
    // dup flip - swap pop => -
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(DUP), RCL_Combinator(FLIP), RCL_LiteralOperation_kind(lo_mul), RCL_Combinator(SWAP), RCL_Combinator(POP)),
        SEQ_FREP(RCL_LiteralOperation_kind(lo_mul)));
    // vq take unquote => id
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(VQ), RCL_Combinator(TAKE), RCL_Combinator(UNQUOTE)),
        SEQ_FREP());
    // vq take take unquote => swap
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(VQ), RCL_Combinator(TAKE), RCL_Combinator(TAKE), RCL_Combinator(UNQUOTE)),
        SEQ_FREP(RCL_Combinator(SWAP)));
    // vq take take take unquote => flip
    seq_replace(
        rcode,
        SEQ_FREP(RCL_Combinator(VQ), RCL_Combinator(TAKE), RCL_Combinator(TAKE), RCL_Combinator(TAKE), RCL_Combinator(UNQUOTE)),
        SEQ_FREP(RCL_Combinator(FLIP)));

    // 4 * => 2 <<
    // Faire un argument `(bool)(when(Value))` ou un truc comme ça
}

// TODO
static void spec_loop1(RawCode *restrict rcode, const hash_t curr_fhash)
{
    // if ...  klw0 unit [Q] [swap inc swap dec <curr_fhash>] ifte  ...
    // => ...  dup flip + swap Q  ...
}

static void peephole_quote(RawCode *restrict rcode, const hash_t curr_fhash)
{
    for (Iterator i = 0; i < rcode->used; i++)
        if (rcode->array[i].kind == RCL_Value_Quotation)
            peepholise(rcode->array[i].u.quote_, curr_fhash);
}

static void peepholise(RawCode *restrict rcode, const hash_t curr_fhash)
{
    // We perform 2 peephole optimization runs, because some reductions that have been made may in turn,
    // with the original code preceding and preceding the sequences, also contain elements to be reduced.
    peephole_table(rcode, curr_fhash);
    peephole_table(rcode, curr_fhash);
    peephole_quote(rcode, curr_fhash);
    peephole_quote(rcode, curr_fhash);
}

void peephole_main(RawCode *restrict rcode)
{
    peepholise(rcode, (hash_t)0);
}

// http://users.ece.cmu.edu/~koopman/stack_compiler/stack_co.html
// https://en.wikipedia.org/wiki/peephole_main
// Inline les fonctions non-récursives :o

void peephole_functions(BResult *restrict bresult)
{
    size_t i = bresult->wordico.functions.used;
    while (i-- > 0)
        peepholise(&bresult->wordico.functions.array[i].body, bresult->wordico.functions.array[i].hash_code);
}
