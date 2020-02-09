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

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <x86intrin.h>
#include <VM\Core\Execution\Compiler\Assembler\Opcode.h>
#include <VM\Core\Execution\Compiler\Assembler\Hash.h>

Register opcode_make_register(enum Register_Size_kind reg_kind_size, reg_enum_t reg)
{
    Register result;
    result.kind = reg_kind_size;
    switch (reg_kind_size)
    {
    case _8:
        result.u.rs_8_.r = (enum Register_8)reg;
        break;

    case _16:
        result.u.rs_8_.r = (enum Register_16)reg;
        break;

    case _32:
        result.u.rs_8_.r = (enum Register_32)reg;
        break;

    case _64:
        result.u.rs_8_.r = (enum Register_64)reg;
        break;

    case _128:
        result.u.rs_8_.r = (enum Register_128)reg;
        break;

    case _256:
        result.u.rs_8_.r = (enum Register_256)reg;
        break;

    case _512:
        result.u.rs_8_.r = (enum Register_512)reg;
        break;
    }
    return result;
}

bool cmp_reg(const Register r1, const Register r2)
{
    switch (r1.kind)
    {
    case _8:
        return r1.u.rs_8_.r == r2.u.rs_8_.r;
    case _16:
        return r1.u.rs_16_.r == r2.u.rs_16_.r;
    case _32:
        return r1.u.rs_32_.r == r2.u.rs_32_.r;
    case _64:
        return r1.u.rs_64_.r == r2.u.rs_64_.r;
    case _128:
        return r1.u.rs_128_.r == r2.u.rs_128_.r;
    case _256:
        return r1.u.rs_256_.r == r2.u.rs_256_.r;
    case _512:
        return r1.u.rs_512_.r == r2.u.rs_512_.r;
    }
}

Register next_reg(const Register reg)
{
    switch (reg.kind)
    {
    case _8:
        return opcode_make_register(_8, reg.u.rs_8_.r + 1);
    case _16:
        return opcode_make_register(_16, reg.u.rs_16_.r + 1);
    case _32:
        return opcode_make_register(_32, reg.u.rs_32_.r + 1);
    case _64:
        return opcode_make_register(_64, reg.u.rs_64_.r + 1);
    case _128:
        return opcode_make_register(_128, reg.u.rs_128_.r + 1);
    case _256:
        return opcode_make_register(_256, reg.u.rs_256_.r + 1);
    case _512:
        return opcode_make_register(_512, reg.u.rs_512_.r + 1);
    }
}

Regi make_regi(Register reg, int loc)
{
    return (Regi){reg, loc};
}

bool cmp_regi(Regi x, Regi y)
{
    if (x.location != y.location)
        return false;
    return cmp_reg(x.reg, y.reg);
}

bool cmp_regi_only_reg(Regi x, Regi y)
{
    return cmp_reg(x.reg, y.reg);
}

inline ASM_Value opcode_make_asm_value__reg(Register reg)
{
    return SIGMA_FILL_CTOR(ASM_Value, REGISTER, register, reg);
}

inline ASM_Value opcode_make_asm_value__num(double num)
{
    return SIGMA_FILL_CTOR(ASM_Value, NUMBER, number, num);
}

/*
Value make_RCL_Value_Parallel(Value f1, Value f2)
{
    Value *tmp1 = (Value *)malloc(sizeof(*tmp1));
    Value *tmp2 = (Value *)malloc(sizeof(*tmp2));
    tmp1->kind = f1.kind;
    tmp2->kind = f2.kind;
    tmp1->u = f1.u;
    tmp2->u = f2.u;
    return (Value){.kind = RCL_Value_Parallel, .u = {.parallel_ = {.v1 = tmp1, .v2 = tmp2}}};
}
*/

inline ASM_Value opcode_make_asm_value__adress_reg(ASM_Value v)
{
    ASM_Value *v_ptr = (ASM_Value *)malloc(sizeof *v_ptr);
    *v_ptr = v;
    return SIGMA_FILL_CTOR(ASM_Value, ADRESS_REG, adress_reg, v_ptr);
}

inline ASM_Value opcode_make_asm_value__adress_raw(void *adrr)
{
    return SIGMA_FILL_CTOR(ASM_Value, ADRESS_RAW, adress_raw, adrr);
}

inline ASM_Value opcode_make_asm_value__label(String identifier)
{
    return SIGMA_FILL_CTOR(ASM_Value, LABEL, label, hash_djb2(identifier), identifier);
}

inline ASM_Value opcode_make_asm_value__add(ASM_Value v1, ASM_Value v2)
{
    ASM_Value *v1_ptr = (ASM_Value *)malloc(sizeof *v1_ptr);
    ASM_Value *v2_ptr = (ASM_Value *)malloc(sizeof *v2_ptr);
    *v1_ptr = v1;
    *v2_ptr = v2;
    return SIGMA_FILL_CTOR(ASM_Value, ADD, asmv_add, v1_ptr, v2_ptr);
}

