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
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>

#include <Tools\Utils\Utils.h>
#include <VM\Core\Syntax\Absyn.h>
#include <Tools\Internal\Internal.h>
#include <VM\Core\Show\Show.h>

#include <VM\Core\Wordefinition\Wordico.h>
#include <VM\Core\Wordefinition\RCL_Function.h>
#include <VM\Core\Wordefinition\RCL_Extern.h>
#include <VM\Core\Wordefinition\RCL_Lambda.h>
#include <VM\Core\Wordefinition\RCL_Structure.h>

#include <VM\Core\Checker\Typechecker\Types\Infer.h>
#include <VM\Core\Checker\Typechecker\Types\Types.h>
#include <VM\Core\Checker\Typechecker\Types\Show.h>

#include <VM\Core\Execution\Compiler\Assembler\Assemble.h>
#include <VM\Core\Execution\Compiler\Assembler\Opcode.h>
#include <VM\Core\Execution\Compiler\Assembler\Primops.h>
#include <VM\Core\Execution\Compiler\Assembler\Show.h>
#include <VM\Core\Execution\Compiler\Assembler\Hash.h>
#include <VM\Core\Execution\Interpreter\Interpreter.h>

struct Bunch init_bunch(const BResult *bresult)
{
    label_count_t lbl_count = 0;
    Lenv_map_t lenv;
    Labels result;
    OnlyStrLbl_const_lit_map_t cst_str_map;
    HashOpBloLbl_const_map_t cst_lbl_map;
    init_OnlyStrLbl_const_lit_map_t(&cst_str_map, count_operations_bykind(bresult->psdata.rcode, RCL_Value_String) + 1);
    init_HashOpBloLbl_const_map_t(&cst_lbl_map, count_operations_bykind(bresult->psdata.rcode, RCL_Value_Quotation) + bresult->wordico.functions.used + 1);
    init_Lenv_map_t(&lenv, 3);
    init_Labels(&result, 3);
    return (struct Bunch){result, bresult, lenv, lbl_count, cst_str_map, cst_lbl_map};
}

static void clean_heap(struct Bunch *bunch_ptr)
{
    if (bunch_ptr->cst_lbl_map.used > 0)
        free(bunch_ptr->cst_lbl_map.array);
    if (bunch_ptr->cst_str_map.used > 0)
        free(bunch_ptr->cst_str_map.array);
    if (bunch_ptr->lenv.used > 0)
        free(bunch_ptr->lenv.array);
}

// if is in JIT mode... //
// --dasm...

// https://www.cs.uaf.edu/2010/fall/cs301/lecture/09_20_functions_parameters.html
// https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(fontScale:14,j:1,lang:___c,selection:(endColumn:1,endLineNumber:11,positionColumn:1,positionLineNumber:11,selectionStartColumn:1,selectionStartLineNumber:11,startColumn:1,startLineNumber:11),source:'int+test(int+x,+int+y,+int+z,+int+a,+int+b)%0A%7B%0A++++return+x+%2B+y+%2B+z+%2B+a+%2B+b%3B%0A%7D%0A%0Aint+foo()%0A%7B%0A++++int+x+%3D+test(42,+8,+6,+3,+2)%3B%0A++++return+x%3B%0A%7D%0A'),l:'5',n:'0',o:'C+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:cg127,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'1',trim:'1'),fontScale:14,j:1,lang:___c,libs:!(),options:'',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'x86+gcc+1.27+(Editor+%231,+Compiler+%231)+C',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4

#define REG(x) OPCODE_VALUE_REG(x)

#define IN_JIT(bresult_ptr) bresult_ptr->exec_infos.ioc == Jited
#define IN_JIT_YES_TEST(bresult_ptr) bresult_ptr->exec_infos.ioc == Jited // =!

#define IS_OSIZE(bunch_ptr) bunch_ptr->bresult->exec_infos.osize

typedef const bool Vectorize;

