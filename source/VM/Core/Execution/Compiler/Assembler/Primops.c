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

#include <gmp.h>
#include <stdlib.h>

#include <VM\Core\Execution\Compiler\Assembler\Primops.h>
#include <VM\Core\Execution\Compiler\Assembler\Opcode.h>

#define cgen_jit_primitive__FNi(lid, gmp_fn)                                   \
    Opcode_block block;                                                        \
    init_Opcode_block(&block, 3);                                              \
    pblock_assemble(&block, 3,                                                 \
                    OPCODE_INSTR_MOV(_EDI, _EDX),                              \
                    OPCODE_INSTR_MOV(_EBX, _ECX),                              \
                    OPCODE_INSTR_JMP(OPCODE_VALUE_ADRESS_RAW(gmp_fn))); \
    push_labels_from_oblock(labels_ptr, lid, block, NULL);

void cgen_jit_primitive__addi(Labels *labels_ptr)
{
    cgen_jit_primitive__FNi("jit_primitive_primop__addi", __gmpz_add_ui);
}

void cgen_jit_primitive__subi(Labels *labels_ptr)
{
    cgen_jit_primitive__FNi("jit_primitive_primop__subi", __gmpz_sub_ui);
}

void cgen_jit_primitive__muli(Labels *labels_ptr)
{
    cgen_jit_primitive__FNi("jit_primitive_primop__muli", __gmpz_mul_ui);
}

void cgen_jit_primitive__divi(Labels *labels_ptr)
{
    cgen_jit_primitive__FNi("jit_primitive_primop__divi", __gmpz_fdiv_ui);
}

