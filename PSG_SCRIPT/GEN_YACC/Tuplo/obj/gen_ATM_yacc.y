%{

          #include <cstddef>
          #include <stdio.h>
  //      #include <string>
	#include "Y_PARSE.h" 

          extern const char * str_of_token( int token );
          #include "../src/EXPRS_STUBS.h"
          using namespace EXPRS; // ... E0 ...
/* yacc generated TOKENs header UNION YYSTYPE */
#include "../obj/gen_ATM_yacc.tab.hh"
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
   case PUNCT_PLUS_PLUS:	 return "++";
   case PUNCT_MINUS_MINUS:	 return "--";
   case PUNCT_L_PAR_PAR_R:	 return "()";
   case PUNCT_L_SQUARE_SQUARE_R:	 return "[]";
   case PUNCT_DOT:	 return ".";
   case PUNCT_MINUS_GT:	 return "->";
   case PUNCT_NOT:	 return "!";
   case PUNCT_TILDE:	 return "~";
   case PUNCT_STAR:	 return "*";
   case PUNCT_SLASH:	 return "/";
   case PUNCT_PERCENT:	 return "%";
   case PUNCT_PLUS:	 return "+";
   case PUNCT_MINUS:	 return "-";
   case PUNCT_LT_LT:	 return "<<";
   case PUNCT_GT_GT:	 return ">>";
   case PUNCT_LT_EQUAL:	 return "<=";
   case PUNCT_GT_EQUAL:	 return ">=";
   case PUNCT_LT:	 return "<";
   case PUNCT_GT:	 return ">";
   case PUNCT_EQUAL_EQUAL:	 return "==";
   case PUNCT_NOT_EQUAL:	 return "!=";
   case PUNCT_EQUAL_EQUAL_EQUAL:	 return "===";
   case PUNCT_AMP:	 return "&";
   case PUNCT_CARET:	 return "^";
   case PUNCT_PIPE:	 return "|";
   case PUNCT_AMP_AMP_EQUAL:	 return "&&=";
   case PUNCT_PIPE_PIPE_EQUAL:	 return "||=";
   case PUNCT_AMP_AMP:	 return "&&";
   case PUNCT_PIPE_PIPE:	 return "||";
   case PUNCT_LT_LT_LT:	 return "<<<";
   case PUNCT_GT_GT_GT:	 return ">>>";
   case PUNCT_LT_LT_EQUAL:	 return "<<=";
   case PUNCT_GT_GT_EQUAL:	 return ">>=";
   case PUNCT_LT_LT_LT_EQUAL:	 return "<<<=";
   case PUNCT_GT_GT_GT_EQUAL:	 return ">>>=";
   case PUNCT_EQUAL:	 return "=";
   case PUNCT_PLUS_EQUAL:	 return "+=";
   case PUNCT_MINUS_EQUAL:	 return "-=";
   case PUNCT_STAR_EQUAL:	 return "*=";
   case PUNCT_SLASH_EQUAL:	 return "/=";
   case PUNCT_PERCENT_EQUAL:	 return "%=";
   case PUNCT_AMP_EQUAL:	 return "&=";
   case PUNCT_CARET_EQUAL:	 return "^=";
   case PUNCT_PIPE_EQUAL:	 return "|=";
   case PUNCT_COMMA:	 return ",";
   case PUNCT_L_PAR:	 return "(";
   case PUNCT_PAR_R:	 return ")";
   case PUNCT_L_CURLY:	 return "{";
   case PUNCT_CURLY_R:	 return "}";
   case PUNCT_SLASH_SLASH:	 return "//";
   case PUNCT_SLASH_STAR:	 return "/*";
   case PUNCT_STAR_SLASH:	 return "*/";

  // RW_ reserved words
   case RW_if:	 return "if";

  // LEX_ items have no value so return SYMBOL
   case LEX_EOLN:	 return "_LEX_EOLN_";
   case LEX_IDENTIFIER:	 return "_LEX_IDENTIFIER_";
   case LEX_WS:	 return "_LEX_WS_";
   case LEX_DOUBLE:	 return "_LEX_DOUBLE_";
   case LEX_INTEGER:	 return "_LEX_INTEGER_";

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
  int token;
  const char * lex_buff;
 }