static Opcode_block assemble_block(struct Bunch *, RawCode);
static Name cst_array_label(struct Bunch *, const Value);

static bool is_vzero(const Value value)
{
    if (value.kind == RCL_Value_Integer)
        return mpz_get_d(value.u.int_) == 0;

    if (value.kind == RCL_Value_Float)
        return mpf_get_d(value.u.float_) == 0;

    return false;
}

static void assemble_pure_mov(Opcode_block *block_ptr, const dest_t dest, const src_t src)
{
    if (hash_asm_value(dest) == hash_asm_value(src))
        return;
    push_Opcode_block(block_ptr, OPCODE_INSTR_MOV(dest, src));
}

// We can reduce the number of identical labels generated (This is automaticaly done for strings labels and arrays)
static Name osize_optimization__push_label(struct Bunch *bunch_ptr, const Opcode_block assembled_label, const Name lbl_name)
{
    const hash_t hash_code = hash_opcode_block(assembled_label);
    const int found_key = key_find_HashOpBloLbl_const(&bunch_ptr->cst_lbl_map, hash_code, &cmp_f_hash);
    if (found_key != map_unfound)
        return bunch_ptr->cst_lbl_map.array[found_key].val;
    add_HashOpBloLbl_const(&bunch_ptr->cst_lbl_map, hash_code, lbl_name);
    push_labels_from_oblock(&bunch_ptr->result, lbl_name, assembled_label, &bunch_ptr->lbl_count);
    return lbl_name;
}

// Create a label for string and return its name adress
static String string_label(struct Bunch *bunch_ptr, const String str)
{
    // Checks if the string value already exists
    const int found_key = key_find_OnlyStrLbl_const_lit(&bunch_ptr->cst_str_map, str, &cmp_f_str);
    if (found_key != map_unfound) // If yes, just returns its adress
        return bunch_ptr->cst_str_map.array[found_key].val;
    // Else, adds it
    const Opcode_block lbl_str = pblock_singleton(OPCODE_DIRECTIVE_ASCIZ(str));
    const Name str_lbl_adress = label_custom_name__cnt_lc(STRING_LABEL_NAME, bunch_ptr->lbl_count);
    push_labels_from_oblock(&bunch_ptr->result, str_lbl_adress, lbl_str, &bunch_ptr->lbl_count);
    // Update `cst_str_map`
    add_OnlyStrLbl_const_lit(&bunch_ptr->cst_str_map, str, str_lbl_adress);
    return str_lbl_adress;
}

// Returns the name of the created label from quotation
static Name quotation_label(const RawCode quotation_rcode, struct Bunch *bunch_ptr)
{
    if (quotation_rcode.used == 0)
        return NULL; // ==> `vq` combinator
    String lbl_name = label_custom_name__cnt_lc(QUOTATION_LABEL_NAME, bunch_ptr->lbl_count);
    Opcode_block assembled_quote;
    init_Opcode_block(&assembled_quote, quotation_rcode.used + 1);
    concat_Opcode_block(&assembled_quote, assemble_block(bunch_ptr, quotation_rcode));
    retit(&assembled_quote);
    return osize_optimization__push_label(bunch_ptr, assembled_quote, lbl_name);
}

// In the context of an array, if one of the boxes is an expression of the style (3 + 1, "abc" ++ "def", ...)
// this function allows to interpret the result of the expression, and modifies the pointer sent accordingly.
// If there are errors, such as an unsuitable expression or a type not respected, then throws an exception (this should not happen).
static void cst_array__seval_case(RawCode *result_ptr, const RawCode exp_case, struct Bunch *bunch_ptr, const enum Value_Kind expected_kind)
{
    *result_ptr = seval(exp_case, bunch_ptr->bresult);

    if (result_ptr->used > 1)
    {
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Non-constant or evaluable array case result: `%s`.", show_quote(*result_ptr));
        return exit(EXIT_FAILURE);
    }
    if (result_ptr->array[0].kind != expected_kind)
    {
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Non-conform type array of case result: `%s`.", show_quote(*result_ptr));
        return exit(EXIT_FAILURE);
    }
}

