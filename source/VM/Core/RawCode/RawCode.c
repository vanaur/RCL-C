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
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <gmp.h>
#include <Tools\Internal\Internal.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\RawCode\rawcodecmp.h>
#include <VM\Core\RawCode\Combinator\Combinators.h>
#include <VM\Core\Syntax\Identifier\Functions.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>
#include <VM\Core\Browse\BResult.h>

struct RCL_Value_DataStruct_Field *getSpecific_DataStruct_field(struct RCL_Value_DataStruct data_struct, hash_t hash_code)
{
    for (Iterator i = 0; i < data_struct.template->field_alloc_used; i++)
        if (data_struct.fields[i].field_type_info->hash_code == hash_code)
            return &data_struct.fields[i];
    return NULL;
}

struct RCL_Value_DataStruct make_DataStruct_unfilled(struct RCL_Structure *parent_structure_ptr)
{
    return (struct RCL_Value_DataStruct){.template = parent_structure_ptr,
                                         .fields = malloc(sizeof(struct RCL_Value_DataStruct_Field) * parent_structure_ptr->field_alloc_used)};
}

String show_kind(const enum Value_Kind kind)
{
    // It is deliberately made that each "kind" in the form of a character string begins with a consonant,
    // it is indeed easier not to worry about the determinant (a/an) for the next uses.
    switch (kind)
    {
    case RCL_Value_Quotation:
        return "quotation";

    case RCL_Value_Float:
        return "floating point number";

    case RCL_Value_Integer:
        return "whole number";

    case RCL_Value_Char:
        return "character";

    case RCL_Value_String:
        return "character string";

    case RCL_Value_Qual:
        return "(qualified) word";

    case RCL_Value_Lambda:
        return "new lambda declaration";

    case RCL_Value_EndLamScope:
        return "end of a lambda scope";

    case RCL_Value_LiteralOperation:
        return "basic literal operation";

    case RCL_Value_Combinator:
        return "stack combinator";

    case RCL_Value_Parallel:
        return "parallel computation";

    case RCL_Value_Array:
        return "table";

    case RCL_Value_Replicated:
        return "syntaxic repeating operation";

    case RCL_Value_Empty:
        return "void stack (= \"nothing\")";

    case RCL_Value_Number:
        return "number (float or integer)";

    default:
        return "non-expected";
    }
}

Value make_RCL_Value_Quotation(const RCL_Value_Quote_t rcode)
{
    return (Value){.kind = RCL_Value_Quotation, .u.quote_ = rcode};
}

Value make_RCL_Value_Integer(const RCL_Value_Integer_t x)
{
    Value result = {.kind = RCL_Value_Integer};
    mpz_init_set(result.u.int_, x);
    return result;
}

Value make_RCL_Value_Float(const RCL_Value_Float_t x)
{
    Value result = {.kind = RCL_Value_Float};
    mpf_init_set(result.u.float_, x);
    return result;
}

Value make_RCL_Value_Integer_i(const signed int x)
{
    Value result = {.kind = RCL_Value_Integer};
    mpz_init_set_si(result.u.int_, (signed long)x);
    return result;
}

Value make_RCL_Value_Float_f(const double x)
{
    Value result = {.kind = RCL_Value_Float};
    //mpf_init_set_si(result.u.float_, (signed long)x);
    mpf_init_set_d(result.u.float_, x);
    return result;
}

Value make_RCL_Value_Char(const RCL_Value_Char_t c)
{
    return (Value){.kind = RCL_Value_Char, .u.char_ = c};
}

Value make_RCL_Value_String(const RCL_Value_String_t s)
{
    return (Value){.kind = RCL_Value_String, .u.string_ = s};
}

Value make_RCL_Value_EndLamScope(const String name)
{
    Value result;
    result.kind = RCL_Value_EndLamScope;
    result.u.endLamScope_.hash_code = hash_djb2(name);
    result.u.endLamScope_.word_str = name;
    return result;
}