inline ASM_Value opcode_make_asm_value__sub(ASM_Value v1, ASM_Value v2)
{
    ASM_Value *v1_ptr = (ASM_Value *)malloc(sizeof *v1_ptr);
    ASM_Value *v2_ptr = (ASM_Value *)malloc(sizeof *v2_ptr);
    *v1_ptr = v1;
    *v2_ptr = v2;
    return SIGMA_FILL_CTOR(ASM_Value, SUB, asmv_sub, v1_ptr, v2_ptr);
}

inline ASM_Value opcode_make_asm_value__mul(ASM_Value v1, ASM_Value v2)
{
    ASM_Value *v1_ptr = (ASM_Value *)malloc(sizeof *v1_ptr);
    ASM_Value *v2_ptr = (ASM_Value *)malloc(sizeof *v2_ptr);
    *v1_ptr = v1;
    *v2_ptr = v2;
    return SIGMA_FILL_CTOR(ASM_Value, MUL, asmv_mul, v1_ptr, v2_ptr);
}

inline ASM_Instr opcode_make_opcode__mov(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_MOV, mov, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__lea(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_LEA, lea, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__imul(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_IMUL, lea, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__test(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_TEST, test, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__xor(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_XOR, xor, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__shr(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_SHR, shr, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__shl(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCDOE_SHL, shl, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__push(src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_PUSH, push, src);
}

inline ASM_Instr opcode_make_opcode__fabs(src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_FABS, fabs, src);
}

inline ASM_Instr opcode_make_opcode__fchs(src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_FCHS, fchs, src);
}

inline ASM_Instr opcode_make_opcode__pop(src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_POP, pop, src);
}

inline ASM_Instr opcode_make_opcode__add(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_ADD, add, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__sub(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_SUB, sub, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__inc(dest_t dest)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_INC, inc, dest);
}

inline ASM_Instr opcode_make_opcode__dec(dest_t dest)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_DEC, dec, dest);
}

inline ASM_Instr opcode_make_opcode__mul(src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_MUL, mul, src);
}

inline ASM_Instr opcode_make_opcode__div(src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_DIV, div, src);
}

inline ASM_Instr opcode_make_opcode__call(adrr_t adrr)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_CALL, call, adrr);
}

inline ASM_Instr opcode_make_opcode__cdq()
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_CDQ, cdq);
}

inline ASM_Instr opcode_make_opcode__ret(void)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_RET, ret);
}

inline ASM_Instr opcode_make_opcode__empty(void)
{
    return SIGMA_FILL_CTOR(ASM_Instr, EMPTY, empty);
}

inline ASM_Instr opcode_make_opcode__fnop(void)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_FNOP, fnop);
}

inline ASM_Instr opcode_make_opcode__syscall(void)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_SYSCALL, syscall);
}

inline ASM_Instr opcode_make_opcode__cmp(dest_t dest, src_t src)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_CMP, cmp, FILL_DS__nf(dest, src));
}

inline ASM_Instr opcode_make_opcode__je(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_JE, je, mem);
}

inline ASM_Instr opcode_make_opcode__jne(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_JNE, jne, mem);
}

inline ASM_Instr opcode_make_opcode__jg(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_JG, jg, mem);
}

inline ASM_Instr opcode_make_opcode__jge(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_JGE, jge, mem);
}

inline ASM_Instr opcode_make_opcode__jl(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_JL, jl, mem);
}

inline ASM_Instr opcode_make_opcode__jle(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_JLE, jle, mem);
}

inline ASM_Instr opcode_make_opcode__jmp(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_JMP, jmp, mem);
}

inline ASM_Instr opcode_make_opcode__sete(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_SETE, sete, mem);
}

inline ASM_Instr opcode_make_opcode__setne(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_SETNE, setne, mem);
}

inline ASM_Instr opcode_make_opcode__setg(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_SETG, setg, mem);
}

inline ASM_Instr opcode_make_opcode__setge(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_SETGE, setge, mem);
}

inline ASM_Instr opcode_make_opcode__setl(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_SETL, setl, mem);
}

inline ASM_Instr opcode_make_opcode__setle(mem_t mem)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_SETLE, setle, mem);
}

inline ASM_Instr opcode_make_opcode__comment(String text)
{
    return SIGMA_FILL_CTOR(ASM_Instr, OPCODE_COMMENT, comment, text);
}

inline ASM_Instr opcode_make_directive__byte(directivy_num_t num)
{
    return SIGMA_FILL_CTOR(ASM_Instr, DIRECTIVE_BYTE, byte, num);
}

inline ASM_Instr opcode_make_directive__2byte(directivy_num_t num)
{
    return SIGMA_FILL_CTOR(ASM_Instr, DIRECTIVE_2BYTE, byte2, num);
}

inline ASM_Instr opcode_make_directive__4byte(directivy_num_t num)
{
    return SIGMA_FILL_CTOR(ASM_Instr, DIRECTIVE_4BYTE, byte8, num);
}

inline ASM_Instr opcode_make_directive__8byte(directivy_num_t num)
{
    return SIGMA_FILL_CTOR(ASM_Instr, DIRECTIVE_8BYTE, byte8, num);
}

inline ASM_Instr opcode_make_directive__ascii(directivy_ascii_t str)
{
    return SIGMA_FILL_CTOR(ASM_Instr, DIRECTIVE_ASCII, ascii, str);
}

