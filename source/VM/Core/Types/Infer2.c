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

#include <Tools\Utils\Utils.h>
#include <VM\Core\Types\Infer2.h>
#include <VM\Core\Types\Types.h>
#include <VM\Core\Types\Primitives.h>
#include <VM\Core\Types\Combinator.h>
#include <VM\Core\Types\Show.h>
#include <VM\Core\Show\Show.h>

// Type of a word (= variable, function, ...)
static RCL_Type gettype_word(const RCL_Value_Word_t word, const IdentifierTypeEnv_map_t *env_ptr)
{
    rcl_assert(word.hash_code != (hash_t)0);
    const int found = key_find_IdentifierTypeEnv(env_ptr, word.hash_code, &cmp_f_hash);
    if (found == map_unfound) // Maybe he hasn't been typed yet.
        return T_VARIABLE(word.word_str);
    return env_ptr->array[found].val;
}

// Type of concatenation (= sequence of operations)
static RCL_Type gettype_concatenation(const RCL_Value_Concatenation_t conc, const IdentifierTypeEnv_map_t *env_ptr, BResult *bresult)
{
    const size_t len = conc->used;
    RCL_Type *ts = malloc(len * sizeof *ts);
    for (Iterator i = 0; i < len; i++)
        ts[i] = gettype(conc->array[i], env_ptr, bresult);
    return make_RCL_Type_stack(len, ts);
}

// Type of quotation
static RCL_Type gettype_quote(const RCL_Value_Quote_t quote, const IdentifierTypeEnv_map_t *env_ptr, BResult *bresult)
{
    const size_t len = quote->used;

    if (len == 0)
        return T_QUOTE(T_EMPTY);

    RCL_Type *types = malloc(len * sizeof *types);
    for (Iterator i = 0; i < len; i++)
        types[i] = gettype(quote->array[i], env_ptr, bresult);

    RCL_Type_ptr ts = malloc(sizeof *ts);
    *ts = make_RCL_Type_stack(len, types);

    return T_QUOTE_PTR(ts);
}

// Type of array
static RCL_Type gettype_array(const RCL_Value_Array_t array, const IdentifierTypeEnv_map_t *env_ptr, BResult *bresult)
{
    const size_t len = array.length;

    if (len == 0)
        return T_ARRAY(T_EMPTY);

    RCL_Type_ptr t0 = malloc(sizeof *t0);
    *t0 = gettype_concatenation(RCL_ARRAY_CONTENT_CASE_FROM_U(array, 0), env_ptr, bresult);

    if (len == 1)
        return T_ARRAY_PTR(t0);

    for (Iterator i = 1; i < len; i++)
    {
        const RCL_Type ti = gettype_concatenation(RCL_ARRAY_CONTENT_CASE_FROM_U(array, i), env_ptr, bresult);
        if (!cmp_types(*t0, ti, false))
        {
            state_put_err_ch("Expected type `%s' in array, not `%s' for the array %s at the %s case.",
                             show_type(*t0),
                             show_type(ti),
                             show_value(make_RCL_Value_Array(array.array, array.length)),
                             indexToWord(i + 1));
            return T_ERR;
        }
    }
    return T_ARRAY_PTR(t0);
}

// Type of lambda
static RCL_Type gettype_lambda(const RCL_Value_LamdaDecl_t lam, IdentifierTypeEnv_map_t *env_ptr)
{
    // TODO: Améliorer l'inférence des lamabdas
    // TODO Il faut en effet ajouter leur type:
    //* add_IdentifierTypeEnv(env_ptr, hash_djb2(lam), T_EMPTY);

    RCL_Type_ptr t1 = malloc(sizeof *t1);
    *t1 = T_ANY('a');
    RCL_Type_ptr t2 = malloc(sizeof *t2);
    *t2 = T_EMPTY;
    return T_ARROW_PTR(t1, t2);
}

