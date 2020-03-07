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
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <Tools\Map\Map.h>
#include <Tools\Utils\Utils.h>
#include <Tools\Internal\Internal.h>
#include <Tools\Vector\Vector.h>
#include <VM\Core\State\State.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\Checker\Typechecker\Types\Show.h>
#include <VM\Core\Checker\Typechecker\Types\Infer.h>
#include <VM\Core\Checker\Typechecker\Types\Types.h>
#include <VM\Core\Checker\Typechecker\Types\TypeError.h>

// This is the type of the stack, evolving as the expressions are analyzed.
typedef Vector(type_stack_t, RCL_Type);
// The context is a dictionary between variable types and real types.
Map(Context, TVar_t, RCL_Type);

struct infer_type__args_t
{
    // The contect of types variables
    Context_map_t *context_ptr;
    // The environment of functions and typed names
    Env_map_t *env_ptr;
    // The type of the expression to be infered
    // It is updated at each new operation on the stack
    type_stack_t *ts_ptr;
    // The state of errorsn warnings and information
    struct State *state;

    // The type of the current operation or
    // value on the expression to be infered
    RCL_Type current_type;
    // The arity of type
    size_t current_arity;
    // The trace of type
    size_t current_trace;
    // The current value
    Value current_val;

    // On the for loop that analyse and infer the expression
    // this is the current index of expression
    Iterator current_itr;
    // The length of the whole expression
    size_t rcode_len;
};

typedef struct infer_type__args_t args_t;

static void infer_type__arrow_on_tstack(args_t *);
static void infer_type__arrow(args_t *);

bool cmp_tvars(const TVar_t t1, const TVar_t t2) { return t1 == t2; }

static String show_tstack(const type_stack_t ts)
{
    String res = "";
    for (Iterator i = 0; i < ts.used; i++)
        rcl_asprintf(&res, "%s . %s", res, show_type(ts.array[i]));
    return res;
}

// Returns the next variable type depending on the context.
// For example the next variable type with a context containing [a, b] will be `c`.
TVar_t set_tvar_str(Context_map_t *context_ptr)
{
    if (empty_Context(context_ptr))
        return 'a';

    TVar_t c = get_last_key_Context(context_ptr) + sizeof(char);
    if (c > 'z')
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Too large context of variable types.\n", NULL);

    return c;
}

// Deletes a variable type and its context mapping.
void remove_tvar_str(Context_map_t *context_ptr, TVar_t tvar)
{
    const int found = key_find_Context(context_ptr, tvar, &cmp_tvars);

    if (found == map_unfound)
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Unfound key: %c.", tvar);

    for (Iterator i = found; i < length_Context(context_ptr) - 1; i++)
        context_ptr->array[i] = context_ptr->array[i + 1];
}

// Individually places all types of a concatenation type in a vector.
// For example if concatenation type is `t1 . t2 . t3`, the returned vector
//  will be [t1, t2, t3].
type_stack_t individualize(Context_map_t *context_ptr, RCL_Type t)
{
    type_stack_t result;
    if (t.kind != TYPE_STACK)
    {
        init_type_stack_t(&result, 1);
        if (t.kind == TYPE_ANY)
            push_type_stack_t(&result, t);
    }
    else
    {
        init_type_stack_t(&result, t.u.rcl_type_stack_.nbr);
        for (Iterator i = 0; i < t.u.rcl_type_stack_.nbr; i++)
            push_type_stack_t(&result, *t.u.rcl_type_stack_.tstack[i]);
    }
    return result;
}

// Checks if stack's types matches the constant vector of type sent.
static bool stackt_match_vecT(type_stack_t stackt, RCL_Type *vecT)
{
    for (Iterator i = 0; i < stackt.used; i++)
        if (!cmp_types(stackt.array[i], vecT[i]))
            return false;
    return true;
}

static RCL_Type unify_tlit(const RCL_Type t1, const RCL_Type t2, struct State *state)
{
    if (t2.u.rcl_type_literal_.tlit == RCL_Value_Number)
    {
        if (t1.u.rcl_type_literal_.tlit == RCL_Value_Integer)
            return T_LITERAL(RCL_Value_Integer);

        if (t1.u.rcl_type_literal_.tlit == RCL_Value_Float)
            return T_LITERAL(RCL_Value_Float);
    }
    if (t1.u.rcl_type_literal_.tlit == RCL_Value_Number)
    {
        if (t2.u.rcl_type_literal_.tlit == RCL_Value_Integer)
            return T_LITERAL(RCL_Value_Integer);

        if (t2.u.rcl_type_literal_.tlit == RCL_Value_Float)
            return T_LITERAL(RCL_Value_Float);
    }
    state_put_err_ch_cst(state, "Can't unify `%s` with `%s`.\n", show_type(t2), show_type(t1));
    return T_ERR;
}

