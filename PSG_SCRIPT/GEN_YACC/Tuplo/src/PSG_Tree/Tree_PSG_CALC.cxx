
#include "Tree_PSG_CALC.h"
// #include "Tree_PSG_LEX_PUNCT_NAME.h"


// Tree_PSG_UDEF is a BASE CLASS with OPTS eg C_EXPR

// _CALC is derived from _UDEF


/* inherit
bool Tree_PSG_CALC:: build_tree() {
	if(! build_tree_lex() ) return FAIL_FAILED(); 
	if(! build_tree_yacc() ) return FAIL_FAILED(); 
	return true;
}
*/

bool Tree_PSG_CALC:: build_tree_lex() {
	if(!add_lex_for_C_EXPR()) return FAIL_FAILED();
	INFO("NOT bool add_yacc_for_C_EXPR()");
	return true;
}
bool Tree_PSG_CALC:: build_tree_yacc() {
	if(!add_yacc_for_C_EXPR()) return FAIL_FAILED();
	// bool add_yacc_for_C_EXPR();
	INFO("NOT bool add_yacc_for_C_EXPR()");
	return true;
}
