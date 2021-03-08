
#include "yacc_lex_UDEF_CALC.h"
#include "yacc_lex_UDEF_CALC.h"
using namespace PSG;


// #include "yacc_lex_LEX_PUNCT_NAME.h"


// yacc_lex_UDEF is a BASE CLASS with OPTS eg C_EXPR

// _CALC is derived from _UDEF


/* inherit
bool yacc_lex_UDEF_CALC:: build_tree() {
	if(! build_tree_lex() ) return FAIL_FAILED(); 
	if(! build_tree_yacc() ) return FAIL_FAILED(); 
	return true;
}
*/

bool yacc_lex_UDEF_CALC:: build_tree_lex() {
	if(!add_lex_for_C_EXPR()) return FAIL_FAILED();
	INFO("C_EXPR");
	return true;
}
bool yacc_lex_UDEF_CALC:: build_tree_yacc() {
	if(!add_yacc_for_C_EXPR()) return FAIL_FAILED();
	INFO("C_EXPR");
	// bool add_yacc_for_C_EXPR();
	return true;
}

