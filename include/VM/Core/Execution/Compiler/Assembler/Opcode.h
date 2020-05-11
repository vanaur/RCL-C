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
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include <Tools\Utils\Utils.h>
#include <Tools\Vector\Vector.h>
#include <Tools\Map\Map.h>
#include <VM\Core\RawCode\RawCode.h>

typedef String Name;

enum Register_8
{
    AL,
    CL,
    DL,
    BL,
    AH,
    CH,
    DH,
    BH
};
enum Register_16
{
    AX,
    CX,
    DX,
    BX,
    SP,
    BP,
    SI,
    DI
};
enum Register_32
{
    EAX,
    ECX,
    EDX,
    EBX,
    ESP,
    EBP,
    ESI,
    EDI
};
enum Register_64
{
    RAX,
    RBX,
    RCX,
    RDX,
    RDI,
    RSI,
    RBP,
    RSP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    RIP,
    CS,
    FS,
    GS,

    MM0,
    MM1,
    MM2,
    MM3,
    MM4,
    MM5,
    MM6,
    MM7
};
enum Register_128
{
    XMM0,
    XMM1,
    XMM2,
    XMM3,
    XMM4,
    XMM5,
    XMM6,
    XMM7
};
enum Register_256
{
    YMM0,
    YMM1,
    YMM2,
    YMM3,
    YMM4,
    YMM5,
    YMM6,
    YMM7
};
enum Register_512
{
    ZMM0,
    ZMM1,
    ZMM2,
    ZMM3,
    ZMM4,
    ZMM5,
    ZMM6,
    ZMM7
};

typedef unsigned short reg_enum_t;

enum Register_Size_kind
{
    _8,
    _16,
    _32,
    _64,
    // _86,
    _128,
    _256,
    _512
};

typedef SIGMA_TYPE(
    Register, enum Register_Size_kind,
    SIGMA_CTOR(rs_8, enum Register_8 r);
    SIGMA_CTOR(rs_16, enum Register_16 r);
    SIGMA_CTOR(rs_32, enum Register_32 r);
    SIGMA_CTOR(rs_64, enum Register_64 r);
    SIGMA_CTOR(rs_128, enum Register_128 r);
    SIGMA_CTOR(rs_256, enum Register_256 r);
    SIGMA_CTOR(rs_512, enum Register_512 r));

bool cmp_reg(const Register, const Register);
Register next_reg(const Register);

enum ASM_Value_kind
{
    REGISTER,
    NUMBER,
    ADRESS_REG,
    ADRESS_RAW,
    LABEL,
    ADD,
    SUB,
    MUL
};

typedef struct
{
    Register reg;
    int location;
} Regi;

Regi make_regi(Register, int);
// Cmp 2 Regis
bool cmp_regi(Regi, Regi);
// Cmp 2 register from their Regi
bool cmp_regi_only_reg(Regi, Regi);

typedef SIGMA_TYPE(
    ASM_Value, enum ASM_Value_kind,
    SIGMA_CTOR(register, Register reg);
    SIGMA_CTOR(number, double num);
    //SIGMA_CTOR(adress_reg, Regi regi);
    SIGMA_CTOR(adress_reg, struct ASM_Value *regi);
    SIGMA_CTOR(adress_raw, void *adrr);
    SIGMA_CTOR(label, hash_t hcode; Name id);
    SIGMA_CTOR(asmv_add, struct ASM_Value *v1, *v2);
    SIGMA_CTOR(asmv_sub, struct ASM_Value *v1, *v2);
    SIGMA_CTOR(asmv_mul, struct ASM_Value *v1, *v2));
 
enum ASM_data_kind_t
{
    BYTE,
    WORD,
    DWORD,
    FWORD,
    QWORD,
    TBYTE,
    XWORD,
    YWORD,
    ZWORD,
    UNDEF
};

