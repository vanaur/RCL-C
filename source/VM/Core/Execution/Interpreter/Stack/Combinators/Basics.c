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
#include <VM\Core\Execution\Interpreter\Arithmetic.h>
#include <VM\Core\Execution\Interpreter\Stack\Stack.h>
#include <VM\Core\Execution\Interpreter\Stack\Combinators\Basics.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\RawCode\cmpvalue.h>
#include <VM\Core\RawCode\cpyvalue.h>

static void doDup(Stack *stack)
{
    rcl_assert(stack->used >= 1);
    Value *value = malloc(sizeof *value);
    valcpy(value, top_ptr(stack));
    push(stack, *value);
    //! `push(stack, *top_ptr(stack))`: cannot be done because some operations change the pointers for optimizations.
}

static void doSwap(Stack *stack)
{
    rcl_assert(stack->used >= 2);
    Value tmp = *topx_ptr(stack, 1);
    *topx_ptr(stack, 1) = *topx_ptr(stack, 2);
    *topx_ptr(stack, 2) = tmp;
}

static void doFlip(Stack *stack)
{
    rcl_assert(stack->used >= 3);
    Value tmp = *topx_ptr(stack, 1);
    *topx_ptr(stack, 1) = *topx_ptr(stack, 3);
    *topx_ptr(stack, 3) = tmp;
}

static void doGis(Stack *stack)
{
    rcl_assert(stack->used >= 2);

    const Value from_val = drop(stack);
    const Value until_val = drop(stack);

    rcl_assert(from_val.kind == RCL_Value_Integer);
    rcl_assert(until_val.kind == RCL_Value_Integer);

    const int from = mpz_get_d(from_val.u.int_);
    const int until = mpz_get_d(until_val.u.int_);

    const int size = until - from;

    if (size <= 0)
        return push(stack, RCL_Combinator(VQ));

    RawCode *quote = malloc(sizeof(*quote));
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
    rcl_assert(stack->used >= 1);
    RawCode *quote = malloc(sizeof(*quote));
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
    rcl_assert(stack->used >= 1);
    return;
}

static void doCat(Stack *stack)
{
    rcl_assert(stack->used >= 2);

    Value *s1 = topx_ptr(stack, 2);
    Value *s2 = top_ptr(stack);

    rcl_assert(s1->kind == RCL_Value_Quotation || s1->kind == RCL_Value_String);
    rcl_assert(s2->kind == RCL_Value_Quotation || s2->kind == RCL_Value_String);

    if (s1->kind == RCL_Value_Quotation && s2->kind == RCL_Value_Quotation)
        return concat_rcode(s1->u.quote_, drop(stack).u.quote_);
    if (s1->kind == RCL_Value_String && s2->kind == RCL_Value_String)
        return (void)strcat(s1->u.string_, drop(stack).u.string_); // TODO: Use 'open_memstream' (...) instead of strcat
    if (s1->kind == RCL_Value_String && s2->kind == RCL_Value_Quotation)
        return (void)strcat(s1->u.string_, quote_of_char_to_string(*drop(stack).u.quote_).u.string_);
    *s1 = quote_of_char_to_string(*s1->u.quote_);
    strcat(s1->u.string_, drop(stack).u.string_);
}

static void doCons(Stack *stack)
{
    // [A] [B] cons => [[A] B]

    rcl_assert(stack->used >= 2);

    doSwap(stack);
    doQuote(stack);
    doSwap(stack);
    doCat(stack);
}

static void doUncons_string(Stack *stack)
{
    RCL_Value_String_t tmp = top_ptr(stack)->u.string_;
    const size_t len = strlen(tmp);
    if (len == 0) // ? Good iead? Or assert?
        return push(stack, RCL_Word(RCL_NIL_WRD));
    push(stack, RCL_Char(tmp[len - 1]));
    tmp[len - 1] = 0;
}

static void doUncons_quote(Stack *stack)
{
    RCL_Value_Quote_t tmp = top_ptr(stack)->u.quote_;
    if (tmp->used == 0) // ? Good iead? Or assert?
        return push(stack, RCL_Word(RCL_NIL_WRD));
    push(stack, tmp->array[tmp->used - 1]);
    pop_rcode(topx_ptr(stack, 2)->u.quote_);
}

static void doUncons(Stack *stack)
{
    rcl_assert(stack->used >= 1);
    rcl_assert(top_ptr(stack)->kind == RCL_Value_String || top_ptr(stack)->kind == RCL_Value_Quotation);
    if (top_ptr(stack)->kind == RCL_Value_String)
        return doUncons_string(stack);
    return doUncons_quote(stack);
}