// Unifies type T1 with type T2
static RCL_Type unify(const RCL_Type t1, const RCL_Type t2, struct State *state)
{
    if (cmp_types(t1, t2))
        return t1;

    if (t2.kind == TYPE_ANY)
        return t1;

    if (t2.kind == TYPE_LITERAL)
        return unify_tlit(t1, t2, state);

    state_put_err_ch_cst(state, "Can't unify `%s` with `%s`.\n", show_type(t2), show_type(t1));
    return T_ERR;
}

static void fill_type_stack_tany(args_t *args, const RCL_Type current_type, type_stack_t *dest)
{
    if (key_exist_Context(args->context_ptr, current_type.u.rcl_type_any_.tany, &cmp_tvars))
    {
        const RCL_Type val_context = get_val_Context(args->context_ptr, current_type.u.rcl_type_any_.tany, &cmp_tvars);
        const RCL_Type unified = unify(val_context, current_type, args->state);
        push_type_stack_t(dest, unified);
    }
}

static void fill_type_stack(args_t *args, type_stack_t *dest, const type_stack_t *src)
{
    // `dest` is the type stack
    // `src` is the arguments of function

    for (Iterator i = 0; i < src->used; i++)
    {
        if (src->array[i].kind == TYPE_EMPTY) // This will just take out the useless empty stack types.
            continue;                         // For example `pop : a -> ()`, we don't need the info of `()`.

        else if (src->array[i].kind == TYPE_ANY)
            //fill_type_stack_tany(src->array[i], dest, src, context_ptr, state);
            fill_type_stack_tany(args, src->array[i], dest);

        else if (src->array[i].kind == TYPE_LITERAL)
            push_type_stack_t(dest, unify(src->array[i], dest->array[i], args->state));

        else
            push_type_stack_t(dest, src->array[i]);
    }
}

static void update_type_stack__total(args_t *args, const type_stack_t *src)
{
    int curr_arity_cpy = args->current_arity;
    while (curr_arity_cpy-- && args->ts_ptr->used)
        pop_type_stack_t(args->ts_ptr);

    fill_type_stack(args, args->ts_ptr, src);
}

static void update_type_stack__partial(args_t *args, const type_stack_t *src, const type_stack_t *targs)
{
    const size_t ts_len = args->ts_ptr->used;
    const size_t inlack = args->current_arity - ts_len;
    const bool ret_void = src->used == 0;

    type_stack_t tmp_t1 = new_type_stack_t(inlack);

    for (Iterator i = 0; i < inlack; i++)
        push_type_stack_t(&tmp_t1, targs->array[i]);

    // Updating context:

    const int sub_ts = args->current_arity - ts_len;

    for (Iterator i = args->ts_ptr->used; i >= /* > */ sub_ts - 1; i--)
        add_Context(args->context_ptr, src->array[i - 1].u.rcl_type_any_.tany, args->ts_ptr->array[i - 1]);

    type_stack_t tmp_t2 = new_type_stack_t(src->used);

    for (Iterator i = 0; i < src->used; i++)
    {
        if (src->array[i].kind == TYPE_ANY)
        {
            const int found = key_find_Context(args->context_ptr, src->array[i].u.rcl_type_any_.tany, &cmp_tvars);
            if (found != map_unfound)
            {
                push_type_stack_t(&tmp_t2, args->context_ptr->array[i].val);
                continue;
            }
        }
        push_type_stack_t(&tmp_t2, src->array[i]);
    }

    RCL_Type *t1 = (RCL_Type *)malloc(sizeof *t1);
    RCL_Type *t2 = (RCL_Type *)malloc(sizeof *t2);
    *t1 = make_RCL_Type_stack(tmp_t1.used, tmp_t1.array);
    *t2 = make_RCL_Type_stack(tmp_t2.used, tmp_t2.array);
    RCL_Type final_type = T_ARROW_PTR(t1, t2);

    init_type_stack_t(args->ts_ptr, 1);
    push_type_stack_t(args->ts_ptr, final_type);
}

static void update_type_stack(args_t *args, const type_stack_t *src, const type_stack_t *targs)
{
    const bool is_partial = args->current_arity > args->ts_ptr->used;

    if (is_partial)
        update_type_stack__partial(args, src, targs);
    else
        update_type_stack__total(args, src);
}

static void subst(Context_map_t *context_ptr, const type_stack_t *stackt_ptr, const type_stack_t *targs, const size_t arity)
{
    const int sub_ts = stackt_ptr->used - arity;

    for (Iterator i = sub_ts; i < stackt_ptr->used; i++)
        add_Context(context_ptr, set_tvar_str(context_ptr), stackt_ptr->array[i]);
}

static RCL_Type type_of_quote(Env_map_t *env, RawCode *rcode, struct State *state)
{
    RCL_Type *quote_type = (RCL_Type *)malloc(sizeof(*quote_type));
    *quote_type = infer_type(env, rcode, state);
    return T_QUOTE(*quote_type);
}

