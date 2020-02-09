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

#ifndef ABSYN_HEADER
#define ABSYN_HEADER

/* C++ Abstract Syntax Interface generated by the BNF Converter.*/

/********************   TypeDef Section    ********************/
typedef int Integer;
typedef char Char;
typedef double Double;
typedef char* String;
typedef char* Ident;
typedef char* UIdent;
typedef char* LIdent;


/********************   Forward Declarations    ********************/

struct ListOperation_;
typedef struct ListOperation_ *ListOperation;
struct ListFFI_Type_Signature_;
typedef struct ListFFI_Type_Signature_ *ListFFI_Type_Signature;
struct ListPtr_;
typedef struct ListPtr_ *ListPtr;
struct ListDefinition_;
typedef struct ListDefinition_ *ListDefinition;
struct ListPreprocessor_;
typedef struct ListPreprocessor_ *ListPreprocessor;
struct ListRCL_Structure_Field_;
typedef struct ListRCL_Structure_Field_ *ListRCL_Structure_Field;
struct Program_;
typedef struct Program_ *Program;
struct Identifier_;
typedef struct Identifier_ *Identifier;
struct Preprocessor_;
typedef struct Preprocessor_ *Preprocessor;
struct Definition_;
typedef struct Definition_ *Definition;
struct RCL_Structure_Field_;
typedef struct RCL_Structure_Field_ *RCL_Structure_Field;
struct FFI_Type_Signature_;
typedef struct FFI_Type_Signature_ *FFI_Type_Signature;
struct Ptr_;
typedef struct Ptr_ *Ptr;
struct FFI_Type_SignatureT_;
typedef struct FFI_Type_SignatureT_ *FFI_Type_SignatureT;
struct Operation_;
typedef struct Operation_ *Operation;
struct LiteralOperation_;
typedef struct LiteralOperation_ *LiteralOperation;
struct Data_;
typedef struct Data_ *Data;


/********************   Abstract Syntax Classes    ********************/

struct Program_
{
  enum { is_PVoid, is_Prog1, is_Prog2, is_Prog3, is_Prog4, is_Prog5, is_Prog6, is_Prog7, is_Prog8 } kind;
  union
  {
    struct { ListDefinition listdefinition_; ListPreprocessor listpreprocessor_; Operation operation_; } prog1_;
    struct { ListDefinition listdefinition_; ListPreprocessor listpreprocessor_; } prog2_;
    struct { ListPreprocessor listpreprocessor_; Operation operation_; } prog3_;
    struct { ListDefinition listdefinition_; Operation operation_; } prog4_;
    struct { ListDefinition listdefinition_; } prog5_;
    struct { Operation operation_; } prog6_;
    struct { Operation operation_; } prog7_;
    struct { ListPreprocessor listpreprocessor_; } prog8_;
  } u;
};

Program make_PVoid(void);
Program make_Prog1(ListPreprocessor p0, ListDefinition p1, Operation p2);
Program make_Prog2(ListPreprocessor p0, ListDefinition p1);
Program make_Prog3(ListPreprocessor p0, Operation p1);
Program make_Prog4(ListDefinition p0, Operation p1);
Program make_Prog5(ListDefinition p0);
Program make_Prog6(Operation p0);
Program make_Prog7(Operation p0);
Program make_Prog8(ListPreprocessor p0);

struct Identifier_
{
  enum { is_Name, is_QualName } kind;
  union
  {
    struct { LIdent lident_; } name_;
    struct { Identifier identifier_; UIdent uident_; } qualname_;
  } u;
};

Identifier make_Name(LIdent p0);
Identifier make_QualName(UIdent p0, Identifier p1);

struct Preprocessor_
{
  enum { is_Import, is_ImportAs, is_Include, is_Define, is_IF, is_ELSE, is_Error, is_Warning, is_Info, is_FaultInt, is_FaultId } kind;
  union
  {
    struct { String string_; } import_;
    struct { String string_; UIdent uident_; } importas_;
    struct { String string_; UIdent uident_; } include_;
    struct { UIdent uident_; } define_;
    struct { Preprocessor preprocessor_; UIdent uident_; } if_;
    struct { Preprocessor preprocessor_1, preprocessor_2; UIdent uident_; } else_;
    struct { String string_; } error_;
    struct { String string_; } warning_;
    struct { String string_; } info_;
    struct { Integer integer_; } faultint_;
    struct { UIdent uident_; } faultid_;
  } u;
};

