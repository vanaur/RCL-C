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
#include <assert.h>

#include <VM\Core\RawCode\Combinator\Combinators.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Execution\Interpreter\Atoms\operation.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\RawCode\cmpvalue.h>

static void doDup(Stack *stack)
{
    push(stack, *top_ptr(stack));
}

static void doSwap(Stack *stack)
{
    Value tmp = *topx_ptr(stack, 1);
    *topx_ptr(stack, 1) = *topx_ptr(stack, 2);
    *topx_ptr(stack, 2) = tmp;
}

static void doFlip(Stack *stack)
{
    Value tmp = *topx_ptr(stack, 1);
    *topx_ptr(stack, 1) = *topx_ptr(stack, 3);
    *topx_ptr(stack, 3) = tmp;
}

static void doGis(Stack *stack)
{
    const int from = mpz_get_d(drop(stack).u.int_);
    const int until = mpz_get_d(drop(stack).u.int_);

    const int size = until - from;

    if (size <= 0)
        return push(stack, RCL_Combinator(VQ));

    RawCode *quote = (RawCode *)malloc(sizeof(*quote));
    init_rcode(quote, abs(until - from) + 1);

    /*     for (Iterator i = from; i < until; i++)
        push_rcode(quote, make_RCL_Value_Integer_i(i)); */

    Value *array = malloc(sizeof(Value) * size);

    int tmp = size;

    while (tmp-- > from)
        array[tmp] = RCL_Integer_I(tmp);

    quote->array = array;
    quote->used = size;

    push(stack, make_RCL_Value_Quotation(quote));
}

static void doPop(Stack *stack)
{
    pop(stack);
}

static void doQuote(Stack *stack)
{
    RawCode *quote = (RawCode *)malloc(sizeof(*quote));
    init_rcode(quote, 1);
    push_rcode(quote, *top_ptr(stack));
    *top_ptr(stack) = make_RCL_Value_Quotation(quote);
}

inline void doUnquote(Stack *stack, const Value *quote, BResult *bresult)
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

static void doId(Stack *stack)
{
    // No implementation needed such it returns the same argument
    return;
}

static void doCat(Stack *stack)
{
    concat_rcode(topx_ptr(stack, 2)->u.quote_, top_ptr(stack)->u.quote_);
    pop(stack);
}

static void doCons(Stack *stack)
{
    // [A] [B] cons => [[A] B]

    doSwap(stack);
    doQuote(stack);
    doSwap(stack);
    doCat(stack);
}

static void doUncons(Stack *stack)
{
    push(stack, top_ptr(stack)->u.quote_->array[top_ptr(stack)->u.quote_->used - 1]);
    pop_rcode(topx_ptr(stack, 2)->u.quote_);
    //push(stack, drop_rcode(top_ptr(stack)->u.quote_));
}

static void doDip(Stack *stack, BResult *bresult)
{
    // a [B] dip => B a

    const Value b_quote = drop(stack);
    const Value a_value = drop(stack);

    doUnquote(stack, &b_quote, bresult);
    push(stack, a_value);
}

static void doSap(Stack *stack, BResult *bresult)
{
    // [A] [B] sap => B A

    Value b = drop(stack);
    Value a = drop(stack);

    doUnquote(stack, &b, bresult);
    doUnquote(stack, &a, bresult);
}

// Is lazy
static void doTake(Stack *stack, BResult *bresult)
{
    // A [B] take => [B A]

    push_rcode(top_ptr(stack)->u.quote_, *topx_ptr(stack, 2));
    doSwap(stack);
    pop(stack);
}

// Is not lazy
static void doTakex(Stack *stack, BResult *bresult)
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

static void doRep(Stack *stack, BResult *bresult)
{
    // [A] rep => A A

    Value to_apply = drop(stack);
    doUnquote(stack, &to_apply, bresult);
    doUnquote(stack, &to_apply, bresult);
}

static void doKap(Stack *stack, BResult *bresult)
{
    // [A] kap => [A] A

    Value to_apply = drop(stack);
    push(stack, to_apply);
    doUnquote(stack, &to_apply, bresult);
}

