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
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include <VM\Core\Browse\BrowsedAbsyn.h>
#include <VM\Core\Browse\Browse.h>
#include <VM\Core\Browse\BResult.h>

#include <VM\Core\RawCode\RawCode.h>
#include <VM\Core\Syntax\Absyn.h>
#include <VM\Core\RawCode\Combinator\Combinators.h>
#include <VM\Core\FFI\Types.h>
#include <VM\Core\FFI\Types.h>
#include <VM\Core\FFI\RCL_File.h>
#include <VM\Core\Syntax\Identifier\Functions.h>
#include <VM\Core\Wordefinition\Wordico.h>
#include <VM\Core\Wordefinition\Sort_hash\Sort_hash.h>
#include <VM\Core\Preprocessor\Import.h>
#include <VM\Core\Preprocessor\Defines.h>
#include <VM\Core\Show\Show.h>
#include <VM\Core\State\State.h>
#include <VM\Core\Optimize\Optimizer.h>

void browsePVoid(BResult *);          // An empty program
void browseProg1(Program, BResult *); // Idata + definitions + code
void browseProg2(Program, BResult *); // Idata + definitions
void browseProg3(Program, BResult *); // Idata + code
void browseProg4(Program, BResult *); // Definitions + code
void browseProg5(Program, BResult *); // Definitions
void browseProg6(Program, BResult *); // Code (with section '.code' indicated)
void browseProg7(Program, BResult *); // Simple code
void browseProg8(Program, BResult *); // Simple preprocessor

void handle_absynInclude(BResult *, Preprocessor);
void handle_absynImportAs(BResult *, Preprocessor);
void handle_absynImport(BResult *, Preprocessor);
void handle_absynDefine(BResult *, Preprocessor);

int canHandleLiteralOp(RawCode, LiteralOperation);
int rcodeContainsEOSV(RawCode *, char *);

size_t count_includes(ListPreprocessor);
size_t count_imports(ListPreprocessor);

struct Code_handler
{
    BResult *bresult;
    ListOperation lo;
} __attribute__((packed));

struct Definition_handler
{
    struct Wordico *wordico;
    BrowsedAbsyn *absyn;
    ListDefinition ld;
} __attribute__((packed));

struct Preprocessor_handler
{
    BResult *bresult;
    ListPreprocessor lp;
} __attribute__((packed));

void handle_code(struct Code_handler *);
void handle_definitions(struct Definition_handler *);
void handle_preprocessor(struct Preprocessor_handler *);

void handle_word(BResult *, String);
void handle_lambda(BResult *, String);
void handle_endlambda(BResult *, String);

void case_operation(BResult *, Operation);
void case_preprocessor(BResult *, Preprocessor p);

BResult browseAbsyn(Program prog, String filename)
{
    BResult bresult;
    bresult.current_name = NULL;

    rcl_asprintf(&bresult.current_filename, "%s", filename);

    switch (prog->kind)
    {
    case is_PVoid:
        browsePVoid(&bresult);
        break;

    case is_Prog1:
        browseProg1(prog, &bresult);
        break;

    case is_Prog2:
        browseProg2(prog, &bresult);
        break;

    case is_Prog3:
        browseProg3(prog, &bresult);
        break;

    case is_Prog4:
        browseProg4(prog, &bresult);
        break;

    case is_Prog5:
        browseProg5(prog, &bresult);
        break;

    case is_Prog6:
        browseProg6(prog, &bresult);
        break;

    case is_Prog7:
        browseProg7(prog, &bresult);
        break;

    case is_Prog8:
        browseProg8(prog, &bresult);
        break;
    }

    pthread_t t1, t2, t3;

    /*
    * Sorting the definitions (functions, external and structures) according to their hash code will then make it possible
    * to be more efficient when searching for one of the elements, using in particular the binary search algorithm ( O(log n) ),
    * so that if we have for example 20,000 functions, it will only be sufficient for a maximum of 14 search iterations,
    * instead of 20,000 with a linear search.
    * The difference is really significant.
    */

    pthread_create(&t1, NULL, (void *(*)(void *))sort_functions, (void *)&bresult.wordico.functions);
    pthread_create(&t2, NULL, (void *(*)(void *))sort_externs, (void *)&bresult.wordico.externs);
    pthread_create(&t3, NULL, (void *(*)(void *))sort_structures, (void *)&bresult.wordico.structures);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return bresult;
}