static void cst_array_label__assemble_ofint(Opcode_block *block_ptr, const Value value, struct Bunch *bunch_ptr)
{
    for (Iterator i = 0; i < value.u.array_.length; i++)
    {
        if (RCL_ARRAY_CONTENT_CASE(value, i)->used == 1)
        {
            push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_LONG((double)mpz_get_d(RCL_ARRAY_CONTENT_CASE(value, i)->array[0].u.int_)));
            continue;
        }
        RawCode case_evaluated_res =
            {RCL_ARRAY_CONTENT_CASE(value, i)->array, RCL_ARRAY_CONTENT_CASE(value, i)->used, RCL_ARRAY_CONTENT_CASE(value, i)->size};
        cst_array__seval_case(&case_evaluated_res, case_evaluated_res, bunch_ptr, RCL_Value_Integer);
        push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_LONG((double)mpz_get_d(case_evaluated_res.array[0].u.int_)));
    }
}

static void cst_array_label__assemble_ofstring(Opcode_block *block_ptr, const Value value, struct Bunch *bunch_ptr)
{
    for (Iterator i = 0; i < value.u.array_.length; i++)
    {
        if (RCL_ARRAY_CONTENT_CASE(value, i)->used == 1)
        {
            const Name curr_str_lbl_id = string_label(bunch_ptr, RCL_ARRAY_CONTENT_CASE(value, i)->array[0].u.string_);
            push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_QUAD(OPCODE_VALUE_LABEL(curr_str_lbl_id)));
            continue;
        }
        RawCode case_evaluated_res =
            {RCL_ARRAY_CONTENT_CASE(value, i)->array, RCL_ARRAY_CONTENT_CASE(value, i)->used, RCL_ARRAY_CONTENT_CASE(value, i)->size};
        cst_array__seval_case(&case_evaluated_res, case_evaluated_res, bunch_ptr, RCL_Value_String);
        const Name curr_str_lbl_id = string_label(bunch_ptr, case_evaluated_res.array[0].u.string_);
        push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_QUAD(OPCODE_VALUE_LABEL(curr_str_lbl_id)));
    }
}

static void cst_array_label__assemble_ofarray(Opcode_block *block_ptr, const Value value, struct Bunch *bunch_ptr)
{
    for (Iterator i = 0; i < value.u.array_.length; i++)
    {
        if (RCL_ARRAY_CONTENT_CASE(value, i)->used == 1)
        {
            const Name curr_carray_lbl_id = cst_array_label(bunch_ptr, RCL_ARRAY_CONTENT_CASE(value, i)->array[0]);
            push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_QUAD(OPCODE_VALUE_LABEL(curr_carray_lbl_id)));
            continue;
        }
        RawCode case_evaluated_res =
            {RCL_ARRAY_CONTENT_CASE(value, i)->array, RCL_ARRAY_CONTENT_CASE(value, i)->used, RCL_ARRAY_CONTENT_CASE(value, i)->size};
        cst_array__seval_case(&case_evaluated_res, case_evaluated_res, bunch_ptr, RCL_Value_Array);
        const Name curr_carray_lbl_id = cst_array_label(bunch_ptr, case_evaluated_res.array[0]);
        push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_QUAD(OPCODE_VALUE_LABEL(curr_carray_lbl_id)));
    }
}