//  https://www.cs.usfca.edu/~galles/compilerdesign/x86.pdf
//  https://www.felixcloutier.com/x86/
//  http://matt.might.net/articles/cps-conversion/
//  https://en.wikipedia.org/wiki/A-normal_form

enum Opcode_kind
{
    /**   Arithmetic and memory manipulation   **/

    OPCODE_MOV,
    OPCODE_PUSH,
    OPCODE_LEA,
    OPCODE_POP,
    OPCODE_ADD,
    OPCODE_SUB,
    OPCODE_INC,
    OPCODE_DEC,
    OPCODE_IMUL,
    OPCODE_MUL,
    OPCODE_DIV,
    OPCODE_CALL,
    OPCODE_XOR,
    OPCODE_CDQ,
    OPCODE_FABS,
    OPCODE_FCHS,
    OPCDOE_SHL,
    OPCODE_SHR,

    /**   Flag register manipulation   **/

    OPCODE_TEST,  //
    OPCODE_CMP,   //  Sets flags register, to be used before a jump or set instruction
    OPCODE_JE,    //  Jump-Equal
    OPCODE_JNE,   //  Jump-Not-Equal
    OPCODE_JG,    //  Jump-Greater
    OPCODE_JGE,   //  Jump-Greater-Equal
    OPCODE_JL,    //  Jump-Less
    OPCODE_JLE,   //  Jump-Less-Equal
    OPCODE_JMP,   //  Jump
    OPCODE_SETE,  //  Set-Equal
    OPCODE_SETNE, //  Set-Not-Equal
    OPCODE_SETG,  //  Set-Greater
    OPCODE_SETGE, //  Set-Greater-Equal
    OPCODE_SETL,  //  Set-Less
    OPCODE_SETLE, //  Set-Less-Equal

    OPCODE_RET,
    OPCODE_FNOP,
    OPCODE_SYSCALL,
    OPCODE_COMMENT,

    /**    Assembly directives   **/

    // Note: The opcode directives assembly should not be used for JIT execution
    //       (except in special cases) but for the AOT compiler.
    //       For example, all constant string will not be recompiled in the JIT,
    //       but their address will be used instead from the JIT interpreter.

    DIRECTIVE_BYTE,
    DIRECTIVE_2BYTE,
    DIRECTIVE_4BYTE,
    DIRECTIVE_8BYTE,
    DIRECTIVE_ASCII,
    DIRECTIVE_ASCIZ,
    DIRECTIVE_LONG,
    DIRECTIVE_QUAD,
    DIRECTIVE_ZERO,

    EMPTY

};

typedef ASM_Value dest_t;
typedef ASM_Value src_t;
typedef ASM_Value adrr_t;
typedef ASM_Value mem_t;
typedef double directivy_num_t;   // .xbyte, .quad, .long
typedef String directivy_ascii_t; // .ascii, .asciz

typedef struct
{
    enum ASM_data_kind_t kind; // size = bytes
    bool is_ptr;               // ptr
} format_t;

typedef struct
{
    format_t format; // Specify the format, but can be NULL
    dest_t dest;     // Specify the destination
    src_t src;       // Specify the source
} dest_src_t;

#define FILL_DS__nf(d, s) ((dest_src_t){{UNDEF, false}, dest, src})
#define FILL_DS__fptr(f, d, s) ((dest_src_t){{f, true}, d, s})
#define FILL_DS__f(f, d, s) ((dest_src_t){{f, false}, d, s})