void browsePVoid(BResult * bresult)
{
    state_init(&bresult->state);
    init_wordico_nbr(&bresult->wordico, 1);
    init_rcode(&bresult->psdata.rcode, 1);
    init_imports__(&bresult->psdata.imports, 1);
    init_includes__(&bresult->psdata.includes, 1);
    state_put_warn_br("`%s' is an empty program (the file actually no contains any code).", bresult->current_filename);
}

void browseProg1(const Program prog, BResult * bresult)
{
    ListPreprocessor lp = prog->u.prog1_.listpreprocessor_;
    ListDefinition ld = prog->u.prog1_.listdefinition_;
    ListOperation lo = prog->u.prog1_.operation_->u.concatenation_.listoperation_;

    struct Preprocessor_handler idata_h;
    struct Definition_handler def_h;
    struct Code_handler code_h;

    state_init(&bresult->state);
    init_wordico(&bresult->wordico, ld);

    idata_h.bresult = bresult;
    idata_h.lp = lp;

    handle_preprocessor(&idata_h);

    def_h.wordico = &bresult->wordico;
    def_h.absyn = &bresult->psdata;
    def_h.ld = ld;

    pthread_t idata_th;
    pthread_create(&idata_th, NULL, (void *(*)(void *))handle_preprocessor, (void *)&idata_h);

    pthread_t def_th;
    pthread_create(&def_th, NULL, (void *(*)(void *))handle_definitions, (void *)&def_h);

    code_h.bresult = bresult;
    code_h.lo = lo;

    handle_code(&code_h);

    pthread_join(idata_th, NULL);
    pthread_join(def_th, NULL);
}

void browseProg2(Program prog, BResult * bresult)
{
    ListPreprocessor lp = prog->u.prog2_.listpreprocessor_;
    ListDefinition ld = prog->u.prog2_.listdefinition_;

    struct Preprocessor_handler idata_h;
    struct Definition_handler def_h;

    state_init(&bresult->state);
    init_wordico(&bresult->wordico, ld);
    init_rcode(&bresult->psdata.rcode, 1);

    idata_h.bresult = bresult;
    idata_h.lp = lp;

    pthread_t idata_th;
    pthread_create(&idata_th, NULL, (void *(*)(void *))handle_preprocessor, (void *)&idata_h);

    def_h.wordico = &bresult->wordico;
    def_h.absyn = &bresult->psdata;
    def_h.ld = ld;

    handle_definitions(&def_h);

    pthread_join(idata_th, NULL);
}

void browseProg3(Program prog, BResult * bresult)
{
    ListPreprocessor lp = prog->u.prog3_.listpreprocessor_;
    ListOperation lo = prog->u.prog3_.operation_->u.concatenation_.listoperation_;

    struct Preprocessor_handler idata_h;
    struct Code_handler code_h;

    state_init(&bresult->state);
    init_rcode(&bresult->psdata.rcode, 1);
    init_wordico_nbr(&bresult->wordico, 1);

    idata_h.bresult = bresult;
    idata_h.lp = lp;

    pthread_t idata_th;
    pthread_create(&idata_th, NULL, (void *(*)(void *))handle_preprocessor, (void *)&idata_h);

    code_h.bresult = bresult;
    code_h.lo = lo;

    handle_code(&code_h);

    pthread_join(idata_th, NULL);
}

void browseProg4(Program prog, BResult * bresult)
{
    ListDefinition ld = prog->u.prog4_.listdefinition_;
    ListOperation lo = prog->u.prog4_.operation_->u.concatenation_.listoperation_;

    struct Definition_handler def_h;
    struct Code_handler code_h;

    state_init(&bresult->state);
    init_wordico(&bresult->wordico, ld);
    init_rcode(&bresult->psdata.rcode, 1);

    def_h.wordico = &bresult->wordico;
    def_h.absyn = &bresult->psdata;
    def_h.ld = ld;

    pthread_t def_th;
    pthread_create(&def_th, NULL, (void *(*)(void *))handle_definitions, (void *)&def_h);

    code_h.bresult = bresult;
    code_h.lo = lo;

    handle_code(&code_h);

    pthread_join(def_th, NULL);
}

void browseProg5(Program prog, BResult * bresult)
{
    ListDefinition ld = prog->u.prog5_.listdefinition_;

    struct Definition_handler def_h;

    state_init(&bresult->state);
    init_wordico(&bresult->wordico, ld);
    init_rcode(&bresult->psdata.rcode, 1);

    def_h.wordico = &bresult->wordico;
    def_h.absyn = &bresult->psdata;
    def_h.ld = ld;

    handle_definitions(&def_h);
}