Value make_RCL_Value_Lambda(const String name)
{
    Value result;
    result.kind = RCL_Value_Lambda;
    result.u.lam_.hash_code = hash_djb2(name);
    result.u.lam_.word_str = name;
    return result;
}

Value make_RCL_Value_LiteralOperation(const RCL_Value_LiteralOperation_t lo)
{
    return (Value){.kind = RCL_Value_LiteralOperation, .u.litOperation_ = lo};
}

Value make_RCL_Value_LiteralOperation_kind(const unsigned short lo)
{
    LiteralOperation tmp = malloc(sizeof(*tmp));
    tmp->kind = lo;
    return (Value){.kind = RCL_Value_LiteralOperation, .u.litOperation_ = tmp};
}

Value make_RCL_Value_Combinator(const RCL_Value_Combinator_t comb)
{
    return (Value){.kind = RCL_Value_Combinator, .u.comb_ = comb};
}

Value make_RCL_Value_Parallel(Value f1, Value f2)
{
    Value *tmp1 = malloc(sizeof(*tmp1));
    Value *tmp2 = malloc(sizeof(*tmp2));
    tmp1->kind = f1.kind;
    tmp2->kind = f2.kind;
    tmp1->u = f1.u;
    tmp2->u = f2.u;
    return (Value){.kind = RCL_Value_Parallel, .u = {.parallel_ = {.v1 = tmp1, .v2 = tmp2}}};
}

Value make_RCL_Value_Field(const RCL_Value_Field_t field)
{
    return (Value){.kind = RCL_Value_Field, .u = {.field_ = field}};
}

Value make_RCL_Value_DataStruct(const RCL_Value_DataStruct_t _struct)
{
    return (Value){.kind = RCL_Value_DataStruct, .u.dataStruct_ = _struct};
}

Value make_RCL_Value_Replicate(size_t n, Value op)
{
    Value *tmp = malloc(sizeof(*tmp));
    tmp->kind = op.kind;
    tmp->u = op.u;
    return (Value){.kind = RCL_Value_Replicated, .u = {.repeat_ = {.times = n, .op = tmp}}};
}

Value make_RCL_Value_Array(RawCode *array, size_t len)
{
    return (Value){.kind = RCL_Value_Array, .u = {.array_ = {.array = array, .length = len}}};
}

Value make_RCL_Value_Concatenation(RawCode *concatenation)
{
    return (Value){.kind = RCL_Value_Concatenation, .u = {.concatenation_ = concatenation}};
}

static RCL_Value_Word_t make_word(const String str)
{
    return (RCL_Value_Word_t){str, hash_djb2(str)};
}

qual_word_t qual_word_from_absyn(const Identifier id)
{
    Identifier curr = id;
    const size_t nbrof_quals = count_quals(id);
    int index = 0;
    RCL_Value_Word_t *quals = malloc(nbrof_quals * sizeof *quals);
    rcl_assert(quals != NULL);

    while (curr->kind != is_Name)
    {
        rcl_assert(curr != NULL);
        quals[index++] = make_word(curr->u.qualname_.uident_);
        curr = curr->u.qualname_.identifier_;
    }
    quals[index] = make_word(curr->u.name_.lident_);
    return (qual_word_t){nbrof_quals, quals};
}

qual_word_t qual_word_from_array(const size_t n, const String xs[n])
{
    RCL_Value_Word_t *tmp = malloc(n * sizeof *tmp);
    for (Iterator i = 0; i < n; i++)
        tmp[i] = make_word(xs[i]);
    return (qual_word_t){n, tmp};
}

Value make_RCL_Value_Qual(const qual_word_t qual)
{
    return (Value){.kind = RCL_Value_Qual, .u = {.qual_ = qual}};
}

String show_qual(const qual_word_t qual)
{
    String result = "";
    for (Iterator i = 0; i < qual.nbrof_qual; i++)
        if (i == 0)
            rcl_asprintf(&result, "%s", qual.quals[i].word_str);
        else
            rcl_asprintf(&result, "%s.%s", result, qual.quals[i].word_str);
    return result;
}

