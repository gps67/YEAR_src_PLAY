/*
  LEX file.
  +-------------
  |  definitions
  | %%
  |  rules
  | %%
  |  user code
  +-------------


   this is in definitions SECTION // see 5.0

*/

 // no LEX START Conditions
 // ns 0 nx 0 

%{
// within definitions %{ is A CODE SECTION BEGIN // like %TOP see 5.1 

// gen_LEX_code_start() // headers 
//somehow before here is stddef stdio - added by flex
#include "buffer1.h"
#include "YY_PARSE.h"
using namespace YY;
/* yacc generated TOKENs header UNION YYSTYPE */
#include "../obj/gen_JSON_yacc.tab.hh"

/*
// GEN	// gen_LEX_lex_return() // gen LEX lex_return() ////////////////
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
        // pos == nlex_pos // this kept lex in circular loop of buffer1
        int pos = nlex_pos;
        // 
        // step to next slot in round_robin loop
        nlex_pos = (nlex_pos+1) % nlex16;
        // 
        // save a copy of the the text of the lex
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
#define SET_TOKEN(t)    (yylval.tokn = t)

/* yacc generated TOKENs header UNION YYSTYPE */
#include "../obj/gen_JSON_yacc.tab.hh"
 // CODE section END
%}

/* still in definitions section */

/* NONE: yywrap is a macro called at every end of file */
%option noyywrap

 /* definions section END */
%%
 /* rules section BEGIN */

 /* added START condition rules for comments and string */

"/*"	{
	BEGIN(CMNT_C);
	}
<CMNT_C>[^*\n]*        /* eat anything that's not a '*' */
<CMNT_C>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<CMNT_C>\n             yylineno++;
<CMNT_C>"*"+"/"	BEGIN(INITIAL);




 /* LIST RW reserved word */

"JSON_RESERVED_WORD_none" return SET_TOKEN(RW_JSON_RESERVED_WORD_none);

 /* gen_LEX_RULES _eoln() AFTER RW_s */

\r\n yylineno++; return SET_TOKEN(LEX_EOLN);
\n yylineno++; return SET_TOKEN(LEX_EOLN);
[ \t\r\n]  return SET_TOKEN(LEX_WS);

 /* gen_LEX_RULES _ident_values() */

[a-zA-Z_][a-zA-Z0-9_]*  return lex_return(LEX_IDENTIFIER);
-?[0-9]+\.[0-9]*        return lex_return(LEX_DOUBLE);
-?[0-9]+                return lex_return(LEX_INTEGER);
<STRING_C>[^"]*        return lex_return(LEX_STRING);

 /* LIST PUNCT */
 /* beware .123 float seeing PUNCT_DOT */


 /* LIST LEX */


 /* INCL LEX */

 /* INCL */ 

 /* INCL // including file '../src/JSON.lex_RULES' */

 /* INCL // --------------- START ----------------- */
 /* INCL // ABSENT FILE '../src/JSON.lex_RULES' */
 /* INCL // --------------- END ----------------- */
 /* INCL // included file '../src/JSON.lex_RULES' */
 /* INCL */
. printf("Unknown token!\n"); yyterminate();

 /* rules section END*/
%%
// code section BEGIN // to eof // is copied through
// 

