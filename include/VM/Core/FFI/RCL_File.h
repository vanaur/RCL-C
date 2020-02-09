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
#include <stdlib.h>
#include <Tools\Vector\Vector.h>
#include <Tools\Utils\Utils.h>

typedef struct
{
    String path; // Path of the file
    String asid; // Name given to the file
} __attribute__((packed)) RCL_File;

typedef Vector(RCL_Files, RCL_File);

// Included DLLs
typedef RCL_Files Includes;
// Imported files
typedef RCL_Files Imports;

#define init_includes__ init_RCL_File
#define init_imports__ init_RCL_File

void init_RCL_File(RCL_Files *, size_t);

void add_RCL_File(RCL_Files *, RCL_File);

RCL_File new_RCL_File(String, String);

RCL_File *getSpecificRCL_File(RCL_Files *, String);