void browseProg6(Program prog, BResult * bresult)
{
    ListOperation lo = prog->u.prog6_.operation_->u.concatenation_.listoperation_;

    struct Code_handler code_h;

    state_init(&bresult->state);
    init_wordico_nbr(&bresult->wordico, 1);

    code_h.bresult = bresult;
    code_h.lo = lo;

    handle_code(&code_h);
}

void browseProg7(Program prog, BResult * bresult)
{
    ListOperation lo = prog->u.prog7_.operation_->u.concatenation_.listoperation_;

    struct Code_handler code_h;

    state_init(&bresult->state);
    init_wordico_nbr(&bresult->wordico, 1);

    code_h.bresult = bresult;
    code_h.lo = lo;

    handle_code(&code_h);
}

void browseProg8(Program prog, BResult * bresult)
{
    ListPreprocessor lp = prog->u.prog8_.listpreprocessor_;

    struct Preprocessor_handler idata_h;

    state_init(&bresult->state);
    init_wordico_nbr(&bresult->wordico, 1);
    init_rcode(&bresult->psdata.rcode, 1);

    idata_h.bresult = bresult;
    idata_h.lp = lp;

    handle_preprocessor(&idata_h);
}

void handle_code(struct Code_handler *code_h)
{
    init_rcode(&code_h->bresult->psdata.rcode, count_operations(code_h->lo));
    while (code_h->lo != NULL)
    {
        case_operation(code_h->bresult, code_h->lo->operation_);
        code_h->lo = code_h->lo->listoperation_;
    }
}

void _handle_word(BResult * bresult, String word)
{
    if (is_combinator(word))
        return push_rcode(&bresult->psdata.rcode, make_RCL_Value_Combinator(str_to_comb(word)));
    push_rcode(&bresult->psdata.rcode, make_RCL_Value_Word(word));
}

void _handle_lambda(BResult * bresult, String name)
{
    if (is_combinator(name))
    {
        state_put_info_br("In function `%s': ", S_CURRENTF);
        state_put_err_br("`%s' is used to initialize a lambda, but the chosen name is a combinator identifier.", name);
        return;
    }

    push_rcode(&bresult->psdata.rcode, make_RCL_Value_Lambda(name));
}

void _handle_endlambda(BResult * bresult, String name)
{
    if (is_combinator(name))
    {
        state_put_info_br("In function `%s': ", S_CURRENTF);
        state_put_err_br("`%s' is used to end the scope of a lambda, but the given lambda name is a combinator identifier", name);
        return;
    }

    push_rcode(&bresult->psdata.rcode, make_RCL_Value_EndLamScope(name));
}

void case_operation(BResult * bresult, const Operation op)
{
    switch (op->kind)
    {
    case is_Var:
        _handle_word(bresult, show_ast_identifier(op->u.var_.identifier_));
        break;

    case is_Lambda:
        _handle_lambda(bresult, op->u.lambda_.lident_);
        break;

    case is_EndLambdaScope:
        _handle_endlambda(bresult, op->u.endlambdascope_.lident_);
        break;

    default:
        push_rcode(&bresult->psdata.rcode, otov(op));
        break;
    }
}

void handle_definitions(struct Definition_handler *def_h)
{
    set_wordico(def_h->wordico, def_h->absyn, def_h->ld);
}

void handle_preprocessor(struct Preprocessor_handler *idata_h)
{
    const int nbr_imports = count_imports(idata_h->lp);
    const int nbr_includes = count_includes(idata_h->lp);

    if (nbr_imports)
    {
        init_imports__(&idata_h->bresult->psdata.imports, nbr_imports);
    }
    if (nbr_includes)
    {
        init_includes__(&idata_h->bresult->psdata.includes, nbr_includes);
    }

    while (idata_h->lp != NULL)
    {
        case_preprocessor(idata_h->bresult, idata_h->lp->preprocessor_);
        idata_h->lp = idata_h->lp->listpreprocessor_;
    }
}

void case_preprocessor(BResult * bresult, Preprocessor p)
{
    switch (p->kind)
    {
    case is_Warning:
        state_put(&bresult->state, make_warning(Browser, p->u.warning_.string_));
        break;

    case is_Info:
        state_put(&bresult->state, make_info(Browser, p->u.info_.string_));
        break;

    case is_Error:
        state_put(&bresult->state, make_error(Browser, p->u.error_.string_));
        break;

    case is_FaultInt:
        printf("\n\t* Code Fault:   %d\n", p->u.faultint_.integer_);
        break;

    case is_FaultId:
        printf("\n\t* Code Fault:   %s\n", p->u.faultid_.uident_);
        break;

    case is_Import:
        handle_absynImport(bresult, p);
        break;

    case is_Include:
        handle_absynInclude(bresult, p);
        break;

    case is_Define:
        handle_absynDefine(bresult, p);
        break;

    default:
        _interr("Unknown preprocessor kind: %d.", p->kind);
    }
}