// Get the type of the given value
// We also need a variable environment to match words/types
RCL_Type gettype(const Value value, IdentifierTypeEnv_map_t *env_ptr, BResult *bresult)
{
    rcl_assert(value.kind != RCL_Value_All); // `All` is not a valid operation.
    rcl_assert(value.kind <= RCL_Value_All); // Beyond `All`, this is a segmentation error.
    rcl_assert(bresult->state.array != NULL);
    rcl_assert(env_ptr != NULL);

    switch (value.kind)
    {
    case RCL_Value_Array:
        return gettype_array(value.u.array_, env_ptr, bresult);

    case RCL_Value_Quotation:
        return gettype_quote(value.u.quote_, env_ptr, bresult);

    case RCL_Value_Integer ... RCL_Value_String:
        return T_LITERAL(value.kind);

    case RCL_Value_Qual: //! This is maybe a qual...
        return gettype_word(value.u.qual_.quals[0], env_ptr);

    case RCL_Value_Lambda:
        return gettype_lambda(value.u.lam_, env_ptr);

    case RCL_Value_EndLamScope:
        return T_EMPTY;

    case RCL_Value_LiteralOperation:
        return primitive_types[value.u.litOperation_->kind];

    case RCL_Value_Combinator:
        return combinator_types[(short)value.u.comb_];

    case RCL_Value_Parallel:
        break;

    case RCL_Value_Field:
        break;

    case RCL_Value_DataStruct:
        break;

    case RCL_Value_Replicated:
        break;

    case RCL_Value_Empty:
        return T_EMPTY;

    case RCL_Value_Number:
        return T_LITERAL(RCL_Value_Number);

    case RCL_Value_Concatenation:
        return gettype_concatenation(value.u.concatenation_, env_ptr, bresult);

    default:
        break;
    }
    _interr_ext("Untypable value for now: `%s'.", show_value(value));
}

// --- SVTA Inference Algorithme --- //

// The first step consists in a simple rewriting of the types in a SVTA (Single Variable Type Assignment) form.
// A list is constructed, containing the types in SVTA form so that they can be reused later,
// and the concatenation of the individual types of the operations of the expression is generated.
// Concrete types (= not variable) are dissolved to be converted to SVTA type as well.
//
// For example, the first phase of inference for the expression `dup flip` follows the following steps:
//    - 2 dup flip                                     // Original expression
//    - t0 (a -> a . a) (a . b . c -> c . b . a)       // Generated SVTA concatenation
//    - {t0(Int), a0, b0, c0}                          // Generated list of different SVTAs

static svta_t make_svta(tvar_t tvar, sva_t sva, RCL_Type *concrete_type)
{
    String s = rcl_sprintf_s("%c%hu", tvar, sva);
    if (concrete_type == NULL)
        return (svta_t){s, tvar, sva, NULL};
    RCL_Type *t = malloc(sizeof *t);
    *t = *concrete_type;
    return (svta_t){s, tvar, sva, t};
}

Vector(type_stack_vec, struct RCL_Type);
Vector(svtas_vec, svta_t);

#define REAL_TVAR_ID 't'
#define VOID_TVAR_ID 'e'
#define QUOTE_TVAR_ID 'q'
#define ARRAY_TVAR_ID 'y'

#define FOUND(x) (x >= 0)
#define UNFOUND(x) (x == -1)
#define IS_CONCRETE(t) (t != NULL)

// Usefull functions about TVAR and SVTA //

static int find_tvar(const svtas_vec svtas, const tvar_t tvar)
{
    for (Iterator i = 0; i < svtas.used; i++)
        if (svtas.array[i].tvar == tvar)
            return i;
    return -1;
}

static int find_last_tvar(svtas_vec svtas, const tvar_t tvar)
{
    for (Iterator i = svtas.used; i >= 0; i--)
        if (svtas.array[i].tvar == tvar)
            return i;
    return -1;
}

static int find_svtat_litt(const svtas_vec current_svtas, const struct RCL_Type t)
{
    for (Iterator i = 0; i < current_svtas.used; i++)
        if (current_svtas.array[i].tvar == REAL_TVAR_ID)
            if (cmp_types(*current_svtas.array[i].concrete_type, t, true))
                return i;
    return -1;
}

static bool cmp_svta(const svta_t svta1, const svta_t svta2)
{
    return ((svta1.sva == svta2.sva) && (svta1.tvar == svta2.tvar));
}

