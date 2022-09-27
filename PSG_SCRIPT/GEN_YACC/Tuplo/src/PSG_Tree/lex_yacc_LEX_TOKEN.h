#ifndef lex_yacc_LEX_TOKEN_H
#define lex_yacc_LEX_TOKEN_H

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

// for building "PUNCT_PLUS_EQUAL" from "+="
#include "lex_yacc_LEX_PUNCT_NAME.h"
#include "lex_yacc_LEX_TOKEN_precedence.h"

using namespace EXPRS;
using namespace PSG;
namespace PSG {

class LEX_TOKEN_DECL : obj_ref0 { public:

	str1 Name;	// eg "PLUS_EQUAL"
	str1 Value;	// eg "+="
	operator_precedence_t op_info; // not all LEX groups are operators
	str1 Desc; 	// only OPCODES have DESC as Name2

	LEX_TOKEN_DECL();
	~LEX_TOKEN_DECL();
};

/*!
	Each type of TOKEN has a POOL GROUP.

	A LIST in order of declaration.
*/
class LEX_TOKEN_GROUP { public: // NOT AN OBJ_REF ???

	str1 PFX; // PUNCT_ LEX_ RW_ //
	obj_list<LEX_TOKEN_DECL> LIST_Token;

	/*
		PFX is carried by the GROUP not each ITEM (not yet)
		There should be a few GROUPS // PUNCT_ LEX_ RW_ //
	*/

	LEX_TOKEN_GROUP();
	~LEX_TOKEN_GROUP();

	STR0 NAME_of_C1C2( buffer1 & PLUS_PLUS_buff, STR0 c1c2 );

	bool
 	 add_PUNCT(
	   STR0 punct,			// "==" // convert to "EQUAL_EQUAL"
	   STR0 op_info_str = NULL,	// "L7" // %left pri 7 // 1=tightest
	   STR0 opcode_desc = NULL	// "cmp IS_EQUAL"
	 ) {
		LEX_TOKEN_DECL * token = new LEX_TOKEN_DECL();
		this->LIST_Token.append( token );
		buffer1 PLUS_PLUS_buff;
		token -> Name = NAME_of_C1C2( PLUS_PLUS_buff, punct );
		token -> Value = punct;
		token -> op_info . set_flags_str( op_info_str );
		token -> Desc = opcode_desc;
		return true;
	}

	bool
 	 add_RW(    			// PFX_ "RW_"
	   STR0 rw  			// "while" );
	) {
		LEX_TOKEN_DECL * token = new LEX_TOKEN_DECL();
		this->LIST_Token.append( token );
		token -> Name = rw;
		token -> Value = rw;
	//	token -> op_info.init_unused();
		return true;
	}

	bool
 	 add_LEX(    			// PFX_ "LEX_"
	   STR0 rw  			// "EOLN" );
	) {
		LEX_TOKEN_DECL * token = new LEX_TOKEN_DECL();
		this->LIST_Token.append( token );
		token -> Name = rw;
	// NULL token -> Value = rw; // CTOR auto NULL // Name == Value
	//	token -> op_info.init_unused(); // CTOR auto
		return true;
	}

	bool lookup_by_Name( LEX_TOKEN_DECL * TOK, STR0 Name );
	bool lookup_by_Value( LEX_TOKEN_DECL * TOK, STR0 Value );
};



}; // namespace
#endif