Preprocessor make_Import(String p0);
Preprocessor make_ImportAs(String p0, UIdent p1);
Preprocessor make_Include(String p0, UIdent p1);
Preprocessor make_Define(UIdent p0);
Preprocessor make_IF(UIdent p0, Preprocessor p1);
Preprocessor make_ELSE(UIdent p0, Preprocessor p1, Preprocessor p2);
Preprocessor make_Error(String p0);
Preprocessor make_Warning(String p0);
Preprocessor make_Info(String p0);
Preprocessor make_FaultInt(Integer p0);
Preprocessor make_FaultId(UIdent p0);

struct Definition_
{
  enum { is_Def, is_Extern, is_Structure } kind;
  union
  {
    struct { LIdent lident_; Operation operation_; } def_;
    struct { FFI_Type_Signature ffi_type_signature_; Identifier identifier_; ListFFI_Type_Signature listffi_type_signature_; } extern_;
    struct { ListRCL_Structure_Field listrcl_structure_field_; UIdent uident_; } structure_;
  } u;
};

Definition make_Def(LIdent p0, Operation p1);
Definition make_Extern(Identifier p0, ListFFI_Type_Signature p1, FFI_Type_Signature p2);
Definition make_Structure(UIdent p0, ListRCL_Structure_Field p1);

struct RCL_Structure_Field_
{
  enum { is_FreeField, is_EnumField, is_SpecField } kind;
  union
  {
    struct { LIdent lident_; } freefield_;
    struct { LIdent lident_; } enumfield_;
    struct { Identifier identifier_; LIdent lident_; } specfield_;
  } u;
};

RCL_Structure_Field make_FreeField(LIdent p0);
RCL_Structure_Field make_EnumField(LIdent p0);
RCL_Structure_Field make_SpecField(LIdent p0, Identifier p1);

struct FFI_Type_Signature_
{
  enum { is_TPrimitive, is_TStruct, is_TSSelf, is_TPtr } kind;
  union
  {
    struct { FFI_Type_SignatureT ffi_type_signaturet_; } tprimitive_;
    struct { ListFFI_Type_Signature listffi_type_signature_; } tstruct_;
    struct { FFI_Type_Signature ffi_type_signature_; ListPtr listptr_; } tptr_;
  } u;
};

FFI_Type_Signature make_TPrimitive(FFI_Type_SignatureT p0);
FFI_Type_Signature make_TStruct(ListFFI_Type_Signature p0);
FFI_Type_Signature make_TSSelf(void);
FFI_Type_Signature make_TPtr(FFI_Type_Signature p0, ListPtr p1);

struct Ptr_
{
  enum { is_Point } kind;
  union
  {
  } u;
};

Ptr make_Point(void);

struct FFI_Type_SignatureT_
{
  enum { is_TVoid, is_TUint8, is_TSint8, is_TUint16, is_TSint16, is_TUint32, is_TSint32, is_TUint64, is_TSint64, is_TFloat, is_TLongDouble, is_TDouble, is_TUchar, is_TSchar, is_TUshort, is_TSshort, is_TUint, is_TSint, is_Tint, is_TUlong, is_TSlong, is_TAny } kind;
  union
  {
  } u;
};

FFI_Type_SignatureT make_TVoid(void);
FFI_Type_SignatureT make_TUint8(void);
FFI_Type_SignatureT make_TSint8(void);
FFI_Type_SignatureT make_TUint16(void);
FFI_Type_SignatureT make_TSint16(void);
FFI_Type_SignatureT make_TUint32(void);
FFI_Type_SignatureT make_TSint32(void);
FFI_Type_SignatureT make_TUint64(void);
FFI_Type_SignatureT make_TSint64(void);
FFI_Type_SignatureT make_TFloat(void);
FFI_Type_SignatureT make_TLongDouble(void);
FFI_Type_SignatureT make_TDouble(void);
FFI_Type_SignatureT make_TUchar(void);
FFI_Type_SignatureT make_TSchar(void);
FFI_Type_SignatureT make_TUshort(void);
FFI_Type_SignatureT make_TSshort(void);
FFI_Type_SignatureT make_TUint(void);
FFI_Type_SignatureT make_TSint(void);
FFI_Type_SignatureT make_Tint(void);
FFI_Type_SignatureT make_TUlong(void);
FFI_Type_SignatureT make_TSlong(void);
FFI_Type_SignatureT make_TAny(void);

