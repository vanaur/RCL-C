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

#include <VM\Core\Syntax\Identifier\Functions.h>
#include <VM\Core\Syntax\Absyn.h>
#include <Tools\Utils\Utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t identifier_length(Identifier id)
{
    switch (id->kind)
    {
    case is_Name:
        return strlen(id->u.name_.lident_);
    case is_QualName:
        return 1 + (strlen(id->u.qualname_.uident_) + identifier_length(id->u.qualname_.identifier_));
    }
    return 0;
}

size_t count_quals(Identifier id)
{
    switch (id->kind)
    {
    case is_Name:
        return 1;
    case is_QualName:
        return count_quals(id->u.qualname_.identifier_) + 1;
    }
    return 0;
}

String show_ast_identifier(Identifier id)
{
    String result = (String)malloc(identifier_length(id));
    String tmp1, tmp2;
    size_t pos = 0;
    switch (id->kind)
    {
    case is_Name:
        tmp1 = id->u.name_.lident_;
        pos += sprintf(&result[pos], "%s", tmp1);
        break;
        
    case is_QualName:
        tmp1 = id->u.qualname_.uident_;
        tmp2 = show_ast_identifier(id->u.qualname_.identifier_);
        pos += sprintf(&result[pos], "%s.%s", tmp1, tmp2);
        break;
    }
    return result;
}

String get_ast_DLL_extern(Definition def)
{
    return def->u.extern_.identifier_->u.qualname_.uident_;
}

String get_DLL_asid(Identifier id)
{
    String tmp1, tmp2;
    switch (id->kind)
    {
    case is_Name:
        return NULL;

    case is_QualName:
        tmp1 = id->u.qualname_.uident_;
        tmp2 = get_DLL_asid(id->u.qualname_.identifier_);
        if (tmp2 == NULL)
            return tmp1;
        break;
    }
    return "";
}

String get_last_qual(Identifier id)
{
    String tmp1, *tmp2;
    switch (id->kind)
    {
    case is_Name:
        return id->u.name_.lident_;
        
    case is_QualName:
        return get_last_qual(id->u.qualname_.identifier_);
    }
    return "";
}

char *get_word_name(char *word)
{
    if (strchr(word, '.') == NULL) return word;
    String result = word;
    result = strrchr(result, '.');
    return ++result;
}

char *get_word_path(char *word)
{
    String result = word;
    rcl_asprintf(&result, "%.*s", (int)(strlen(word) - strlen(get_word_name(word)) - 1), result + 0);
    return result;
}

