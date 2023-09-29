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

 // START Conditions - exclusive

/* CXX line comment */
%x CMNT_CPP_TO_EOLN

/* C comment */
%x CMNT_C

/* CXX string */
%x STRING_C



%{
// within definitions %{ is A CODE SECTION BEGIN // like %TOP see 5.1 

// gen_LEX_code_start() // headers 
//somehow before here is stddef stdio - added by flex
#include "buffer1.h"
#include "Y_PARSE.h"
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
#define TOKEN(t)    (yylval.tokn = t)

/* yacc generated TOKENs header UNION YYSTYPE */
#include "../obj/gen_JSON_yacc.tab.hh"
 // CODE section END
%}

/* still in definitions section */

/* yywrap is a macro called at every end of file */
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

"if"     return TOKEN(RW_if);

 /* gen_LEX_RULES _eoln() AFTER RW_s */

\r\n yylineno++; return TOKEN(LEX_EOLN);
\n yylineno++; return TOKEN(LEX_EOLN);
[ \t\r\n]  return TOKEN(LEX_WS);

 /* gen_LEX_RULES _ident_values() */

[a-zA-Z_][a-zA-Z0-9_]*  return lex_return(LEX_IDENTIFIER);
-?[0-9]+\.[0-9]*        return lex_return(LEX_DOUBLE);
-?[0-9]+                return lex_return(LEX_INTEGER);
<STRING_C>[^"]*        return lex_return(LEX_STRING);

 /* LIST PUNCT */
 /* beware .123 float seeing PUNCT_DOT */

"++"     return TOKEN(PUNCT_PLUS_PLUS);
"--"     return TOKEN(PUNCT_MINUS_MINUS);
"()"     return TOKEN(PUNCT_L_PAR_PAR_R);
"[]"     return TOKEN(PUNCT_L_SQUARE_SQUARE_R);
"."      return TOKEN(PUNCT_DOT);
"->"     return TOKEN(PUNCT_MINUS_GT);
"!"      return TOKEN(PUNCT_NOT);
"~"      return TOKEN(PUNCT_TILDE);
"*"      return TOKEN(PUNCT_STAR);
"/"      return TOKEN(PUNCT_SLASH);
"%"      return TOKEN(PUNCT_PERCENT);
"+"      return TOKEN(PUNCT_PLUS);
"-"      return TOKEN(PUNCT_MINUS);
"<<"     return TOKEN(PUNCT_LT_LT);
">>"     return TOKEN(PUNCT_GT_GT);
"<="     return TOKEN(PUNCT_LT_EQUAL);
">="     return TOKEN(PUNCT_GT_EQUAL);
"<"      return TOKEN(PUNCT_LT);
">"      return TOKEN(PUNCT_GT);
"=="     return TOKEN(PUNCT_EQUAL_EQUAL);
"!="     return TOKEN(PUNCT_NOT_EQUAL);
"==="    return TOKEN(PUNCT_EQUAL_EQUAL_EQUAL);
"&"      return TOKEN(PUNCT_AMP);
"^"      return TOKEN(PUNCT_CARET);
"|"      return TOKEN(PUNCT_PIPE);
"&&="    return TOKEN(PUNCT_AMP_AMP_EQUAL);
"||="    return TOKEN(PUNCT_PIPE_PIPE_EQUAL);
"&&"     return TOKEN(PUNCT_AMP_AMP);
"||"     return TOKEN(PUNCT_PIPE_PIPE);
"<<<"    return TOKEN(PUNCT_LT_LT_LT);
">>>"    return TOKEN(PUNCT_GT_GT_GT);
"<=>"    return TOKEN(PUNCT_LT_EQUAL_GT);
"<<="    return TOKEN(PUNCT_LT_LT_EQUAL);
">>="    return TOKEN(PUNCT_GT_GT_EQUAL);
"<<<="   return TOKEN(PUNCT_LT_LT_LT_EQUAL);
">>>="   return TOKEN(PUNCT_GT_GT_GT_EQUAL);
"="      return TOKEN(PUNCT_EQUAL);
"+="     return TOKEN(PUNCT_PLUS_EQUAL);
"-="     return TOKEN(PUNCT_MINUS_EQUAL);
"*="     return TOKEN(PUNCT_STAR_EQUAL);
"/="     return TOKEN(PUNCT_SLASH_EQUAL);
"%="     return TOKEN(PUNCT_PERCENT_EQUAL);
"&="     return TOKEN(PUNCT_AMP_EQUAL);
"^="     return TOKEN(PUNCT_CARET_EQUAL);
"|="     return TOKEN(PUNCT_PIPE_EQUAL);
","      return TOKEN(PUNCT_COMMA);
"("      return TOKEN(PUNCT_L_PAR);
")"      return TOKEN(PUNCT_PAR_R);
"{"      return TOKEN(PUNCT_L_CURLY);
"}"      return TOKEN(PUNCT_CURLY_R);
"//"     return TOKEN(PUNCT_SLASH_SLASH);
"/*"     return TOKEN(PUNCT_SLASH_STAR);
"*/"     return TOKEN(PUNCT_STAR_SLASH);

 /* LIST LEX */


. printf("Unknown token!\n"); yyterminate();

 /* rules section END*/
%%
// code section BEGIN // to eof // is copied through
// 

