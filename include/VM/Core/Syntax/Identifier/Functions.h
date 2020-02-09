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

#include <VM\Core\Syntax\Absyn.h>
#include <ctype.h>

// Functions to work with AST nodes of identifiers only.
// They do not take into account the context of use to provide a result.

// Return the size of the identifier (as string size)
size_t identifierLength(Identifier);
// Return the number of qual names (X.Y.Z => 3 quals)
size_t countQuals(Identifier);
// Return a string formed by all the Identifier
char *showIdentifier(Identifier);
// Return the DLL name of the `extern` field
char *getDLL_extern(Definition);
// Return the DLL given identifier of an identifier field assuming it is a DLL name
char *getDLL_asid(Identifier);
// Return the last qual
char *getLast(Identifier);
// From a word as `Abc.def` return `def`
char *getWordName(char *);
// From a word as `Abc.def` return `Abc`
char *getWordPath(char *);
