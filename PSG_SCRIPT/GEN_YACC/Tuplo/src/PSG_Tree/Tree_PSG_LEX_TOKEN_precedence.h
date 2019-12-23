#ifndef Tree_PSG_LEX_TOKEN_precedence_H
#define Tree_PSG_LEX_TOKEN_precedence_H

#include "str0.h"

namespace EXPRS {

static const int operator_assoc_LEFT = 0x1;
static const int operator_assoc_RIGHT = 0x2;
static const int operator_assoc_priority_times = 0x4;

struct operator_precedence_t {
	int info;

	bool assoc_left;
	bool assoc_right;
	bool priority;
	// could be 2-step namespace/item address of a sentient type
	// could be a looked-up by name NODE or TOK -selects- NODE
	operator_precedence_t( STR0 flags = NULL )
	: priority( 0 ) // default 0 // use lowest of 1 otherwise
	{
		set_flags_str( flags );
	} 

	void set_flags_str( str0 flags ); // move to not inline

	void init_unused() {} ; // leave as was, as CTOR left it
	void set_left() { info |= operator_assoc_LEFT; }
	void set_right() { info |= operator_assoc_RIGHT; }
	void set_precedence(int p) {
		info &= 0x03 ;
		info |= (p <<2) ;
	}
};

};
#endif

