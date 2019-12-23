#include "Y_PARSE.h"
#include "dgb.h"

using namespace YY;

int Y_Parse_t::
call_yyparse()
{
	int t = yyparse( this ); 
	switch (t) {
	 case 0:
		INFO("0 means PASS yyparse");
	 break;
	 case 1:
		FAIL("1 means ERROR yyparse");
	 break;
	 case 2:
		FAIL("2 means MEMORY ERROR yyparse");
	 break;
	}
	return t;
}

void yyerror( Y_Parse_t * parser, const char * msg )
{
	FAIL("msg %s", msg );
}