// (I think quotations' arrays could be very useful and efficient for jump table)
static void cst_array_label_assemble_ofquotation(Opcode_block *block_ptr, const Value value, struct Bunch *bunch_ptr)
{
    for (Iterator i = 0; i < value.u.array_.length; i++)
    {
        if (RCL_ARRAY_CONTENT_CASE(value, i)->used == 1)
        {
            const Name curr_quote_lbl_id = quotation_label(*RCL_ARRAY_CONTENT_CASE(value, i)->array[0].u.quote_, bunch_ptr);
            push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_QUAD(OPCODE_VALUE_LABEL(curr_quote_lbl_id)));
            continue;
        }
        RawCode case_evaluated_res =
            {RCL_ARRAY_CONTENT_CASE(value, i)->array, RCL_ARRAY_CONTENT_CASE(value, i)->used, RCL_ARRAY_CONTENT_CASE(value, i)->size};
        cst_array__seval_case(&case_evaluated_res, case_evaluated_res, bunch_ptr, RCL_Value_Quotation);
        const Name curr_quote_lbl_id = quotation_label(*case_evaluated_res.array[0].u.quote_, bunch_ptr);
        push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_QUAD(OPCODE_VALUE_LABEL(curr_quote_lbl_id)));
    }
}

static void cst_array_label__assemble_ofchar(Opcode_block *block_ptr, const Value value, struct Bunch *bunch_ptr)
{
    // An array of char is actualy a string without '\0'
    String result = malloc(value.u.array_.length * sizeof(char));
    for (Iterator i = 0; i < value.u.array_.length; i++)
    {
        if (RCL_ARRAY_CONTENT_CASE(value, i)->used == 1)
        {
            result[i] = RCL_ARRAY_CONTENT_CASE(value, i)->array[0].u.char_;
        }
        else
        {
            RawCode case_evaluated_res =
                {RCL_ARRAY_CONTENT_CASE(value, i)->array, RCL_ARRAY_CONTENT_CASE(value, i)->used, RCL_ARRAY_CONTENT_CASE(value, i)->size};
            cst_array__seval_case(&case_evaluated_res, case_evaluated_res, bunch_ptr, RCL_Value_Char);
            result[i] = case_evaluated_res.array[0].u.char_;
        }
    }
    /**** TODO
    if (result && *result && result[strlen(result) - 1] == '\0')
        push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_ASCIZ(result));
    else */
    push_Opcode_block(block_ptr, OPCODE_DIRECTIVE_ASCII(result++));
}

// Returns the name of the created label from constant array
// Expected array example: {8, 3, 2, 6, 7}
//                     or  {3 2 +, 7 6 9 * -}
static Name cst_array_label(struct Bunch *bunch_ptr, const Value value)
{
    if (value.u.array_.length == 0)
        return NULL;

    String lbl_name = label_custom_name__cnt_lc(CST_ARRAY_LABEL_NAME, bunch_ptr->lbl_count);
    Opcode_block assembled_array;
    init_Opcode_block(&assembled_array, value.u.array_.length);

    // Type of array, passer par paramètre
    switch (RCL_ARRAY_CONTENT_CASE(value, 0)->array[0].kind)
    {
    case RCL_Value_Integer:
        cst_array_label__assemble_ofint(&assembled_array, value, bunch_ptr);
        break;

    case RCL_Value_Char:
        cst_array_label__assemble_ofchar(&assembled_array, value, bunch_ptr);
        break;

    case RCL_Value_String:
        cst_array_label__assemble_ofstring(&assembled_array, value, bunch_ptr);
        break;

    case RCL_Value_Array:
        cst_array_label__assemble_ofarray(&assembled_array, value, bunch_ptr);
        break;

    case RCL_Value_Quotation:
        cst_array_label_assemble_ofquotation(&assembled_array, value, bunch_ptr);
        break;

    default:
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Unarrayable data kind: %s.", show_kind(RCL_ARRAY_CONTENT_CASE(value, 0)->array[0].kind));
    }
    return osize_optimization__push_label(bunch_ptr, assembled_array, lbl_name);
}

static ASM_Value assemble_structure(struct RCL_Value_DataStruct _struct, RCL_Value_Field_t field, bool is_jit, struct Bunch *bunch_ptr)
{
    if (is_jit)
        return OPCODE_VALUE_ADRESS_RAW(field);
    switch (field->kind)
    {
    case _is_Enum:
        /*         const int index = field_index(*_struct.template, field->u.enumfield_.lident_);
        return OPCODE_VALUE_NUM(index); */
        // Il faudra se servir dans le bunch
        // Pour le reste, en fait field n'est qu'un template: tout seul il est inutile (sauf case ENUM)
        // Il faudra faire autre chose que cette fonction, ou juste l'adapter pour le cas ENUM
        break;

    default:
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Can't handle structure field.", NULL);
    }
}