inline ASM_Instr opcode_make_directive__asciz(directivy_ascii_t str)
{
    return SIGMA_FILL_CTOR(ASM_Instr, DIRECTIVE_ASCIZ, asciz, str);
}

inline ASM_Instr opcode_make_directive__long(directivy_num_t num)
{
    return SIGMA_FILL_CTOR(ASM_Instr, DIRECTIVE_LONG, long, num);
}

inline ASM_Instr opcode_make_directive__quad(ASM_Value num)
{
    return SIGMA_FILL_CTOR(ASM_Instr, DIRECTIVE_QUAD, quad, num);
}

inline ASM_Instr opcode_make_directive__zero(ASM_Value num)
{
    return SIGMA_FILL_CTOR(ASM_Instr, DIRECTIVE_ZERO, zero, num);
}

inline bool cmp_asm_block(const Opcode_block o1, const Opcode_block o2)
{
    return hash_opcode_block(o1) == hash_opcode_block(o2);
}

inline bool cmp_asm_instr(const ASM_Instr i1, const ASM_Instr i2)
{
    return cmp_asm_block(pblock_singleton(i1), pblock_singleton(i2));
}

Opcode_block opcode_make_block(ASM_Instr *opcode_array, size_t used, size_t size)
{
    return (Opcode_block){.array = opcode_array, .used = used, .size = size};
}

void retit(Opcode_block *block)
{
    push_Opcode_block(block, OPCODE_INSTR_RET());
}

Labels concat_labels(Labels lbls1, Labels lbls2)
{
    Labels result;
    init_Labels(&result, lbls1.used + lbls2.used);
    for (Iterator i = 0; i < lbls1.used; i++)
        push_labels(&result, lbls1.array[i], NULL);
    for (Iterator i = 0; i < lbls2.used; i++)
        push_labels(&result, lbls2.array[i], NULL);
    return result;
}

Label opcode_make_label(String name, Opcode_block body)
{
    return (Label){.identifier = name, .body = body, .description = NULL};
}

Label opcode_make_label_desc(String name, Opcode_block body, String description)
{
    return (Label){.identifier = name, .body = body, .description = description};
}

Assembled_Program opcode_make_assembled_program(Labels program, String entry_label, String format)
{
    return (Assembled_Program){program, entry_label, format};
}

void pblock_assemble(Opcode_block *block, const int n_instrs, ...)
{
    va_list instrs_ptr;
    va_start(instrs_ptr, n_instrs);
    for (int i = 0; i < n_instrs; i++)
        push_Opcode_block(block, va_arg(instrs_ptr, ASM_Instr));
    va_end(instrs_ptr);
}

Opcode_block pblock_create_assemble(const int n_instrs, ...)
{
    Opcode_block result;
    init_Opcode_block(&result, n_instrs);
    va_list instrs_ptr;
    va_start(instrs_ptr, n_instrs);
    for (int i = 0; i < n_instrs; i++)
        push_Opcode_block(&result, va_arg(instrs_ptr, ASM_Instr));
    va_end(instrs_ptr);
    return result;
}

Opcode_block pblock_singleton(const ASM_Instr sing_instr)
{
    Opcode_block result;
    init_Opcode_block(&result, 1);
    push_Opcode_block(&result, sing_instr);
    return result;
}

void push_labels(Labels *labels, Label item, label_count_t *lbl_count)
{
    push_Labels(labels, item);
    inc_lc(lbl_count);
}

void push_labels_from_oblock(Labels *labels, String name, const Opcode_block block, label_count_t *lbl_count)
{
    push_labels(labels, opcode_make_label(name, block), lbl_count);
}

void push_labels_from_oblock_desc(Labels *labels, String name, Opcode_block block, String desc, label_count_t *lbl_count)
{
    push_labels(labels, opcode_make_label_desc(name, block, desc), lbl_count);
}

String label_name__cnt_lc(const label_count_t lbl_count)
{
    return rcl_sprintf_s(".lc%d", abs(lbl_count));
}

String label_custom_name__cnt_lc(const String custom_name, const label_count_t lbl_count)
{
    return rcl_sprintf_s(".%s_%d", custom_name, abs(lbl_count));
}

void inc_lc(label_count_t *lbl_count_ptr)
{
    if (lbl_count_ptr != NULL)
        *lbl_count_ptr += 1;
}

label_count_t pred_lc(const label_count_t lbl_count)
{
    return lbl_count == 0 ? 0 : lbl_count - 1;
}

label_count_t succ_lc(const label_count_t lbl_count)
{
    return lbl_count + 1;
}

Register new_reg(Lenv_map_t *lenv_ptr, enum Register_Size_kind *size_kind_opt)
{
    if (empty_Lenv(lenv_ptr))
    {
        if (size_kind_opt == NULL)
        {
            // If no specified size, return a 32 bits general puprose register
            return __EAX;
        }
        else
        {
            return opcode_make_register(*size_kind_opt, (reg_enum_t)(0));
        }
    }
    else
    {
        return next_reg(get_last_key_Lenv(lenv_ptr).reg);
    }
}
