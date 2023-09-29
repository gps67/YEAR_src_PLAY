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
   case PUNCT_LT_EQUAL_GT:	 return "<=>";
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
   case LEX_EOLN:	 return "_LEX_EOLN_"; // print_TOKEN_name_3 //
   case LEX_IDENTIFIER:	 return "_LEX_IDENTIFIER_"; // print_TOKEN_name_3 //
   case LEX_WS:	 return "_LEX_WS_"; // print_TOKEN_name_3 //
   case LEX_DOUBLE:	 return "_LEX_DOUBLE_"; // print_TOKEN_name_3 //
   case LEX_INTEGER:	 return "_LEX_INTEGER_"; // print_TOKEN_name_3 //
   case LEX_STRING:	 return "_LEX_STRING_"; // print_TOKEN_name_3 //
   case LEX_COMMENT:	 return "_LEX_COMMENT_"; // print_TOKEN_name_3 //

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

%token <tokn> PUNCT_PLUS_PLUS
%token <tokn> PUNCT_MINUS_MINUS
%token <tokn> PUNCT_L_PAR_PAR_R
%token <tokn> PUNCT_L_SQUARE_SQUARE_R
%token <tokn> PUNCT_DOT
%token <tokn> PUNCT_MINUS_GT
%token <tokn> PUNCT_NOT
%token <tokn> PUNCT_TILDE
%token <tokn> PUNCT_STAR
%token <tokn> PUNCT_SLASH
%token <tokn> PUNCT_PERCENT
%token <tokn> PUNCT_PLUS
%token <tokn> PUNCT_MINUS
%token <tokn> PUNCT_LT_LT
%token <tokn> PUNCT_GT_GT
%token <tokn> PUNCT_LT_EQUAL
%token <tokn> PUNCT_GT_EQUAL
%token <tokn> PUNCT_LT
%token <tokn> PUNCT_GT
%token <tokn> PUNCT_EQUAL_EQUAL
%token <tokn> PUNCT_NOT_EQUAL
%token <tokn> PUNCT_EQUAL_EQUAL_EQUAL
%token <tokn> PUNCT_AMP
%token <tokn> PUNCT_CARET
%token <tokn> PUNCT_PIPE
%token <tokn> PUNCT_AMP_AMP_EQUAL
%token <tokn> PUNCT_PIPE_PIPE_EQUAL
%token <tokn> PUNCT_AMP_AMP
%token <tokn> PUNCT_PIPE_PIPE
%token <tokn> PUNCT_LT_LT_LT
%token <tokn> PUNCT_GT_GT_GT
%token <tokn> PUNCT_LT_EQUAL_GT
%token <tokn> PUNCT_LT_LT_EQUAL
%token <tokn> PUNCT_GT_GT_EQUAL
%token <tokn> PUNCT_LT_LT_LT_EQUAL
%token <tokn> PUNCT_GT_GT_GT_EQUAL
%token <tokn> PUNCT_EQUAL
%token <tokn> PUNCT_PLUS_EQUAL
%token <tokn> PUNCT_MINUS_EQUAL
%token <tokn> PUNCT_STAR_EQUAL
%token <tokn> PUNCT_SLASH_EQUAL
%token <tokn> PUNCT_PERCENT_EQUAL
%token <tokn> PUNCT_AMP_EQUAL
%token <tokn> PUNCT_CARET_EQUAL
%token <tokn> PUNCT_PIPE_EQUAL
%token <tokn> PUNCT_COMMA
%token <tokn> PUNCT_L_PAR
%token <tokn> PUNCT_PAR_R
%token <tokn> PUNCT_L_CURLY
%token <tokn> PUNCT_CURLY_R
%token <tokn> PUNCT_SLASH_SLASH
%token <tokn> PUNCT_SLASH_STAR
%token <tokn> PUNCT_STAR_SLASH

%token <tokn> RW_if

%token <tokn> LEX_EOLN
%token <tokn> LEX_IDENTIFIER
%token <tokn> LEX_WS
%token <tokn> LEX_DOUBLE
%token <tokn> LEX_INTEGER
%token <tokn> LEX_STRING
%token <tokn> LEX_COMMENT

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