static bool cmp_svta_ptr(const svta_ptr_t svta_ptr1, const svta_ptr_t svta_ptr2)
{
    return ((svta_ptr1->sva == svta_ptr2->sva) && (svta_ptr1->tvar == svta_ptr2->tvar));
}

// Just for debug
static void print_svtas(String name, svtas_vec svtas)
{
    printf("%s: ", name);
    for (Iterator i = 0; i < svtas.used; i++)
        if (IS_CONCRETE(svtas.array[i].concrete_type))
            printf("%s(%s), ", svtas.array[i].visualize, show_type(*svtas.array[i].concrete_type));
        else
            printf("%s, ", svtas.array[i].visualize);
    printf("\n");
}

// Svta builders //

static void build_svtas(svtas_vec *, svtas_vec *, const struct RCL_Type, const RCL_Type_ptr);
static struct RCL_Type to_svta(const svtas_vec, const struct RCL_Type);

static void build_svtas_any_wf(svtas_vec *global_svtas, svtas_vec *current_svtas, const tvar_t tvar, const RCL_Type_ptr concrete_type, sva_t sva)
{
    const svta_t new_svta = make_svta(tvar, sva, concrete_type);
    push_svtas_vec(global_svtas, new_svta);
    push_svtas_vec(current_svtas, new_svta);
}

static void build_svtas_any_tt(svtas_vec *global_svtas, svtas_vec *current_svtas, const tvar_t tvar, const RCL_Type_ptr concrete_type)
{
    const int gfound = find_svtat_litt(*global_svtas, *concrete_type);
    const int cfound = find_svtat_litt(*current_svtas, *concrete_type);
    const int g2found = find_last_tvar(*global_svtas, tvar);
    const int sva = FOUND(g2found) ? global_svtas->array[g2found].sva + 1 : 0;

    if ((UNFOUND(gfound) || (FOUND(gfound) && UNFOUND(cfound))))
        build_svtas_any_wf(global_svtas, current_svtas, tvar, concrete_type, sva);
}

static void build_svtas_any(svtas_vec *global_svtas, svtas_vec *current_svtas, tvar_t tvar, const RCL_Type_ptr concrete_type)
{
    if (IS_CONCRETE(concrete_type))
        return build_svtas_any_tt(global_svtas, current_svtas, tvar, concrete_type);

    if (tvar == REAL_TVAR_ID) // This is because the variable type was generated successively ?
        tvar = tvar + 1;      // If yes, we have to attribuate a new tvar (because REAL_TVAR_ID is reserved)

    const int gfound = find_last_tvar(*global_svtas, tvar);
    const int cfound = find_last_tvar(*current_svtas, tvar);

    const int sva = FOUND(gfound) ? global_svtas->array[gfound].sva + 1 : 0;

    if ((UNFOUND(gfound) || (FOUND(gfound) && UNFOUND(cfound))))
        build_svtas_any_wf(global_svtas, current_svtas, tvar, concrete_type, sva);
}

static void build_svtas_stack(svtas_vec *global_svtas, svtas_vec *current_svtas, const struct rcl_type_stack_ ts, const RCL_Type_ptr concrete_type)
{
    for (Iterator i = 0; i < ts.nbr; i++)
        build_svtas(global_svtas, current_svtas, *ts.tstack[i], concrete_type);
}

static void build_svtas_arrow(svtas_vec *global_svtas, svtas_vec *current_svtas, const struct rcl_type_arrow_ ta, const RCL_Type_ptr concrete_type)
{
    build_svtas(global_svtas, current_svtas, *ta.t1, concrete_type);
    build_svtas(global_svtas, current_svtas, *ta.t2, concrete_type);
}

static RCL_Type_ptr define_concrete_type(const RCL_Type_ptr real_ptr)
{
    RCL_Type_ptr res = NULL;
    if (IS_CONCRETE(real_ptr))
    {
        res = malloc(sizeof *res);
        *res = *real_ptr;
    }
    return res;
}

static void build_svtas_empty(svtas_vec *global_svtas, svtas_vec *current_svtas)
{
    RCL_Type_ptr et = malloc(sizeof *et);
    *et = T_EMPTY;
    build_svtas(global_svtas, current_svtas, T_ANY(VOID_TVAR_ID), et);
}

