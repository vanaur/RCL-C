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
#include <Tools\Utils\Utils.h>

enum REPL_Command
{
    REPL_STACK,  // Display the current stack                                        : stack
    REPL_TYPEOF, // Display the type of the given expression                         : typeof E
    REPL_STEP,   // Evaluate the expression step by step                             : step E
    REPL_EXEC,   // Evaluate the expression                                          : exec E
    REPL_EXECT,  // Evaluate the expression and display the time needed              : exect E
    REPL_SHOW,   // Display the value of variable / function                         : show F
    REPL_OPT,    // Apply optimization lvl3 on the given function                    : opt F
    REPL_SET,    // Set a RCL option                                                 : set O
    REPL_OOF,    // Display the algorithm time complexity of the given function      : oof F
    REPL_FREE,   // Free all lambdas                                                 : free
    REPL_QUIT,   // Exit the REPL                                                    : quit
    REPL_FUNS,   // Display all the loaded functions                                 : funs
    REPL_STRUCTS,// Display all the loaded structures                                : structs
    REPL_EXTERNS,// Display all the loaded external functions                        : externs
    REPL_HELP,   // Display the help                                                 : help
    CMDERR       // When there is an error of parsing
};

typedef SIGMA_TYPE(
    REPL_AST, enum REPL_Command,
    SIGMA_CTOR(repl_typeof, String expr);
    SIGMA_CTOR(repl_step, String expr);
    SIGMA_CTOR(repl_exec, String expr);
    SIGMA_CTOR(repl_exect, String expr);
    SIGMA_CTOR(repl_show, String fname);
    SIGMA_CTOR(repl_opt, String opt; String fname);
    SIGMA_CTOR(repl_set, String option);
    SIGMA_CTOR(repl_oof, String fname);
    SIGMA_CTOR(cmderr, String err);
    SIGMA_CTOR(repl_funs);
    SIGMA_CTOR(repl_structs);
    SIGMA_CTOR(repl_externs);
    SIGMA_CTOR(repl_stack);
    SIGMA_CTOR(repl_free);
    SIGMA_CTOR(repl_quit);
    SIGMA_CTOR(repl_help));

REPL_AST repl_cmd_parse(const String);
