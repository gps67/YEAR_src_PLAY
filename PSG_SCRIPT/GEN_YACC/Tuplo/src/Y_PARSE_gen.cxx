#include "Y_PARSE.h"
#include "dgb.h"

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

 // NO // extern int yylineno;
 // NO // extern char * yytext;

//void yyerror( YYLTYPE * locp, Y_Parse_t & parser, const char * msg )
void yyerror( Y_Parse_t & parser, const char * msg )
{
	// NO // INFO("yylineno %d yytext'%s", yylineno, "yytext" );
	FAIL("msg:Y_parse_gen.cxx %s", msg );
}

/*
	see Y_Parse.cxx
	for Y_Parse:: call_yyparse()
	ret_from_yyparse = yyparse( * this ); // matches T & V

*/
