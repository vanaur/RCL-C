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
#include <pthread.h>
#include <gmp.h>

#include <VM\Core\RawCode\Combinator\Combinators.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Execution\Interpreter\Eval_Atom\Eval_op.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Show\Show.h>

static void doDup(Stack *restrict stack)
{
    push(stack, *top_ptr(stack));
}

static void doSwap(Stack *restrict stack)
{
    Value tmp = *topx_ptr(stack, 1);
    *topx_ptr(stack, 1) = *topx_ptr(stack, 2);
    *topx_ptr(stack, 2) = tmp;
}

static void doFlip(Stack *restrict stack)
{
    Value tmp = *topx_ptr(stack, 1);
    *topx_ptr(stack, 1) = *topx_ptr(stack, 3);
    *topx_ptr(stack, 3) = tmp;
}

static void doGis(Stack *restrict stack)
{
    const int from = mpz_get_d(drop(stack).u.int_);
    const int until = mpz_get_d(drop(stack).u.int_);

    RawCode *quote = (RawCode *)malloc(sizeof(*quote));
    init_rcode(quote, abs(until - from) + 1);

    for (Iterator i = from; i < until; i++)
        push_rcode(quote, make_RCL_Value_Integer_i(i));
    
    push(stack, make_RCL_Value_Quotation(quote));
}

static void doPop(Stack *restrict stack)
{
    pop(stack);
}

static void doQuote(Stack *restrict stack)
{
    RawCode *quote = (RawCode *)malloc(sizeof(*quote));
    init_rcode(quote, 1);
    push_rcode(quote, *top_ptr(stack));
    *top_ptr(stack) = make_RCL_Value_Quotation(quote);
}

inline void doUnquote(Stack *restrict stack, const Value *restrict quote, BResult *restrict bresult)
{
    // If we are in a recursion, we do not immediately evaluate the elements, they will be evaluated when the interpreter needs them;
    // this is a form of lazy-evaluation, which is necessary here for the proper functioning of recursions.
    if (stack->in_rec)
    {
        for (Iterator i = 0; i < quote->u.quote_->used; i++)
            push(stack, quote->u.quote_->array[i]);
    }
    else
    {
        for (Iterator i = 0; i < quote->u.quote_->used; i++)
            evalop(stack, &quote->u.quote_->array[i], bresult);
    }
}

static void doId(Stack *restrict stack)
{
    // No implementation needed such it returns the same argument
    return;
}

static void doCat(Stack *restrict stack)
{
    concat_rcode(topx_ptr(stack, 2)->u.quote_, top_ptr(stack)->u.quote_);
    pop(stack);
}

static void doCons(Stack *restrict stack)
{
    // [A] [B] cons => [[A] B]

    doSwap(stack);
    doQuote(stack);
    doSwap(stack);
    doCat(stack);
}

static void doUncons(Stack *restrict stack)
{
    push(stack, top_ptr(stack)->u.quote_->array[top_ptr(stack)->u.quote_->used - 1]);
    pop_rcode(topx_ptr(stack, 2)->u.quote_);
    //push(stack, drop_rcode(top_ptr(stack)->u.quote_));
}

static void doDip(Stack *restrict stack, BResult *restrict bresult)
{
    // a [B] dip => B a

    const Value b_quote = drop(stack);
    const Value a_value = drop(stack);

    doUnquote(stack, &b_quote, bresult);
    push(stack, a_value);
}

static void doSap(Stack *restrict stack, BResult *restrict bresult)
{
    // [A] [B] sap => B A

    Value b = drop(stack);
    Value a = drop(stack);

    doUnquote(stack, &b, bresult);
    doUnquote(stack, &a, bresult);
}

// Is lazy
static void doTake(Stack *restrict stack, BResult *restrict bresult)
{
    // A [B] take => [B A]

    push_rcode(top_ptr(stack)->u.quote_, *topx_ptr(stack, 2));
    doSwap(stack);
    pop(stack);
}

// Is not lazy
static void doTakex(Stack *restrict stack, BResult *restrict bresult)
{
    // [A] [B] take => [B [A]]

    RawCode *result = (RawCode *)malloc(sizeof(*result));
    init_rcode(result, top_ptr(stack)->u.quote_->used + 1);

    Value b_quote = drop(stack);
    Stack b_stack_tmp;
    init_stack(&b_stack_tmp, b_quote.u.quote_->used);
    doUnquote(&b_stack_tmp, &b_quote, bresult);

    for (Iterator i = 0; i < b_stack_tmp.used; i++)
        push_rcode(result, b_stack_tmp.array[i]);

    push_rcode(result, drop(stack));

    push(stack, make_RCL_Value_Quotation(result));

    free(result);
}

static void doRep(Stack *restrict stack, BResult *restrict bresult)
{
    // [A] rep => A A

    Value to_apply = drop(stack);
    doUnquote(stack, &to_apply, bresult);
    doUnquote(stack, &to_apply, bresult);
}

static void doKap(Stack *restrict stack, BResult *restrict bresult)
{
    // [A] kap => [A] A

    Value to_apply = drop(stack);
    push(stack, to_apply);
    doUnquote(stack, &to_apply, bresult);
}

static void doPak(Stack *restrict stack, BResult *restrict bresult)
{
    // [A] pak => A [A]

    Value to_apply = drop(stack);
    doUnquote(stack, &to_apply, bresult);
    push(stack, to_apply);
}

static void doQap(Stack *restrict stack)
{
    // A [B] qap => [A B]

    RawCode *result = (RawCode *)malloc(sizeof(*result));
    init_rcode(result, top_ptr(stack)->u.quote_->used + 1);

    RawCode quote = *drop(stack).u.quote_;
    Value item = drop(stack);

    push_rcode(result, item);
    concat_rcode(result, &quote);

    push(stack, make_RCL_Value_Quotation(result));

    free(result);
}

