%{

          #include <cstddef>
          #include <stdio.h>
  //      #include <string>
	#include "Y_PARSE.h" 

          extern const char * str_of_token( int token );
          #include "../src/EXPRS_STUBS.h"
          using namespace EXPRS; // ... E0 ...
/* yacc generated TOKENs header UNION YYSTYPE */
#include "../obj/gen_JSON_yacc.tab.hh"
          #include "str1.h"
  #if 0
          struct YYSTYPE;
          struct YYLTYPE;
          int  yylex (YYSTYPE*, YYLTYPE*);
  #else
          int yylex(void);
  #endif
          extern int yylineno;
          extern char * yytext;
          // extern int yyparse (void);

 const char * str_of_token( int tok ) {
  switch( tok ) {
  // PUNCT_ any literal strings almost

  // RW_ reserved words
   case RW_StartFontMetrics:	 return "StartFontMetrics";
   case RW_Comment:	 return "Comment";
   case RW_FontName:	 return "FontName";
   case RW_FullName:	 return "FullName";
   case RW_FamilyName:	 return "FamilyName";
   case RW_Weight:	 return "Weight";
   case RW_ItalicAngle:	 return "ItalicAngle";
   case RW_IsFixedPitch:	 return "IsFixedPitch";
   case RW_UnderlinePosition:	 return "UnderlinePosition";
   case RW_UnderlineThickness:	 return "UnderlineThickness";
   case RW_Version:	 return "Version";
   case RW_Notice:	 return "Notice";
   case RW_EncodingScheme:	 return "EncodingScheme";
   case RW_FontBBox:	 return "FontBBox";
   case RW_CapHeight:	 return "CapHeight";
   case RW_XHeight:	 return "XHeight";
   case RW_Descender:	 return "Descender";
   case RW_Ascender:	 return "Ascender";
   case RW_StartCharMetrics:	 return "StartCharMetrics";
   case RW_C:	 return "C";
   case RW_WX:	 return "WX";
   case RW_N:	 return "N";
   case RW_B:	 return "B";

  // LEX_ items have no value so return SYMBOL
   case LEX_EOLN:	 return "_LEX_EOLN_"; // print_TOKEN_name_3 //
   case LEX_WORD:	 return "_LEX_WORD_"; // print_TOKEN_name_3 //
   case LEX_WS:	 return "_LEX_WS_"; // print_TOKEN_name_3 //
   case LEX_DOUBLE:	 return "_LEX_DOUBLE_"; // print_TOKEN_name_3 //
   case LEX_INTEGER:	 return "_LEX_INTEGER_"; // print_TOKEN_name_3 //
   case LEX_BOOL:	 return "_LEX_BOOL_"; // print_TOKEN_name_3 //

   default:
     static char as_hex[20];
     snprintf( as_hex, sizeof(as_hex), "x%2X", tok );
     return as_hex;

  } // switch
 } // fn gen_YACC_str_of_token

%}

// %parse-param { FIRST } { SECOND }
// PARAMETER { Type * Item }  // to yyerror 
// see https://www.gnu.org/software/bison/manual/html_node/Parser-Function.html 
// C might not complain about missing parameters 
//
%parse-param {Y_Parse_t & psg} // PARAMETER // NB REF AMP 

 %union {
  struct EXPR * expr;
  u32 e32;
  int tokn;
  const char * lex_buff;
 }


%token <tokn> RW_StartFontMetrics
%token <tokn> RW_Comment
%token <tokn> RW_FontName
%token <tokn> RW_FullName
%token <tokn> RW_FamilyName
%token <tokn> RW_Weight
%token <tokn> RW_ItalicAngle
%token <tokn> RW_IsFixedPitch
%token <tokn> RW_UnderlinePosition
%token <tokn> RW_UnderlineThickness
%token <tokn> RW_Version
%token <tokn> RW_Notice
%token <tokn> RW_EncodingScheme
%token <tokn> RW_FontBBox
%token <tokn> RW_CapHeight
%token <tokn> RW_XHeight
%token <tokn> RW_Descender
%token <tokn> RW_Ascender
%token <tokn> RW_StartCharMetrics
%token <tokn> RW_C
%token <tokn> RW_WX
%token <tokn> RW_N
%token <tokn> RW_B

%token <tokn> LEX_EOLN
%token <tokn> LEX_WORD
%token <tokn> LEX_WS
%token <tokn> LEX_DOUBLE
%token <tokn> LEX_INTEGER
%token <tokn> LEX_BOOL

%type <expr> expr_ident
%type <expr> expr
%type <expr> EXPR_line
%type <expr> lines

%left PUNCT_PLUS PUNCT_MINUS
%left PUNCT_STAR PUNCT_SLASH
%right PUNCT_CARET
%start top

%%

// RULES 
// RULES - I thought this was virtual overruled
// RULES 
//
// THIS IS e1.y_RULES START
//

expr_ident : LEX_WS expr_ident { $$ = $2; }
expr_ident : expr_ident LEX_WS { $$ = $1; }
expr_ident : LEX_IDENTIFIER { $$ = EXPRS:: E0( (STR0) yylval.lex_buff ); }

expr : expr_ident { $$ = $1; }

expr : expr PUNCT_CARET expr { $$ = E_power_E( $1, $3 ); }
expr : expr PUNCT_STAR  expr { $$ = E_times_E( $1, $3 ); }
expr : expr PUNCT_PLUS  expr { $$ = E_plus_E( $1, $3 ); }
expr :      PUNCT_MINUS expr { $$ = E1( $1, $2 ); }   
expr :      PUNCT_L_PAR expr PUNCT_PAR_R{ $$ = E_paren( $2 ) ; }
// //	...: '-'  expr      %prec  '*'

lines
: %empty {}
| lines expr LEX_EOLN { EXPR * E = $2; $$ = E->EXPR_branch_result(); }
// | lines EXPR_line { EXPR * E = $2; $$ = E->EXPR_branch_result(); }

 // lines: lines EXPR_line { EXPR * E = $2; $$ = E->EXPR_branch_result(); }

//
// save PSG_line as RETVAL // all wrong
// PASS back "TOKEN_line" or something
// USE a global PARAMETER save it
//

// EXPR_line : expr LEX_EOLN { $$ = $1; INFO("EOLN"); }
// EXPR_line : expr LEX_EOLN { EXPR * E = $1; $$ = E->EXPR_branch_result(); }

top: lines

//
// THIS IS e1.y_RULES END
//

 /* EOF */