typedef SIGMA_TYPE(
    ASM_Instr, enum Opcode_kind,
    SIGMA_CTOR(add, dest_src_t dest_src);
    SIGMA_CTOR(sub, dest_src_t dest_src);
    SIGMA_CTOR(mov, dest_src_t dest_src);
    SIGMA_CTOR(test, dest_src_t dest_src);
    SIGMA_CTOR(lea, dest_src_t dest_src);
    SIGMA_CTOR(xor, dest_src_t dest_src);
    SIGMA_CTOR(cmp, dest_src_t dest_src);
    SIGMA_CTOR(shl, dest_src_t dest_src);
    SIGMA_CTOR(shr, dest_src_t dest_src);
    SIGMA_CTOR(imul, dest_src_t dest_src);
    SIGMA_CTOR(mul, src_t src);
    SIGMA_CTOR(div, src_t src);
    SIGMA_CTOR(push, src_t src);
    SIGMA_CTOR(pop, src_t src);
    SIGMA_CTOR(fabs, src_t src);
    SIGMA_CTOR(fchs, src_t src);
    SIGMA_CTOR(inc, dest_t dest);
    SIGMA_CTOR(dec, dest_t dest);
    SIGMA_CTOR(jmp, mem_t mem);
    SIGMA_CTOR(je, mem_t mem);
    SIGMA_CTOR(jne, mem_t mem);
    SIGMA_CTOR(jg, mem_t mem);
    SIGMA_CTOR(jge, mem_t mem);
    SIGMA_CTOR(jl, mem_t mem);
    SIGMA_CTOR(jle, mem_t mem);
    SIGMA_CTOR(sete, mem_t mem);
    SIGMA_CTOR(setne, mem_t mem);
    SIGMA_CTOR(setg, mem_t mem);
    SIGMA_CTOR(setge, mem_t mem);
    SIGMA_CTOR(setl, mem_t mem);
    SIGMA_CTOR(setle, mem_t mem);
    SIGMA_CTOR(comment, String text);
    SIGMA_CTOR(call, adrr_t adrr);
    SIGMA_CTOR(byte, directivy_num_t n);
    SIGMA_CTOR(byte2, directivy_num_t n);
    SIGMA_CTOR(byte4, directivy_num_t n);
    SIGMA_CTOR(byte8, directivy_num_t n);
    SIGMA_CTOR(ascii, directivy_ascii_t s);
    SIGMA_CTOR(asciz, directivy_ascii_t s);
    SIGMA_CTOR(long, directivy_num_t n);
    SIGMA_CTOR(quad, ASM_Value n);
    SIGMA_CTOR(zero, ASM_Value n);
    SIGMA_CTOR(cdq);
    SIGMA_CTOR(fnop);
    SIGMA_CTOR(syscall);
    SIGMA_CTOR(ret);
    SIGMA_CTOR(empty));

Vector(Opcode_block, ASM_Instr);

#define Anonymous_label "@@"
#define Anonymous_label_top "@b"
#define Anonymous_label_bot "@f"

typedef struct
{
    String identifier;
    hash_t hash_code;
    Opcode_block body;
    String description;
} Label;

Vector(Labels, Label);

Map(HashOpBloLbl_const, hash_t, String);

typedef struct
{
    // Preprocessor
    Labels labels;
    String Entry;
    String Format;
} Assembled_Program;

/** MACROS  --  general opcodes **/

#define OPCODE_REG(size_kind, reg) opcode_make_register(size_kind, reg)

#define OPCODE_VALUE_REG(reg) opcode_make_asm_value__reg(reg)
#define OPCODE_VALUE_NUM(num) opcode_make_asm_value__num(num)
#define OPCODE_VALUE_LABEL(id) opcode_make_asm_value__label(id)
#define OPCODE_VALUE_ADRESS_REG(v) opcode_make_asm_value__adress_reg(v)
#define OPCODE_VALUE_ADRESS_RAW(adrr) opcode_make_asm_value__adress_raw(adrr)
#define OPCODE_VALUE_ADD(v1, v2) opcode_make_asm_value__add(v1, v2)
#define OPCODE_VALUE_SUB(v1, v2) opcode_make_asm_value__sub(v1, v2)
#define OPCODE_VALUE_MUL(v1, v2) opcode_make_asm_value__mul(v1, v2)

