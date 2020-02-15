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

#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Wordefinition\Wordico.h>
#include <VM\Core\State\State.h>
#include <VM\Core\Argv.h>
#include <VM\Core\Exec.h>

// `BResult` (for Browse Result, i.e. the result obtained after the naming phase)
// contains absolutely all the information about the program to be used.
typedef struct BResult
{
    // Information about the main function and about included and imported files.
    BrowsedAbsyn psdata;
    // Contains all information on the functions, external resources and structures used in the program.
    // It can evolve during the evaluation by including or removing lambdas.
    struct Wordico wordico;
    // Contains only the information on the program evaluation stop, when the program will be executed.
    RawCode resulting;
    // Informs about the state of the virtual machine through a list of errors, attentions or information,
    // these can be added at any time, and is therefore not constant.
    // The state will be displayed at the end, if it contains something.
    struct State state;
    // In case the program requires boot arguments (those of the main function),
    // their information will be available here if the program is launched from the interpreter or the JIT.
    struct RCL_Argv argvs;
    // Represents a record of all the information that the user has sent to the virtual machine for his program.
    Exec exec_infos;
    // For information reports, contains the name of the file currently being processed. 
    String current_filename;
    // For information reports, contains the name of the function currently being processed.
    String current_name;
} __attribute__((packed)) BResult;

// Returns an initialized `BResult` structure, but without any information.
struct BResult new_bresult();
