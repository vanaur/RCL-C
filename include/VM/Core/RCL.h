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
#include <VM\Core\Browse\BResult.h>
#include <VM\Core\Exec.h>

#define PRINT_RCL_TEXT                                                                                                           \
    printf("  _____             _   _                   _____                 _      _ _                          \n");          \
    printf(" |  __ \\           | | (_)                 / ____|               | |    (_) |                         \n");         \
    printf(" | |__) |   _ _ __ | |_ _ _ __ ___   ___  | |     ___  _ __ ___  | |     _| |__  _ __ __ _ _ __ _   _ \n");          \
    printf(" |  _  / | | | '_ \\| __| | '_ ` _ \\ / _ \\ | |    / _ \\| '__/ _ \\ | |    | | '_ \\| '__/ _` | '__| | | |\n");    \
    printf(" | | \\ \\ |_| | | | | |_| | | | | | |  __/ | |___| (_) | | |  __/ | |____| | |_) | | | (_| | |  | |_| |\n");        \
    printf(" |_|  \\_\\__,_|_| |_|\\__|_|_| |_| |_|\\___|  \\_____\\___/|_|  \\___| |______|_|_.__/|_|  \\__,_|_|   \\__, |\n"); \
    printf("                                                                                                 __/ |\n");          \
    printf("                                                                                                |___/ \n");          \
    printf(" ______________________________________________________________________________________________________\n\n");

#define RCL_GITHUB_LINK "https://github.com/vanaur/RCL"

int main(int, String *);
void clean_heap(BResult *, const Exec);
void make_noise(const BResult bresult, const Exec exec, const String files[], const size_t argc, const uint64_t all_time, const uint64_t eval_time, const uint64_t t1, const uint64_t t2, const uint64_t e1, const uint64_t e2);
int noarg();