static ASM_Value assemble_value__int(const mpz_t n, const bool is_in_jit)
{
    if (!is_in_jit)
        return OPCODE_VALUE_NUM((int)mpz_get_d(n));
    return OPCODE_VALUE_ADRESS_RAW(n);
}

static ASM_Value assemble_value__float(const mpf_t n, const bool is_in_jit)
{
    if (!is_in_jit)
        return OPCODE_VALUE_NUM((double)mpf_get_d(n));
    return OPCODE_VALUE_ADRESS_RAW(n);
}

static ASM_Value assemble_value__char(const char c)
{
    return OPCODE_VALUE_NUM((double)c);
}

static ASM_Value assemble_value__string(String *str_ptr, const bool is_in_jit, struct Bunch *bunch_ptr)
{
    if (is_in_jit)
        return OPCODE_VALUE_ADRESS_RAW(str_ptr);
    const Name str_lbl_adress = string_label(bunch_ptr, *str_ptr);
    return OPCODE_VALUE_LABEL(str_lbl_adress);
}

static ASM_Value assemble_value__quote(RawCode *quote_ptr, const bool is_in_jit, struct Bunch *bunch_ptr)
{
    if (is_in_jit)
        return OPCODE_VALUE_ADRESS_RAW(quote_ptr);
    const Name quote_lbl_adress = quotation_label(*quote_ptr, bunch_ptr);
    return OPCODE_VALUE_LABEL(quote_lbl_adress);
}

static ASM_Value assemble_value__array(RCL_Value_Array_t *array_ptr, const bool is_in_jit, struct Bunch *bunch_ptr)
{
    if (is_in_jit)
        return OPCODE_VALUE_ADRESS_RAW(array_ptr);
    const Name array_lbl_adress = cst_array_label(bunch_ptr, make_RCL_Value_Array(array_ptr->array, array_ptr->length));
    return OPCODE_VALUE_LABEL(array_lbl_adress);
}

static ASM_Value assemble_value(struct Bunch *bunch_ptr, Value *value_ptr)
{
    const bool is_jit = IN_JIT_YES_TEST(bunch_ptr->bresult);
    switch (value_ptr->kind)
    {
    case RCL_Value_Integer:
        return assemble_value__int(value_ptr->u.int_, is_jit);

    case RCL_Value_Float:
        return assemble_value__float(value_ptr->u.float_, is_jit);

    case RCL_Value_Char:
        return assemble_value__char(value_ptr->u.char_);

    case RCL_Value_String:
        return assemble_value__string(&value_ptr->u.string_, is_jit, bunch_ptr);

    case RCL_Value_Quotation:
        return assemble_value__quote(value_ptr->u.quote_, is_jit, bunch_ptr);

    case RCL_Value_Array:
        return assemble_value__array(&value_ptr->u.array_, is_jit, bunch_ptr);

        //case RCL_Value_Field:

    default:
        _internal_error(__FILE__, __LINE__, __FUNCTION_NAME__, "Not handled value: %s.", show_value(*value_ptr));
        exit(EXIT_FAILURE);
    }
}

static Opcode_block assemble_combinator(struct Bunch *bunch_ptr, const Combinator combinator)
{
}

static Opcode_block assembly__swap_reg(const Register r1, const Register r2)
{
    return pblock_create_assemble(
        3,
        OPCODE_INSTR_PUSH(OPCODE_VALUE_ADRESS_REG(REG(r1))),
        OPCODE_INSTR_MOV(OPCODE_VALUE_ADRESS_REG(REG(r1)), OPCODE_VALUE_ADRESS_REG(REG(r2))),
        OPCODE_INSTR_POP(OPCODE_VALUE_ADRESS_REG(REG(r2))));
}