static void doPak(Stack *stack, BResult *bresult)
{
    // [A] pak => A [A]

    Value to_apply = drop(stack);
    doUnquote(stack, &to_apply, bresult);
    push(stack, to_apply);
}

static void doQap(Stack *stack)
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
static void doComb_map(Stack *stack, BResult *bresult)
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

static void doRec(Stack *stack, BResult *bresult)
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

static void doStep(Stack *stack, BResult *bresult)
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

static void doSteq(Stack *stack, BResult *bresult)
{
    Value program = drop(stack);
    const Value seq = drop(stack);

    extend_size_RawCode((RawCode *)stack, seq.u.quote_->used);

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

static void doVq(Stack *stack)
{
    // This is just a void quote : [ ]

    RawCode *quote = malloc(sizeof *quote);
    init_rcode(quote, 1);
    push(stack, make_RCL_Value_Quotation(quote));
}

static void do_nprec(Stack *stack)
{
    // x y nprec
    // Set prec Y to X
    if (topx_ptr(stack, 2)->kind == RCL_Value_Float)
        mpf_set_prec(topx_ptr(stack, 2)->u.float_, (mp_bitcnt_t)mpz_get_d(drop(stack).u.int_));
}

static void do_ifte(Stack *stack, BResult *bresult)
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

static void do_puts(Stack *stack)
{
    const String str = drop(stack).u.string_;
    fwrite(str, sizeof(char), strlen(str), stdout);
}

static void do_cmp_ii(Stack *stack, const Value a, const Value b, const Combinator cmp_kind)
{
    const bool is_eq_ii = mpz_cmp(a.u.int_, b.u.int_) == 0 ? true : false;

    if (is_eq_ii && (cmp_kind == LEQ || cmp_kind == GEQ || cmp_kind == EQ))
        return push(stack, RCL_Integer_I((int)is_eq_ii));

    else if (cmp_kind == NEQ)
        return push(stack, RCL_Integer_I((int)!is_eq_ii));

    switch (cmp_kind)
    {
    case LW:
    case LEQ:
        push(stack, RCL_Integer_I((int)(mpz_cmp(a.u.int_, b.u.int_) < 0 ? true : false)));
        return;

    case GR:
    case GEQ:
        push(stack, RCL_Integer_I((int)(mpz_cmp(a.u.int_, b.u.int_) > 0 ? true : false)));
        return;

    default:
        return push(stack, RCL_Integer_I((int)false));
    }
}

static void do_cmp_ff(Stack *stack, const Value a, const Value b, const Combinator cmp_kind)
{
    const bool is_eq_ff = mpf_cmp(a.u.float_, b.u.float_) == 0 ? true : false;

    if (is_eq_ff && (cmp_kind == LEQ || cmp_kind == GEQ || cmp_kind == EQ))
        return push(stack, RCL_Integer_I((int)is_eq_ff));

    else if (cmp_kind == NEQ)
        return push(stack, RCL_Integer_I((int)!is_eq_ff));

    switch (cmp_kind)
    {
    case LW:
    case LEQ:
        push(stack, RCL_Integer_I((int)(mpf_cmp(a.u.float_, b.u.float_) < 0 ? true : false)));
        return;

    case GR:
    case GEQ:
        push(stack, RCL_Integer_I((int)(mpf_cmp(a.u.float_, b.u.float_) > 0 ? true : false)));
        return;

    default:
        return push(stack, RCL_Integer_I((int)false));
    }
}

static void do_cmp_if(Stack *stack, const Value a, const Value b, const Combinator cmp_kind)
{
    // TODO
    // Convert to string for cmp?
    // https://gmplib.org/manual/Assigning-Integers.html#Assigning-Integers
}

static void do_cmp_fi(Stack *stack, const Value a, const Value b, const Combinator cmp_kind)
{
    // TODO
    // Convert to string for cmp?
    // https://gmplib.org/manual/Assigning-Integers.html#Assigning-Integers
}

static void do_cmp(Stack *stack, const Combinator cmp_kind)
{
    const Value a = drop(stack);
    const Value b = drop(stack);

    if (cmp_kind == EQ && (a.kind != RCL_Value_Integer || b.kind != RCL_Value_Integer))
        return push(stack, RCL_Integer_I(cmpvalue(a, b)));

    if (a.kind == RCL_Value_Integer && b.kind == RCL_Value_Integer)
        return do_cmp_ii(stack, a, b, cmp_kind);

    if (a.kind == RCL_Value_Integer && b.kind == RCL_Value_Float)
        return do_cmp_if(stack, a, b, cmp_kind);

    if (a.kind == RCL_Value_Float && b.kind == RCL_Value_Integer)
        return do_cmp_fi(stack, a, b, cmp_kind);

    if (a.kind == RCL_Value_Float && b.kind == RCL_Value_Float)
        return do_cmp_ff(stack, a, b, cmp_kind);
    else
        return (void)_internal_error(_is_Field, __LINE__, __FUNCTION_NAME__, "Can't apply comparaison between `%s' and `%s'.", show_value(b), show_value(a));
}

static void do_not(Stack *stack)
{
    const Value a = drop(stack);

    if (a.kind == RCL_Value_Integer)
        return push(stack, RCL_Integer_I(!mpz_get_d(a.u.int_)));

    if (a.kind == RCL_Value_Float)
        return push(stack, RCL_Integer_I(!mpf_get_d(a.u.float_)));
}

inline void simple_eval_rcode(Stack *stack, const RawCode rcode, BResult *bresult)
{
    for (Iterator i = 0; i < rcode.used; i++)
        evalop(stack, &rcode.array[i], bresult);
}

static void do_wtdo(Stack *stack, BResult *bresult)
{

    // [condition] [body] wtdo
    // While condition is true => execute body.

    const RawCode body = *drop(stack).u.quote_;
    const RawCode cond = *drop(stack).u.quote_;

eval_cond:

    simple_eval_rcode(stack, cond, bresult);

    if (!mpz_cmp_d(drop(stack).u.int_, true))
    {
        simple_eval_rcode(stack, body, bresult);
        goto eval_cond;
    }
}

struct th_simple_eval_rcode_args_t
{
    Stack *stack;
    const RawCode rcode;
    BResult *bresult;
};

static void th_simple_eval_rcode(struct th_simple_eval_rcode_args_t args)
{
    simple_eval_rcode(args.stack, args.rcode, args.bresult);
}

static void do_stoi(Stack *stack)
{
    const Value v = drop(stack);
    assert(v.kind == RCL_Value_String);
    const String s = v.u.string_;
    mpz_t res;
    mpz_init_set_str(res, s, 10);
    push(stack, RCL_Integer(res));
}

static void do_itos(Stack *stack)
{
    const Value v = drop(stack);
    assert(v.kind == RCL_Value_Integer);
    String res = mpz_get_str(res, 10, v.u.int_);
    push(stack, RCL_String(res));
}

inline void doComb(Stack *stack, const Combinator comb, BResult *bresult)
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
        return do_puts(stack);

    case NVSHOW:
        push(stack, make_RCL_Value_String(show_value(drop(stack))));
        break;

    case IFTE:
        return do_ifte(stack, bresult);

    case EQ ... NEQ:
        return do_cmp(stack, comb);

    case NOT:
        return do_not(stack);

    case QLEN:
        return push(stack, RCL_Integer_I(drop(stack).u.quote_->used));

    case ALEN:
        return push(stack, RCL_Integer_I(drop(stack).u.array_.length));

    case HALT:
        exit(EXIT_SUCCESS);

    case WTDO:
        return do_wtdo(stack, bresult);

    case SELECT:
    case CASE:
    case GENREC:
    case LINREC: // https://hypercubed.github.io/joy/html/j05cmp.html
    case BINREC:
    case TAILREC:
    case PRIMREC:

    case STOI:
        return do_stoi(stack);

    case ITOS:
        return do_itos(stack);

    case FTOI:
    case ITOF:
    case ITOC:
        return push(stack, RCL_Char(mpz_get_d(drop(stack).u.int_)));

    case CTOI:
    case ITOB:
        printf("todo");
        break;

    default:
        _interr("Unknown combinator kind: %d.", comb);
    }
}
