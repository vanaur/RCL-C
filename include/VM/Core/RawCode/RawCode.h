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
#include <stddef.h>
#include <gmp.h>
#include <VM\Core\RawCode\Combinator\Combinators.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>

typedef struct RawCode RawCode;

// An enumeration of all possible types of values
// (there are currently 15 + 1 if you count concatenation)
enum Value_Kind
{
    RCL_Value_Array,
    RCL_Value_Quotation,
    RCL_Value_Integer,
    RCL_Value_Float,
    RCL_Value_Char,
    RCL_Value_String,
    RCL_Value_Word,
    RCL_Value_Lambda,
    RCL_Value_EndLamScope,
    RCL_Value_LiteralOperation,
    RCL_Value_Combinator,
    RCL_Value_Parallel,
    RCL_Value_Field,
    RCL_Value_DataStruct,
    RCL_Value_Replicated,
    RCL_Value_Empty,
    RCL_Value_Number,
    RCL_Value_Concatenation,
    RCL_Value_All
};

#define lo_add 0
#define lo_sub 1
#define lo_pow 2
#define lo_mul 3
#define lo_div 4
#define lo_mod 5
#define lo_inc 6
#define lo_dec 7
#define lo_band 8
#define lo_bor 9
#define lo_bxor 10
#define lo_bleft 11
#define lo_bright 12

typedef struct ValueTag Value;

// Returns the value kind in textual form.
String show_kind(enum Value_Kind);

struct RCL_Value_DataStruct_Field
{
    // Infos about the field of the defined structure as pointer => template
    struct RCL_Structure_field *field_type_info;
    // The value of the field; if the field is an enumeration field, then there is no value
    Value *field_value;
    /*     // The parent structure template
    struct RCL_Structure *template; */
} __attribute__((packed));

// This is a structure data on the stack
struct RCL_Value_DataStruct
{
    // Infos about the origin defined structure (type infos pointer)
    struct RCL_Structure *template;
    // Array of RCL_Value fields
    struct RCL_Value_DataStruct_Field *fields;
} __attribute__((packed));

// Return a pointer of the specific element or NULL of it dosen't exist
struct RCL_Value_DataStruct_Field *getSpecific_DataStruct_field(struct RCL_Value_DataStruct, String);

// Make a data structure value without filled fields => just structure template pointer
struct RCL_Value_DataStruct make_DataStruct_unfilled(struct RCL_Structure *);

typedef RawCode *RCL_Value_Quote_t;
typedef RawCode *RCL_Value_Concatenation_t;
typedef mpz_t RCL_Value_Integer_t;
typedef mpf_t RCL_Value_Float_t;
typedef char RCL_Value_Char_t;
typedef String RCL_Value_String_t;
typedef struct
{
    String word_str;
    hash_t hash_code;
} RCL_Value_Word_t;
typedef String RCL_Value_LamdaDecl_t;
typedef String RCL_Value_EndLambdaScope_t;
typedef LiteralOperation RCL_Value_LiteralOperation_t;
typedef Combinator RCL_Value_Combinator_t;
typedef struct
{
    Value *v1, *v2;
} __attribute__((packed)) RCL_Value_Parallel_t;
typedef RCL_Structure_Field RCL_Value_Field_t;
typedef struct RCL_Value_DataStruct RCL_Value_DataStruct_t;
typedef struct
{
    RawCode *array;
    size_t length;
} __attribute__((packed)) RCL_Value_Array_t;
typedef struct
{
    Value *op;
    size_t times;
} __attribute__((packed)) RCL_Value_Repeat_t;

typedef struct ValueTag
{
    enum Value_Kind kind;
    union {
        RCL_Value_Quote_t quote_;
        RCL_Value_Integer_t int_;
        RCL_Value_Float_t float_;
        RCL_Value_Char_t char_;
        RCL_Value_String_t string_;
        RCL_Value_Word_t word_;
        RCL_Value_LamdaDecl_t lam_;
        RCL_Value_EndLambdaScope_t endLamScope_;
        RCL_Value_LiteralOperation_t litOperation_;
        RCL_Value_Combinator_t comb_;
        RCL_Value_Parallel_t parallel_;
        RCL_Value_Field_t field_;
        RCL_Value_DataStruct_t dataStruct_;
        RCL_Value_Array_t array_;
        RCL_Value_Concatenation_t concatenation_;
        RCL_Value_Repeat_t repeat_;
    } u;
} __attribute__((packed)) Value;

// This is the content of case `case` of the array `vname`
// This is a concatenation of terms
#define RCL_ARRAY_CONTENT_CASE(vname, case) vname.u.array_.array->array[case].u.concatenation_

// Shorter names as macros
// To build a more visual AST :

