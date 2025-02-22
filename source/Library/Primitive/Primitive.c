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

#include <Library\Primitive\Primitive.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>
#include <VM\Core\Wordefinition\Wordico.h>

#define INIT_DEFAULT_SIZE 12

String rcl_show_ir_expr(const rcl_expr_t e)
{
    return show_rcode(e);
}

String rcl_show_ir_fun(const rcl_fun_t f)
{
    return rcl_sprintf_s("%s = %s ;", f.name, show_rcode(f.body));
}

String rcl_show_ir_struct(const rcl_struct_t s)
{
    return rcl_sprintf_s("structure %s ;", s.name);
}

rcl_program_t new_rcl_program(const String prog_name, const String src_lang, const rcl_exec_infos_t exec_infos)
{
    rcl_program_t result;
    result.program_name = prog_name;
    result.src_language = src_lang;
    result.pexecinfos = exec_infos;
    init_rcode(&result.pmain, INIT_DEFAULT_SIZE);
    init_rcl_const_array_vec(&result.pconstarrays, INIT_DEFAULT_SIZE);
    init_rcl_fun_vec(&result.pfunctions, INIT_DEFAULT_SIZE);
    init_rcl_struct_vec(&result.pstructures, INIT_DEFAULT_SIZE);
    return result;
}