#define OPCODE_INSTR_MOV(dest, src) opcode_make_opcode__mov(dest, src)
#define OPCODE_INSTR_LEA(dest, src) opcode_make_opcode__lea(dest, src)
#define OPCODE_INSTR_IMUM(dest, src) opcode_make_opcode__imul(dest, src)
#define OPCODE_INSTR_TEST(dest, src) opcode_make_opcode__test(dest, src)
#define OPCODE_INSTR_XOR(dest, src) opcode_make_opcode__xor(dest, src)
#define OPCODE_INSTR_ADD(dest, src) opcode_make_opcode__add(dest, src)
#define OPCODE_INSTR_SUB(dest, src) opcode_make_opcode__sub(dest, src)
#define OPCODE_INSTR_SHL(dest, src) opcode_make_opcode__shl(dest, src)
#define OPCODE_INSTR_SHR(dest, src) opcode_make_opcode__shr(dest, src)
#define OPCODE_INSTR_FABS(src) opcode_make_opcode__fabs(src)
#define OPCODE_INSTR_FCHS(src) opcode_make_opcode__fchs(src)
#define OPCODE_INSTR_INC(dest) opcode_make_opcode__inc(dest)
#define OPCODE_INSTR_DEC(dest) opcode_make_opcode__dec(dest)
#define OPCODE_INSTR_MUL(src) opcode_make_opcode__mul(src)
#define OPCODE_INSTR_DIV(src) opcode_make_opcode__div(src)
#define OPCODE_INSTR_CALL(adrr) opcode_make_opcode__call(adrr)
#define OPCODE_INSTR_RET() opcode_make_opcode__ret()
#define OPCODE_INSTR_EMPTY() opcode_make_opcode__empty()
#define OPCODE_INSTR_CDQ() opcode_make_opcode__cdq()
#define OPCODE_INSTR_FNOP() opcode_make_opcode__fnop()
#define OPCODE_INSTR_SYSCALL() opcode_make_opcode__syscall()
#define OPCODE_INSTR_PUSH(src) opcode_make_opcode__push(src)
#define OPCODE_INSTR_POP(dest) opcode_make_opcode__pop(dest)
#define OPCODE_INSTR_CMP(dest, src) opcode_make_opcode__cmp(dest, src)
#define OPCODE_INSTR_JE(mem) opcode_make_opcode__je(mem)
#define OPCODE_INSTR_JNE(mem) opcode_make_opcode__jne(mem)
#define OPCODE_INSTR_JG(mem) opcode_make_opcode__jg(mem)
#define OPCODE_INSTR_JGE(mem) opcode_make_opcode__jge(mem)
#define OPCODE_INSTR_JL(mem) opcode_make_opcode__jl(mem)
#define OPCODE_INSTR_JLE(mem) opcode_make_opcode__jle(mem)
#define OPCODE_INSTR_JMP(mem) opcode_make_opcode__jmp(mem)
#define OPCODE_INSTR_SETE(mem) opcode_make_opcode__sete(mem)
#define OPCODE_INSTR_SETNE(mem) opcode_make_opcode__setne(mem)
#define OPCODE_INSTR_SETG(mem) opcode_make_opcode__setg(mem)
#define OPCODE_INSTR_SETGE(mem) opcode_make_opcode__setge(mem)
#define OPCODE_INSTR_SETL(mem) opcode_make_opcode__setl(mem)
#define OPCODE_INSTR_SETLE(mem) opcode_make_opcode__setle(mem)
#define OPCODE_INSTR_COMMENT(cmt) opcode_make_opcode__comment(cmt)
#define OPCODE_NEWLINE() OPCODE_INSTR_COMMENT(NULL)
#define OPCODE_DIRECTIVE_BYTE(n) opcode_make_directive__byte(n)
#define OPCODE_DIRECTIVE_2BYTE(n) opcode_make_directive__2byte(n)
#define OPCODE_DIRECTIVE_4BYTE(n) opcode_make_directive__4byte(n)
#define OPCODE_DIRECTIVE_8BYTE(n) opcode_make_directive__8byte(n)
#define OPCODE_DIRECTIVE_ASCII(n) opcode_make_directive__ascii(n)
#define OPCODE_DIRECTIVE_ASCIZ(n) opcode_make_directive__asciz(n)
#define OPCODE_DIRECTIVE_LONG(n) opcode_make_directive__long(n)
#define OPCODE_DIRECTIVE_QUAD(n) opcode_make_directive__quad(n)
#define OPCODE_DIRECTIVE_ZERO(n) opcode_make_directive__zero(n)

