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

#include <ffi.h>
#include <VM\Core\Syntax\Absyn.h>

// Converts RCL C FFI types to real C FFI type
ffi_type *ffi_ctype_to_real_ctype(const FFI_Type_Signature);
// Converts RCL C list FFI types to real FFI types
void ffi_ctype_to_real_ctypes(ffi_type **, const ListFFI_Type_Signature);
// Counts the number of types in the given parameters of a function.
// The `void` types are ignored.
size_t count_ffi_types(const ListFFI_Type_Signature);