void handle_absynImport(BResult * bresult, Preprocessor p)
{
    imports(bresult, p);
}

void handle_absynImportAs(BResult * bresult, Preprocessor p)
{
    perror("TO4DO!");
    exit(1);
}

#define EXEC bresult->exec_infos
#define IFDEF(val, then) if (def_hash == hash_djb2(OPT val)) { then; }

void handle_absynDefine(BResult * bresult, Preprocessor p)
{
    const String def_name = to_lower_s(p->u.define_.uident_);
    const hash_t def_hash = hash_djb2(def_name);

         IFDEF("strict", EXEC.sool = Strict)
    else IFDEF("optimistic", EXEC.sool = Optimistic)
    else IFDEF("lazy", EXEC.sool = Lazy)
    else IFDEF("interp", EXEC.ioc = Interpreted)
    else IFDEF("jit", EXEC.ioc = Jited)
    else IFDEF("compile", EXEC.ioc = Compiled)
    else IFDEF("noise", EXEC.noise_level = Noise)
    else IFDEF("silent", EXEC.noise_level = Silent)
    else IFDEF("oz0", EXEC.optimize_level = O0)
    else IFDEF("oz1", EXEC.optimize_level = O1)
    else IFDEF("oz2", EXEC.optimize_level = O2)
    else IFDEF("oz3", EXEC.optimize_level = O3)
    else IFDEF("orec", EXEC.optimize_rec = true)
    else IFDEF("ogc", EXEC.gc_free = true)
    else IFDEF("osize", EXEC.osize = true)
    else IFDEF("kasm", EXEC.kasm = true)
    else IFDEF("kir", EXEC.kir = true)
    else IFDEF("docasm", EXEC.docasm = true)
    else IFDEF("st", EXEC.show_steps = true)
    else IFDEF("tcheck", EXEC.type_check = true)
    else IFDEF("ext_bignum", EXEC.ext_bignum = true)
    else IFDEF("ext_ptr", EXEC.ext_ptr = true)
    else IFDEF("low", EXEC.low = true)
    else IFDEF("shw_inside_struct", EXEC.shw_inside_struct = true)
    else state_put_warn_br("`%s' is a unknown preprocessor definition, at now RCL don't handle them.", p->u.define_.uident_);
}

void handle_absynInclude(BResult * bresult, Preprocessor p)
{
    if (fopen(p->u.include_.string_, "r"))
    {
        RCL_File *tmp_file_already_exists = getSpecificRCL_File(&bresult->psdata.includes, p->u.include_.uident_);
        if (tmp_file_already_exists != NULL)
        {
            state_put_info_br("In file `%s': ", bresult->current_filename);
            state_put_warn_br("An included external dynamic library called `%s' already exists in the table of external ressources", p->u.include_.uident_);
            state_put_info_br("--- The current handled library path is from `%s' ;", p->u.include_.string_);
            state_put_info_br("--- The already existing library path is from `%s'.", tmp_file_already_exists->path);
            if (!strcmp(p->u.include_.string_, tmp_file_already_exists->path))
            {
                state_put_info_br("------ Since they redirect to the same location, nothing's changed.", NULL);
            }
            else
            {
                state_put_info_br("------ Please change the name of the included library.", NULL);
                state_put_info_br("       The current one has no been added.", NULL);
            }
        }
        else
        {
            add_RCL_File(&bresult->psdata.includes, new_RCL_File(p->u.include_.string_, p->u.include_.uident_));
        }
    }
    else
    {
        state_put_err_br("The included dynamic library called `%s' wasn't found in the given path: \"%s\".", p->u.include_.uident_, p->u.include_.string_);
    }
}

void handle_parallelConcat(RawCode * rcode, Operation fx, Operation gx)
{
    push_rcode(rcode, make_RCL_Value_Parallel(otov(fx), otov(gx)));
}

size_t count_includes(ListPreprocessor lp)
{
    ListPreprocessor tmp = lp;
    size_t result;
    while (tmp != NULL)
    {
        if (tmp->preprocessor_->kind == is_Include)
            result++;
        tmp = tmp->listpreprocessor_;
    }
    return result;
}

size_t count_imports(ListPreprocessor lp)
{
    ListPreprocessor tmp = lp;
    size_t result;
    while (tmp != NULL)
    {
        if (tmp->preprocessor_->kind == is_Import)
            result++;
        tmp = tmp->listpreprocessor_;
    }
    return result;
}
