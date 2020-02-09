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

uint8_t *
jit_emit__sub_reg32_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout)
{
    if (NEED_REX(regin) || NEED_REX(regout))
        *p++ =
            REX(0, NEED_REX(regin), 0, NEED_REX(regout));
    *p++ = 0x29;
    *p++ = MODRM(MOD_REGDIRECT, HOSTREG(regin), HOSTREG(regout));
    return p;
}

uint8_t *
jit_emit__sub_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout)
{
    if (NEED_REX(regout))
        *p++ = REX_R;
    *p++ = 0x81;
    *p++ = MODRM(MOD_REGDIRECT, OX_SUB, HOSTREG(regout));
    *(int32_t *)p = imm;
    p += sizeof(int32_t);

    return p;
}

uint8_t *
jit_emit__cmp_reg32_to_reg(uint8_t *p, jit_host_reg regin, jit_host_reg regout)
{
    if (NEED_REX(regin) || NEED_REX(regout))
        *p++ =
            REX(0, NEED_REX(regin), 0, NEED_REX(regout));
    *p++ = 0x39;
    *p++ = MODRM(MOD_REGDIRECT, HOSTREG(regin), HOSTREG(regout));
    return p;
}

uint8_t *
jit_emit__cmp_imm32_to_reg(uint8_t *p, int32_t imm, jit_host_reg regout)
{
    if (NEED_REX(regout))
        *p++ = REX_R;
    *p++ = 0x81;
    *p++ = MODRM(MOD_REGDIRECT, OX_CMP, HOSTREG(regout));
    *(int32_t *)p = imm;
    p += sizeof(int32_t);

    return p;
}