// Is called when a division is performed
static void assembly_exception__div_by_zero(Opcode_block *block_ptr, struct Bunch *bunch_ptr)
{
}

static ASM_Instr assembly_push_value(struct Bunch *bunch_ptr, Value *value)
{
    return OPCODE_INSTR_PUSH(assemble_value(bunch_ptr, value));
}

static ASM_Instr assembly_pop_value(struct Bunch *bunch_ptr, Value *value)
{
    return OPCODE_INSTR_POP(assemble_value(bunch_ptr, value));
}

static ASM_Instr assembly_mov_value(struct Bunch *bunch_ptr, const dest_t asm_dest, Value *value)
{
    // Micro optimization:
    if (is_vzero(*value)) // mov REG, 0   <=>   xor REG, REG
        return OPCODE_INSTR_XOR(asm_dest, asm_dest);
    return OPCODE_INSTR_MOV(asm_dest, assemble_value(bunch_ptr, value));
}

static ASM_Instr assembly_cmp_value(struct Bunch *bunch_ptr, const dest_t asm_dest, Value *value)
{
    // Micro optimization:
    if (is_vzero(*value))
        return OPCODE_INSTR_EMPTY();
    return OPCODE_INSTR_CMP(asm_dest, assemble_value(bunch_ptr, value));
}

static ASM_Instr assembly_lea_value(struct Bunch *bunch_ptr, const dest_t asm_dest, Value *value)
{
    return OPCODE_INSTR_LEA(asm_dest, assemble_value(bunch_ptr, value));
}

static ASM_Instr assembly_test_value(struct Bunch *bunch_ptr, const dest_t asm_dest, Value *value)
{
    return OPCODE_INSTR_TEST(asm_dest, assemble_value(bunch_ptr, value));
}

// Fonctions Add_reg_val
//  par exemple `assembly_cmp_int`
//              `assembly_cmp_float`
//              `assembly_cmp_quote`
//              `assembly_cmp_string`

// Faire de l'arithmétique dans les registres
// Par exemple [edx * 4 + 3]

static ASM_Instr assembly_add_value(struct Bunch *bunch_ptr, const dest_t asm_dest, Value *value)
{
    return OPCODE_INSTR_ADD(asm_dest, assemble_value(bunch_ptr, value));
}

// http://www.nynaeve.net/?p=64
// http://mark.masmcode.com/

/*

Il faut tout passer par pointeur en fait
=> Augmente les performances
=> Permet de gérer le JIT

*/

// Modulo operation of two RCL IR values
// (`arithfi` = Arithmetic Function Integers)
static Opcode_block assemble_arithfi__mod(struct Bunch *bunch_ptr, Value *v1_ptr, Value *v2_ptr)
{
    const Opcode_block mod_f =
        pblock_create_assemble(
            5,
            OPCODE_INSTR_MOV(_EAX, _EDI),
            OPCODE_INSTR_CDQ(),
            OPCODE_INSTR_DIV(_ESI),
            OPCODE_INSTR_MOV(_EAX, _EDX),
            OPCODE_INSTR_RET());
    const hash_t hash_code = hash_opcode_block(mod_f);
    const int found_key = key_find_HashOpBloLbl_const(&bunch_ptr->cst_lbl_map, hash_code, &cmp_f_hash);
    if (found_key == map_unfound)
        push_labels_from_oblock(&bunch_ptr->result, "__rcl_arithfi_mod", mod_f, &bunch_ptr->lbl_count);
    Opcode_block result;
    init_Opcode_block(&result, 3);
    push_Opcode_block(&result, assembly_mov_value(bunch_ptr, _ESI, v2_ptr));
    push_Opcode_block(&result, assembly_mov_value(bunch_ptr, _EDI, v1_ptr));
    push_Opcode_block(&result, OPCODE_INSTR_JMP(OPCODE_VALUE_LABEL("__rcl_arithfi_mod")));
    return result;
}

