/*
  LEX file.

    definitions
   %%
    rules
   %%
    user code

   non // comments can be at BOLN except in rules section
   // comments can only be in copied C++ or gcc code
   user code goes at end but the definitions at top
   rule code needs includes for them to be legal
   %{ COPIED CODE %} // %top{ COPIED CODE TOP }
   // OWN LINE RULES for '%top{' '}' and '%{' '%}' 


   this is in definitions SECTION // see 5.0
*/

%{
// within definitions A CODE SECTION START // like %TOP see 5.1 
// C++ comments copied through, upto gcc

// gen_LEX_code_start() // headers 
//somehow before here is stddef stdio - added by flex
#include "buffer1.h"
#include "Y_PARSE.h"
using namespace YY;
/* yacc generated TOKENs header UNION YYSTYPE */
#include "../obj/gen_ATM_yacc.tab.hh"

/*
	// gen_LEX_lex_return() // gen LEX lex_return()
	usage  lex_return(tok) || FAIL
		int lex_return( int tok ); // STR in yylval.lex_buff 


	//	RETURN token as lex retval
	//	RETURN string in UNION.lex_buff field

	//	We must keep the right string, of several, so we keep 16
	//	with this simple round robin loop of slots

	//	PROBLEM: lookahead may mean any number of tokens, not 1
	//	QUICK TEST SHOWS actually OK 1
	//	as long as yacc strdup's immediately
	//	Not sure but maybe

	//	So call return lex_return( TOKEN ); and it obtains lex_buff
	//	for LEX.value for LEX.cident99 for LEX.anystr
	}


*/

// nlex16 = N
	// circular list of vars for single return value
	// We dont know when each will be dropped just have plenty

	// static const int nlex16 = 64;
	   static const int nlex16 = 16;
	// static const int nlex16 = 1;

	// array [ N ] of buffer1 // copy of LEX str in lex_pool[idx]
	static buffer1 lex_pool[nlex16]; // backing store for LEX STR0 

	// _pos in loop
	static int nlex_pos = 0;

/*!
        int lex found( LEX_ITEM_TOKEN ); // STR0 avail, kept here 
	//	RETURN token as lex retval
	//	RETURN string in UNION.lex_buff field

        lex found the string at yytext, yyleng
        keep a copy of the parsed str
        keep in the next storage slot (reuse nlex16 items later)
        copy string to buffer, as str0
        set yylval /union/ .lex_buff as returning string value
        return tok as lex type

 */
int lex_return( int tok )
{
        // skip first cell once, get it next time, or init -1
        // 
        // step to next slot in round_robin loop
        nlex_pos = (nlex_pos+1) % nlex16;
        // 
        // pos == nlex_pos
        int pos = nlex_pos;
        // 
        // save the text pos and leng of the lex
        lex_pool[ pos ].set( yytext, yyleng ); // buffer2
        // 
        // return a STR0 of LEXs text; 
        yylval.lex_buff = (str0) lex_pool[ pos ]; // STR0 from buffer2
        // 
        return tok;
}

// yylval.fieldname = UNION.fieldname ; // returned extra info for TOKEN
// define SAVE_TOKEN  yylval.string = new std::string(yytext, yyleng)
// define XXXX_TOKEN  yylval.lex_buff = NEXT_SLOT(yytext, yyleng)
#define TOKEN(t)    (yylval.token = t)

// lex_start_symbol
/* yacc generated TOKENs header UNION YYSTYPE */
#include "../obj/gen_ATM_yacc.tab.hh"
 // CODE section END
%}

/* still in definitions section */

/* yywrap is a macro called at every end of file */
%option noyywrap

/* definions SECTION END */
%%
 /* RULES SECTION */

 /* gen_LEX_RULES _eoln() */

\r\n yylineno++; return TOKEN(LEX_EOLN);
\n yylineno++; return TOKEN(LEX_EOLN);
[ \t\r\n]  return TOKEN(LEX_WS);

 /* gen_LEX_RULES _ident_values() */

