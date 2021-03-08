#ifndef lex_yacc_LEX_TOKEN_H
#define lex_yacc_LEX_TOKEN_H

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

// for building PUNCT_PLUS_EQUAL = "+="
#include "lex_yacc_LEX_PUNCT_NAME.h"
#include "lex_yacc_LEX_TOKEN_precedence.h"

using namespace EXPRS;
using namespace PSG;
namespace PSG {

class LEX_TOKEN_DECL : obj_ref0 { public:
	str1 Name;
	str1 Value;
	operator_precedence_t op_info; // LAZY but adding here is easy, not all USE it
	str1 Desc; 	// only OPCODES have DESC as Name2

	LEX_TOKEN_DECL();
	~LEX_TOKEN_DECL();
};

class LEX_TOKEN_GROUP { public: // NOT AN OBJ_REF ???
	str1 PFX;
	obj_list<LEX_TOKEN_DECL> LIST_Token;

	LEX_TOKEN_GROUP();
	~LEX_TOKEN_GROUP();

	bool
 	 add_PUNCT(
	   STR0 punct,			// "=="
	   STR0 op_info_str = NULL,	// "L7" // %left pri 7 // 1=tightest
	   STR0 opcode_desc = NULL	// "cmp IS_EQUAL"
	 );

	bool
 	 add_PUNCT_4(    // PFX_
	   STR0 tail,  // "_EQUAL_EQUAL",
	   STR0 punct,  // "==" );
	   STR0 op_info_str = NULL,	// "L7" // %left pri 7 // 1=tightest
	   STR0 opcode_desc = NULL	// "cmp IS_EQUAL"
	) {
		LEX_TOKEN_DECL * token = new LEX_TOKEN_DECL();
		this->LIST_Token.append( token );
		token -> Name = tail;
		token -> Value = punct;
		token -> op_info . set_flags_str( op_info_str );
		token -> Desc = opcode_desc;
		return true;
	}

	bool
 	 add_RW(    // PFX_
	   STR0 rw  // "while" );
	) {
		LEX_TOKEN_DECL * token = new LEX_TOKEN_DECL();
		this->LIST_Token.append( token );
		token -> Name = rw;
		token -> Value = rw;
	//	token -> op_info.init_unused();
		return true;
	}

	bool
 	 add_LEX(    // PFX_
	   STR0 rw  // "EOLN" );
	) {
		LEX_TOKEN_DECL * token = new LEX_TOKEN_DECL();
		this->LIST_Token.append( token );
		token -> Name = rw;
		// NULL // token -> Value = rw;
		// token -> op_info.init_unused();
		return true;
	}

	bool lookup_by_Name( LEX_TOKEN_DECL * TOK, STR0 Name );
	bool lookup_by_Value( LEX_TOKEN_DECL * TOK, STR0 Value );
};



}; // namespace
#endif