#define RCL_Quotation(Quote) make_RCL_Value_Quotation(Quote)
#define RCL_Integer(Integer) make_RCL_Value_Integer(Integer)
#define RCL_Float(Float) make_RCL_Value_Float(Float)
#define RCL_Integer_I(Integer) make_RCL_Value_Integer_i(Integer)
#define RCL_Float_F(Float) make_RCL_Value_Float_f(Float)
#define RCL_Char(Char) make_RCL_Value_Char(Char)
#define RCL_String(String) make_RCL_Value_String(String)
#define RCL_Word(Word) make_RCL_Value_Word(Word)
#define RCL_EndLamScope(EndLambdaScope) make_RCL_Value_EndLamScope(EndLambdaScope)
#define RCL_Lambda(LamdaDecl) make_RCL_Value_Lambda(LamdaDecl)
#define RCL_All() \
    (Value) { .kind = RCL_Value_All }
#define RCL_LiteralOperation_kind(LiteralOperation) make_RCL_Value_LiteralOperation_kind(LiteralOperation)
#define RCL_Combinator(Combinator) make_RCL_Value_Combinator(Combinator)
#define RCL_Parallel(Value1, Value2) make_RCL_Value_Parallel(Value1, Value2)
#define RCL_Field(Field) make_RCL_Value_Field(Field)
#define RCL_DataStruct(DataStruct) make_RCL_Value_DataStruct(DataStruct)
#define RCL_Replicate(Size, Value) make_RCL_Value_Replicate(Size, Value)
#define RCL_Array(Array, Size) make_RCL_Value_Array(Array, Size)

Value make_RCL_Value_Quotation(const RCL_Value_Quote_t);
Value make_RCL_Value_Integer(const RCL_Value_Integer_t);
Value make_RCL_Value_Integer_i(const signed int);
Value make_RCL_Value_Float(const RCL_Value_Float_t);
Value make_RCL_Value_Float_f(const double);
Value make_RCL_Value_Char(const RCL_Value_Char_t);
Value make_RCL_Value_String(const RCL_Value_String_t);
Value make_RCL_Value_Word(const String);
Value make_RCL_Value_EndLamScope(const RCL_Value_EndLambdaScope_t);
Value make_RCL_Value_Lambda(const RCL_Value_LamdaDecl_t);
Value make_RCL_Value_LiteralOperation(const RCL_Value_LiteralOperation_t);
Value make_RCL_Value_LiteralOperation_kind(const unsigned short);
Value make_RCL_Value_Combinator(const RCL_Value_Combinator_t);
Value make_RCL_Value_Parallel(Value, Value);
Value make_RCL_Value_Field(const RCL_Value_Field_t);
Value make_RCL_Value_DataStruct(const RCL_Value_DataStruct_t);
Value make_RCL_Value_Replicate(const size_t, const Value);
Value make_RCL_Value_Array(RawCode *, const size_t);
Value make_RCL_Value_Concatenation(RawCode *);
Value otov(Operation);

#define RCL_NIL_WRD "nil__"

/* struct RawCode
{
    Value *array;
    size_t used;
    size_t size;
} __attribute__((packed)); */

/*** Values vectorization under concatenation form ***/

typedef Vector(RawCode, Value);

#define RCODE_TO_STACK(rcode)     (*((Stack *)&rcode))
#define STACK_TO_RCODE(stack)     (*((RawCode *)&stack))
#define RCODE_TO_STACK_PTR(rcode) ((Stack *)rcode)
#define STACK_TO_RCODE_PTR(stack) ((RawCode *)stack)

void init_rcode(RawCode *, size_t);
void push_rcode(RawCode *, Value);
void pop_rcode(RawCode *);

// Concatenates the first `RawCode` with the second given.
void concat_rcode(RawCode *, RawCode *);
// Concatenates the first `RawCode` with the second given but from the given index.
void concat_rcode_from(RawCode *, RawCode *, size_t);
// Concatenates the first `RawCode` with the second given but between the given indexes.
void concat_rcode_until(RawCode *, RawCode *, size_t, size_t);
// Get a subvector of the given raw code.
RawCode fast_rcode_subv(const RawCode src, const size_t from, const size_t until);

// Replace all sequences corresponding to `seq1` with `seq2` in the sent `RawCode`.
// Length must be specified.
void seq_replace(
    RawCode *restrict rcode,
    const Value seq1[], size_t size1,
    const Value seq2[], size_t size2);

// Pop `RawCode` and returns is value.
Value drop_rcode(RawCode *);
// Returns the top of the `RawCode`.
Value top_rcode(RawCode *);

size_t count_operations(ListOperation);
size_t count_operations_bykind(RawCode, enum Value_Kind);

static const Value emptyRawCodeValue = {.kind = RCL_Value_Empty};
