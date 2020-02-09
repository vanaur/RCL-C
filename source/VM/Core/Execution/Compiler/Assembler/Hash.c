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

#include <stdint.h>
#include <Tools\Utils\Utils.h>
#include <Tools\Hash\djb2.h>
#include <VM\Core\Execution\Compiler\Assembler\Opcode.h>
#include <VM\Core\Execution\Compiler\Assembler\Hash.h>

#define COMMENT_NEUTRAL_HASH 0

static hash_t hash_asm_reg(const Register reg)
{
    switch (reg.kind)
    {
    case _8:
        return (hash_t)SIGMA_GETV_BYVAL(reg, rs_8, r);

    case _16:
        return (hash_t)SIGMA_GETV_BYVAL(reg, rs_16, r);

    case _32:
        return (hash_t)SIGMA_GETV_BYVAL(reg, rs_32, r);

    case _64:
        return (hash_t)SIGMA_GETV_BYVAL(reg, rs_64, r);

    case _128:
        return (hash_t)SIGMA_GETV_BYVAL(reg, rs_128, r);

    case _256:
        return (hash_t)SIGMA_GETV_BYVAL(reg, rs_256, r);

    case _512:
        return (hash_t)SIGMA_GETV_BYVAL(reg, rs_512, r);

    default:
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Unhashable register (kind = %d).", reg.kind);
    }
}

hash_t hash_asm_value(const ASM_Value val)
{
    switch (val.kind)
    {
    case REGISTER:
        return hash_asm_reg(SIGMA_GETV_BYVAL(val, register, reg)) << 5;

    case NUMBER:
        return (hash_t)SIGMA_GETV_BYVAL(val, number, num) << 5;

    case ADRESS_RAW:
        return (hash_t)((intptr_t)SIGMA_GETV_BYVAL(val, adress_raw, adrr) << 5);

    case ADRESS_REG:
        return hash_asm_value(*SIGMA_GETV_BYVAL(val, adress_reg, regi)) << 5;

    case LABEL:
        return SIGMA_GETV_BYVAL(val, label, hcode);

    case ADD:
        return hash_asm_value(*SIGMA_GETV_BYVAL(val, asmv_add, v1)) + hash_asm_value(*SIGMA_GETV_BYVAL(val, asmv_add, v2));

    case SUB:
        return hash_asm_value(*SIGMA_GETV_BYVAL(val, asmv_sub, v1)) + hash_asm_value(*SIGMA_GETV_BYVAL(val, asmv_mul, v2));

    case MUL:
        return hash_asm_value(*SIGMA_GETV_BYVAL(val, asmv_mul, v1)) + hash_asm_value(*SIGMA_GETV_BYVAL(val, asmv_sub, v2));

    default:
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Unhashable value (kind = %d).", val.kind);
    }
}

static hash_t hash_asm_instr(const ASM_Instr instr)
{
    switch (instr.kind)
    {
    case OPCODE_MOV:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, mov, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, mov, dest_src).src);

    case OPCODE_PUSH:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, push, src));

    case OPCODE_LEA:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, lea, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, lea, dest_src).src);

    case OPCODE_IMUL:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, imul, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, imul, dest_src).src);

    case OPCODE_POP:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, pop, src));

    case OPCODE_ADD:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, add, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, add, dest_src).src);

    case OPCODE_SUB:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, sub, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, sub, dest_src).src);

    case OPCODE_INC:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, inc, dest));

    case OPCODE_DEC:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, dec, dest));

    case OPCODE_MUL:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, mul, src));

    case OPCODE_DIV:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, div, src));

    case OPCODE_CALL:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, call, adrr));

    case OPCODE_XOR:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, xor, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, xor, dest_src).src);

    case OPCODE_FABS:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, fabs, src));

    case OPCODE_FCHS:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, fchs, src));

    case OPCDOE_SHL:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, shl, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, shl, dest_src).src);

    case OPCODE_SHR:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, shr, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, shr, dest_src).src);

    case OPCODE_TEST:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, test, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, test, dest_src).src);

    case OPCODE_CMP:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, cmp, dest_src).dest) | hash_asm_value(SIGMA_GETV_BYVAL(instr, cmp, dest_src).src);

    case OPCODE_JE:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, je, mem));

    case OPCODE_JNE:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, jne, mem));

    case OPCODE_JG:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, jg, mem));

    case OPCODE_JGE:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, jge, mem));

    case OPCODE_JL:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, jl, mem));

    case OPCODE_JLE:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, jle, mem));

    case OPCODE_JMP:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, jmp, mem));

    case OPCODE_SETE:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, sete, mem));

    case OPCODE_SETNE:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, setne, mem));

    case OPCODE_SETG:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, setg, mem));

    case OPCODE_SETGE:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, setge, mem));

    case OPCODE_SETL:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, setl, mem));

    case OPCODE_SETLE:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, setle, mem));

    case DIRECTIVE_BYTE:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, push, src));

    case DIRECTIVE_2BYTE:
        return (hash_t)SIGMA_GETV_BYVAL(instr, byte2, n) << 5;

    case DIRECTIVE_4BYTE:
        return (hash_t)SIGMA_GETV_BYVAL(instr, byte4, n) << 5;

    case DIRECTIVE_8BYTE:
        return (hash_t)SIGMA_GETV_BYVAL(instr, byte8, n) << 5;

    case DIRECTIVE_ASCII:
        return hash_djb2(SIGMA_GETV_BYVAL(instr, ascii, s));

    case DIRECTIVE_ASCIZ:
        return hash_djb2(SIGMA_GETV_BYVAL(instr, asciz, s));

    case DIRECTIVE_LONG:
        return (hash_t)SIGMA_GETV_BYVAL(instr, long, n) << 5;

    case DIRECTIVE_QUAD:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, quad, n));

    case DIRECTIVE_ZERO:
        return hash_asm_value(SIGMA_GETV_BYVAL(instr, zero, n));

    case OPCODE_RET:
    case OPCODE_CDQ:
    case OPCODE_FNOP:
    case OPCODE_SYSCALL:

        return (hash_t)instr.kind << 3;

    case EMPTY:
    case OPCODE_COMMENT:
        return COMMENT_NEUTRAL_HASH;

    default:
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Unhashable instruction (kind = %d).", instr.kind);
    }
}

hash_t hash_opcode_block(const Opcode_block block)
{
    hash_t result = 5381;

    for (Iterator i = 0; i < block.used; i++)
        result = ((result << 5) + result) + hash_asm_instr(block.array[i]);

    return abs(result);
}