/** MACROS  --  register as opcode register **/

#define __AX (OPCODE_REG(_16, AX))
#define __BX (OPCODE_REG(_16, BX))
#define __CX (OPCODE_REG(_16, CX))
#define __DX (OPCODE_REG(_16, DX))
#define __SP (OPCODE_REG(_16, SP))
#define __BP (OPCODE_REG(_16, BP))
#define __SI (OPCODE_REG(_16, SI))
#define __DI (OPCODE_REG(_16, DI))

#define __EAX (OPCODE_REG(_32, EAX))
#define __EBX (OPCODE_REG(_32, EBX))
#define __ECX (OPCODE_REG(_32, ECX))
#define __EDX (OPCODE_REG(_32, EDX))
#define __ESP (OPCODE_REG(_32, ESP))
#define __EBP (OPCODE_REG(_32, EBP))
#define __ESI (OPCODE_REG(_32, ESI))
#define __EDI (OPCODE_REG(_32, EDI))

#define __RAX (OPCODE_REG(_64, RAX))
#define __RBX (OPCODE_REG(_64, RBX))
#define __RCX (OPCODE_REG(_64, RCX))
#define __RDX (OPCODE_REG(_64, RDX))
#define __RDI (OPCODE_REG(_64, RDI))
#define __RSI (OPCODE_REG(_64, RSI))
#define __RBP (OPCODE_REG(_64, RBP))
#define __RSP (OPCODE_REG(_64, RSP))
#define __R8 (OPCODE_REG(_64, R8))
#define __R9 (OPCODE_REG(_64, R9))
#define __R10 (OPCODE_REG(_64, R10))
#define __R11 (OPCODE_REG(_64, R11))
#define __R12 (OPCODE_REG(_64, R12))
#define __R13 (OPCODE_REG(_64, R13))
#define __R14 (OPCODE_REG(_64, R14))
#define __R15 (OPCODE_REG(_64, R15))
#define __RIP (OPCODE_REG(_64, RIP))
#define __CS (OPCODE_REG(_64, CS))
#define __FS (OPCODE_REG(_64, FS))
#define __GS (OPCODE_REG(_64, GS))

#define __MM0 (OPCODE_REG(_64, MM0))
#define __MM1 (OPCODE_REG(_64, MM1))
#define __MM2 (OPCODE_REG(_64, MM2))
#define __MM3 (OPCODE_REG(_64, MM3))
#define __MM4 (OPCODE_REG(_64, MM4))
#define __MM5 (OPCODE_REG(_64, MM5))
#define __MM6 (OPCODE_REG(_64, MM6))
#define __MM7 (OPCODE_REG(_64, MM7))

#define __XMM0 (OPCODE_REG(_128, XMM0))
#define __XMM1 (OPCODE_REG(_128, XMM1))
#define __XMM2 (OPCODE_REG(_128, XMM2))
#define __XMM3 (OPCODE_REG(_128, XMM3))
#define __XMM4 (OPCODE_REG(_128, XMM4))
#define __XMM5 (OPCODE_REG(_128, XMM5))
#define __XMM6 (OPCODE_REG(_128, XMM6))
#define __XMM7 (OPCODE_REG(_128, XMM7))

#define __YMM0 (OPCODE_REG(_256, YMM0))
#define __YMM1 (OPCODE_REG(_256, YMM1))
#define __YMM2 (OPCODE_REG(_256, YMM2))
#define __YMM3 (OPCODE_REG(_256, YMM3))
#define __YMM4 (OPCODE_REG(_256, YMM4))
#define __YMM5 (OPCODE_REG(_256, YMM5))
#define __YMM6 (OPCODE_REG(_256, YMM6))
#define __YMM7 (OPCODE_REG(_256, YMM7))

