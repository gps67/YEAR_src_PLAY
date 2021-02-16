#ifndef Tree_PSG_LEX_TOKEN_precedence_H
#define Tree_PSG_LEX_TOKEN_precedence_H

#include "str0.h"
#include "PSG_STUBS.h"
using namespace PSG;

namespace EXPRS {

/*!
	precedence is the priority. and side-binding left or right

	The lower the number the tighter the binding, and happens first

	Used as part of LEX_TOKEN_DECL, within _GROUP.

	Invoked from LEX_TOKEN_GROUP LEX_POOL. add_PUNCT_4()

	Uses a STR0 prec == "L3"

	TODO: probably not used ATM, using order of source to do that

	ZERO means unset, and various LEX's have no side-bind nor precedence

*/
struct operator_precedence_t {

	// or make this enum
	bool assoc_left;
	bool assoc_right;

	int precedence; // 1 = tightest // 0 == unset

	// could be 2-step namespace/item address of a sentient type
	// could be a looked-up by name NODE or TOK -selects- NODE
	operator_precedence_t( STR0 flags = NULL )
	: assoc_left( false )
	, assoc_right( false )
	, precedence( 0 ) // default 0 // use lowest of 1 otherwise
	{
		set_flags_str( flags );
	} 

	void set_flags_str( str0 flags ); // move to not inline

	void init_unused() {} ; // leave as was, as CTOR left it
	void set_left() { assoc_left = true; }
	void set_right() { assoc_right = true; } 
	void set_precedence(int p) {
		precedence = p;
	}
};

};
#endif

