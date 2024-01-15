#ifndef lex_yacc_YACC_type_list_H
#define lex_yacc_YACC_type_list_H

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

// #include "lex_yacc_LEX_type_list.h"

using namespace EXPRS;
using namespace PSG;
namespace PSG {

class YACC_type_rule : obj_ref0 { public:

	// %type <type_name> rule_name 
	str1 type_name;	// eg "PLUS_EQUAL"
	str1 rule_name;	// eg "+="

	YACC_type_rule( STR0 T, STR0 R )
	: type_name(T)
	, rule_name(R)
	{
	}

	~YACC_type_rule()
	{
	}

	bool print_decl_line( buffer2 & out )
	{
		return out.print("%%type <%s> %s\n",
			(STR0) type_name,
			(STR0) rule_name );
	}
};

/*!
	each rule R has a return type T
*/
class YACC_type_list { public: // NOT AN OBJ_REF ???

	obj_list<YACC_type_rule> type_list;
	
	bool add_type_rule( STR0 T, STR0 R )
	{
		int N = type_list.N();
		for( int i = 0; i< N; i++ ) {
			if( type_list[i]->rule_name == R ) {
				if( type_list[i]->type_name == T ) {
					INFO("DUPLICATE %s %s", T, R );
					return true;
				}
				STR0 T1 = type_list[i]->type_name;
				INFO("DIFFERENT (%s) %s %s", T1, T, R );
				return true;
			}
		}
		YACC_type_rule * decl = new YACC_type_rule( T, R );
		type_list.append( decl );
		INFO("DECL %s %s", T, R );
		return true;
	}

	bool print_list( buffer2 & out )
	{
		int N = type_list.N();
		for( int i = 0; i< N; i++ ) {
			type_list[i]->print_decl_line( out );
		}
		return true;
	}

}; // class

}; // namespace
#endif
