#ifndef lex_yacc_RULE_H
#define lex_yacc_RULE_H

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

// #include "lex_yacc_LEX_TOKEN.h"
// #include "lex_yacc_LEX_START.h"
#include "lex_yacc_UNION.h"
// #include "lex_yacc_YACC_type_list.h"

#include "PSG_STUBS.h"

using namespace PSG;
namespace PSG {

class lex_yacc_RULE { public:

	str1 name;		// "expr_ident" // -> one_of_seq( tbs )

	union_field_t * union_field; // "expr" "EXPR * EXPR"

		// so no need for type_rule to gen  // %type <expr> expr_ident
		// ie merge required
		// NALEX // gen and run //

	lex_yacc_RULE ( STR0 _name, union_field_t * _union_field );

	bool gen_some()
	{
		union_field_t *
		union_field
		= new union_field_t( "expr", "EXPR * expr" );
		lex_yacc_RULE * R1 = new lex_yacc_RULE( "E1", union_field );

		return FAIL("TODO");
	}
};

class lex_yacc_RULE_list : public obj_list<lex_yacc_RULE> {
	// as derive // obj_list<lex_yacc_RULE> rule_list;

};
}; // namespace
#endif