static args_t new_uncurrent_infert_args(Env_map_t *env_ptr, Context_map_t *context_ptr, type_stack_t *ts_ptr, struct State *state, size_t rcode_len)
{
    args_t res;
    res.env_ptr = env_ptr;
    res.context_ptr = context_ptr;
    res.ts_ptr = ts_ptr;
    res.state = state;
    res.rcode_len = rcode_len;
    return res;
}

static void fill_current_infert_args(const Value curr_val, args_t *args_ptr, RCL_Type curr_type, size_t curr_arity, size_t curr_trace, Iterator itr)
{
    args_ptr->current_arity = curr_arity;
    args_ptr->current_trace = curr_trace;
    args_ptr->current_type = curr_type;
    args_ptr->current_itr = itr;
    args_ptr->current_val = curr_val;
}

static void infer_type__quote(type_stack_t *ts_ptr, Env_map_t *env_ptr, const Value current_val, struct State *state)
{
    push_type_stack_t(ts_ptr, type_of_quote(env_ptr, current_val.u.quote_, state));
}

static void inferiT(args_t *args)
{
    if (args->current_val.kind == RCL_Value_Quotation)
        return infer_type__quote(args->ts_ptr, args->env_ptr, args->current_val, args->state);

    if (args->ts_ptr->used > 0)
        if (top_ptr_type_stack_t(args->ts_ptr)->kind == TYPE_ARROW)
            return infer_type__arrow_on_tstack(args);

    if (args->current_type.kind == TYPE_ARROW)
        return infer_type__arrow(args);

    return push_type_stack_t(args->ts_ptr, args->current_type);
}

// 4 'x' flip flip
// flip_1 => a -> Char Int a
// flip_2 => Char Int a -> a Int Char
static void infer_type__arrow_on_tstack(args_t *args)
{
    const RCL_Type ts_top = *top_ptr_type_stack_t(args->ts_ptr);
    RCL_Type *t1 = (RCL_Type *)malloc(sizeof *t1);
    *t1 = *ts_top.u.rcl_type_arrow_.t1;
    type_stack_t t2_indvs = individualize(args->context_ptr, *ts_top.u.rcl_type_arrow_.t2);

    args_t tmp_args = new_uncurrent_infert_args(args->env_ptr, args->context_ptr, &t2_indvs, args->state, args->rcode_len);
    fill_current_infert_args(args->current_val, &tmp_args, args->current_type, args->current_arity, args->current_trace, args->current_itr);

    inferiT(&tmp_args);

    RCL_Type *t2 = (RCL_Type *)malloc(sizeof *t2);
    *t2 = make_RCL_Type_stack(t2_indvs.used, t2_indvs.array);
    const RCL_Type final_type = T_ARROW_PTR(t1, t2);
    *args->ts_ptr = new_type_stack_t(1);
    push_type_stack_t(args->ts_ptr, final_type);
}

static void infer_type__arrow(args_t *args)
{
    type_stack_t targs = individualize(args->context_ptr, *SIGMA_GETV_BYVAL(args->current_type, rcl_type_arrow, t1));
    type_stack_t new_types = individualize(args->context_ptr, *SIGMA_GETV_BYVAL(args->current_type, rcl_type_arrow, t2));

    subst(args->context_ptr, args->ts_ptr, &targs, args->current_arity);
    update_type_stack(args, &new_types, &targs);

    // To reconsider:
    free_Context(args->context_ptr);
    init_Context(args->context_ptr, args->rcode_len - args->current_itr);
}

RCL_Type infer_type(Env_map_t *env, RawCode *rcode, struct State *state)
{
    const size_t rcode_len = rcode->used;

    type_stack_t vacc_type_stack;
    init_type_stack_t(&vacc_type_stack, rcode_len);

    Context_map_t context;
    init_Context_map_t(&context, rcode_len);

    args_t infert_args = new_uncurrent_infert_args(env, &context, &vacc_type_stack, state, rcode_len);

    for (Iterator i = 0; i < rcode_len; i++)
    {
        const Value current_val = rcode->array[i];
        const RCL_Type current_type = type_of(&current_val, NULL, NULL);
        const size_t current_arity = arity(current_type);
        const size_t current_trace = trace(current_type);

        fill_current_infert_args(current_val, &infert_args, current_type, current_arity, current_trace, i);

        inferiT(&infert_args);
    }

    if (vacc_type_stack.used == 0)
        return T_EMPTY;

    return make_RCL_Type_stack(vacc_type_stack.used, vacc_type_stack.array);
}

/* --- Note to self:

Take care about T_ARROW and T_ARROW_PTR when the value to be put in this type
need to be a pointer.

*/

/* TODO:

    + When expression returns nothing, like `pop`
    + When we can unify past expression type with actual, like `dup +` ==> Num
    + When function asking for more arguments, like `swap flip`
    + Context freeing (linked to ^)
    + General environment

 */
