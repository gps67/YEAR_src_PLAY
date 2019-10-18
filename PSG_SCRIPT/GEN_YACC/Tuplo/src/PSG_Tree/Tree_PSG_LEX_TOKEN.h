#ifndef Tree_PSG_LEX_TOKEN_H
#define Tree_PSG_LEX_TOKEN_H

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

// for building PUNCT_PLUS_EQUAL = "+="
#include "Tree_PSG_LEX_PUNCT_NAME.h"


class LEX_TOKEN_DECL : obj_ref0 { public:
	str1 Name;
	str1 Value;

	LEX_TOKEN_DECL();
	~LEX_TOKEN_DECL();
};

class LEX_TOKEN_GROUP { public: // NOT AN OBJ_REF ???
	str1 PFX;
	obj_list<LEX_TOKEN_DECL> LIST_Token;

	LEX_TOKEN_GROUP();
	~LEX_TOKEN_GROUP();

	bool
 	 add_PUNCT( STR0 punct ); // "==" ); // STR0 tail,  // "_EQUAL_EQUAL"  

	bool
 	 add_PUNCT(    // PFX_
	   STR0 tail,  // "_EQUAL_EQUAL",
	   STR0 punct  // "==" );
	) {
		LEX_TOKEN_DECL * token = new LEX_TOKEN_DECL();
		this->LIST_Token.append( token );
		token -> Name = tail;
		token -> Value = punct;
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
		return true;
	}

	bool lookup_by_Name( LEX_TOKEN_DECL * TOK, STR0 Name );
	bool lookup_by_Value( LEX_TOKEN_DECL * TOK, STR0 Value );
};



#endif