[a-zA-Z_][a-zA-Z0-9_]*  return lex_return(LEX_IDENTIFIER);
-?[0-9]+\.[0-9]*        return lex_return(LEX_DOUBLE);
-?[0-9]+                return lex_return(LEX_INTEGER);

 /* LIST RW reserved word */

"if" return TOKEN(RW_if);

 /* LIST PUNCT */

"++" return TOKEN(PUNCT_PLUS_PLUS);
"--" return TOKEN(PUNCT_MINUS_MINUS);
"()" return TOKEN(PUNCT_L_PAR_PAR_R);
"[]" return TOKEN(PUNCT_L_SQUARE_SQUARE_R);
"." return TOKEN(PUNCT_DOT);
"->" return TOKEN(PUNCT_MINUS_GT);
"!" return TOKEN(PUNCT_NOT);
"~" return TOKEN(PUNCT_TILDE);
"*" return TOKEN(PUNCT_STAR);
"/" return TOKEN(PUNCT_SLASH);
"%" return TOKEN(PUNCT_PERCENT);
"+" return TOKEN(PUNCT_PLUS);
"-" return TOKEN(PUNCT_MINUS);
"<<" return TOKEN(PUNCT_LT_LT);
">>" return TOKEN(PUNCT_GT_GT);
"<=" return TOKEN(PUNCT_LT_EQUAL);
">=" return TOKEN(PUNCT_GT_EQUAL);
"<" return TOKEN(PUNCT_LT);
">" return TOKEN(PUNCT_GT);
"==" return TOKEN(PUNCT_EQUAL_EQUAL);
"!=" return TOKEN(PUNCT_NOT_EQUAL);
"===" return TOKEN(PUNCT_EQUAL_EQUAL_EQUAL);
"&" return TOKEN(PUNCT_AMP);
"^" return TOKEN(PUNCT_CARET);
"|" return TOKEN(PUNCT_PIPE);
"&&=" return TOKEN(PUNCT_AMP_AMP_EQUAL);
"||=" return TOKEN(PUNCT_PIPE_PIPE_EQUAL);
"&&" return TOKEN(PUNCT_AMP_AMP);
"||" return TOKEN(PUNCT_PIPE_PIPE);
"<<<" return TOKEN(PUNCT_LT_LT_LT);
">>>" return TOKEN(PUNCT_GT_GT_GT);
"<<=" return TOKEN(PUNCT_LT_LT_EQUAL);
">>=" return TOKEN(PUNCT_GT_GT_EQUAL);
"<<<=" return TOKEN(PUNCT_LT_LT_LT_EQUAL);
">>>=" return TOKEN(PUNCT_GT_GT_GT_EQUAL);
"=" return TOKEN(PUNCT_EQUAL);
"+=" return TOKEN(PUNCT_PLUS_EQUAL);
"-=" return TOKEN(PUNCT_MINUS_EQUAL);
"*=" return TOKEN(PUNCT_STAR_EQUAL);
"/=" return TOKEN(PUNCT_SLASH_EQUAL);
"%=" return TOKEN(PUNCT_PERCENT_EQUAL);
"&=" return TOKEN(PUNCT_AMP_EQUAL);
"^=" return TOKEN(PUNCT_CARET_EQUAL);
"|=" return TOKEN(PUNCT_PIPE_EQUAL);
"," return TOKEN(PUNCT_COMMA);
"(" return TOKEN(PUNCT_L_PAR);
")" return TOKEN(PUNCT_PAR_R);
"{" return TOKEN(PUNCT_L_CURLY);
"}" return TOKEN(PUNCT_CURLY_R);
"//" return TOKEN(PUNCT_SLASH_SLASH);
"/*" return TOKEN(PUNCT_SLASH_STAR);
"*/" return TOKEN(PUNCT_STAR_SLASH);

 /* LIST LEX */


. printf("Unknown token!\n"); yyterminate();

%%
// CODE SECTION // is copied through
// 

