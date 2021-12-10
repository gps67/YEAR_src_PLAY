
#include "lex_yacc_UDEF_CALC.h"
#include "lex_yacc_UDEF_CALC.h"
using namespace PSG;


// #include "lex_yacc_LEX_PUNCT_NAME.h"


// lex_yacc_UDEF is a BASE CLASS with OPTS eg C_EXPR

// _CALC is derived from _UDEF


/* inherit
bool lex_yacc_UDEF_CALC:: build_tree() {
	if(! build_tree_lex() ) return FAIL_FAILED(); 
	if(! build_tree_yacc() ) return FAIL_FAILED(); 
	return true;
}
*/

bool lex_yacc_UDEF_CALC:: build_tree_lex() {
	// COMMENTS need START condition section 10
	POOL_START.add_START("CMNT_CPP_TO_EOLN", true, "CXX line comment");
	POOL_START.add_START("CMNT_C", true, "C comment");
	POOL_START.add_START("STRING_C", true, "CXX string");

	if(!add_lex_for_C_EXPR()) return FAIL_FAILED();
	INFO("C_EXPR");
	return true;
}
bool lex_yacc_UDEF_CALC:: build_tree_yacc() {
	if(!add_yacc_for_C_EXPR()) return FAIL_FAILED();
	INFO("C_EXPR");
	// bool add_yacc_for_C_EXPR();
	return true;
}