#define __ZMM0 (OPCODE_REG(_512, ZMM0))
#define __ZMM1 (OPCODE_REG(_512, ZMM1))
#define __ZMM2 (OPCODE_REG(_512, ZMM2))
#define __ZMM3 (OPCODE_REG(_512, ZMM3))
#define __ZMM4 (OPCODE_REG(_512, ZMM4))
#define __ZMM5 (OPCODE_REG(_512, ZMM5))
#define __ZMM6 (OPCODE_REG(_512, ZMM6))
#define __ZMM7 (OPCODE_REG(_512, ZMM7))

/** MACROS  --  register as opcode value **/

#define _AX OPCODE_VALUE_REG(__AX)
#define _BX OPCODE_VALUE_REG(__BX)
#define _CX OPCODE_VALUE_REG(__CX)
#define _DX OPCODE_VALUE_REG(__DX)
#define _SP OPCODE_VALUE_REG(__SP)
#define _BP OPCODE_VALUE_REG(__BP)
#define _SI OPCODE_VALUE_REG(__SI)
#define _DI OPCODE_VALUE_REG(__DI)

#define _EAX OPCODE_VALUE_REG(__EAX)
#define _EBX OPCODE_VALUE_REG(__EBX)
#define _ECX OPCODE_VALUE_REG(__ECX)
#define _EDX OPCODE_VALUE_REG(__EDX)
#define _ESP OPCODE_VALUE_REG(__ESP)
#define _EBP OPCODE_VALUE_REG(__EBP)
#define _ESI OPCODE_VALUE_REG(__ESI)
#define _EDI OPCODE_VALUE_REG(__EDI)

#define _RAX OPCODE_VALUE_REG(__RAX)
#define _RBX OPCODE_VALUE_REG(__RBX)
#define _RCX OPCODE_VALUE_REG(__RCX)
#define _RDX OPCODE_VALUE_REG(__RDX)
#define _RDI OPCODE_VALUE_REG(__RDI)
#define _RSI OPCODE_VALUE_REG(__RSI)
#define _RBP OPCODE_VALUE_REG(__RBP)
#define _RSP OPCODE_VALUE_REG(__RSP)
#define _R8 OPCODE_VALUE_REG(__R8)
#define _R9 OPCODE_VALUE_REG(__R9)
#define _R10 OPCODE_VALUE_REG(__R10)
#define _R11 OPCODE_VALUE_REG(__R11)
#define _R12 OPCODE_VALUE_REG(__R12)
#define _R13 OPCODE_VALUE_REG(__R13)
#define _R14 OPCODE_VALUE_REG(__R14)
#define _R15 OPCODE_VALUE_REG(__R15)
#define _RIP OPCODE_VALUE_REG(__RIP)
#define _CS OPCODE_VALUE_REG(__CS)
#define _FS OPCODE_VALUE_REG(__FS)
#define _GS OPCODE_VALUE_REG(__GS)

#define _MM0 OPCODE_VALUE_REG(__MM0)
#define _MM1 OPCODE_VALUE_REG(__MM1)
#define _MM2 OPCODE_VALUE_REG(__MM2)
#define _MM3 OPCODE_VALUE_REG(__MM3)
#define _MM4 OPCODE_VALUE_REG(__MM4)
#define _MM5 OPCODE_VALUE_REG(__MM5)
#define _MM6 OPCODE_VALUE_REG(__MM6)
#define _MM7 OPCODE_VALUE_REG(__MM7)

