#include "Y_PARSE.h"
#include "dgb.h"

using namespace YY;

bool 
gen_yyparse_parameter( buffer2 & out)
{
//	out.printf("%%parse-param {Y_Parse_t * parser} // PARAMETER \n");
	out.printf("%%parse-param {Y_Parse_t & psg} // PARAMETER \n");
//	out.printf("%%code provides \n{ Y_Parse_t & psg = * parser; \n } \n");
//	out.printf("%%code requires \n{ Y_Parse_t & psg = * parser; \n } \n");
//	out.printf("%%code \n{ Y_Parse_t & psg = * parser; \n } \n");
// none of them - all landed outside of yyparse
	return true;
}

void yyerror( Y_Parse_t & parser, const char * msg )
{
	FAIL("msg %s", msg );
}
