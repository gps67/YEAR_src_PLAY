#include "lex_yacc_RULE.h"

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

// #include "lex_yacc_LEX_TOKEN.h"
// #include "lex_yacc_LEX_START.h"
// #include "lex_yacc_UNION.h"
// #include "lex_yacc_YACC_type_list.h"

#include "PSG_STUBS.h"

using namespace PSG;
namespace PSG { }


#if 0
	enum RULE_TYPE {
	 rule_ONE_OF,
	 rule_SEQ,
	 rule_DATA, // PSG_STRUCT _SPEC DECODER( TOKEN ) { DIAG } api
//	 rule_ _LIST _VECT _ITEM_t api EXPRS("anystr")
	 rule_TOKEN, // PLUS // alias explains // UDEF
	 rule_lex_ITEM, // n_bytes probably maybe P0P2
	 rule_NAMED, // ITEM_NAMED //
	 rule_all_of_these_are_unwritten // at the mo //
	};
#endif

lex_yacc_RULE::
	lex_yacc_RULE ( STR0 _name, union_field_t * _union_field )
	: name(_name)
	, union_field(_union_field)
	{
	 if(_union_field) {
		INFO("%s RET .%s", _name, (STR0) union_field->union_field_name);
	 } else {
		INFO("%s RET .NULL", _name);
	 }
	}

/*
	bool gen_some()
	{
		union_field_t *
		union_field
		= new union_field_t( "expr", "EXPR * expr" );
		lex_yacc_RULE * R1 = new lex_yacc_RULE( "E1", union_field );

		return FAIL("TODO");
	}
*/

class lex_yacc_RULE_list : public obj_list<lex_yacc_RULE> {
	// as derive // obj_list<lex_yacc_RULE> rule_list;

};