static void build_svtas_quote(svtas_vec *global_svtas, svtas_vec *current_svtas, const struct rcl_type_quote_ tq, const RCL_Type_ptr concrete_type)
{
    // TODO Il faudrait déjà faire une réduction d'inférence ici !
    // TODO Dans la quote

    const RCL_Type t = *tq.tquote;
    RCL_Type_ptr svta_tq = malloc(sizeof *svta_tq);
    build_svtas(global_svtas, current_svtas, *tq.tquote, concrete_type);
    *svta_tq = to_svta(*current_svtas, t);
    RCL_Type_ptr ct = malloc(sizeof *ct);
    *ct = T_QUOTE_PTR(svta_tq);
    build_svtas(global_svtas, current_svtas, T_ANY(QUOTE_TVAR_ID), ct);
}

static void build_svtas_array(svtas_vec *global_svtas, svtas_vec *current_svtas, const struct rcl_type_array_ ta, const RCL_Type_ptr concrete_type)
{
    // TODO Il faudrait déjà faire une réduction d'inférence ici !
    // TODO Dans le array

    const RCL_Type t = *ta.tarray;
    RCL_Type_ptr svta_ta = malloc(sizeof *svta_ta);
    build_svtas(global_svtas, current_svtas, *ta.tarray, concrete_type);
    *svta_ta = to_svta(*current_svtas, t);
    RCL_Type_ptr ct = malloc(sizeof *ct);
    *ct = T_ARRAY_PTR(svta_ta);
    build_svtas(global_svtas, current_svtas, T_ANY(ARRAY_TVAR_ID), ct);
}

static void build_svtas(svtas_vec *global_svtas, svtas_vec *current_svtas, const struct RCL_Type origin, const RCL_Type_ptr real_ptr)
{
    const RCL_Type_ptr concrete_type = define_concrete_type(real_ptr);

    switch (origin.kind)
    {
    case TYPE_ANY:
        return build_svtas_any(global_svtas, current_svtas, origin.u.rcl_type_any_.tany, concrete_type);

    case TYPE_STACK:
        return build_svtas_stack(global_svtas, current_svtas, origin.u.rcl_type_stack_, concrete_type);

    case TYPE_ARROW:
        return build_svtas_arrow(global_svtas, current_svtas, origin.u.rcl_type_arrow_, concrete_type);

    case TYPE_QUOTE:
        return build_svtas_quote(global_svtas, current_svtas, origin.u.rcl_type_quote_, concrete_type);

    case TYPE_ARRAY:
        return build_svtas_array(global_svtas, current_svtas, origin.u.rcl_type_array_, concrete_type);

    case TYPE_EMPTY:
        return build_svtas_empty(global_svtas, current_svtas);

    default:
        return build_svtas(global_svtas, current_svtas, T_ANY(REAL_TVAR_ID), &origin);
    }
    // May no reach
    _interr_ext("Control flow shouldn't have gotten here.", NULL);
}

static size_t get_svta_nbr(const struct RCL_Type t)
{
    if (t.kind == TYPE_ARROW)
    {
        const size_t n1 = get_svta_nbr(*t.u.rcl_type_arrow_.t1);
        const size_t n2 = get_svta_nbr(*t.u.rcl_type_arrow_.t2);
        return n1 + n2;
    }
    if (t.kind == TYPE_STACK)
    {
        return t.u.rcl_type_stack_.nbr;
    }
    return 1;
}

static size_t get_svtas_nbr(const type_stack_vec tstack)
{
    size_t result = 0;
    for (Iterator i = 0; i < tstack.used; i++)
        result += get_svta_nbr(tstack.array[i]);
    return result;
}

static RCL_Type to_svta_any(const svtas_vec current_svtas, const struct rcl_type_any_ tany)
{
    const svta_t svta = current_svtas.array[find_tvar(current_svtas, tany.tany)];
    return T_SVTA(svta);
}

static RCL_Type to_svta_empty(const svtas_vec current_svtas)
{
    const svta_t svta = current_svtas.array[find_last_tvar(current_svtas, VOID_TVAR_ID)];
    return T_SVTA(svta);
}

