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

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <tmmintrin.h>

#include <Tools\Utils\Utils.h>

bool startsWith(const String pre, const String str)
{
    if (strncmp(pre, str, strlen(str)) == 0)
        return true;
    return false;
}

String timWs(String str)
{
    String end;
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return str;
}

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

size_t levenshtein(const String s1, size_t s1len, const String s2, size_t s2len)
{
    unsigned int x, y;
    unsigned int matrix[s2len + 1][s1len + 1];
    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++)
        matrix[x][0] = matrix[x - 1][0] + 1;
    for (y = 1; y <= s1len; y++)
        matrix[0][y] = matrix[0][y - 1] + 1;
    for (x = 1; x <= s2len; x++)
        for (y = 1; y <= s1len; y++)
            matrix[x][y] = MIN3(matrix[x - 1][y] + 1, matrix[x][y - 1] + 1, matrix[x - 1][y - 1] + (s1[y - 1] == s2[x - 1] ? 0 : 1));

    return (matrix[s2len][s1len]);
}

String indexToWord(size_t n)
{
    switch (n)
    {
    case 1:
        return "first";

    case 2:
        return "second";

    case 3:
        return "third";

    case 4:
        return "fourth";

    case 5:
        return "fifth";

    case 6:
        return "sixth";

    case 7:
        return "seventh";

    case 8:
        return "eighth";

    case 9:
        return "ninth";

    case 10:
        return "tenth";

    case 11:
        return "eleventh";

    case 12:
        return "twelfth";

    case 13:
        return "thirteenth";

    case 14:
        return "fourteenth";

    case 15:
        return "fifteenth";

    case 16:
        return "sixteenth";

    case 17:
        return "seventeenth";

    case 18:
        return "eighteen";

    case 19:
        return "nineteen";

    case 20:
        return "twentieth";
    }
    String result;
    rcl_asprintf(&result, "%lldst", n);
    return result;
}

char *getline(void)
{
    char *line = malloc(100), *linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if (line == NULL)
        return NULL;

    for (;;)
    {
        c = fgetc(stdin);
        if (c == EOF)
            break;

        if (--len == 0)
        {
            len = lenmax;
            char *linen = realloc(linep, lenmax *= 2);

            if (linen == NULL)
            {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if ((*line++ = c) == '\n')
        {
            *line--;
            break;
        }
    }
    *line = '\0';
    return linep;
}

void *array_concat(const void *a, size_t an, const void *b, size_t bn, size_t s)
{
    char *p = malloc(s * (an + bn));
    memcpy(p, a, an * s);
    memcpy(p + an * s, b, bn * s);
    return p;
}

String show_version(Version version)
{
    String result;
    rcl_asprintf(&result, "%lld.%lld.%lld", version.large, version.spec, version.aspect);
    return result;
}

size_t allocated_size(void *ptr)
{
    return ((size_t *)ptr)[-1];
}

String to_upper_s(String str)
{
    String result = strdup(str);
    for (int index = 0; result[index] != '\0'; ++index)
    {
        result[index] = toupper(result[index]);
    }
    return result;
}

String to_lower_s(String str)
{
    String result = strdup(str);
    for (int index = 0; result[index] != '\0'; ++index)
    {
        result[index] = tolower(result[index]);
    }
    return result;
}

int first_digit(int n)
{
    while (n >= 10)
        n /= 10;
    return n;
}

int rcl_vasprintf(String *str_ptr, const String format, va_list ap)
{
    int len = _vscprintf(format, ap);

    if (len == -1)
        return -1;

    size_t size = (size_t)(len + 1);
    String str = malloc(size);
    if (!str)
        return -1;

    int res = vsprintf_s(str, len + 1, format, ap);
    if (res == -1)
    {
        free(str);
        return -1;
    }

    *str_ptr = str;
    return res;
}

int rcl_asprintf(String *str_ptr, const String format, ...)
{
    va_list ap;
    va_start(ap, format);
    int res = rcl_vasprintf(str_ptr, format, ap);
    va_end(ap);
    return res;
}

String rcl_sprintf_s(const String format, ...)
{
    va_list ap;
    va_start(ap, format);
    String result;
    rcl_vasprintf(&result, format, ap);
    va_end(ap);
    return result;
}

String trim(const String str)
{
    String res = strdup(str);

    char *end;

    while (isspace((unsigned char)*res))
        res++;

    if (*res == 0)
        return res;

    end = res + strlen(res) - 1;
    while (end > res && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';

    return res;
}

static void reverse(String start, String end)
{
    while (start < end)
    {
        const char c = *start;
        *start = *end;
        *end = c;
        ++start;
        --end;
    }
}

void reverse_str(String str, size_t len)
{
    for (Iterator i = len / 2 - 1; i >= 0; --i)
    {
        char c = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = c;
    }
}
