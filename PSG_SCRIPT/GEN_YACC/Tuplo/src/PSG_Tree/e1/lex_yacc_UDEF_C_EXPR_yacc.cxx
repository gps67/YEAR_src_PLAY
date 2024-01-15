#include "lex_yacc_UDEF.h"
using namespace PSG;

bool lex_yacc_UDEF:: add_yacc_for_C_EXPR() {
	YACC_type_list_1.add_type_rule( "expr", "expr_ident" );
	YACC_type_list_1.add_type_rule( "expr", "expr_ident" );
	YACC_type_list_1.add_type_rule( "expr", "expr" );
	YACC_type_list_1.add_type_rule( "expr", "EXPR_line" );
	YACC_type_list_1.add_type_rule( "expr", "LINES" );

	WARN("empty because e1.y.RULES does it");
	return true;
}