static RCL_Type to_svta_quote(const svtas_vec current_svtas)
{
    const svta_t svta = current_svtas.array[find_last_tvar(current_svtas, QUOTE_TVAR_ID)];
    return T_SVTA(svta);
}

static RCL_Type to_svta_array(const svtas_vec current_svtas)
{
    const svta_t svta = current_svtas.array[find_last_tvar(current_svtas, ARRAY_TVAR_ID)];
    return T_SVTA(svta);
}

static RCL_Type to_svta_literal(const svtas_vec current_svtas, const struct RCL_Type t)
{
    const int cfound = find_svtat_litt(current_svtas, t);
    rcl_assert(FOUND(cfound));
    return T_SVTA(current_svtas.array[cfound]);
}

static RCL_Type to_svta_arrow(const svtas_vec current_svtas, const struct rcl_type_arrow_ tarrow)
{
    RCL_Type_ptr t1 = malloc(sizeof *t1);
    RCL_Type_ptr t2 = malloc(sizeof *t2);
    *t1 = to_svta(current_svtas, *tarrow.t1);
    *t2 = to_svta(current_svtas, *tarrow.t2);
    return T_ARROW_PTR(t1, t2);
}

static RCL_Type to_svta_stack(const svtas_vec current_svtas, const struct rcl_type_stack_ rts)
{
    RCL_Type_ptr res = malloc(rts.nbr * sizeof *res);
    for (Iterator i = 0; i < rts.nbr; i++)
        res[i] = to_svta(current_svtas, *rts.tstack[i]);
    return make_RCL_Type_stack(rts.nbr, res);
}

static struct RCL_Type to_svta(const svtas_vec current_svtas, const struct RCL_Type t)
{
    rcl_assert(current_svtas.array != NULL);
    rcl_assert(t.kind <= TYPE_ERROR);

    switch (t.kind)
    {
    case TYPE_ANY:
        return to_svta_any(current_svtas, t.u.rcl_type_any_);

    case TYPE_ARROW:
        return to_svta_arrow(current_svtas, t.u.rcl_type_arrow_);

    case TYPE_LITERAL:
        return to_svta_literal(current_svtas, t);

    case TYPE_QUOTE:
        return to_svta_quote(current_svtas);

    case TYPE_ARRAY:
        return to_svta_array(current_svtas);

    case TYPE_EMPTY:
        return to_svta_empty(current_svtas);

    case TYPE_STACK:
        return to_svta_stack(current_svtas, t.u.rcl_type_stack_);

    case TYPE_SVTA:
        return t;

    default:
        _interr("Unexpected type kind `%d'.", t.kind);
        return T_ERR;
    }
    return T_ERR;
}

// This is the type of result from phase 1.
struct type_bunch_t
{
    svtas_vec svtas;
    type_stack_vec type_stack;
};

static struct type_bunch_t convert_to_svta(const type_stack_vec origin_tstack)
{
    type_stack_vec tstack = new_type_stack_vec(origin_tstack.used);
    svtas_vec svtas = new_svtas_vec(get_svtas_nbr(origin_tstack));

    for (Iterator i = 0; i < origin_tstack.used; i++)
    {
        const struct RCL_Type current_type = origin_tstack.array[i];
        svtas_vec current_svtas = new_svtas_vec(get_svta_nbr(current_type));
        build_svtas(&svtas, &current_svtas, current_type, NULL);
        push_type_stack_vec(&tstack, to_svta(current_svtas, current_type));
    }
    return (struct type_bunch_t){svtas, tstack};
}

// Creates a list of the type of each individual operation of the given concatenated expression.
// For example: `3 swap dup` => {Int, (a . b -> b . a), (a -> a . a)}.
static type_stack_vec primary_type_attrib(const RawCode rcode, IdentifierTypeEnv_map_t *env_ptr, BResult *bresult)
{
    type_stack_vec result = new_type_stack_vec(rcode.used);
    for (Iterator i = 0; i < rcode.used; i++)
        push_type_stack_vec(&result, gettype(rcode.array[i], env_ptr, bresult));
    return result;
}

static type_stack_vec tstack_to_vec(const struct rcl_type_stack_ ts)
{
    type_stack_vec res = new_type_stack_vec(ts.nbr);
    res.array = *ts.tstack;
    res.used = ts.nbr;
    return res;
}

