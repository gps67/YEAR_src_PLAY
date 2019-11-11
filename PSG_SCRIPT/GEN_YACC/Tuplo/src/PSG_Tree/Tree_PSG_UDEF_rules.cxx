
#include "Tree_PSG_UDEF.h"
// #include "Tree_PSG_LEX_PUNCT_NAME.h"

bool Tree_PSG_UDEF:: build_tree_yacc() {

#if 0
	Tree_PSG_RULE_SEQ * SEQ = new Tree_PSG_RULE_SEQ( "expr_ident" );
	SEQ -> name = "expr_ident";
	SEQ -> add_step_LEX( "WS" );
	SEQ -> add_step_RULE( "expr_ident" );
	SEQ -> set_code(" $$ = $2; ");
#endif

	return true;
}

