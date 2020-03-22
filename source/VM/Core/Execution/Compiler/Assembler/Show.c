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
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>

#include <VM\Core\Browse\BResult.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\Execution\Compiler\Assembler\Opcode.h>

String show_register(Register reg)
{
    switch (reg.kind)
    {
    case _8:
        switch (reg.u.rs_8_.r)
        {
        case AL:
            return "al";
        case CL:
            return "cl";
        case DL:
            return "dl";
        case BL:
            return "bl";
        case AH:
            return "ah";
        case CH:
            return "ch";
        case DH:
            return "dh";
        case BH:
            return "bh";
        }
        break;
    case _16:
        switch (reg.u.rs_16_.r)
        {
        case AX:
            return "ax";
        case CX:
            return "cx";
        case DX:
            return "dx";
        case BX:
            return "bx";
        case SP:
            return "sp";
        case BP:
            return "bp";
        case SI:
            return "si";
        case DI:
            return "di";
        }
        break;
    case _32:
        switch (reg.u.rs_32_.r)
        {
        case EAX:
            return "eax";
        case ECX:
            return "ecx";
        case EDX:
            return "edx";
        case EBX:
            return "ebx";
        case ESP:
            return "esp";
        case EBP:
            return "ebp";
        case ESI:
            return "esi";
        case EDI:
            return "edi";
        }
        break;
    case _64:
        switch (reg.u.rs_64_.r)
        {
        case RAX:
            return "rax";
        case RBX:
            return "rbx";
        case RCX:
            return "rcx";
        case RDX:
            return "rdx";
        case RDI:
            return "rdi";
        case RSI:
            return "rsi";
        case RBP:
            return "rbp";
        case RSP:
            return "rsp";
        case R8:
            return "r8";
        case R9:
            return "r9";
        case R10:
            return "r10";
        case R11:
            return "r11";
        case R12:
            return "r12";
        case R13:
            return "r13";
        case R14:
            return "r14";
        case R15:
            return "r15";
        case RIP:
            return "rip";
        case CS:
            return "cs";
        case FS:
            return "fs";
        case GS:
            return "gs";
        case MM0:
            return "mm0";
        case MM1:
            return "mm1";
        case MM2:
            return "mm2";
        case MM3:
            return "mm3";
        case MM4:
            return "mm4";
        case MM5:
            return "mm5";
        case MM6:
            return "mm6";
        case MM7:
            return "mm7";
        }
        break;
    case _128:
        switch (reg.u.rs_128_.r)
        {
        case XMM0:
            return "xmm0";
        case XMM1:
            return "xmm1";
        case XMM2:
            return "xmm2";
        case XMM3:
            return "xmm3";
        case XMM4:
            return "xmm4";
        case XMM5:
            return "xmm5";
        case XMM6:
            return "xmm6";
        case XMM7:
            return "xmm7";
        }
        break;
    case _256:
        switch (reg.u.rs_256_.r)
        {
        case YMM0:
            return "ymm0";
        case YMM1:
            return "ymm1";
        case YMM2:
            return "ymm2";
        case YMM3:
            return "ymm3";
        case YMM4:
            return "ymm4";
        case YMM5:
            return "ymm5";
        case YMM6:
            return "ymm6";
        case YMM7:
            return "ymm7";
        }
        break;
    case _512:
        switch (reg.u.rs_512_.r)
        {
        case ZMM0:
            return "zmm0";
        case ZMM1:
            return "zmm1";
        case ZMM2:
            return "zmm2";
        case ZMM3:
            return "zmm3";
        case ZMM4:
            return "zmm4";
        case ZMM5:
            return "zmm5";
        case ZMM6:
            return "zmm6";
        case ZMM7:
            return "zmm7";
        }
        break;
    }
}

static String show_asm_value(ASM_Value asm_value)
{
    switch (asm_value.kind)
    {
    case REGISTER:
        return show_register(asm_value.u.register_.reg);

    case NUMBER:
    {
        String result;
        //if (asm_value.u.number_.num == floor(asm_value.u.number_.num))
        rcl_asprintf(&result, "%#x", (int)asm_value.u.number_.num);
        /*         else
            rcl_asprintf(&result, "0x%a", asm_value.u.number_.num); */
        return result;
    }
    case ADRESS_REG:
    {
        String result;
/*         if (asm_value.u.adress_reg_.regi.location == 0)
            rcl_asprintf(&result, "[%s]", show_register(asm_value.u.adress_reg_.regi.reg));
        else
            rcl_asprintf(&result, "[%s %s %d]",
                         show_register(asm_value.u.adress_reg_.regi.reg),
                         asm_value.u.adress_reg_.regi.location >= 0 ? "+" : "-",
                         abs(asm_value.u.adress_reg_.regi.location)); */
        rcl_asprintf(&result, "[%s]", show_asm_value(*asm_value.u.adress_reg_.regi));
        return result;
    }
    case ADRESS_RAW:
    {
        String result;
        rcl_asprintf(&result, "0x%" PRIXPTR, (uintptr_t)asm_value.u.adress_raw_.adrr);
        return result;
    }
    case ADD:
    {
        String result;
        rcl_asprintf(&result, "%s + %s", show_asm_value(*asm_value.u.asmv_add_.v1), show_asm_value(*asm_value.u.asmv_add_.v2));
        return result;
    }
    case SUB:
    {
        String result;
        rcl_asprintf(&result, "%s - %s", show_asm_value(*asm_value.u.asmv_sub_.v1), show_asm_value(*asm_value.u.asmv_sub_.v2));
        return result;
    }
    case MUL:
    {
        String result;
        rcl_asprintf(&result, "%s * %s", show_asm_value(*asm_value.u.asmv_mul_.v1), show_asm_value(*asm_value.u.asmv_mul_.v2));
        return result;
    }
    case LABEL:
        return asm_value.u.label_.id;
    }
    return "<?>";
}