Map(overlay, svta_ptr_t, svta_ptr_t);

static svta_ptr_t get_svta_ptr(const svta_t svta)
{
    svta_ptr_t svta_ptr = malloc(sizeof *svta_ptr);
    *svta_ptr = svta;
    return svta_ptr;
}

static void overlayer(overlay_map_t *overlay_ptr, const type_stack_vec rhot, const type_stack_vec omet)
{
    const size_t rho = rhot.used;
    const size_t ome = omet.used;

    if (rho == ome)
    {
        for (Iterator i = 0; i < rho; i++)
        {
            const svta_ptr_t key = get_svta_ptr(omet.array[i].u.rcl_type_svta_.svta);
            const svta_ptr_t val = get_svta_ptr(rhot.array[i].u.rcl_type_svta_.svta);
            add_overlay(overlay_ptr, key, val);
        }
        return;
    }
    if (rho > ome)
    {
    }
    if (rho < ome)
    {
    }

    _interr_ext("May no reach.", NULL);
}

// --- REDUCTION --- //

// The reduction phase makes it possible to restrict the SVTA variable types set into an equivalent and irreducible set.
// For example, consider the following trivial combination and types:
//   Expr: swap swap
//   Type: (a . b -> b . a) (a . b -> b . a)
//   SVTA: (a0 . b0 -> b0 . a0) (a1 . b1 -> b1 . a1)
// We can apply the overlay:
//  a0 . b0 -> b0 . a0
//             a1 . b1 -> b1 . a1
// And make the equalities:
//      a0 = a0
//      b0 = b0
//      b1 = a0
//      a1 = b0
//      b1 = b1
//      a1 = a1
// By reviewing the ties deduced by the overlay, we get a new set of ties:
//      a0 = a0
//      b0 = b0
//      b1 = a0
//      a1 = b0
//      b1 = a0
//      a1 = b0
// The last column (to the right of the equations) now contains only the minimum set of SVTA variable types:  a0, b0.
// We now know that  b1 = b0  and  a1 = a0.
// With this information, we now know that, in the original SVTA type, only a0 and b0 are true variable types,
// the rest (a1 and b1) are in fact only temporary because dissolved in the expression.
// We can therefore revise the original type by only taking into account the underlined elements:
//  a0 . b0 -> b0 . a0
//  ^^^^^^^    a1 . b1 -> b1 . a1
//                        ^^^^^^^
// And so reduce:  a0 . b0 -> b1 . a1
// And replace what is known:  a0 . b0 -> a0 . b0
// By embellishing the type obtained:  a . b -> a . b
// This example is trivial, but can be generalized for all IR typable expressions

static void deeprec_overlay_reduce(overlay_map_t *overlay_ptr, svta_ptr_t origin_key, svta_ptr_t origin_val)
{
    if (cmp_svta_ptr(origin_key, origin_val))
        return;

    const int found = key_find_overlay(overlay_ptr, origin_val, &cmp_svta_ptr);
    if (found == map_unfound)
        return;

    deeprec_overlay_reduce(overlay_ptr, origin_key, overlay_ptr->array[found].key); // TODO: + vérifier le concrete_type
}

static void reduce(overlay_map_t *overlay_ptr)
{
    for (Iterator i = 0; i < overlay_ptr->used; i++)
        deeprec_overlay_reduce(overlay_ptr, overlay_ptr->array[i].key, overlay_ptr->array[i].val);
}

static void unify(type_stack_vec *ts_ptr, const overlay_map_t overlay_ptr)
{
    for (Iterator i = 0; i < ts_ptr->used; i++)
    {
        const int found = key_find_overlay(&overlay_ptr, &ts_ptr->array[i].u.rcl_type_svta_.svta, &cmp_svta_ptr);
        if (FOUND(found))
        {
            // ts_ptr->array[i].u.rcl_type_svta_.svta.concrete_type
            // malloc
            printf("found");
        }
    }
}

static void print_overlay(const overlay_map_t overlay)
{
    for (Iterator i = 0; i < overlay.used; i++)
        printf("%s = %s\n", overlay.array[i].key->visualize, overlay.array[i].val->visualize);
}

