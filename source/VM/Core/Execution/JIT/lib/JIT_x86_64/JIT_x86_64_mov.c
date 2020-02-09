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

#include <VM\Core\Execution\JIT\lib\JIT_x86_64\JIT_x86_64.h>
#include <VM\Core\Execution\JIT\lib\JIT_x86_64\JIT_x86_64_lib.h>

uint8_t *
jit_emit__mov_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg reg)
{
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0xb8 + HOSTREG(reg);
    *(int32_t *)p = imm;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__mov_imm16_to_reg(uint8_t *p, int16_t imm, jit_host_reg reg)
{
    *p++ = 0x66;
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0xb8 + HOSTREG(reg);
    *(int16_t *)p = imm;
    p += sizeof(int16_t);

    return p;
}

uint8_t *
jit_emit__mov_imm8_to_reg(uint8_t *p, int8_t imm, jit_host_reg reg)
{
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0xb0 + HOSTREG(reg);
    *(int32_t *)p = imm;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__mov_m32_to_reg(uint8_t *p, int32_t *m, jit_host_reg reg)
{
    size_t ibs = !!(NEED_REX(reg)) + 1 + 1 + sizeof(int32_t);
    int32_t disp = (int32_t)((int64_t)m - (int64_t)p - ibs);
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0x8b;
    *p++ = MODRM(MOD_RIP_SIB, HOSTREG(reg), RM_DISP32);
    *(int32_t *)p = disp;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__mov_m16_to_reg(uint8_t *p, int32_t *m, jit_host_reg reg)
{
    size_t ibs = 1 + !!(NEED_REX(reg)) + 1 + 1 + sizeof(int32_t);
    int32_t disp = (int32_t)((int64_t)m - (int64_t)p - ibs);
    *p++ = 0x66;
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0x8b;
    *p++ = MODRM(MOD_RIP_SIB, HOSTREG(reg), RM_DISP32);
    *(int32_t *)p = disp;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__mov_m8_to_reg(uint8_t *p, int32_t *m, jit_host_reg reg)
{
    size_t ibs = !!(NEED_REX(reg)) + 1 + 1 + sizeof(int32_t);
    int32_t disp = (int32_t)((int64_t)m - (int64_t)p - ibs);
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0x8a;
    *p++ = MODRM(MOD_RIP_SIB, HOSTREG(reg), RM_DISP32);
    *(int32_t *)p = disp;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__lea_immdisp32_to_reg(uint8_t *p, void *m, jit_host_reg reg)
{
    size_t ibs = !!(NEED_REX(reg)) + 1 + 1 + sizeof(int32_t);
    int32_t disp = (int32_t)((int64_t)m - (int64_t)p - ibs);
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0x8d;
    *p++ = MODRM(MOD_RIP_SIB, HOSTREG(reg), RM_DISP32);
    *(int32_t *)p = disp;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__mov_regptr32_to_reg(uint8_t *p, struct jit_host_ptr *rp,
                              jit_host_reg reg)
{
    *p++ = 0x67;
    if (NEED_REX(reg) || NEED_REX(rp->base) || NEED_REX(rp->index))
        *p++ = REX(0, NEED_REX(reg), NEED_REX(rp->index),
                   NEED_REX(rp->base));
    *p++ = 0x8b;
    *p++ = MODRM(MOD_DISP8, HOSTREG(reg), HOSTREG(rp->base));
    *p++ = (int8_t)rp->offset;
    return p;
}

uint8_t *
jit_emit__mov_reg32_to_m(uint8_t *p, jit_host_reg reg, int32_t *m)
{
    size_t ibs = !!(NEED_REX(reg)) + 1 + 1 + sizeof(int32_t);
    int32_t disp = (int32_t)((int64_t)m - (int64_t)p - ibs);
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0x89;
    *p++ = MODRM(MOD_RIP_SIB, HOSTREG(reg), RM_DISP32);
    *(int32_t *)p = disp;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__mov_reg16_to_m(uint8_t *p, jit_host_reg reg, int32_t *m)
{
    size_t ibs = 1 + !!(NEED_REX(reg)) + 1 + 1 + sizeof(int32_t);
    int32_t disp = (int32_t)((int64_t)m - (int64_t)p - ibs);
    *p++ = 0x66;
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0x89;
    *p++ = MODRM(MOD_RIP_SIB, HOSTREG(reg), RM_DISP32);
    *(int32_t *)p = disp;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__mov_reg8_to_m(uint8_t *p, jit_host_reg reg, int32_t *m)
{
    size_t ibs = !!(NEED_REX(reg)) + 1 + 1 + sizeof(int32_t);
    int32_t disp = (int32_t)((int64_t)m - (int64_t)p - ibs);
    if (NEED_REX(reg))
        *p++ = REX_R;
    *p++ = 0x88;
    *p++ = MODRM(MOD_RIP_SIB, HOSTREG(reg), RM_DISP32);
    *(int32_t *)p = disp;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__mov_reg32_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout)
{
    if (NEED_REX(regin) || NEED_REX(regout))
        *p++ =
            REX(0, NEED_REX(regin), 0, NEED_REX(regout));
    *p++ = 0x89;
    *p++ = MODRM(MOD_REGDIRECT, HOSTREG(regin), HOSTREG(regout));
    return p;
}
