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
   case RW_JSON_RESERVED_WORD_none:	 return "JSON_RESERVED_WORD_none";

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


%token <tokn> RW_JSON_RESERVED_WORD_none

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

 /* INCL YACC */
// INCL // 
// INCL // including file '../src/JSON.y_RULES' //
// INCL // --------------- START ----------------- //
//
// THIS IS JSON.y_RULES START
//
 /*

 */

// top: HEADERS

// expr_ident : LEX_WS expr_ident { $$ = $2; }
// expr_ident : expr_ident LEX_WS { $$ = $1; }
// expr_ident : LEX_IDENTIFIER { $$ = EXPRS:: E0( (STR0) yylval.lex_buff ); }

// expr : expr_ident { $$ = $1; }

// expr : expr PUNCT_CARET expr { $$ = E_power_E( $1, $3 ); }
// expr : expr PUNCT_STAR  expr { $$ = E_times_E( $1, $3 ); }
// expr : expr PUNCT_PLUS  expr { $$ = E_plus_E( $1, $3 ); }
// expr :      PUNCT_MINUS expr { $$ = E1( $1, $2 ); }   
// expr :      PUNCT_L_PAR expr PUNCT_PAR_R{ $$ = E_paren( $2 ) ; }
// //	...: '-'  expr      %prec  '*'

expr :      PUNCT_PLUS expr PUNCT_PLUS{ $$ = E_paren( $2 ) ; }

lines
: %empty {}
| lines expr LEX_EOLN { EXPR * E = $2; $$ = E->EXPR_branch_result(); }
// // | lines EXPR_line { EXPR * E = $2; $$ = E->EXPR_branch_result(); }

 // // lines: lines EXPR_line { EXPR * E = $2; $$ = E->EXPR_branch_result(); }

// //
// // save PSG_line as RETVAL // all wrong
// // PASS back "TOKEN_line" or something
// // USE a global PARAMETER save it
// //

// EXPR_line : expr LEX_EOLN { $$ = $1; INFO("EOLN"); }
// EXPR_line : expr LEX_EOLN { EXPR * E = $1; $$ = E->EXPR_branch_result(); }

top: lines

//
// THIS IS JSON.y_RULES END
//
// INCL // --------------- END ----------------- //
// INCL // included file '../src/JSON.y_RULES' //
// INCL //

 /* EOF */

