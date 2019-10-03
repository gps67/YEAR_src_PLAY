%{

          #include <cstddef>
          #include <stdio.h>
  //      #include <string>
          extern const char * str_of_token( int token );
          #include "EXPRS.h"
          using namespace EXPRS;
          #include "e1.tab.hh" // bison lists PUNCT_PLUS as int
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
   case RW_if:	return "if";
   case RW_while:	return "while";
   case RW_for:	return "for";
   case RW_do:	return "do";
   case RW_extern:	return "extern";
   case RW_return:	return "return";
   case PUNCT_CEQ:	return "==";
   case PUNCT_CNE:	return "!=";
   case PUNCT_CLT:	return "<";
   case PUNCT_CLE:	return "<=";
   case PUNCT_CGT:	return ">";
   case PUNCT_CGE:	return ">=";
   case PUNCT_EQUAL:	return "=";
   case PUNCT_LPAREN:	return "(";
   case PUNCT_RPAREN:	return ")";
   case PUNCT_LBRACE:	return "{";
   case PUNCT_RBRACE:	return "}";
   case PUNCT_DOTOK_:	return ".";
   case PUNCT_COMMA:	return ",";
   case PUNCT_PLUS:	return "+";
   case PUNCT_MINUS:	return "-";
   case PUNCT_ASTERISK:	return "*";
   case PUNCT_SLASH:	return "/";
   case PUNCT_CARET:	return "^";
   case PUNCT_AMPERSAND:	return "&";
   case PUNCT_PERCENT:	return "%";
   case PUNCT_PIPE:	return "|";
   default:
    static char as_hex[20];
    snprintf( as_hex, sizeof(as_hex), "x%2X", tok );
    return as_hex;
  }
 }

%}


// yylval . fieldname

%union {
  EXPR * expr;
  int token;
  const char * lex_buff;
}

%token <token> RW_if
%token <token> RW_while
%token <token> RW_for
%token <token> RW_do
%token <token> RW_extern
%token <token> RW_return
%token <token> PUNCT_CEQ
%token <token> PUNCT_CNE
%token <token> PUNCT_CLT
%token <token> PUNCT_CLE
%token <token> PUNCT_CGT
%token <token> PUNCT_CGE
%token <token> PUNCT_EQUAL
%token <token> PUNCT_LPAREN
%token <token> PUNCT_RPAREN
%token <token> PUNCT_LBRACE
%token <token> PUNCT_RBRACE
%token <token> PUNCT_DOTOK_
%token <token> PUNCT_COMMA
%token <token> PUNCT_PLUS
%token <token> PUNCT_MINUS
%token <token> PUNCT_ASTERISK
%token <token> PUNCT_SLASH
%token <token> PUNCT_CARET
%token <token> PUNCT_AMPERSAND
%token <token> PUNCT_PERCENT
%token <token> PUNCT_PIPE
%token <token> LEX_EOLN
%token <token> LEX_WS
%token <token> LEX_IDENTIFIER
%token <token> LEX_DOUBLE
%token <token> LEX_INTEGER

%type <expr> expr_ident
%type <expr> expr
%type <token> BOP

%right PUNCT_CARET ///////// dumb

%start top

%%


// expr_ident : LEX_IDENTIFIER { $$ = E1( $1 ); }

// expr_ident : LEX_IDENTIFIER { $$ = E1( "LEX_IDENTIFIER" ); }
expr_ident : LEX_WS expr_ident { $$ = $2; }
expr_ident : expr_ident LEX_WS { $$ = $1; }
expr_ident : LEX_IDENTIFIER { $$ = E1( yylval.lex_buff ); }

BOP	: 
	  PUNCT_MINUS
	| PUNCT_SLASH
	| PUNCT_PERCENT
	| PUNCT_PIPE
	| PUNCT_AMPERSAND
	| PUNCT_COMMA
	;

// // this is re-branding expr sp as an expr of priority 123
// expr : LEX_WS expr { $$ = $2; }
// expr : expr LEX_WS { $$ = $1; }

expr : expr PUNCT_PLUS expr { $$ = E_plus_E( $1, $3 ); }
expr : expr PUNCT_ASTERISK expr { $$ = E_times_E( $1, $3 ); }
expr : expr PUNCT_CARET expr { $$ = E_power_E( $1, $3 ); }

expr    :       LEX_WS expr LEX_WS { $$ = $2; }
	|	expr  BOP  expr { $$ = E2( $1, $2, $3 ); }
	|       PUNCT_MINUS  expr  { $$ = E1( $1, $2 ); }   
//	|       '-'  expr      %prec  '*'
	|	expr_ident { $$ = $1; }
	|	PUNCT_LPAREN expr PUNCT_RPAREN { $$ = $2; } // keep not of () ?
	;

// no match on eoln ?
// example Left Recursive list (without the list builder)
lines
: %empty
| lines line

line : expr LEX_EOLN { EXPR * E = $1; E->printer(0); }

top: lines

   // # EOT #
