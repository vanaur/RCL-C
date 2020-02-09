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

#ifndef _JIT_X86_64_H_
#define _JIT_X86_64_H_

#ifdef __CPLUSPLUS
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include <VM\Core\Execution\JIT\lib\JIT_x86_64\JIT_x86_64_lib.h>

#define FAILPATH(err) {e=(err);goto l_exit;}

#define NUM_HOST_REGS 16
#define NUM_SPILL_SLOTS 32

enum e_jit_host_reg {
    JIT_HOST_REG_INVALID = -1,
    rax = 0, rcx, rdx, rbx, rsp, rbp, rsi, rdi,
    r8, r9, r10, r11, r12, r13, r14, r15,
};

typedef enum e_jit_host_reg jit_host_reg;

enum e_jit_reg_access {
    JIT_ACCESS_R, JIT_ACCESS_W, JIT_ACCESS_RW,
};

typedef enum e_jit_reg_access jit_reg_access;

typedef int32_t jit_reg_age;

/* The x86_64 variant of the emitter reference in jit_state. */
struct jit_emitter {
    jit_reg host_regmap[NUM_HOST_REGS];
    jit_reg_age host_agemap[NUM_HOST_REGS];
    uint32_t host_busy;

    int64_t spill_vreg[NUM_SPILL_SLOTS];
    uint32_t spill_busy;
};

/* A variant of jit_pointer using host registers. */
struct jit_host_ptr {
    jit_host_reg base;
    jit_host_reg index;
    int32_t scale;
    int32_t offset;
};

#define REX(w,r,x,b) (0x40 | (!!(w)<<3) | (!!(r)<<2) | (!!(x)<<1) | (!!(b)))
#define MODRM(mod, reg, rm) ((((mod) & 3) << 6) | (((reg) & 7) << 3) | (rm))

#define NEED_REX(r) ((r)&0x8)
#define HOSTREG(r) ((r)&0x7)

#define REX_W REX(1,0,0,0)
#define REX_R REX(0,1,0,0)
#define REX_X REX(0,0,1,0)
#define REX_B REX(0,0,0,1)
#define REX_WB REX(1,0,0,1)

#define MOD_RIP_SIB 0
#define MOD_DISP8 1
#define MOD_DISP32 2
#define MOD_REGDIRECT 3
#define RM_DISP32 5

#define OX_ADD 0
#define OX_OR  1
#define OX_ADC 2
#define OX_SBB 3
#define OX_AND 4
#define OX_SUB 5
#define OX_XOR 6
#define OX_CMP 7

#define OX_ROL 0
#define OX_ROR 1
#define OX_RCL 2
#define OX_RCR 3
#define OX_SHL 4
#define OX_SAL 4
#define OX_SHR 5
#define OX_SAR 7

typedef uint8_t* (*pfn_e_r_to_r)(uint8_t *p, jit_host_reg rin, jit_host_reg rout);
typedef uint8_t* (*pfn_e_r_r_to_r)(uint8_t *p, jit_host_reg rin1, jit_host_reg rin2, jit_host_reg rout);
typedef uint8_t* (*pfn_e_imm32_r_to_r)(uint8_t *p, int32_t imm, jit_host_reg rin, jit_host_reg rout);
typedef uint8_t* (*pfn_e_imm32_to_r)(uint8_t *p, int32_t imm, jit_host_reg rout);
typedef uint8_t* (*pfn_e_imm16_to_r)(uint8_t *p, int16_t imm, jit_host_reg rout);
typedef uint8_t* (*pfn_e_imm8_to_r)(uint8_t *p, int8_t imm, jit_host_reg rout);


uint8_t* jit_emit__mov_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg reg);
uint8_t* jit_emit__mov_imm16_to_reg(uint8_t *p, int16_t imm, jit_host_reg reg);
uint8_t* jit_emit__mov_imm8_to_reg(uint8_t *p, int8_t imm, jit_host_reg reg);
uint8_t* jit_emit__mov_reg32_to_m(uint8_t *p, jit_host_reg reg, int32_t *m);
uint8_t* jit_emit__mov_reg16_to_m(uint8_t *p, jit_host_reg reg, int32_t *m);
uint8_t* jit_emit__mov_reg8_to_m(uint8_t *p, jit_host_reg reg, int32_t *m);
uint8_t* jit_emit__mov_m32_to_reg(uint8_t *p, int32_t *m, jit_host_reg reg);
uint8_t* jit_emit__mov_m16_to_reg(uint8_t *p, int32_t *m, jit_host_reg reg);
uint8_t* jit_emit__mov_m8_to_reg(uint8_t *p, int32_t *m, jit_host_reg reg);
uint8_t* jit_emit__lea_immdisp32_to_reg(uint8_t *p, void *m, jit_host_reg reg);
uint8_t* jit_emit__mov_regptr32_to_reg(uint8_t *p, struct jit_host_ptr *rp, jit_host_reg reg);
uint8_t* jit_emit__mov_reg32_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regto);

uint8_t* jit_emit__add_reg32_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regto);
uint8_t* jit_emit__add_reg16_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regto);
uint8_t* jit_emit__add_reg8_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__add_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__add_imm16_to_reg(uint8_t *p, int16_t imm, jit_host_reg regout);
uint8_t* jit_emit__add_imm8_to_reg(uint8_t *p, int8_t imm, jit_host_reg regout);

uint8_t* jit_emit__sub_reg32_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__sub_reg16_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__sub_reg8_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__sub_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__sub_imm16_to_reg(uint8_t *p, int16_t imm, jit_host_reg regout);
uint8_t* jit_emit__sub_imm8_to_reg(uint8_t *p, int8_t imm, jit_host_reg regout);

uint8_t* jit_emit__and_reg32_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__and_reg16_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__and_reg8_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__and_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__and_imm16_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__and_imm8_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);

uint8_t* jit_emit__or_reg32_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__or_reg16_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__or_reg8_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__or_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__or_imm16_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__or_imm8_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);

uint8_t* jit_emit__xor_reg32_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__xor_reg16_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__xor_reg8_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout);
uint8_t* jit_emit__xor_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__xor_imm16_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__xor_imm8_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);

uint8_t* jit_emit__shl_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__shr_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);
uint8_t* jit_emit__sar_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout);

uint8_t* jit_emit__call_m32(uint8_t *p, void *m);
uint8_t* jit_emit__ret(uint8_t *p);
uint8_t* jit_emit__push_reg(uint8_t *p, jit_host_reg regout);
uint8_t* jit_emit__pop_reg(uint8_t *p, jit_host_reg regout);

#ifdef __CPLUSPLUS
}
#endif

#endif