struct Operation_
{
  enum { is_Concatenation, is_RepeatOperation, is_ParallelConcat, is_Lambda, is_EndLambdaScope, is_Quote, is_List, is_Var, is_Lit, is_BasicLiteralOp, is_Field } kind;
  union
  {
    struct { ListOperation listoperation_; } concatenation_;
    struct { Integer integer_; Operation operation_; } repeatoperation_;
    struct { Operation operation_1, operation_2; } parallelconcat_;
    struct { LIdent lident_; } lambda_;
    struct { LIdent lident_; } endlambdascope_;
    struct { Operation operation_; } quote_;
    struct { ListOperation listoperation_; } list_;
    struct { Identifier identifier_; } var_;
    struct { Data data_; } lit_;
    struct { LiteralOperation literaloperation_; } basicliteralop_;
    struct { RCL_Structure_Field rcl_structure_field_; } field_;
  } u;
};

Operation make_Concatenation(ListOperation p0);
Operation make_RepeatOperation(Integer p0, Operation p1);
Operation make_ParallelConcat(Operation p0, Operation p1);
Operation make_Lambda(LIdent p0);
Operation make_EndLambdaScope(LIdent p0);
Operation make_Quote(Operation p0);
Operation make_List(ListOperation p0);
Operation make_Var(Identifier p0);
Operation make_Lit(Data p0);
Operation make_BasicLiteralOp(LiteralOperation p0);
Operation make_Field(RCL_Structure_Field p0);

struct LiteralOperation_
{
  enum { is_Add, is_Sub, is_Pow, is_Mul, is_Div, is_Mod, is_Inc, is_Dec, is_BAnd, is_BOr, is_BXor, is_BLeft, is_BRight } kind;
  union
  {
  } u;
};

LiteralOperation make_Add(void);
LiteralOperation make_Sub(void);
LiteralOperation make_Pow(void);
LiteralOperation make_Mul(void);
LiteralOperation make_Div(void);
LiteralOperation make_Mod(void);
LiteralOperation make_Inc(void);
LiteralOperation make_Dec(void);
LiteralOperation make_BAnd(void);
LiteralOperation make_BOr(void);
LiteralOperation make_BXor(void);
LiteralOperation make_BLeft(void);
LiteralOperation make_BRight(void);

struct Data_
{
  enum { is_Int, is_Flt, is_Chr, is_Str } kind;
  union
  {
    struct { Integer integer_; } int_;
    struct { Double double_; } flt_;
    struct { Char char_; } chr_;
    struct { String string_; } str_;
  } u;
};

Data make_Int(Integer p0);
Data make_Flt(Double p0);
Data make_Chr(Char p0);
Data make_Str(String p0);

struct ListOperation_
{
  Operation operation_;
  ListOperation listoperation_;
};

ListOperation make_ListOperation(Operation p1, ListOperation p2);
struct ListFFI_Type_Signature_
{
  FFI_Type_Signature ffi_type_signature_;
  ListFFI_Type_Signature listffi_type_signature_;
};

ListFFI_Type_Signature make_ListFFI_Type_Signature(FFI_Type_Signature p1, ListFFI_Type_Signature p2);
struct ListPtr_
{
  Ptr ptr_;
  ListPtr listptr_;
};

ListPtr make_ListPtr(Ptr p1, ListPtr p2);
struct ListDefinition_
{
  Definition definition_;
  ListDefinition listdefinition_;
};

ListDefinition make_ListDefinition(Definition p1, ListDefinition p2);
struct ListPreprocessor_
{
  Preprocessor preprocessor_;
  ListPreprocessor listpreprocessor_;
};

ListPreprocessor make_ListPreprocessor(Preprocessor p1, ListPreprocessor p2);
struct ListRCL_Structure_Field_
{
  RCL_Structure_Field rcl_structure_field_;
  ListRCL_Structure_Field listrcl_structure_field_;
};

ListRCL_Structure_Field make_ListRCL_Structure_Field(RCL_Structure_Field p1, ListRCL_Structure_Field p2);


#endif
