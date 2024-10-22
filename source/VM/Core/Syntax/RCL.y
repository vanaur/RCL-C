/* This Bison file was machine-generated by BNFC */
%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Absyn>
#define initialize_lexer RCL_initialize_lexer
extern int yyparse(void);
extern int yylex(void);
int yy_mylinenumber;
extern int initialize_lexer(FILE * inp);
void yyerror(const char *str)
{
  extern char *RCLtext;
  fprintf(stderr,"error: line %d: %s at %s\n",
    yy_mylinenumber + 1, str, RCLtext);
}

Program YY_RESULT_Program_ = 0;
Program pProgram(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Program_;
  }
}

Identifier YY_RESULT_Identifier_ = 0;
Identifier pIdentifier(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Identifier_;
  }
}

Preprocessor YY_RESULT_Preprocessor_ = 0;
Preprocessor pPreprocessor(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Preprocessor_;
  }
}

Definition YY_RESULT_Definition_ = 0;
Definition pDefinition(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Definition_;
  }
}

RCL_Structure_Field YY_RESULT_RCL_Structure_Field_ = 0;
RCL_Structure_Field pRCL_Structure_Field(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_RCL_Structure_Field_;
  }
}

FFI_Type_Signature YY_RESULT_FFI_Type_Signature_ = 0;
FFI_Type_Signature pFFI_Type_Signature(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_FFI_Type_Signature_;
  }
}

Ptr YY_RESULT_Ptr_ = 0;
Ptr pPtr(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Ptr_;
  }
}

FFI_Type_SignatureT YY_RESULT_FFI_Type_SignatureT_ = 0;
FFI_Type_SignatureT pFFI_Type_SignatureT(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_FFI_Type_SignatureT_;
  }
}

Operation YY_RESULT_Operation_ = 0;
Operation pOperation(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Operation_;
  }
}

LiteralOperation YY_RESULT_LiteralOperation_ = 0;
LiteralOperation pLiteralOperation(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_LiteralOperation_;
  }
}

Data YY_RESULT_Data_ = 0;
Data pData(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Data_;
  }
}

ListOperation YY_RESULT_ListOperation_ = 0;
ListOperation pListOperation(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListOperation_;
  }
}

ListFFI_Type_Signature YY_RESULT_ListFFI_Type_Signature_ = 0;
ListFFI_Type_Signature pListFFI_Type_Signature(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListFFI_Type_Signature_;
  }
}

ListPtr YY_RESULT_ListPtr_ = 0;
ListPtr pListPtr(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListPtr_;
  }
}

ListDefinition YY_RESULT_ListDefinition_ = 0;
ListDefinition pListDefinition(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListDefinition_;
  }
}

ListPreprocessor YY_RESULT_ListPreprocessor_ = 0;
ListPreprocessor pListPreprocessor(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListPreprocessor_;
  }
}

ListRCL_Structure_Field YY_RESULT_ListRCL_Structure_Field_ = 0;
ListRCL_Structure_Field pListRCL_Structure_Field(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListRCL_Structure_Field_;
  }
}