%token <token> PUNCT_PLUS_PLUS
%token <token> PUNCT_MINUS_MINUS
%token <token> PUNCT_L_PAR_PAR_R
%token <token> PUNCT_L_SQUARE_SQUARE_R
%token <token> PUNCT_DOT
%token <token> PUNCT_MINUS_GT
%token <token> PUNCT_NOT
%token <token> PUNCT_TILDE
%token <token> PUNCT_STAR
%token <token> PUNCT_SLASH
%token <token> PUNCT_PERCENT
%token <token> PUNCT_PLUS
%token <token> PUNCT_MINUS
%token <token> PUNCT_LT_LT
%token <token> PUNCT_GT_GT
%token <token> PUNCT_LT_EQUAL
%token <token> PUNCT_GT_EQUAL
%token <token> PUNCT_LT
%token <token> PUNCT_GT
%token <token> PUNCT_EQUAL_EQUAL
%token <token> PUNCT_NOT_EQUAL
%token <token> PUNCT_EQUAL_EQUAL_EQUAL
%token <token> PUNCT_AMP
%token <token> PUNCT_CARET
%token <token> PUNCT_PIPE
%token <token> PUNCT_AMP_AMP_EQUAL
%token <token> PUNCT_PIPE_PIPE_EQUAL
%token <token> PUNCT_AMP_AMP
%token <token> PUNCT_PIPE_PIPE
%token <token> PUNCT_LT_LT_LT
%token <token> PUNCT_GT_GT_GT
%token <token> PUNCT_LT_LT_EQUAL
%token <token> PUNCT_GT_GT_EQUAL
%token <token> PUNCT_LT_LT_LT_EQUAL
%token <token> PUNCT_GT_GT_GT_EQUAL
%token <token> PUNCT_EQUAL
%token <token> PUNCT_PLUS_EQUAL
%token <token> PUNCT_MINUS_EQUAL
%token <token> PUNCT_STAR_EQUAL
%token <token> PUNCT_SLASH_EQUAL
%token <token> PUNCT_PERCENT_EQUAL
%token <token> PUNCT_AMP_EQUAL
%token <token> PUNCT_CARET_EQUAL
%token <token> PUNCT_PIPE_EQUAL
%token <token> PUNCT_COMMA
%token <token> PUNCT_L_PAR
%token <token> PUNCT_PAR_R
%token <token> PUNCT_L_CURLY
%token <token> PUNCT_CURLY_R
%token <token> PUNCT_SLASH_SLASH
%token <token> PUNCT_SLASH_STAR
%token <token> PUNCT_STAR_SLASH

%token <token> RW_if

%token <token> LEX_EOLN
%token <token> LEX_IDENTIFIER
%token <token> LEX_WS
%token <token> LEX_DOUBLE
%token <token> LEX_INTEGER

%type <expr> expr_ident
%type <expr> expr
%type <expr> EXPR_line
%type <expr> lines
%type <e32> e32_expr

%left PUNCT_PLUS PUNCT_MINUS
%left PUNCT_STAR PUNCT_SLASH
%right PUNCT_CARET
%start top

%%

// RULES 
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
: %empty
| lines EXPR_line { EXPR * E = $2; $$ = E->EXPR_branch_result(); }

//
// save PSG_line as RETVAL // all wrong
// PASS back "TOKEN_line" or something
// USE a global PARAMETER save it
//

EXPR_line : expr LEX_EOLN { $$ = $1; INFO("EOLN"); }
// EXPR_line : expr LEX_EOLN { EXPR * E = $1; $$ = E->EXPR_branch_result(); }

top: lines

//
// THIS IS e1.y_RULES END
//
