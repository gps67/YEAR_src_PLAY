#ifndef Y_PARSE_H
#define Y_PARSE_H

/*
	This is flawed - it should not matter that yyparse does not exist yet
	in test1 no actual parsing happens, it is all generating
	Probably switching to not-a-class-stic might fix it
*/

#include "EXPRS_STUBS.h"
#include "str1.h"

// namespace ??
namespace YY {
	// NOT YET
}; // namespace

 struct Y_Parse_t {
 	str1 Name;

	int call_yyparse();
 };

using namespace YY;

extern	int yyparse( Y_Parse_t * parser );
extern	void yyerror( Y_Parse_t * parser, const char * msg );
extern	bool gen_yyparse_parameter( buffer2 & out);
#endif