static void doDip(Stack *stack, BResult *bresult)
{
    // a [B] dip => B a

    rcl_assert(stack->used >= 2);

    const Value b_quote = drop(stack);
    const Value a_value = drop(stack);

    rcl_assert(b_quote.kind == RCL_Value_Quotation);

    doUnquote(stack, &b_quote, bresult);
    push(stack, a_value);
}

static void doSap(Stack *stack, BResult *bresult)
{
    // [A] [B] sap => B A

    rcl_assert(stack->used >= 2);

    Value b = drop(stack);
    Value a = drop(stack);

    rcl_assert(a.kind == RCL_Value_Quotation);
    rcl_assert(b.kind == RCL_Value_Quotation);

    doUnquote(stack, &b, bresult);
    doUnquote(stack, &a, bresult);
}

// Is lazy
static void doTake(Stack *stack, BResult *bresult)
{
    // A [B] take => [B A]

    rcl_assert(stack->used >= 2);

    Value *q_val = top_ptr(stack);

    rcl_assert(q_val->kind == RCL_Value_Quotation);

    push_rcode(q_val->u.quote_, *topx_ptr(stack, 2));
    doSwap(stack);
    pop(stack);
}

// Is not lazy
static void doTakex(Stack *stack, BResult *bresult)
{
    // [A] [B] take => [B [A]]

    RawCode *result = malloc(sizeof(*result));
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

    rcl_assert(stack->used >= 1);

    Value to_apply = drop(stack);

    rcl_assert(to_apply.kind == RCL_Value_Quotation);

    doUnquote(stack, &to_apply, bresult);
    doUnquote(stack, &to_apply, bresult);
}

static void doKap(Stack *stack, BResult *bresult)
{
    // [A] kap => [A] A

    rcl_assert(stack->used >= 1);

    Value to_apply = drop(stack);

    rcl_assert(to_apply.kind == RCL_Value_Quotation);

    push(stack, to_apply);
    doUnquote(stack, &to_apply, bresult);
}

static void doPak(Stack *stack, BResult *bresult)
{
    // [A] pak => A [A]

    rcl_assert(stack->used >= 1);

    Value to_apply = drop(stack);

    rcl_assert(to_apply.kind == RCL_Value_Quotation);

    doUnquote(stack, &to_apply, bresult);
    push(stack, to_apply);
}

static void doQap(Stack *stack)
{
    // A [B] qap => [A B]

    rcl_assert(stack->used >= 2);

    RawCode *result = malloc(sizeof(*result));
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

    RawCode *result = malloc(sizeof(*result));
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

    rcl_assert(stack->used >= 1);

    RawCode *result = malloc(sizeof(*result));
    init_rcode(result, 2);

    Value quote = drop(stack);
    push_rcode(result, quote);
    push_rcode(result, RCL_Combinator(REC));

    push(stack, make_RCL_Value_Quotation(result));
    doUnquote(stack, &quote, bresult);

    free(result);
}

static void doStep_string(Stack *stack, const Value program, const RCL_Value_String_t seq_s, BResult *bresult)
{
    const size_t len = strlen(seq_s);

    for (Iterator i = 0; i < len; i++)
    {
        push(stack, RCL_Char(seq_s[i]));
        doUnquote(stack, &program, bresult);
    }
}

static void doStep_quote(Stack *stack, const Value program, const RCL_Value_Quote_t seq, BResult *bresult)
{
    for (Iterator i = 0; i < seq->used; i++)
    {
        push(stack, seq->array[i]);
        doUnquote(stack, &program, bresult);
    }
}

static void doStep(Stack *stack, BResult *bresult)
{
    rcl_assert(stack->used >= 2);

    const Value program = drop(stack);
    const Value seq = drop(stack);

    rcl_assert(program.kind == RCL_Value_Quotation);
    rcl_assert(seq.kind == RCL_Value_Quotation || seq.kind == RCL_Value_String);

    if (seq.kind == RCL_Value_String)
        return doStep_string(stack, program, seq.u.string_, bresult);
    return doStep_quote(stack, program, seq.u.quote_, bresult);
}

static void doSteq_quote(Stack *stack, BResult *bresult, const Value program, const RCL_Value_Quote_t seq)
{
    const size_t len = seq->used;

    Stack tmp;
    init_stack(&tmp, len);

    for (Iterator i = 0; i < len; i++)
    {
        push(&tmp, seq->array[i]);
        doUnquote(&tmp, &program, bresult);
    }

    RawCode *content = malloc(sizeof *content);
    *content = stack_to_rcode(tmp);
    free(tmp.array);
    push(stack, RCL_Quotation(content));
}