#define _XMM0 (OPCODE_REG(_128, __XMM0))
#define _XMM1 (OPCODE_REG(_128, __XMM1))
#define _XMM2 (OPCODE_REG(_128, __XMM2))
#define _XMM3 (OPCODE_REG(_128, __XMM3))
#define _XMM4 (OPCODE_REG(_128, __XMM4))
#define _XMM5 (OPCODE_REG(_128, __XMM5))
#define _XMM6 (OPCODE_REG(_128, __XMM6))
#define _XMM7 (OPCODE_REG(_128, __XMM7))

#define _YMM0 (OPCODE_REG(_256, __YMM0))
#define _YMM1 (OPCODE_REG(_256, __YMM1))
#define _YMM2 (OPCODE_REG(_256, __YMM2))
#define _YMM3 (OPCODE_REG(_256, __YMM3))
#define _YMM4 (OPCODE_REG(_256, __YMM4))
#define _YMM5 (OPCODE_REG(_256, __YMM5))
#define _YMM6 (OPCODE_REG(_256, __YMM6))
#define _YMM7 (OPCODE_REG(_256, __YMM7))

#define _ZMM0 (OPCODE_REG(_512, __ZMM0))
#define _ZMM1 (OPCODE_REG(_512, __ZMM1))
#define _ZMM2 (OPCODE_REG(_512, __ZMM2))
#define _ZMM3 (OPCODE_REG(_512, __ZMM3))
#define _ZMM4 (OPCODE_REG(_512, __ZMM4))
#define _ZMM5 (OPCODE_REG(_512, __ZMM5))
#define _ZMM6 (OPCODE_REG(_512, __ZMM6))
#define _ZMM7 (OPCODE_REG(_512, __ZMM7))

// Note: __REGISTER     => register as opcode register
//       _REGISTER      => register as value register

Register opcode_make_register(enum Register_Size_kind, reg_enum_t);

ASM_Value opcode_make_asm_value__reg(Register);
ASM_Value opcode_make_asm_value__num(double);
ASM_Value opcode_make_asm_value__adress_reg(ASM_Value);
ASM_Value opcode_make_asm_value__adress_raw(void *);
ASM_Value opcode_make_asm_value__label(String);
ASM_Value opcode_make_asm_value__add(ASM_Value, ASM_Value);
ASM_Value opcode_make_asm_value__sub(ASM_Value, ASM_Value);
ASM_Value opcode_make_asm_value__mul(ASM_Value, ASM_Value);

ASM_Instr opcode_make_opcode__add(dest_t, src_t);
ASM_Instr opcode_make_opcode__sub(dest_t, src_t);
ASM_Instr opcode_make_opcode__mov(dest_t, src_t);
ASM_Instr opcode_make_opcode__lea(dest_t, src_t);
ASM_Instr opcode_make_opcode__imul(dest_t, src_t);
ASM_Instr opcode_make_opcode__test(dest_t, src_t);
ASM_Instr opcode_make_opcode__xor(dest_t, src_t);
ASM_Instr opcode_make_opcode__shl(dest_t, src_t);
ASM_Instr opcode_make_opcode__shr(dest_t, src_t);
ASM_Instr opcode_make_opcode__inc(dest_t);
ASM_Instr opcode_make_opcode__dec(dest_t);
ASM_Instr opcode_make_opcode__mul(src_t);
ASM_Instr opcode_make_opcode__div(src_t);
ASM_Instr opcode_make_opcode__push(src_t);
ASM_Instr opcode_make_opcode__fabs(src_t);
ASM_Instr opcode_make_opcode__fchs(src_t);
ASM_Instr opcode_make_opcode__pop(src_t);
ASM_Instr opcode_make_opcode__call(adrr_t);
ASM_Instr opcode_make_opcode__cdq(void);
ASM_Instr opcode_make_opcode__cmp(dest_t, src_t);
ASM_Instr opcode_make_opcode__je(mem_t);
ASM_Instr opcode_make_opcode__jne(mem_t);
ASM_Instr opcode_make_opcode__jg(mem_t);
ASM_Instr opcode_make_opcode__jge(mem_t);
ASM_Instr opcode_make_opcode__jl(mem_t);
ASM_Instr opcode_make_opcode__jle(mem_t);
ASM_Instr opcode_make_opcode__jmp(mem_t);
ASM_Instr opcode_make_opcode__sete(mem_t);
ASM_Instr opcode_make_opcode__setne(mem_t);
ASM_Instr opcode_make_opcode__setg(mem_t);
ASM_Instr opcode_make_opcode__setge(mem_t);
ASM_Instr opcode_make_opcode__setl(mem_t);
ASM_Instr opcode_make_opcode__setle(mem_t);
ASM_Instr opcode_make_opcode__comment(String);
ASM_Instr opcode_make_directive__byte(directivy_num_t);
ASM_Instr opcode_make_directive__2byte(directivy_num_t);
ASM_Instr opcode_make_directive__4byte(directivy_num_t);
ASM_Instr opcode_make_directive__8byte(directivy_num_t);
ASM_Instr opcode_make_directive__ascii(directivy_ascii_t);
ASM_Instr opcode_make_directive__asciz(directivy_ascii_t);
ASM_Instr opcode_make_directive__long(directivy_num_t);
ASM_Instr opcode_make_directive__quad(ASM_Value);
ASM_Instr opcode_make_directive__zero(ASM_Value);
ASM_Instr opcode_make_opcode__ret(void);
ASM_Instr opcode_make_opcode__empty(void);
ASM_Instr opcode_make_opcode__fnop(void);
ASM_Instr opcode_make_opcode__syscall(void);