unsigned int count_set_bits(int n)
{
    n = ((0xaaaaaaaa & n) >> 1) + (0x55555555 & n);
    n = ((0xcccccccc & n) >> 2) + (0x33333333 & n);
    n = ((0xf0f0f0f0 & n) >> 4) + (0x0f0f0f0f & n);
    n = ((0xff00ff00 & n) >> 8) + (0x00ff00ff & n);
    n = ((0xffff0000 & n) >> 16) + (0x0000ffff & n);
    return n;
}

bool is_power_of2(int x)
{
    return x && (!(x & (x - 1)));
}

static Opcode_block assemble_imul(struct Bunch *bunch_ptr, dest_t asm_dest, const int n)
{
    return pblock_singleton(OPCODE_INSTR_IMUM(asm_dest, OPCODE_VALUE_NUM(n)));
}

static Opcode_block assemble_div_cst(struct Bunch *bunch_ptr, dest_t asm_dest, const Value value)
{
    // if in jit...
    // if float...
    // if div by 0...

    // https://www.agner.org/optimize/optimizing_assembly.pdf#page=147&zoom=100,116,697

    Opcode_block result;

    const int cst_nbr = mpz_get_d(value.u.int_);

    if (cst_nbr == 0)
    {
        state_put(&bunch_ptr->bresult->state, make_warning(Compiler, "Divising by constant zero!"));
        // attention division par zero
        init_Opcode_block(&result, 1);
        push_Opcode_block(&result, OPCODE_INSTR_XOR(asm_dest, asm_dest));
    }
    else if (is_power_of2(cst_nbr))
    {
        // If number to divide is unsigned:
        int shrb = log2(cst_nbr);
        init_Opcode_block(&result, 1);
        push_Opcode_block(&result, OPCODE_INSTR_SHR(asm_dest, OPCODE_VALUE_NUM(shrb)));
        // If number to divide is signed: TODO
    }
    else // Divide by unsigned
    {
        const int b = count_set_bits(cst_nbr);
        const int w = 32;
        const int r = w + b;
        double f = pow(2, r) / cst_nbr;

        init_Opcode_block(&result, 3);

        if (ceilf(f) == f)
        {
            int shrb = log2(cst_nbr);
            push_Opcode_block(&result, OPCODE_INSTR_SHR(asm_dest, OPCODE_VALUE_NUM(shrb)));
        }
        else
        {
            assemble_pure_mov(&result, _EAX, asm_dest);
            if ((f - floor(f)) > 0.5)
            {
                f = round(f);
            }
            else
            {
                f = floor(f);
                push_Opcode_block(&result, OPCODE_INSTR_ADD(asm_dest, OPCODE_VALUE_NUM(1)));
            }
            push_Opcode_block(&result, OPCODE_INSTR_MOV(_EDX, OPCODE_VALUE_NUM((int)f)));
            push_Opcode_block(&result, OPCODE_INSTR_MUL(_EDX));
            push_Opcode_block(&result, OPCODE_INSTR_SHR(_EDX, OPCODE_VALUE_NUM(r)));
            assemble_pure_mov(&result, asm_dest, _EAX);
        }
    }
    return result;
}

static void assemble_combinator__stack__dup(struct Bunch *bunch_ptr, Opcode_block *block_ptr, Value *value_ptr)
{
    // We just have to push 2 time the value onto the stack
    push_Opcode_block(block_ptr, assembly_push_value(bunch_ptr, value_ptr));
    push_Opcode_block(block_ptr, assembly_push_value(bunch_ptr, value_ptr));
}

static void assemble_combinator__stack__multi_dup(struct Bunch *bunch_ptr, Opcode_block *block_ptr, Value *value_ptr, size_t time)
{
    while (time--)
        push_Opcode_block(block_ptr, assembly_push_value(bunch_ptr, value_ptr));
}

