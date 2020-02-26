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
#include <stdio.h>
#include <assert.h>

#include <VM\Core\Execution\Compiler\Assembler\Assemble.h>
#include <VM\Core\Execution\Compiler\Assembler\Opcode.h>
#include <VM\Core\Execution\Compiler\Assembler\Show.h>

#include <VM\Core\Execution\JIT\RCL_JIT.h>
#include <VM\Core\Execution\JIT\lib\JIT_x86_64\JIT_x86_64_lib.h>

#include <VM\Core\Browse\BResult.h>
#include <VM\Core\RawCode\RawCode.h>

#include <Tools\CPS\Trampoline.h>
#include <Tools\Windows\mman.h>
#include <Tools\Utils\Utils.h>
#include <Tools\Time_Measure\Time_Measure.h>

typedef struct RCL_JIT_INFO_STATE RCL_JIT_t;

/***** x86_64 ASM AST builder *****/

#include <VM\Core\Execution\Compiler\Assembler\Opcode.h>

static Assembled_Program jit_assemble(RCL_JIT_t *rcl_jit, BResult * bresult)
{
    Assembled_Program result;
    result.Format = NULL;
    result.Entry = "RCL_main";
    InitVector((&result.labels), bresult->wordico.functions.used + 1, Label);
    //PushToVector((&result.labels), Label, NULL);
}

/**************************/

struct RCL_JIT_INFO_STATE
{
    size_t num_instrs;
    jit_state *jstate;
    jit_error jerr;
    int program_return;
    void *buffer;
    void *abuffer;
    struct jit_instr *instr[];
};

#define SET_NINSTRS(j, n) j.num_instrs = n;
#define PBYTES 4096

// Main return
typedef int (*p_fn)(void);

static void encode_jit_program(RCL_JIT_t *rcl_jit, BResult * bresult)
{
}

static void fn_RCL_JIT_INFO_STATE_init(RCL_JIT_t *rcl_jit, BResult * bresult)
{
    assert(rcl_jit != NULL);
    *rcl_jit->instr = malloc(rcl_jit->num_instrs * sizeof(*rcl_jit->instr));
    rcl_jit->jerr = jit_create(&rcl_jit->jstate, JIT_FLAG_NONE);
    rcl_jit->buffer = calloc(PBYTES * 1 + PBYTES, 1);
    rcl_jit->abuffer = (void *)(((uintptr_t)rcl_jit->buffer + PBYTES - 1) & ~(PBYTES - 1));
    for (Iterator i = 0; i < rcl_jit->num_instrs; i++)
        rcl_jit->instr[i] = jit_instr_new(rcl_jit->jstate);
}

static void fn_RCL_JIT_INFO_STATE_end(RCL_JIT_t *rcl_jit)
{
    mprotect(rcl_jit->abuffer, PBYTES, PROT_READ | PROT_WRITE | PROT_EXEC);
    rcl_jit->program_return = ((p_fn)rcl_jit->abuffer)();
    rcl_jit->jerr = rcl_jit->program_return ? JIT_SUCCESS : JIT_ERROR_UNKNOWN;
    free(rcl_jit->buffer);
    jit_destroy(rcl_jit->jstate);
}

int test()
{
    return 73;
}

void jit(BResult * bresult)
{
    // ----- Create the assembly program ----- //
    Assembled_Program program = assemble(bresult);
    
    // ----- Initialize the JIT ----- //
    RCL_JIT_t rcl_jit;
    SET_NINSTRS(rcl_jit, 2);
    fn_RCL_JIT_INFO_STATE_init(&rcl_jit, bresult);

    // ----- Write the program into memory ----- //
    CALL_M(rcl_jit.instr[0], (int32_t *)test, JIT_32BIT);
    RET(rcl_jit.instr[1]);

    // ----- Run JIT ----- //
    jit_begin_block(rcl_jit.jstate, rcl_jit.abuffer);
    jit_emit_all(rcl_jit.jstate);
    jit_end_block(rcl_jit.jstate);
    fn_RCL_JIT_INFO_STATE_end(&rcl_jit);

    // ----- End of JIT, display infirmation ----- //
    printf("Program returned: %d\n", rcl_jit.program_return);
    printf("Jerr: %d\n", rcl_jit.jerr);
    //printf("Time taken: %s\n", show_time(tstart1, tend1));
}

// https://github.com/hellerve/cj/tree/master/src
// https://github.com/tykel/libjit/blob/master/test.c
