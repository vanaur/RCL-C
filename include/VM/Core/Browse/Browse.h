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
#include <stdlib.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\FFI\RCL_File.h>
#include <VM\Core\Wordefinition\Wordico.h>
#include <VM\Core\Browse\BrowsedAbsyn.h>
#include <VM\Core\Browse\BResult.h>

#define IsInFn 1
#define IsNotInFn 0

BResult browseAbsyn(Program, String);

/*** Browse type of programs ***/

void browsePVoid(BResult *);          // An empty program
void browseProg1(Program, BResult *); // Idata + definitions + code
void browseProg2(Program, BResult *); // Idata + definitions
void browseProg3(Program, BResult *); // Idata + code
void browseProg4(Program, BResult *); // Definitions + code
void browseProg5(Program, BResult *); // Definitions
void browseProg6(Program, BResult *); // Code (with section '.code' indicated)
void browseProg7(Program, BResult *); // Simple code
void browseProg8(Program, BResult *); // Simple preprocessor

void handle_absynInclude(BResult *, Preprocessor);
void handle_absynImportAs(BResult *, Preprocessor);
void handle_absynImport(BResult *, Preprocessor);

int canHandleLiteralOp(RawCode, LiteralOperation);
int rcodeContainsEOSV(RawCode *, char *);

size_t count_includes(ListPreprocessor);
size_t count_imports(ListPreprocessor);

struct Code_handler
{
    BResult *bresult;
    ListOperation lo;
} __attribute__((packed));

struct Definition_handler
{
    struct Wordico *wordico;
    BrowsedAbsyn *absyn;
    ListDefinition ld;
} __attribute__((packed));

struct Preprocessor_handler
{
    BResult *bresult;
    ListPreprocessor lp;
} __attribute__((packed));

void handle_code(struct Code_handler *);
void handle_definitions(struct Definition_handler *);
void handle_preprocessor(struct Preprocessor_handler *);

void handle_word(BResult *, String);
void handle_lambda(BResult *, String);
void handle_endlambda(BResult *, String);

void case_operation(BResult *, Operation);
void case_preprocessor(BResult *, Preprocessor p);
