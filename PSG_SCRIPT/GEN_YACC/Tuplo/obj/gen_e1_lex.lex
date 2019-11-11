
%{

// GEN
#include <string>
#include "buffer1.h"
#include "../obj/gen_e1_yacc.tab.hh"
/*
        lookahead may mean any number of tokens, not 1

        quick test shows actually OK 1 as long as yacc strdup's immediately
        but maybe
*/

// static const int nlex16 = 64;
   static const int nlex16 = 16;
// static const int nlex16 = 1;
static buffer1 lex_pool[nlex16];
static int nlex_pos = 0;

/*
        lex found, return tok

        lex has found the string at yytext, yyleng
        use next storage slot (reuse nlex16 items later)
        copy string to buffer, as str0
        set yylval /union/ .lex_buff as returning string value
        return tok as lex type

 */
int lex_return( int tok )
{
        // skip first cell once, get it next time, or init -1
        nlex_pos = (nlex_pos+1) % nlex16;
        int pos = nlex_pos;
        lex_pool[ pos ].set( yytext, yyleng );
        // TESTED with includes removed FOUND
        // YYtext and yyleng are available
        // yylval isnt - it is the union later in the same gen2 file
        // LATER means LINE 2200
        // EXCEPT this is LINE 3630
        // 
        // 
        yylval.lex_buff = (str0) lex_pool[ pos ];
        return tok;
}

// define SAVE_TOKEN  yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t)    (yylval.token = t)

#include "../obj/gen_e1_yacc.tab.hh"

%}

%option noyywrap

%%

\r\n	yylineno++; return TOKEN(LEX_EOLN);
\n	yylineno++; return TOKEN(LEX_EOLN);
[ \t\r\n]	return TOKEN(LEX_WS);
[a-zA-Z_][a-zA-Z0-9_]*	return lex_return( LEX_IDENTIFIER );
-?[0-9]+\.[0-9]*	return lex_return( LEX_DOUBLE );
-?[0-9]+	return lex_return( LEX_INTEGER );

"if" return TOKEN(RW_if);

"<<=" return TOKEN(PUNCT_LT_LT_EQUAL);
">>=" return TOKEN(PUNCT_GT_GT_EQUAL);
"==" return TOKEN(PUNCT_EQUAL_EQUAL);
"!=" return TOKEN(PUNCT_NOT_EQUAL);
"<=" return TOKEN(PUNCT_LT_EQUAL);
">=" return TOKEN(PUNCT_GT_EQUAL);
"<<" return TOKEN(PUNCT_LT_LT);
">>" return TOKEN(PUNCT_GT_GT);
"++" return TOKEN(PUNCT_PLUS_PLUS);
"*" return TOKEN(PUNCT_STAR);
"+" return TOKEN(PUNCT_PLUS);
"^" return TOKEN(PUNCT_CIRC);
"<" return TOKEN(PUNCT_LT);
">" return TOKEN(PUNCT_GT);
"=" return TOKEN(PUNCT_EQUAL);
"(" return TOKEN(PUNCT_L_PAR);
")" return TOKEN(PUNCT_PAR_R);
"{" return TOKEN(PUNCT_L_CURLY);
"}" return TOKEN(PUNCT_CURLY_R);
"." return TOKEN(PUNCT_DOT);
"-" return TOKEN(PUNCT_MINUS);
"/" return TOKEN(PUNCT_SLASH);
"&" return TOKEN(PUNCT_AMP);
"%" return TOKEN(PUNCT_PERCENT);
"|" return TOKEN(PUNCT_PIPE);
"~" return TOKEN(PUNCT_TILDE);


. printf("Unknown token!\n"); yyterminate();

%%

