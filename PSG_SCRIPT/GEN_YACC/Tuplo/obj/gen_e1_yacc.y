%{

          #include <cstddef>
          #include <stdio.h>
  //      #include <string>
          extern const char * str_of_token( int token );
          #include "../src/EXPRS.h"
          using namespace EXPRS; // ... E1 ...
#include "../obj/gen_e1_yacc.tab.hh"
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
          // extern "C" int yyparse ();
          extern int yyparse (void);
          void yyerror(const char * msg) {
                  printf("# ERR # line %d # %s\n", yylineno, msg );
          // no   printf("# ERR # col %d # %s\n", @$.first_column, msg );
          }

 const char * str_of_token( int tok ) {
  switch( tok ) {
   case PUNCT_LT_LT_EQUAL:	 return "<<=";
   case PUNCT_GT_GT_EQUAL:	 return ">>=";
   case PUNCT_EQUAL_EQUAL:	 return "==";
   case PUNCT_NOT_EQUAL:	 return "!=";
   case PUNCT_LT_EQUAL:	 return "<=";
   case PUNCT_GT_EQUAL:	 return ">=";
   case PUNCT_LT_LT:	 return "<<";
   case PUNCT_GT_GT:	 return ">>";
   case PUNCT_PLUS_PLUS:	 return "++";
   case PUNCT_STAR:	 return "*";
   case PUNCT_PLUS:	 return "+";
   case PUNCT_CIRC:	 return "^";
   case PUNCT_LT:	 return "<";
   case PUNCT_GT:	 return ">";
   case PUNCT_EQUAL:	 return "=";
   case PUNCT_L_PAR:	 return "(";
   case PUNCT_PAR_R:	 return ")";
   case PUNCT_L_CURLY:	 return "{";
   case PUNCT_CURLY_R:	 return "}";
   case PUNCT_DOT:	 return ".";
   case PUNCT_MINUS:	 return "-";
   case PUNCT_SLASH:	 return "/";
   case PUNCT_AMP:	 return "&";
   case PUNCT_PERCENT:	 return "%";
   case PUNCT_PIPE:	 return "|";
   case PUNCT_TILDE:	 return "~";
   case RW_if:	 return "if";
   case LEX_EOLN:	 return "_EOLN_";
   case LEX_IDENTIFIER:	 return "_IDENTIFIER_";
   case LEX_WS:	 return "_WS_";
   case LEX_DOUBLE:	 return "_DOUBLE_";
   case LEX_INTEGER:	 return "_INTEGER_";
   default:
     static char as_hex[20];
     snprintf( as_hex, sizeof(as_hex), "x%2X", tok );
     return as_hex;
  }
 }

%}
%union {
 struct EXPR * expr;
 int token;
 const char * lex_buff;
}
%token <token> PUNCT_LT_LT_EQUAL
%token <token> PUNCT_GT_GT_EQUAL
%token <token> PUNCT_EQUAL_EQUAL
%token <token> PUNCT_NOT_EQUAL
%token <token> PUNCT_LT_EQUAL
%token <token> PUNCT_GT_EQUAL
%token <token> PUNCT_LT_LT
%token <token> PUNCT_GT_GT
%token <token> PUNCT_PLUS_PLUS
%token <token> PUNCT_STAR
%token <token> PUNCT_PLUS
%token <token> PUNCT_CIRC
%token <token> PUNCT_LT
%token <token> PUNCT_GT
%token <token> PUNCT_EQUAL
%token <token> PUNCT_L_PAR
%token <token> PUNCT_PAR_R
%token <token> PUNCT_L_CURLY
%token <token> PUNCT_CURLY_R
%token <token> PUNCT_DOT
%token <token> PUNCT_MINUS
%token <token> PUNCT_SLASH
%token <token> PUNCT_AMP
%token <token> PUNCT_PERCENT
%token <token> PUNCT_PIPE
%token <token> PUNCT_TILDE
%token <token> RW_if
%token <token> LEX_EOLN
%token <token> LEX_IDENTIFIER
%token <token> LEX_WS
%token <token> LEX_DOUBLE
%token <token> LEX_INTEGER
%type <expr> expr_ident
%type <expr> expr
%right PUNCT_CARET // dumb
%start top
%%
// RULES 
// RULES 


expr_ident : LEX_WS expr_ident { $$ = $2; }
expr_ident : expr_ident LEX_WS { $$ = $1; }
expr_ident : LEX_IDENTIFIER { $$ = EXPRS:: E1( (STR0) yylval.lex_buff ); }


// BOP	: 
//	  PUNCT_MINUS
//	| PUNCT_SLASH
//	| PUNCT_PERCENT
//	| PUNCT_PIPE
//	| PUNCT_AMP
//	;

// // this is re-branding expr sp as an expr of priority 123
// expr : LEX_WS expr { $$ = $2; }
// expr : expr LEX_WS { $$ = $1; }

expr : expr_ident { $$ = $1; }

expr : expr PUNCT_CARET expr { $$ = E_power_E( $1, $3 ); }
expr : expr PUNCT_STAR expr { $$ = E_times_E( $1, $3 ); }
expr : expr PUNCT_PLUS expr { $$ = E_plus_E( $1, $3 ); }

// expr    :       LEX_WS expr LEX_WS { $$ = $2; }
// //	|	expr  BOP  expr { $$ = E2( $1, $2, $3 ); }
// 	|       PUNCT_MINUS  expr  { $$ = E1( $1, $2 ); }   
// //	|       '-'  expr      %prec  '*'
// 	|	PUNCT_L_PAR expr PUNCT_PAR_R { $$ = $2; } // keep not of () ?
// 	;

// no match on eoln ?
// example Left Recursive list (without the list builder)
lines
: %empty
| lines line

// line : expr LEX_EOLN { EXPR * E = $1; E->printer(0); }

line : expr LEX_EOLN { EXPR * E = $1; E->print_to_NULL(); }

top: lines

