#include "Y_PARSE.h"
#include "dgb.h"

// this goes into gen_XXX_yacc.y // the PSG decl
// which gets into gen_XXX_yacc.tab.cc // the machine

using namespace YY;

/*
	see %lex-param for ARG to yyparse
	https://www.gnu.org/software/bison/manual/html_node/Pure-Calling.html

	%lex-param
	%parse-param
	%param

		int
		yylex(
	//	 %define api.pure full
	//	 moves yylval yylloc from local to ARGS 
		 YYSTYPE * lval, // *lval = retval_value // retval = token
	//	 YYLTYPE * lloc,
		 PARAMETER_PLUS
		);

		int yyparse(
		 PARAMETER_PLUS
		);

	%define api.pure full
	%locations

	YY S TYPE
	YY L TYPE

*/

bool 
gen_yyparse_parameter( buffer2 & out)
{
	out.printf("\n"); // blank line

	out.printf("// %%parse-param { FIRST } { SECOND }\n");
	out.printf("// PARAMETER { Type * Item }  // to yyerror \n");
	out.printf("// see https://www.gnu.org/software/bison/manual/html_node/Parser-Function.html \n");
	out.printf("// C might not complain about missing parameters \n");
	out.printf("//\n");
//	out.printf("%%parse-param {Y_Parse_t * parser} // PARAMETER \n");
	out.printf("%%parse-param {Y_Parse_t & psg} // PARAMETER // NB REF AMP \n");

//	out.printf("%%code provides \n{ Y_Parse_t & psg = * parser; \n } \n");
//	out.printf("%%code requires \n{ Y_Parse_t & psg = * parser; \n } \n");
//	out.printf("%%code \n{ Y_Parse_t & psg = * parser; \n } \n");
// none of them - all landed outside of yyparse

	// out.printf("%%define api.pure full\n"); // see detailed
	return true;
}

 #if 0
 extern int yylineno;
 extern char * yytext;
 #endif
 // it is a local to obj/gen_XXX_lex.cc
 // it is a exported from obj/gen_XXX_lex.cc
 // order on the command line
 // this is the gen_yacc not run it
 /*
 g++
 test1_gen_yacc.o
 Y_PARSE_gen.o
 lex_yacc_LEX_PUNCT_NAME.o
 lex_yacc_LEX_TOKEN.o
 lex_yacc_LEX_TOKEN_precedence.o
 lex_yacc.o
 lex_yacc_UDEF.o
 lex_yacc_UDEF_C_EXPR_lex.o
 lex_yacc_UDEF_C_EXPR_yacc.o
 lex_yacc_UDEF_CALC.o
 lex_yacc_UDEF_AFM.o
 STO_E32.o
 -Wl,--rpath=/home/gps/YEAR/src/PLAY/libs_apps/lib
 -Wl,--rpath=/home/gps/YEAR/src/PLAY/libs_apps/build_LGX_E6320
 -L/home/gps/YEAR/src/PLAY/libs_apps/lib
 -L/home/gps/YEAR/src/PLAY/libs_apps/build_LGX_E6320
 -lbase
 -o test1_gen_yacc.exec

/usr/bin/ld: Y_PARSE_gen.o: in function `yyerror(Y_Parse_t&, char const*)':
/home/gps/YEAR/src/PLAY/PSG_SCRIPT/GEN_YACC/Tuplo/obj/../src/Y_PARSE_gen.cxx:70: undefined reference to `yylineno'

 */

 // NO // extern int yylineno;
 // NO // extern char * yytext;

//void yyerror( YYLTYPE * locp, Y_Parse_t & parser, const char * msg )
 #if 0
void yyerror( Y_Parse_t & parser, const char * msg )
{
	// NO // INFO("yylineno %d yytext'%s", yylineno, "yytext" );

 #if 0
	// 1867
 	INFO("yylineno %d yytext'%s", yylineno, "yytext" );
 #endif
	FAIL("msg:Y_parse_gen.cxx %s", msg );
}
 #endif

/*
	see Y_Parse.cxx
	for Y_Parse:: call_yyparse()
	ret_from_yyparse = yyparse( * this ); // matches T & V

*/
