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

typedef char TVar_t;
Vector(type_stack_t, RCL_Type);
Map(Context, TVar_t, RCL_Type);

bool cmp_tvars(TVar_t t1, TVar_t t2)
{
    return t1 == t2;
}

TVar_t set_tvar_str(Context_map_t *context_ptr)
{
    if (empty_Context(context_ptr))
        return 'a';
    return get_last_key_Context(context_ptr) + sizeof(char);
}

void remove_tvar_str(Context_map_t *context_ptr, TVar_t tvar)
{
    int found = key_find_Context(context_ptr, tvar, &cmp_tvars);
    if (found == map_unfound)
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Unfound key: %c.", tvar);
    for (Iterator i = found; i < length_Context(context_ptr) - 1; i++)
        context_ptr->array[i] = context_ptr->array[i + 1];
}

struct type_stack_t individualize(RCL_Type t)
{
    struct type_stack_t result;
    if (t.kind != TYPE_STACK)
    {
        init_type_stack_t(&result, 1);
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

static bool stackt_match_arrayt(struct type_stack_t stackt, RCL_Type *arrayt)
{
    for (Iterator i = 0; i < stackt.used; i++)
        if (!cmp_types(stackt.array[i], arrayt[i]))
            return false;
    return true;
}

static RCL_Type unify(const RCL_Type t1, const RCL_Type t2, struct State *state)
{
    if (cmp_types(t1, t2))
        return t1;

    switch (t2.kind)
    {
    case TYPE_ANY:
        return t1;

    case TYPE_LITERAL:
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

    default:
        break;
    }
    //_put_new_typerr_cont("Can't unify `%s` with `%s`.", show_type(t2), show_type(t1));
    printf("Can't unify `%s` with `%s`.\n", show_type(t2), show_type(t1));
    return T_ERR;
}

static void fill_type_stack(struct type_stack_t *dest, const struct type_stack_t *src, Context_map_t *context_ptr)
{
    for (Iterator i = 0; i < src->used; i++)
    {
        if (src->array[i].kind == TYPE_EMPTY) // This will just take out the useless empty stack types.
            continue;                         // For example `pop : a -> ()`, we don't need the info of `()`.

        else if (src->array[i].kind == TYPE_ANY)
        {
            if (key_exist_Context(context_ptr, src->array[i].u.rcl_type_any_.tany[0], &cmp_tvars))
            {
                const RCL_Type val_context = get_val_Context(context_ptr, src->array[i].u.rcl_type_any_.tany[0], &cmp_tvars);
                const RCL_Type unified = unify(val_context, src->array[i], NULL);
                push_type_stack_t(dest, unified);
            }
            continue;
        }

        else if (src->array[i].kind == TYPE_LITERAL)
            push_type_stack_t(dest, unify(src->array[i], dest->array[i], NULL));

        else
            push_type_stack_t(dest, src->array[i]);
    }
}

static void update_type_stack(struct type_stack_t *ts_ptr, size_t arity_poper, const struct type_stack_t *src, Context_map_t *context_ptr)
{
    size_t arity_cpy = arity_poper;

    while (arity_poper-- && ts_ptr->used)
        pop_type_stack_t(ts_ptr);

    fill_type_stack(ts_ptr, src, context_ptr);
}

static void subst(Context_map_t *context_ptr, const struct type_stack_t *stackt_ptr, const struct type_stack_t *targs, const size_t arity)
{
    for (Iterator i = 0; i < arity; i++)
    {
        add_Context(context_ptr, set_tvar_str(context_ptr), stackt_ptr->array[i]);
        // if [i] already exists as TYPE_ANY, then change [i] to another letter
        /*         if (targs->array[i].kind == TYPE_ANY)
        {
            if (key_exist_Context(context_ptr, targs->array[i].u.rcl_type_any_.tany[0], &cmp_tvars))
            {
                update_val_Context(context_ptr, key_find_Context(context_ptr, targs->array[i].u.rcl_type_any_.tany[0], &cmp_tvars), stackt_ptr->array[i]);
                continue;
            }
            else
            {
                add_Context(context_ptr, set_tvar_str(context_ptr), stackt_ptr->array[i]);
            }
        }
        else
        {
            add_Context(context_ptr, set_tvar_str(context_ptr), stackt_ptr->array[i]);
        } */
    }

/*     for (Iterator i = 0; i < context_ptr->used; i++)
        printf("%c <=> %s\n", context_ptr->array[i].key, show_type(context_ptr->array[i].val));
    printf("\n"); */
}

RCL_Type infer_type(Env_map_t *env, RawCode *rcode, struct State *state)
{
    struct type_stack_t vacc_type_stack;
    init_type_stack_t(&vacc_type_stack, rcode->used);

    Context_map_t context;
    init_Context_map_t(&context, rcode->used);

    for (Iterator i = 0; i < rcode->used; i++)
    {
        RCL_Type current = type_of(&rcode->array[i], NULL, NULL);
        size_t current_arity = arity(current);

        if (current_arity > 0) // /!\ ca suppose pour le moment que c'est un type flèche
        {
            struct type_stack_t targs = individualize(*SIGMA_GETV_BYVAL(current, rcl_type_arrow, t1));
            struct type_stack_t new_types = individualize(*SIGMA_GETV_BYVAL(current, rcl_type_arrow, t2));

            subst(&context, &vacc_type_stack, &targs, current_arity);
            update_type_stack(&vacc_type_stack, current_arity, &new_types, &context);
            continue;
        }

        push_type_stack_t(&vacc_type_stack, current);
    }

    if (vacc_type_stack.used == 0)
        return T_EMPTY;
    return make_RCL_Type_stack(vacc_type_stack.used, vacc_type_stack.array);
}

// Remplacer les TYPE_ANY(String / Char) par Int qu'on incrémente ?
//  => Je pense que c'est mieux