static Opcode_block assemble_block(struct Bunch *bunch_ptr, const RawCode rcode_ptr)
{
    Opcode_block result;
    init_Opcode_block(&result, rcode_ptr.used * 2);

    for (Iterator i = 0; i < rcode_ptr.used; i++)
    {
        const Value *current = &rcode_ptr.array[i];
        /*         const RCL_Type delt = type_of(current, ENTRY_NAME, bunch_ptr->bresult);
        const size_t currho = arity(delt); */

        const size_t currho = 0;

        if (current->kind == RCL_Value_Quotation)
            inc_lc(&bunch_ptr->lbl_count);

        if (currho == 0) // Not a relationship operation
                         // + Créer un déducteur de complexité (algorithmique) de fonction
            //concat_Opcode_block(&result, pblock_singleton(assembly_test_value(bunch_ptr, _EAX, current)));
            concat_Opcode_block(&result, assemble_div_cst(bunch_ptr, _EAX, *current));
    }

    return result;
}

static void assemble_function(struct Bunch *bunch_ptr, Labels *labels_res_ptr, const String name, const String desc, RawCode *rcode_ptr)
{
    Opcode_block body = assemble_block(bunch_ptr, *rcode_ptr);
    concat_Opcode_block(&body, pblock_singleton(OPCODE_INSTR_RET()));
    if (name == NULL)
        push_labels_from_oblock(labels_res_ptr, name, body, NULL);
    push_labels_from_oblock_desc(labels_res_ptr, name, body, desc, NULL);
}

static void cgen(const RawCode rcode, struct Bunch *bunch_ptr)
{
    Opcode_block main_block;
    init_Opcode_block(&main_block, 1);

    const size_t msize = bunch_ptr->bresult->psdata.rcode.used;
    size_t i = msize;

    for (Iterator i = 0; i < bunch_ptr->bresult->wordico.functions.used; i++)
    {
        struct RCL_Function current = bunch_ptr->bresult->wordico.functions.array[i];
        assemble_function(bunch_ptr, &bunch_ptr->result, current.name, NULL, &current.body);
    }
    assemble_function(bunch_ptr, &bunch_ptr->result, ENTRY_NAME, "This is the entry point", &bunch_ptr->bresult->psdata.rcode);
}

static void *th_assemble_externs(struct VEC_Externs * externs) {}
static void *th_assemble_structure(struct VEC_Structures * structures) {}
static void *th_assemble_functions(struct VEC_Functions * functions) {}

static Labels assemble_main(RawCode * main, struct Wordico *wordico, label_count_t *lbl_count)
{
    Labels result;
    init_Labels(&result, count_operations_bykind(*main, RCL_Value_Quotation + 1));

    return result;
}

Assembled_Program assemble(const BResult * bresult)
{
    pthread_t
        th_fn, // Assemble functions
        th_ex, // Assemble external functions
        th_st; // Build data structures in memory

    /*     pthread_create(&th_fn, NULL, (void *(*)(void *))th_assemble_functions, ((void *)(&bresult->wordico.functions)));
    pthread_create(&th_ex, NULL, (void *(*)(void *))th_assemble_externs, ((void *)(&bresult->wordico.externs)));
    pthread_create(&th_st, NULL, (void *(*)(void *))th_assemble_structure, ((void *)(&bresult->wordico.structures)));
    
    pthread_join(th_fn, NULL);
    pthread_join(th_ex, NULL);
    pthread_join(th_st, NULL); */

    struct Bunch bunch = init_bunch(bresult);

    cgen(bresult->psdata.rcode, &bunch);

    /*     for (Iterator i = 0; i < bunch.result.used; i++)
        printf("%u\n", hash_label(bunch.result.array[i])); */

    /*     cgen_jit_primitive__addi(&result);
    cgen_jit_primitive__subi(&result);
    cgen_jit_primitive__muli(&result);
    cgen_jit_primitive__divi(&result); */

    clean_heap(&bunch);
    return opcode_make_assembled_program(bunch.result, ENTRY_NAME, NULL);
}
