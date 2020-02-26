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

#include <ctype.h>
#include <string.h>

#include <Tools\Hash\djb2.h>
#include <Tools\Utils\Utils.h>
#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\Syntax\Identifier\Functions.h>
#include <VM\Core\Browse\BrowsedAbsyn.h>
#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>
#include <VM\Core\Wordefinition\Wordico.h>

void init_wordico(struct Wordico * wordico, ListDefinition ld)
{
    vec_init_externs(&wordico->externs, count_externs(ld) + 1);
    vec_init_function(&wordico->functions, count_functions(ld) + 1);
    vec_init_structures(&wordico->structures, count_structures(ld) + 1);
    vec_init_lambdas(&wordico->lambdas, 1);
}

void init_wordico_nbr(struct Wordico * wordico, size_t nbr)
{
    vec_init_externs(&wordico->externs, nbr);
    vec_init_function(&wordico->functions, nbr);
    vec_init_structures(&wordico->structures, nbr);
    vec_init_lambdas(&wordico->lambdas, 1);
}

void set_wordico(struct Wordico * wordico, BrowsedAbsyn * psdata, ListDefinition ld)
{
    while (ld != NULL)
    {
        switch (ld->definition_->kind)
        {
        case is_Def:
            vec_add_function(&wordico->functions, psdata, ld->definition_);
            break;

        case is_Extern:
            vec_add_externs(&wordico->externs, ld->definition_);
            break;

        case is_Structure:
            vec_add_structures(&wordico->structures, ld->definition_);
            break;

        default:
            printf("Internal [%s:%d]\n", __FILE__, __LINE__);
            break;
        }
        ld = ld->listdefinition_;
    }
}

struct RCL_Function *getSpecific_function(struct Wordico * wordico, const hash_t hash_word)
{
    int left = 0;
    int right = wordico->functions.used;
    int mid = 0;

    while (left <= right)
    {
        mid = left + (right - left) / 2;

        if (wordico->functions.array[mid].hash_code == hash_word)
            return &wordico->functions.array[mid];

        if (wordico->functions.array[mid].hash_code < hash_word)
            left = mid + 1;

        else
            right = mid - 1;
    }

    return NULL;
}

struct RCL_Extern *getSpecific_extern(struct Wordico * wordico, const hash_t hash_word)
{
    int left = 0;
    int right = wordico->externs.used;
    int mid = 0;

    while (left <= right)
    {
        mid = left + (right - left) / 2;

        if (wordico->externs.array[mid].hash_code == hash_word)
            return &wordico->externs.array[mid];

        if (wordico->externs.array[mid].hash_code < hash_word)
            left = mid + 1;

        else
            right = mid - 1;
    }

    return NULL;
}

struct RCL_Structure *getSpecific_structure(struct Wordico * wordico, const hash_t hash_word)
{
    int left = 0;
    int right = wordico->structures.used;
    int mid = 0;

    while (left <= right)
    {
        mid = left + (right - left) / 2;

        if (wordico->structures.array[mid].hash_code == hash_word)
            return &wordico->structures.array[mid];

        if (wordico->structures.array[mid].hash_code < hash_word)
            left = mid + 1;

        else
            right = mid - 1;
    }

    return NULL;
}

struct RCL_Lambda *getSpecific_lambda(struct Wordico * wordico, const hash_t hash_word)
{
    for (Iterator i = 0; i < wordico->lambdas.used; i++)
        if (wordico->lambdas.array[i].hash_code == hash_word)
            return &wordico->lambdas.array[i];
    return NULL;
}

struct RCL_Structure_field *getSpecific_field(struct Wordico * wordico, const String word)
{
    for (Iterator i = 0; i < wordico->structures.used; i++)
        for (Iterator j = 0; j < wordico->structures.array[i].field_alloc_used; j++)
            if (!strcmp(wordico->structures.array[i].fields[j].name, word))
                return &wordico->structures.array[i].fields[j];
    return NULL;
}

size_t count_functions(ListDefinition ld)
{
    ListDefinition tmp = ld;
    size_t result = 0;
    while (tmp != NULL)
    {
        if (tmp->definition_->kind == is_Def)
            result++;
        tmp = tmp->listdefinition_;
    }
    return result;
}

size_t count_externs(ListDefinition ld)
{
    ListDefinition tmp = ld;
    size_t result = 0;
    while (tmp != NULL)
    {
        if (tmp->definition_->kind == is_Extern)
            result++;
        tmp = tmp->listdefinition_;
    }
    return result;
}

size_t count_structures(ListDefinition ld)
{
    ListDefinition tmp = ld;
    size_t result = 0;
    while (tmp != NULL)
    {
        if (tmp->definition_->kind == is_Structure)
            result++;
        tmp = tmp->listdefinition_;
    }
    return result;
}

bool yetDefined(String *kst, struct Wordico * wordico, const String name)
{
/*     if (wordico->functions.used)
        if (getSpecific_function(wordico, hash_djb2(name)) != NULL)
        {
            rcl_asprintf(kst, "function");
            return true;
        }
    if (wordico->externs.used)
        if (getSpecific_extern(wordico, name) != NULL)
        {
            rcl_asprintf(kst, "external function");
            return true;
        }
    if (wordico->structures.used)
        if (getSpecific_structure(wordico, name) != NULL)
        {
            rcl_asprintf(kst, "structure");
            return true;
        }
    return false; */
}
