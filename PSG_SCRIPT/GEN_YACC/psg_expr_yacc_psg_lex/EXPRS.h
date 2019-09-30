#include "str1.h"

namespace EXPRS {
 struct EXPR;
};
using namespace EXPRS;
#include "e1.tab.hh"

namespace EXPRS {
 struct EXPR {
	EXPR * lhs;
	EXPR * rhs;
	int op;
	str1 name;
	// const char * name;
	EXPR()
	{
		init_null();
	}
	void init_null()
	{
		lhs = NULL;
		rhs = NULL;
		op = ' ';
		name = (const char *) NULL;
	}
	void ind(int indent) {
		for(int i=0; i<indent; i++ ) {
			printf("| ");
		}
	}
	void printer(int indent) {
		if(!indent)
			printf("#### line ####\n");
		if(name) {
			ind(indent);
			printf("%s\n", (STR0)name );
			return;
		}
		if(lhs) {
			lhs->printer(indent+1);
		}
		ind(indent);
		printf("%s\n", str_of_token(op) );
		if(rhs) {
			rhs->printer(indent+1);
		}
	}
};
EXPR * E1( const char * name )
{
	EXPR * E = new EXPR();
	E->name = name;
	return E;
}
EXPR * E2( EXPR * lhs_, int op_, EXPR * rhs_ )
{
	EXPR * E = new EXPR();
	E->lhs = lhs_;
	E->rhs = rhs_;
	E->op = op_;
	return E;
}
EXPR * mk_E_id( const char * name_ ) {
	EXPR * E = new EXPR();
	E->name = name_;
	return E;
}
EXPR * E1( int op_, EXPR * rhs_ ) {
	return E2( NULL, op_, rhs_);
}
EXPR * E_plus_E( EXPR * lhs,  EXPR * rhs ) {
	return E2( lhs, PUNCT_PLUS, rhs );
}
EXPR * E_times_E( EXPR * lhs,  EXPR * rhs ) {
	return E2( lhs, PUNCT_ASTERISK, rhs );
}
EXPR * E_power_E( EXPR * lhs,  EXPR * rhs ) {
	return E2( lhs, PUNCT_CARET, rhs );
}

};
