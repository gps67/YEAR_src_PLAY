#ifndef lex_yacc_LEX_START_H
#define lex_yacc_LEX_START_H

/*
	RTFM
	 info bison
	  10 Start Conditions

	  In definitions section of .lex

	  	%x name1 name2
		%s name3 name4
	
	  	x means exclusive,	(I think this is my default)
		s means start condition
	
	  In Rules section

		LEXPR	LEX_ACTIONS BEGIN(name1)
	  	<name1>LEXPR LEX_ACTION 

		<name1>LEXPR ... BEGIN(INITIAL) ...

		<*>LEXPR	ECHO // match any|none start condition
	
	  Rules with no LEX_ACTION drop yytext at end // or start new one
	
	SO we need to keep a list of %x names and %s names and print them
	RULES (for now) are opaque LEXPR rules too (for now)

	So that is all we do here, append item to list, print list

	rename START_MODE or RULE_START_MODE
*/

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

// for building PUNCT_PLUS_EQUAL = "+="

// using namespace PSG;
namespace PSG {

class LEX_START_DECL : obj_ref0 { public:
	str1 Name;	// "name1"
	str1 Desc; 	// comments are complex

	LEX_START_DECL();
	~LEX_START_DECL();
};

class LEX_START_GROUP { public: // NOT AN OBJ_REF ???
	str1 PFX;
	obj_list<LEX_START_DECL> LIST_Token_s;
	obj_list<LEX_START_DECL> LIST_Token_x;

	LEX_START_GROUP();
	~LEX_START_GROUP();

	bool
 	 add_START(
	   STR0 name,			// "name1"
	   bool excl = true,		// %x 
	   STR0 desc = NULL	// "within a Q2 mode"
	);

// two lists, lookup, not needed anyway
//	bool lookup_by_Name( LEX_START_DECL * TOK, STR0 Name );

	bool print_lines( buffer1 & buf );
};



}; // namespace
#endif

