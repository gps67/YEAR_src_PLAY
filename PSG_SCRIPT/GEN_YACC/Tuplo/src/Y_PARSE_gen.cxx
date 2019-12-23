#include "Y_PARSE.h"
#include "dgb.h"

using namespace YY;

bool 
gen_yyparse_parameter( buffer2 & out)
{
	out.printf("%%parse-param {Y_Parse_t * parser} // PARAMETER \n");
	return true;
}

void yyerror( Y_Parse_t * parser, const char * msg )
{
	FAIL("msg %s", msg );
}