static Value otov_lit(Operation op)
{
    switch (op->u.lit_.data_->kind)
    {
    case is_Int:
    {
        mpz_t tmp;
        mpz_init_set_d(tmp, op->u.lit_.data_->u.int_.integer_);
        return make_RCL_Value_Integer(tmp);
    }
    case is_Flt:
    {
        mpf_t tmp;
        mpf_init_set_d(tmp, op->u.lit_.data_->u.flt_.double_);
        return make_RCL_Value_Float(tmp);
    }
    case is_Chr:
        return make_RCL_Value_Char(op->u.lit_.data_->u.chr_.char_);

    case is_Str:
        return make_RCL_Value_String(op->u.lit_.data_->u.str_.string_);
    }
}

static Value otov_quote(Operation op)
{
    RawCode *quote = malloc(sizeof(*quote));
    ListOperation lo = op->u.quote_.operation_->u.concatenation_.listoperation_;
    init_rcode(quote, count_operations(lo));
    while (lo != NULL)
    {
        push_rcode(quote, otov(lo->operation_));
        lo = lo->listoperation_;
    }
    return make_RCL_Value_Quotation(quote);
}

static bool is_identifier_combinator(const Identifier id)
{
    if (count_quals(id) == 1)
        return is_combinator(id->u.name_.lident_);
    return false;
}

static Value otov_var(Operation op)
{
    const Identifier id = op->u.var_.identifier_;
    if (is_identifier_combinator(id))
        return make_RCL_Value_Combinator(str_to_comb(id->u.name_.lident_));
    return make_RCL_Value_Qual(qual_word_from_absyn(op->u.var_.identifier_));
}

static Value otov_concatenation(Operation op)
{
    ListOperation lo = op->u.concatenation_.listoperation_;
    size_t len = count_operations(lo);
    RawCode *concatenation = malloc(sizeof(*concatenation));
    init_rcode(concatenation, len);
    size_t i = 0;
    while (lo != NULL)
    {
        push_rcode(concatenation, otov(lo->operation_));
        lo = lo->listoperation_;
        i++;
    }
    return make_RCL_Value_Concatenation(concatenation);
}

static Value otov_list(Operation op)
{
    ListOperation lo = op->u.list_.listoperation_;
    size_t len = count_operations(lo);
    RawCode *array = malloc(sizeof(*array));
    init_rcode(array, len);
    size_t i = 0;
    while (lo != NULL)
    {
        push_rcode(array, otov_concatenation(lo->operation_));
        lo = lo->listoperation_;
        i++;
    }
    return make_RCL_Value_Array(array, len);
}

Value otov(Operation op)
{
    switch (op->kind)
    {
    case is_BasicLiteralOp:
        return make_RCL_Value_LiteralOperation(op->u.basicliteralop_.literaloperation_);

    case is_Lit:
        return otov_lit(op);

    case is_ParallelConcat:
        return make_RCL_Value_Parallel(otov(op->u.parallelconcat_.operation_1), otov(op->u.parallelconcat_.operation_2));

    case is_Quote:
        return otov_quote(op);

    case is_Lambda:
        return make_RCL_Value_Lambda(op->u.lambda_.lident_);

    case is_EndLambdaScope:
        return make_RCL_Value_EndLamScope(op->u.endlambdascope_.lident_);

    case is_Var:
        return otov_var(op);

    case is_RepeatOperation:
        return make_RCL_Value_Replicate((size_t)op->u.repeatoperation_.integer_, otov(op->u.repeatoperation_.operation_));

    case is_List:
        return otov_list(op);

    case is_Concatenation:
        return otov_concatenation(op);

    case is_Field:
        return make_RCL_Value_Field(op->u.field_.rcl_structure_field_);

    default: // May no reach
        _interr("Unknown value kind: %d.", op->kind);
        exit(EXIT_FAILURE);
    }
}

