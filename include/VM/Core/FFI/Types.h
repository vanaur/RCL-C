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
/*
 * File to facilitate the interfacing of external functions as to their type and those of the RCL,
 * also provides components to work on the stack at the level of pointers and
 * structures including specialized stack operators.
 */

#include <ffi.h>

#include <VM\Core\Syntax\Absyn.h>


// Converts RCL C FFI types to real FFI type
ffi_type* convertRCLT_FFIT(FFI_Type_Signature);

// Converts RCL C list FFI types to real FFI types
void convertRCLT_FFITS(ffi_type**, ListFFI_Type_Signature);

size_t count_ffi_types(ListFFI_Type_Signature);