static String show_comment(String raw_s, bool indent)
{
    if (raw_s == NULL)
        return "";

    String token = strtok(strdup(raw_s), "\r\n");
    String result = "";

    rcl_asprintf(&result, "; %s", token);

    token = strtok(NULL, "\r\n");

    while (token != NULL)
    {
        if (!indent)
        {
            rcl_asprintf(&result, "%s\n; %s", result, token);
        }
        else
        {
            rcl_asprintf(&result, "%s\n   ; %s", result, token);
        }
        token = strtok(NULL, "\r\n");
    }

    return result;
}

static String show_opcode(ASM_Instr opcode)
{
    String result;
    switch (opcode.kind)
    {
    case OPCODE_MOV:
        rcl_asprintf(&result, "mov %s, %s", show_asm_value(opcode.u.mov_.dest_src.dest), show_asm_value(opcode.u.mov_.dest_src.src));
        break;

    case OPCODE_LEA:
        rcl_asprintf(&result, "lea %s, %s", show_asm_value(opcode.u.lea_.dest_src.dest), show_asm_value(opcode.u.lea_.dest_src.src));
        break;

    case OPCODE_IMUL:
        rcl_asprintf(&result, "imul %s, %s", show_asm_value(opcode.u.imul_.dest_src.dest), show_asm_value(opcode.u.imul_.dest_src.src));
        break;

    case OPCODE_XOR:
        rcl_asprintf(&result, "xor %s, %s", show_asm_value(opcode.u.xor_.dest_src.dest), show_asm_value(opcode.u.xor_.dest_src.src));
        break;

    case OPCODE_TEST:
        rcl_asprintf(&result, "test %s, %s", show_asm_value(opcode.u.test_.dest_src.dest), show_asm_value(opcode.u.test_.dest_src.src));
        break;

    case OPCODE_ADD:
        rcl_asprintf(&result, "add %s, %s", show_asm_value(opcode.u.add_.dest_src.dest), show_asm_value(opcode.u.add_.dest_src.src));
        break;

    case OPCODE_SUB:
        rcl_asprintf(&result, "sub %s, %s", show_asm_value(opcode.u.sub_.dest_src.dest), show_asm_value(opcode.u.sub_.dest_src.src));
        break;

    case OPCODE_SHR:
        rcl_asprintf(&result, "shr %s, %s", show_asm_value(opcode.u.shr_.dest_src.dest), show_asm_value(opcode.u.shr_.dest_src.src));
        break;

    case OPCDOE_SHL:
        rcl_asprintf(&result, "shl %s, %s", show_asm_value(opcode.u.shl_.dest_src.dest), show_asm_value(opcode.u.shl_.dest_src.src));
        break;

    case OPCODE_INC:
        rcl_asprintf(&result, "inc %s", show_asm_value(opcode.u.inc_.dest));
        break;

    case OPCODE_DEC:
        rcl_asprintf(&result, "dec %s", show_asm_value(opcode.u.dec_.dest));
        break;

    case OPCODE_MUL:
        rcl_asprintf(&result, "mul %s", show_asm_value(opcode.u.mul_.src));
        break;

    case OPCODE_FABS:
        rcl_asprintf(&result, "fabs %s", show_asm_value(opcode.u.fabs_.src));
        break;

    case OPCODE_FCHS:
        rcl_asprintf(&result, "fchs %s", show_asm_value(opcode.u.fchs_.src));
        break;

    case OPCODE_DIV:
        rcl_asprintf(&result, "div %s", show_asm_value(opcode.u.div_.src));
        break;

    case OPCODE_PUSH:
        rcl_asprintf(&result, "push %s", show_asm_value(opcode.u.push_.src));
        break;

    case OPCODE_POP:
        rcl_asprintf(&result, "pop %s", show_asm_value(opcode.u.pop_.src));
        break;

    case OPCODE_CALL:
        rcl_asprintf(&result, "call %s", show_asm_value(opcode.u.call_.adrr));
        break;

    case OPCODE_CMP:
        rcl_asprintf(&result, "cmp %s, %s", show_asm_value(opcode.u.cmp_.dest_src.dest), show_asm_value(opcode.u.cmp_.dest_src.src));
        break;

    case OPCODE_JE:
        rcl_asprintf(&result, "je %s", show_asm_value(opcode.u.je_.mem));
        break;

    case OPCODE_JNE:
        rcl_asprintf(&result, "jne %s", show_asm_value(opcode.u.jne_.mem));
        break;

    case OPCODE_JG:
        rcl_asprintf(&result, "jg %s", show_asm_value(opcode.u.jg_.mem));
        break;

    case OPCODE_JGE:
        rcl_asprintf(&result, "jge %s", show_asm_value(opcode.u.jge_.mem));
        break;

    case OPCODE_JL:
        rcl_asprintf(&result, "jl %s", show_asm_value(opcode.u.jl_.mem));
        break;

    case OPCODE_JLE:
        rcl_asprintf(&result, "jle %s", show_asm_value(opcode.u.jle_.mem));
        break;

    case OPCODE_JMP:
        rcl_asprintf(&result, "jmp %s", show_asm_value(opcode.u.jmp_.mem));
        break;

    case OPCODE_SETE:
        rcl_asprintf(&result, "sete %s", show_asm_value(opcode.u.sete_.mem));
        break;

    case OPCODE_SETNE:
        rcl_asprintf(&result, "setne %s", show_asm_value(opcode.u.setne_.mem));
        break;

    case OPCODE_SETG:
        rcl_asprintf(&result, "setg %s", show_asm_value(opcode.u.setg_.mem));
        break;

    case OPCODE_SETGE:
        rcl_asprintf(&result, "setge %s", show_asm_value(opcode.u.setge_.mem));
        break;

    case OPCODE_SETL:
        rcl_asprintf(&result, "setl %s", show_asm_value(opcode.u.setl_.mem));
        break;

    case OPCODE_SETLE:
        rcl_asprintf(&result, "setle %s", show_asm_value(opcode.u.setle_.mem));
        break;

    case OPCODE_COMMENT:
        rcl_asprintf(&result, "%s", show_comment(opcode.u.comment_.text, true));
        break;

    case DIRECTIVE_BYTE:
        rcl_asprintf(&result, ".byte %#x", (int)opcode.u.byte_.n);
        break;

    case DIRECTIVE_2BYTE:
        rcl_asprintf(&result, ".2byte %#x", (int)opcode.u.byte2_.n);
        break;

    case DIRECTIVE_4BYTE:
        rcl_asprintf(&result, ".4byte %#x", (int)opcode.u.byte4_.n);
        break;

    case DIRECTIVE_8BYTE:
        rcl_asprintf(&result, ".8byte %#x", (int)opcode.u.byte8_.n);
        break;

    case DIRECTIVE_ASCII:
        rcl_asprintf(&result, ".ascii \"%s\"", opcode.u.ascii_.s);
        break;

    case DIRECTIVE_ASCIZ:
        rcl_asprintf(&result, ".asciz \"%s\"", opcode.u.asciz_.s);
        break;

    case DIRECTIVE_LONG:
        rcl_asprintf(&result, ".long %#x", (int)opcode.u.long_.n);
        break;

    case DIRECTIVE_QUAD:
        rcl_asprintf(&result, ".quad %s", show_asm_value(opcode.u.quad_.n));
        break;

    case DIRECTIVE_ZERO:
        rcl_asprintf(&result, ".zero %s", show_asm_value(opcode.u.quad_.n));
        break;

    case OPCODE_RET:
        return "ret";

    case OPCODE_CDQ:
        return "cdq";

    case OPCODE_SYSCALL:
        return "syscall";

    case EMPTY:
        return "";

    case OPCODE_FNOP:
        return "fnop";
    }
    return result;
}

String show_assembled_program(Assembled_Program p, const BResult bresult)
{
    //String result = "; Program compiled by .: RCL :.\n";
    String result = rcl_sprintf_s(
        "; Program compiled by the RCL, version %s.\
\n; Compilation mode: %d.\
\n; Optimizations: %d.\
\n; Target platform: %s.\
\n; Use `--dasm` flag to generate an assembly code commenting.\
\n\n",
        show_version(CURRENT_VERSION),
        bresult.exec_infos.ioc,
        bresult.exec_infos.optimize_level,
        "Windows PE x32-64-86");

    for (Iterator i = 0; i < p.labels.used; i++)
    {
        if (p.labels.array[i].description != NULL)
        {
            rcl_asprintf(&result, "%s\n%s", result, show_comment(p.labels.array[i].description, false));
        }
        rcl_asprintf(&result, "%s\n%s:\n", result, p.labels.array[i].identifier);
        for (Iterator j = 0; j < p.labels.array[i].body.used; j++)
        {
            if (p.labels.array[i].body.array[j].kind != EMPTY)
                rcl_asprintf(&result, "%s   %s\n", result, show_opcode(p.labels.array[i].body.array[j]));
        }
    }
    return result;
}