static void doSteq_string(Stack *stack, BResult *bresult, const Value program, const RCL_Value_String_t str)
{
    const size_t len = strlen(str);

    Stack tmp;
    init_stack(&tmp, len);

    for (Iterator i = 0; i < len; i++)
    {
        push(&tmp, RCL_Char(str[i]));
        doUnquote(&tmp, &program, bresult);
    }

    RawCode *content = malloc(sizeof *content);
    *content = stack_to_rcode(tmp);
    free(tmp.array);
    const Value sv = quote_of_char_to_string(*content);
    push(stack, sv);
}

static void doSteq(Stack *stack, BResult *bresult)
{
    rcl_assert(stack->used >= 2);

    Value program = drop(stack);
    Value seq = drop(stack);

    rcl_assert(program.kind == RCL_Value_Quotation);
    rcl_assert(seq.kind == RCL_Value_String || seq.kind == RCL_Value_Quotation);

    if (seq.kind == RCL_Value_String)
        return doSteq_string(stack, bresult, program, seq.u.string_);
    doSteq_quote(stack, bresult, program, seq.u.quote_);
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
    rcl_assert(stack->used >= 3);

    // [condition] [then] [else] ifte
    const Value _else = drop(stack);
    const Value _then = drop(stack);
    const Value _cond = drop(stack);

    rcl_assert(_else.kind == RCL_Value_Quotation);
    rcl_assert(_then.kind == RCL_Value_Quotation);
    rcl_assert(_cond.kind == RCL_Value_Quotation);

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
    rcl_assert(stack->used >= 1);
    const Value val = drop(stack);
    rcl_assert(val.kind == RCL_Value_String);
    const String str = val.u.string_;
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
    rcl_assert(stack->used >= 2);

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
    rcl_assert(stack->used >= 1);

    const Value a = drop(stack);

    rcl_assert(a.kind == RCL_Value_Integer || a.kind == RCL_Value_Float);

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

    rcl_assert(stack->used >= 2);

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

/*
    4
    [7 5 9]
    [["This is 7"] ["This is 5"] ["This is 9"]]
    ["Unknown pattern!"]
    select
*/

static void do_select(Stack *stack, BResult *bresult)
{
    rcl_assert(stack->used >= 4);
    const Value otherwise = drop(stack);
    const RawCode results = *drop(stack).u.quote_;
    const RawCode cases = *drop(stack).u.quote_;
    const Value value = drop(stack);
    rcl_assert(results.used == cases.used);
    Iterator i = 0;
    for (i; i < cases.used; i++)
        if (cmpvalue(cases.array[i], value))
            break;
    if (i == cases.used)
        return doUnquote(stack, &otherwise, bresult);
    doUnquote(stack, &results.array[i], bresult);
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
        const Value quote = drop(stack);
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
        return push(stack, RCL_Integer_I(top_ptr(stack)->kind == RCL_Value_String ? strlen(drop(stack).u.string_) : drop(stack).u.quote_->used));

    case ALEN:
        return push(stack, RCL_Integer_I(drop(stack).u.array_.length));

    case HALT:
        exit(EXIT_SUCCESS);

    case WTDO:
        return do_wtdo(stack, bresult);

    case SELECT:
        return do_select(stack, bresult);

        // TODO:

    case CASE:
    case GENREC:
    case LINREC: // https://hypercubed.github.io/joy/html/j05cmp.html
    case BINREC:
    case TAILREC:
    case PRIMREC:

        /* conversion combinators */

    case STOI:
        return do_stoi(stack);

    case ITOS:
        return do_itos(stack);

    case FTOI:
        __fast_assign(*top_ptr(stack), RCL_Integer(*ftoz(top_ptr(stack)->u.float_)));
        break;

    case ITOF:
        __fast_assign(*top_ptr(stack), RCL_Float(*ztof(top_ptr(stack)->u.int_)));
        break;

    case ITOC:
        __fast_assign(*top_ptr(stack), RCL_Char((char)(mpz_get_d(top_ptr(stack)->u.int_))));
        break;

    case CTOI:
        __fast_assign(*top_ptr(stack), RCL_Integer_I((int)(top_ptr(stack)->u.char_)));
        break;

    case ITOB:
        __fast_assign(*top_ptr(stack), RCL_Integer_I((bool)(mpz_get_d(top_ptr(stack)->u.int_))));
        break;

    default:
        _interr("Unknown combinator kind: %d.", comb);
    }
}