static void apply_overlay(const overlay_map_t *overlay_ptr, type_stack_vec *tvec)
{
    for (Iterator i = 0; i < tvec->used; i++)
    {
        const int found = key_find_overlay(overlay_ptr, &tvec->array[i].u.rcl_type_svta_.svta, &cmp_svta_ptr);
        tvec->array[i] = FOUND(found) ? T_SVTA(*overlay_ptr->array[found].val) : tvec->array[i];
    }
}

static RCL_Type hubcap_svta_tot(const RCL_Type to_hubcap)
{
    return to_hubcap;
}

static void infer_couple(type_stack_vec *ts_ptr, const svtas_vec svtas, const IdentifierTypeEnv_map_t *env, BResult *bresult)
{
    const RCL_Type t0 = ts_ptr->array[0];
    const RCL_Type t1 = ts_ptr->array[1];

    RCL_Type final_type_result;

    if (t0.kind == TYPE_ARROW && t1.kind == TYPE_ARROW)
    {
        rcl_assert(t0.u.rcl_type_arrow_.t1->kind == TYPE_STACK);
        rcl_assert(t0.u.rcl_type_arrow_.t2->kind == TYPE_STACK);
        rcl_assert(t1.u.rcl_type_arrow_.t1->kind == TYPE_STACK);
        rcl_assert(t1.u.rcl_type_arrow_.t2->kind == TYPE_STACK);

        const type_stack_vec rhot = tstack_to_vec(t0.u.rcl_type_arrow_.t2->u.rcl_type_stack_);
        const type_stack_vec omet = tstack_to_vec(t1.u.rcl_type_arrow_.t1->u.rcl_type_stack_);

        type_stack_vec tleft = tstack_to_vec(t0.u.rcl_type_arrow_.t1->u.rcl_type_stack_);
        type_stack_vec tright = tstack_to_vec(t1.u.rcl_type_arrow_.t2->u.rcl_type_stack_);

        const size_t rho = rhot.used;
        const size_t ome = omet.used;

        rcl_assert(rho > 0);
        rcl_assert(ome > 0);

        overlay_map_t overlay = new_overlay_map_t(__max(rho, ome));
        overlayer(&overlay, rhot, omet);
        reduce(&overlay);
        apply_overlay(&overlay, &tleft);
        apply_overlay(&overlay, &tright);

        if (rho == ome)
        {
            RCL_Type *ts0 = malloc(sizeof *ts0);
            *ts0 = make_RCL_Type_stack(tleft.used, tleft.array);
            RCL_Type *ts1 = malloc(sizeof *ts1);
            *ts1 = make_RCL_Type_stack(tright.used, tright.array);
            final_type_result = T_ARROW_PTR(ts0, ts1);
        }
    }
    final_type_result = hubcap_svta_tot(final_type_result);
    ts_ptr->array[0] = final_type_result;
}

// -Ofast

static RCL_Type infer(const struct type_bunch_t tb, const IdentifierTypeEnv_map_t *env, BResult *bresult)
{
    if (tb.type_stack.used == 1)
        return make_RCL_Type_stack(tb.type_stack.used, tb.type_stack.array);

    type_stack_vec tmp = new_type_stack_vec(2);
    tmp.used = 2; // TODO : toujours 2 ?
    tmp.array[0] = tb.type_stack.array[0];

    for (Iterator i = 1; i < tb.type_stack.used; i++)
    {
        // printf("[0]: %s\n", show_type(tmp.array[0]));
        tmp.array[1] = tb.type_stack.array[i];
        infer_couple(&tmp, tb.svtas, env, bresult);
    }
    return tmp.array[0];
    //! RETURN
    return T_ERR;
}

void foo(BResult *bresult)
{
    IdentifierTypeEnv_map_t env = new_IdentifierTypeEnv_map_t(bresult->wordico.functions.used);
    const struct type_bunch_t tb = convert_to_svta(primary_type_attrib(bresult->psdata.rcode, &env, bresult));
    const RCL_Type infered = infer(tb, &env, bresult);

    printf("Infered: %s\n", show_type(infered));

    if (infered.kind == TYPE_ERROR)
        ;
}