// To review! Ne correspond pas exactement au model MAP
static void doComb_map(Stack *restrict stack, BResult *restrict bresult)
{
    // [a, b, c] [f] comb_map => [a f b f c f]

    RawCode *result = (RawCode *)malloc(sizeof(*result));
    init_rcode(result, topx_ptr(stack, 2)->u.quote_->used);

    const Value fn = drop(stack);
    const RawCode quote_target = *drop(stack).u.quote_;

    Stack tmp_stack;
    init_stack(&tmp_stack, quote_target.used);

    for (Iterator i = 0; i < quote_target.used; i++)
    {
        push(&tmp_stack, quote_target.array[i]);
        doUnquote(&tmp_stack, &fn, bresult);
    }

    for (Iterator i = 0; i < tmp_stack.used; i++)
        push_rcode(result, tmp_stack.array[i]);

    push(stack, make_RCL_Value_Quotation(result));

    free(result);
}

static void doRec(Stack *restrict stack, BResult *restrict bresult)
{
    // [P] rec => [[P] rec] P

    RawCode *result = (RawCode *)malloc(sizeof(*result));
    init_rcode(result, 2);

    Value quote = drop(stack);
    push_rcode(result, quote);
    push_rcode(result, RCL_Combinator(REC));

    push(stack, make_RCL_Value_Quotation(result));
    doUnquote(stack, &quote, bresult);

    free(result);
}

static void doStep(Stack *restrict stack, BResult *restrict bresult)
{
    const Value program = drop(stack);
    const Value seq = drop(stack);

    for (Iterator i = 0; i < seq.u.quote_->used; i++)
    {
        push(stack, seq.u.quote_->array[i]);
        doUnquote(stack, &program, bresult);
    }
}

/*
    vq
    [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20]
    [~10 cube square quote cat]
    step
*/

static void doSteq(Stack *restrict stack, BResult *restrict bresult)
{
    Value program = drop(stack);
    const Value seq = drop(stack);

    extend_size_RawCode(program.u.quote_, 2);
    push_rcode(program.u.quote_, make_RCL_Value_Combinator(QUOTE));
    push_rcode(program.u.quote_, make_RCL_Value_Combinator(CAT));

    RawCode *quote = malloc(sizeof *quote);
    init_rcode(quote, 1);
    push(stack, make_RCL_Value_Quotation(quote));

    for (Iterator i = 0; i < seq.u.quote_->used; i++)
    {
        push(stack, seq.u.quote_->array[i]);
        doUnquote(stack, &program, bresult);
    }
}

static void doVq(Stack *restrict stack)
{
    // This is just a void quote : [ ]

    RawCode *quote = malloc(sizeof *quote);
    init_rcode(quote, 1);
    push(stack, make_RCL_Value_Quotation(quote));
}

static void do_nprec(Stack *restrict stack)
{
    // x y nprec
    // Set prec Y to X
    if (topx_ptr(stack, 2)->kind == RCL_Value_Float)
        mpf_set_prec(topx_ptr(stack, 2)->u.float_, (mp_bitcnt_t)mpz_get_d(drop(stack).u.int_));
}

static void do_ifte(Stack *restrict stack, BResult *restrict bresult)
{
    // [condition] [then] [else] ifte
    const Value _else = drop(stack);
    const Value _then = drop(stack);
    const Value _cond = drop(stack);

    Stack stk_cond;
    init_stack(&stk_cond, _cond.u.quote_->used);

    doUnquote(&stk_cond, &_cond, bresult);

    if (mpz_get_d(top_ptr(&stk_cond)->u.int_))
        doUnquote(stack, &_then, bresult);
    else
        doUnquote(stack, &_else, bresult);
    
    free(stk_cond.array);
}

inline void doComb(Stack *restrict stack, const Combinator comb, BResult *restrict bresult)
{
    switch (comb)
    {
    case VQ:
        return doVq(stack);

    case POP:
        return doPop(stack);

    case DUP:
        return doDup(stack);

    case SWAP:
        return doSwap(stack);

    case FLIP:
        return doFlip(stack);

    case GIS:
        return doGis(stack);

    case DIP:
        return doDip(stack, bresult);

    case QUOTE:
        return doQuote(stack);

    case UNQUOTE:
    {
        Value quote = drop(stack);
        doUnquote(stack, &quote, bresult);
    }
    break;

    case QAP:
        return doQap(stack);

    case KAP:
        return doKap(stack, bresult);

    case PAK:
        return doPak(stack, bresult);

    case CAT:
        return doCat(stack);

    case CONS:
        return doCons(stack);

    case UNCONS:
        return doUncons(stack);

    case SAP:
        return doSap(stack, bresult);

    case REP:
        return doRep(stack, bresult);

    case TAKE:
        return doTake(stack, bresult);

    case TAKEX:
        return doTakex(stack, bresult);

    case COMB_MAP:
        return doComb_map(stack, bresult);
    
    case REC:
        return doRec(stack, bresult);

    case ID:
        return doId(stack);

    case STEP:
        return doStep(stack, bresult);

    case STEQ:
        return doSteq(stack, bresult);

    case NPREC:
        return do_nprec(stack);

    case PUTC:
        putc(drop(stack).u.char_, stdout);
        break;

    case PUTS:
        printf("%s", drop(stack).u.string_);
        break;

    case NVSHOW:
        push(stack, make_RCL_Value_String(show_value(drop(stack))));
        break;

    case IFTE:
        return do_ifte(stack, bresult);

    default:
        printf("Combinator not implemented yet\n");
    }
}
