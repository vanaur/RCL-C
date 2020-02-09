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

#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

#include <VM\Core\Syntax\Absyn.h>

typedef union
{
  int int_;
  char char_;
  double double_;
  char* string_;
  Program program_;
  Identifier identifier_;
  Preprocessor preprocessor_;
  Definition definition_;
  RCL_Structure_Field rcl_structure_field_;
  FFI_Type_Signature ffi_type_signature_;
  Ptr ptr_;
  FFI_Type_SignatureT ffi_type_signaturet_;
  Operation operation_;
  LiteralOperation literaloperation_;
  Data data_;
  ListFFI_Type_Signature listffi_type_signature_;
  ListPtr listptr_;
  ListOperation listoperation_;
  ListDefinition listdefinition_;
  ListPreprocessor listpreprocessor_;
  ListRCL_Structure_Field listrcl_structure_field_;
} YYSTYPE;

#define _ERROR_ 258
#define _SYMB_0 259
#define _SYMB_1 260
#define _SYMB_2 261
#define _SYMB_3 262
#define _SYMB_4 263
#define _SYMB_5 264
#define _SYMB_6 265
#define _SYMB_7 266
#define _SYMB_8 267
#define _SYMB_9 268
#define _SYMB_10 269
#define _SYMB_11 270
#define _SYMB_12 271
#define _SYMB_13 272
#define _SYMB_14 273
#define _SYMB_15 274
#define _SYMB_16 275
#define _SYMB_17 276
#define _SYMB_18 277
#define _SYMB_19 278
#define _SYMB_20 279
#define _SYMB_21 280
#define _SYMB_22 281
#define _SYMB_23 282
#define _SYMB_24 283
#define _SYMB_25 284
#define _SYMB_26 285
#define _SYMB_27 286
#define _SYMB_28 287
#define _SYMB_29 288
#define _SYMB_30 289
#define _SYMB_31 290
#define _SYMB_32 291
#define _SYMB_33 292
#define _SYMB_34 293
#define _SYMB_35 294
#define _SYMB_36 295
#define _SYMB_37 296
#define _SYMB_38 297
#define _SYMB_39 298
#define _SYMB_40 299
#define _SYMB_41 300
#define _SYMB_42 301
#define _SYMB_43 302
#define _SYMB_44 303
#define _SYMB_45 304
#define _SYMB_46 305
#define _SYMB_47 306
#define _SYMB_48 307
#define _SYMB_49 308
#define _SYMB_50 309
#define _SYMB_51 310
#define _SYMB_52 311
#define _SYMB_53 312
#define _SYMB_54 313
#define _SYMB_55 314
#define _SYMB_56 315
#define _SYMB_57 316
#define _SYMB_58 317
#define _SYMB_59 318
#define _SYMB_60 319
#define _SYMB_61 320
#define _SYMB_62 321
#define _SYMB_63 322
#define _SYMB_64 323
#define _SYMB_65 324
#define _SYMB_66 325
#define _SYMB_67 326
#define _SYMB_68 327
#define _SYMB_69 328
#define _SYMB_70 329
#define _STRING_ 330
#define _CHAR_ 331
#define _INTEGER_ 332
#define _DOUBLE_ 333

extern YYSTYPE yylval;
Program pProgram(FILE *inp);


#endif