void init_rcode(RawCode *rcode, size_t initSize)
{
    InitVector(rcode, initSize, Value);
}

void push_rcode(RawCode *rcode, Value item)
{
    PushToVector(rcode, Value, item);
}

void pop_rcode(RawCode *rcode)
{
    PopVector(rcode);
}

Value drop_rcode(RawCode *rcode)
{
    Value last = top_rcode(rcode);
    pop_rcode(rcode);
    return last;
}

Value top_rcode(RawCode *rcode)
{
    if (rcode->size == 0)
        return emptyRawCodeValue;
    return rcode->array[rcode->used - 1];
}

size_t count_operations(ListOperation ls)
{
    ListOperation current = ls;
    size_t result = 0;
    while (current != NULL)
    {
        result++;
        if (current->operation_->kind == is_RepeatOperation)
            result += current->operation_->u.repeatoperation_.integer_;
        current = current->listoperation_;
    }
    return result;
}

void concat_rcode(RawCode *dest_ptr, RawCode *src_ptr)
{
    for (Iterator i = 0; i < src_ptr->used; i++)
        push_rcode(dest_ptr, src_ptr->array[i]);
}

void concat_rcode_from(RawCode *dest_ptr, RawCode *src_ptr, size_t from)
{
    assert(src_ptr->used >= from);

    memcpy(
        dest_ptr->array + (dest_ptr->used - from) * sizeof(Value),
        src_ptr->array + from,
        src_ptr->used * sizeof(Value));

    dest_ptr->used += src_ptr->used - from;

    if (dest_ptr->size <= dest_ptr->used)
        dest_ptr->size = 2 * dest_ptr->used;
}

void concat_rcode_until(RawCode *dest_ptr, RawCode *src_ptr, size_t from, size_t until)
{
    if (from >= until)
        return;

    assert(src_ptr->used >= from);
    assert(src_ptr->used >= until);

    memcpy(
        dest_ptr->array + (dest_ptr->used - (from - until)) * sizeof(Value),
        src_ptr->array + from - until,
        src_ptr->used * sizeof(Value));

    dest_ptr->used += src_ptr->used - (from - until);

    if (dest_ptr->size <= dest_ptr->used)
        dest_ptr->size = 2 * dest_ptr->used;
}

inline void seq_replace(
    RawCode *rcode,
    const Value seq1[], size_t size1,
    const Value seq2[], size_t size2)
{
    Value *p = rcode->array, *end = rcode->array + rcode->used;

    while (p < end)
    {
        if (p + size1 <= end && rawcodecmp(p, seq1, size1))
        {
            memmove(p + size2, p + size1, (end - p - size1) * sizeof(*p));
            memcpy(p, seq2, size2 * sizeof(*p));
            // p += size2; // uncomment to avoid replacements in seq2 itself
            rcode->used = rcode->used - size1 + size2;
            end = rcode->array + rcode->used;
        }
        else
        {
            p++;
        }
    }
}

size_t count_operations_bykind(RawCode rcode, enum Value_Kind kind)
{
    size_t result = 0;
    for (Iterator i = 0; i < rcode.used; i++)
        if (rcode.array[i].kind == kind)
            result += 1;
    return result;
}

RawCode fast_rcode_subv(const RawCode src, const size_t from, const size_t until)
{
    const size_t size = abs(until - from);
    RawCode result = new_RawCode(size);
    for (Iterator i = from; i < until; i++)
        result.array[result.used++] = src.array[i];
    return result;
}

Value quote_of_char_to_string(const RawCode rcode)
{
    if (rcode.used == 0)
        return RCL_String("");
    RCL_Value_String_t result = malloc(rcode.used + 1);
    for (Iterator i = 0; i < rcode.used; i++)
        result[i] = rcode.array[i].u.char_;
    result[rcode.used + 1] = '\0';
    return RCL_String(result);
}