bool cmp_asm_block(const Opcode_block, const Opcode_block);
bool cmp_asm_instr(const ASM_Instr, const ASM_Instr);

Opcode_block opcode_make_block(ASM_Instr *, size_t, size_t);

void retit(Opcode_block *block);

typedef int label_count_t;

// Returns a label name as   `LCX:` with X being a Label Count
String label_name__cnt_lc(const label_count_t);
// Returns a label with a custom name as `._X__NAME:` with X being a Label Count and NAME the chosen name
String label_custom_name__cnt_lc(const String, const label_count_t);
void inc_lc(label_count_t *);
label_count_t pred_lc(const label_count_t);
label_count_t succ_lc(const label_count_t);

// Usage of label_counter:
//  First: create the LC label opcode block and fill it.
//  Second: call this LC label when needed in the current instance with `label_name__cnt_lc`.
//  Third: push the LC label to the rest of the code and give the label_count_t in parameter.

Label opcode_make_label(String, Opcode_block);
Label opcode_make_label_desc(String, Opcode_block, String);

Assembled_Program opcode_make_assembled_program(Labels, String, String);

void pblock_assemble(Opcode_block *, const int, ...);
Opcode_block pblock_create_assemble(const int, ...);
Opcode_block pblock_singleton(const ASM_Instr);

// Make `label_count_t *` NULL if the pused label is not an inter jumping label AKA lcx

void push_labels(Labels *, Label, label_count_t *);
void push_labels_from_oblock(Labels *, String, const Opcode_block, label_count_t *);
void push_labels_from_oblock_desc(Labels *, String, Opcode_block, String, label_count_t *);

Labels concat_labels(Labels, Labels);

enum ASM_RegState
{
    REG_FREE,      // When not yet in use
    REG_UNUSED,    // When no longer in use
    REG_USED,      // When is used
    REG_RESERVED,  // When is reserved but not yet used
    REG_NOT_USABLE // When is not usable (index too deep, for example )
};

// https://docs.oracle.com/cd/E26502_01/html/E28388/eoiyg.html

// Represents the local environment of an opcode block,
// in order to have all the information necessary to use the registers to facilitate their use.
Map(Lenv, Regi, enum ASM_RegState);

Register new_reg(Lenv_map_t *, enum Register_Size_kind *);

// Groups any constant string literal used in the program in a dictionary
// referencing them to avoid useless duplications.
Map(OnlyStrLbl_const_lit, String, Name);

Map(YetExisting_lbl, hash_t, Name);
