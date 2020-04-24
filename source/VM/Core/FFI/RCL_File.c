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
#include <string.h>
#include <VM\Core\FFI\RCL_File.h>

void init_RCL_File(RCL_Files *files, size_t initSize)
{
    files->array = malloc(initSize * sizeof(RCL_File));
    files->used = 0;
    files->size = initSize;
}

void add_RCL_File(RCL_Files *files, RCL_File new)
{
    if (files->used == files->size)
    {
        files->size *= 2;
        files->array = (RCL_File *)realloc(files->array, files->size * sizeof(RCL_File));
        if (files->array == NULL)
            perror("Out of files memory");
    }
    files->array[files->used++] = new;
}

RCL_File new_RCL_File(char *path, char *asid)
{
    RCL_File result = {.path = path, .asid = asid};
    return result;
}

RCL_File *getSpecificRCL_File(RCL_Files *files, char *asid)
{
    for (Iterator i = 0; i < files->used; i++)
        if (!strcmp(files->array[i].asid, asid))
            return &files->array[i];
    return NULL;
}
