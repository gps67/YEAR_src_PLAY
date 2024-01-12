#ifndef lex_yacc_UNION_H
#define lex_yacc_UNION_H

#include "PSG_STUBS.h"
#include "dgb.h"
#include "str1.h"
#include "buffer2.h"
#include "dyn_array.h"

// using namespace PSG;

namespace PSG {
class union_field_t { public: /* LEX return field YACC return field */

	str1 union_field_name; // expr // yacc name for field // rule name
	str1 union_field_line; // EXPR * expr // C_DECL line of field

	// LIST used in rules 
	union_field_t() 
//	: union_field_name( _union_field_name )
//	, union_field_line( _union_field_line )
	{ 
		INFO("INIT_EMPTY");
	}

	// LIST used in rules 
	union_field_t(
	 STR0 _union_field_name,	//     "int_val"
	 STR0 _union_field_line		// "int int_val" // no ';'
	) 
	: union_field_name( _union_field_name )
	, union_field_line( _union_field_line )
	{ 
		INFO("%s {%s}", _union_field_name, _union_field_line );
	}

	// LIST used in rules 
	bool set_name_line(
	 STR0 _union_field_name,	//     "int_val"
	 STR0 _union_field_line		// "int int_val" // no ';'
	) 
	{ 
		union_field_name = _union_field_name;
		union_field_line = _union_field_line;
		INFO("%s {%s}", _union_field_name, _union_field_line );
		return true;
	}

	bool print_with_semicolon( buffer2 & out);
};

class union_field_list_t { public:
	dyn_array<union_field_t *> union_field_list;

	union_field_t * lookup_add( STR0 name, STR0 line );
	bool print_lines( buffer2 & out);
};

}; // NAMESPACE

#endif