ListOperation reverseListOperation(ListOperation l)
{
  ListOperation prev = 0;
  ListOperation tmp = 0;
  while (l)
  {
    tmp = l->listoperation_;
    l->listoperation_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListFFI_Type_Signature reverseListFFI_Type_Signature(ListFFI_Type_Signature l)
{
  ListFFI_Type_Signature prev = 0;
  ListFFI_Type_Signature tmp = 0;
  while (l)
  {
    tmp = l->listffi_type_signature_;
    l->listffi_type_signature_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListPtr reverseListPtr(ListPtr l)
{
  ListPtr prev = 0;
  ListPtr tmp = 0;
  while (l)
  {
    tmp = l->listptr_;
    l->listptr_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListDefinition reverseListDefinition(ListDefinition l)
{
  ListDefinition prev = 0;
  ListDefinition tmp = 0;
  while (l)
  {
    tmp = l->listdefinition_;
    l->listdefinition_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListPreprocessor reverseListPreprocessor(ListPreprocessor l)
{
  ListPreprocessor prev = 0;
  ListPreprocessor tmp = 0;
  while (l)
  {
    tmp = l->listpreprocessor_;
    l->listpreprocessor_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListRCL_Structure_Field reverseListRCL_Structure_Field(ListRCL_Structure_Field l)
{
  ListRCL_Structure_Field prev = 0;
  ListRCL_Structure_Field tmp = 0;
  while (l)
  {
    tmp = l->listrcl_structure_field_;
    l->listrcl_structure_field_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}

%}

%union
{
  int int_;
  char char_;
  double double_;
  char* string_;
  Program program_;
  Identifier identifier_;
  Preprocessor preprocessor_;
  Definition definition_;
  RCL_Structure_Field rcl_structure_field_;
  FFI_Type_Signature ffi_type_signature_;
  Ptr ptr_;
  FFI_Type_SignatureT ffi_type_signaturet_;
  Operation operation_;
  LiteralOperation literaloperation_;
  Data data_;
  ListOperation listoperation_;
  ListFFI_Type_Signature listffi_type_signature_;
  ListPtr listptr_;
  ListDefinition listdefinition_;
  ListPreprocessor listpreprocessor_;
  ListRCL_Structure_Field listrcl_structure_field_;

}

%token _ERROR_
%token _SYMB_0    /*   .idata   */
%token _SYMB_1    /*   .define   */
%token _SYMB_2    /*   .main   */
%token _SYMB_3    /*   .   */
%token _SYMB_4    /*   =   */
%token _SYMB_5    /*   (   */
%token _SYMB_6    /*   )   */
%token _SYMB_7    /*   ->   */
%token _SYMB_8    /*   !   */
%token _SYMB_9    /*   ::   */
%token _SYMB_10    /*   [   */
%token _SYMB_11    /*   ]   */
%token _SYMB_12    /*   *   */
%token _SYMB_13    /*   ~   */
%token _SYMB_14    /*   #   */
%token _SYMB_15    /*   \   */
%token _SYMB_16    /*   $   */
%token _SYMB_17    /*   {   */
%token _SYMB_18    /*   }   */
%token _SYMB_19    /*   +   */
%token _SYMB_20    /*   -   */
%token _SYMB_21    /*   **   */
%token _SYMB_22    /*   /   */
%token _SYMB_23    /*   %   */
%token _SYMB_24    /*   ++   */
%token _SYMB_25    /*   --   */
%token _SYMB_26    /*   &   */
%token _SYMB_27    /*   |   */
%token _SYMB_28    /*   ^   */
%token _SYMB_29    /*   <<   */
%token _SYMB_30    /*   >>   */
%token _SYMB_31    /*   ,   */
%token _SYMB_32    /*   ;   */
%token _SYMB_33    /*   any   */
%token _SYMB_34    /*   as   */
%token _SYMB_35    /*   define   */
%token _SYMB_36    /*   double   */
%token _SYMB_37    /*   else   */
%token _SYMB_38    /*   error   */
%token _SYMB_39    /*   extern   */
%token _SYMB_40    /*   fault   */
%token _SYMB_41    /*   float   */
%token _SYMB_42    /*   ifdef   */
%token _SYMB_43    /*   import   */
%token _SYMB_44    /*   include   */
%token _SYMB_45    /*   info   */
%token _SYMB_46    /*   int   */
%token _SYMB_47    /*   long   */
%token _SYMB_48    /*   schar   */
%token _SYMB_49    /*   self   */
%token _SYMB_50    /*   sint   */
%token _SYMB_51    /*   sint16   */
%token _SYMB_52    /*   sint32   */
%token _SYMB_53    /*   sint64   */
%token _SYMB_54    /*   sint8   */
%token _SYMB_55    /*   slong   */
%token _SYMB_56    /*   sshort   */
%token _SYMB_57    /*   structure   */
%token _SYMB_58    /*   then   */
%token _SYMB_59    /*   uchar   */
%token _SYMB_60    /*   uint   */
%token _SYMB_61    /*   uint16   */
%token _SYMB_62    /*   uint32   */
%token _SYMB_63    /*   uint64   */
%token _SYMB_64    /*   uint8   */
%token _SYMB_65    /*   ulong   */
%token _SYMB_66    /*   ushort   */
%token _SYMB_67    /*   void   */
%token _SYMB_68    /*   warning   */
%token<string_> _SYMB_69    /*   UIdent   */
%token<string_> _SYMB_70    /*   LIdent   */

%type <program_> Program
%type <identifier_> Identifier
%type <preprocessor_> Preprocessor
%type <definition_> Definition
%type <rcl_structure_field_> RCL_Structure_Field
%type <ffi_type_signature_> FFI_Type_Signature
%type <ptr_> Ptr
%type <ffi_type_signaturet_> FFI_Type_SignatureT
%type <operation_> Operation1
%type <operation_> Operation
%type <literaloperation_> LiteralOperation
%type <data_> Data
%type <listoperation_> ListOperation1
%type <listffi_type_signature_> ListFFI_Type_Signature
%type <listptr_> ListPtr
%type <listoperation_> ListOperation
%type <listdefinition_> ListDefinition
%type <listpreprocessor_> ListPreprocessor
%type <listrcl_structure_field_> ListRCL_Structure_Field

%token<string_> _STRING_
%token<char_> _CHAR_
%token<int_> _INTEGER_
%token<double_> _DOUBLE_

%start Program
%%
Program : /* empty */ { $$ = make_PVoid(); YY_RESULT_Program_= $$; } 
  | _SYMB_0 ListPreprocessor _SYMB_1 ListDefinition _SYMB_2 Operation1 { $$ = make_Prog1($2, $4, $6); YY_RESULT_Program_= $$; }
  | _SYMB_0 ListPreprocessor _SYMB_1 ListDefinition { $$ = make_Prog2($2, $4); YY_RESULT_Program_= $$; }
  | _SYMB_0 ListPreprocessor _SYMB_2 Operation1 { $$ = make_Prog3($2, $4); YY_RESULT_Program_= $$; }
  | _SYMB_1 ListDefinition _SYMB_2 Operation1 { $$ = make_Prog4($2, $4); YY_RESULT_Program_= $$; }
  | _SYMB_1 ListDefinition { $$ = make_Prog5($2); YY_RESULT_Program_= $$; }
  | _SYMB_2 Operation1 { $$ = make_Prog6($2); YY_RESULT_Program_= $$; }
  | Operation1 { $$ = make_Prog7($1); YY_RESULT_Program_= $$; }
  | _SYMB_0 ListPreprocessor { $$ = make_Prog8($2); YY_RESULT_Program_= $$; }
;
Identifier : _SYMB_70 { $$ = make_Name($1);  } 
  | _SYMB_69 _SYMB_3 Identifier { $$ = make_QualName($1, $3);  }
;
Preprocessor : _SYMB_43 _STRING_ { $$ = make_Import($2);  } 
  | _SYMB_43 _STRING_ _SYMB_34 _SYMB_69 { $$ = make_ImportAs($2, $4);  }
  | _SYMB_44 _STRING_ _SYMB_34 _SYMB_69 { $$ = make_Include($2, $4);  }
  | _SYMB_35 _SYMB_69 { $$ = make_Define($2);  }
  | _SYMB_42 _SYMB_69 _SYMB_58 Preprocessor { $$ = make_IF($2, $4);  }
  | _SYMB_42 _SYMB_69 _SYMB_58 Preprocessor _SYMB_37 Preprocessor { $$ = make_ELSE($2, $4, $6);  }
  | _SYMB_38 _STRING_ { $$ = make_Error($2);  }
  | _SYMB_68 _STRING_ { $$ = make_Warning($2);  }
  | _SYMB_45 _STRING_ { $$ = make_Info($2);  }
  | _SYMB_40 _INTEGER_ { $$ = make_FaultInt($2);  }
  | _SYMB_40 _SYMB_69 { $$ = make_FaultId($2);  }
;
Definition : _SYMB_70 _SYMB_4 Operation1 { $$ = make_Def($1, $3);  } 
  | _SYMB_39 Identifier _SYMB_5 ListFFI_Type_Signature _SYMB_6 _SYMB_7 FFI_Type_Signature { $$ = make_Extern($2, $4, $7);  }
  | _SYMB_57 _SYMB_69 _SYMB_4 ListRCL_Structure_Field { $$ = make_Structure($2, $4);  }
;
RCL_Structure_Field : _SYMB_3 _SYMB_70 { $$ = make_FreeField($2);  } 
  | _SYMB_3 _SYMB_70 _SYMB_8 { $$ = make_EnumField($2);  }
  | _SYMB_3 _SYMB_70 _SYMB_9 Identifier { $$ = make_SpecField($2, $4);  }
;
FFI_Type_Signature : FFI_Type_SignatureT { $$ = make_TPrimitive($1);  } 
  | _SYMB_10 ListFFI_Type_Signature _SYMB_11 { $$ = make_TStruct($2);  }
  | _SYMB_49 { $$ = make_TSSelf();  }
  | FFI_Type_Signature ListPtr { $$ = make_TPtr($1, reverseListPtr($2));  }
;
Ptr : _SYMB_12 { $$ = make_Point();  } 
;
FFI_Type_SignatureT : _SYMB_67 { $$ = make_TVoid();  } 
  | _SYMB_64 { $$ = make_TUint8();  }
  | _SYMB_54 { $$ = make_TSint8();  }
  | _SYMB_61 { $$ = make_TUint16();  }
  | _SYMB_51 { $$ = make_TSint16();  }
  | _SYMB_62 { $$ = make_TUint32();  }
  | _SYMB_52 { $$ = make_TSint32();  }
  | _SYMB_63 { $$ = make_TUint64();  }
  | _SYMB_53 { $$ = make_TSint64();  }
  | _SYMB_41 { $$ = make_TFloat();  }
  | _SYMB_47 _SYMB_36 { $$ = make_TLongDouble();  }
  | _SYMB_36 { $$ = make_TDouble();  }
  | _SYMB_59 { $$ = make_TUchar();  }
  | _SYMB_48 { $$ = make_TSchar();  }
  | _SYMB_66 { $$ = make_TUshort();  }
  | _SYMB_56 { $$ = make_TSshort();  }
  | _SYMB_60 { $$ = make_TUint();  }
  | _SYMB_50 { $$ = make_TSint();  }
  | _SYMB_46 { $$ = make_Tint();  }
  | _SYMB_65 { $$ = make_TUlong();  }
  | _SYMB_55 { $$ = make_TSlong();  }
  | _SYMB_33 { $$ = make_TAny();  }
;
Operation1 : ListOperation { $$ = make_Concatenation($1);  } 
;
Operation : _SYMB_13 _INTEGER_ Operation { $$ = make_RepeatOperation($2, $3);  } 
  | Operation _SYMB_14 Operation { $$ = make_ParallelConcat($1, $3);  }
  | _SYMB_70 _SYMB_15 { $$ = make_Lambda($1);  }
  | _SYMB_70 _SYMB_16 { $$ = make_EndLambdaScope($1);  }
  | _SYMB_10 Operation1 _SYMB_11 { $$ = make_Quote($2);  }
  | _SYMB_17 ListOperation1 _SYMB_18 { $$ = make_List($2);  }
  | Identifier { $$ = make_Var($1);  }
  | Data { $$ = make_Lit($1);  }
  | LiteralOperation { $$ = make_BasicLiteralOp($1);  }
  | RCL_Structure_Field { $$ = make_Field($1);  }
;
LiteralOperation : _SYMB_19 { $$ = make_Add();  } 
  | _SYMB_20 { $$ = make_Sub();  }
  | _SYMB_21 { $$ = make_Pow();  }
  | _SYMB_12 { $$ = make_Mul();  }
  | _SYMB_22 { $$ = make_Div();  }
  | _SYMB_23 { $$ = make_Mod();  }
  | _SYMB_24 { $$ = make_Inc();  }
  | _SYMB_25 { $$ = make_Dec();  }
  | _SYMB_26 { $$ = make_BAnd();  }
  | _SYMB_27 { $$ = make_BOr();  }
  | _SYMB_28 { $$ = make_BXor();  }
  | _SYMB_29 { $$ = make_BLeft();  }
  | _SYMB_30 { $$ = make_BRight();  }
;
Data : _INTEGER_ { $$ = make_Int($1);  } 
  | _DOUBLE_ { $$ = make_Flt($1);  }
  | _CHAR_ { $$ = make_Chr($1);  }
  | _STRING_ { $$ = make_Str($1);  }
;
ListOperation1 : /* empty */ { $$ = 0;  } 
  | Operation1 { $$ = make_ListOperation($1, 0);  }
  | Operation1 _SYMB_31 ListOperation1 { $$ = make_ListOperation($1, $3);  }
;
ListFFI_Type_Signature : /* empty */ { $$ = 0;  } 
  | FFI_Type_Signature { $$ = make_ListFFI_Type_Signature($1, 0);  }
  | FFI_Type_Signature _SYMB_31 ListFFI_Type_Signature { $$ = make_ListFFI_Type_Signature($1, $3);  }
;
ListPtr : /* empty */ { $$ = 0;  } 
  | ListPtr Ptr { $$ = make_ListPtr($2, $1);  }
;
ListOperation : Operation { $$ = make_ListOperation($1, 0);  } 
  | Operation ListOperation { $$ = make_ListOperation($1, $2);  }
;
ListDefinition : Definition { $$ = make_ListDefinition($1, 0);  } 
  | Definition ListDefinition { $$ = make_ListDefinition($1, $2);  }
  | /* empty */ { $$ = 0;  }
  | Definition _SYMB_32 ListDefinition { $$ = make_ListDefinition($1, $3);  }
;
ListPreprocessor : Preprocessor { $$ = make_ListPreprocessor($1, 0);  } 
  | Preprocessor ListPreprocessor { $$ = make_ListPreprocessor($1, $2);  }
;
ListRCL_Structure_Field : RCL_Structure_Field { $$ = make_ListRCL_Structure_Field($1, 0);  } 
  | RCL_Structure_Field _SYMB_31 ListRCL_Structure_Field { $$ = make_ListRCL_Structure_Field($1, $3);  }
;

