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
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Execution\Compiler\Assembler\Opcode.h>

#define ENTRY_NAME "RCL_entry_main"
#define QUOTATION_LABEL_NAME "qte"
#define CST_ARRAY_LABEL_NAME "ctb"
#define STRING_LABEL_NAME "str"

struct Bunch
{
    Labels result;
    const BResult *bresult;
    Lenv_map_t lenv;
    label_count_t lbl_count;
    OnlyStrLbl_const_lit_map_t cst_str_map;
    HashOpBloLbl_const_map_t cst_lbl_map;
};

struct Bunch init_bunch(const BResult *);
Assembled_Program assemble(const BResult *);
