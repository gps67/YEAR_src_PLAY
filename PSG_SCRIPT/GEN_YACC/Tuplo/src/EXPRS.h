#ifndef EXPR_H
#define EXPR_H

#include "str1.h"
#if 0
// still having problems giving NameSpace TypeName via FLEX 
namespace EXPRS {
 struct EXPR;
};
using namespace EXPRS;
#endif

/*
	import + REUSE .y token names in EXPRS

	eg PSG_Token PUNCT_CARET_as_EA_in_ROM_of_OBJECT

	u32_hilo EA_token = EA_in_STO_of_OBJECT = OBJ.EA_in_STO
		that is a foreign id placed in OBJ
		guarantee init_NULL init_M1 init_ZERO init_SOON
		if EA == ZERO || EA == M1
		 EA = CALL find_or_recompute OBJ.EA_in_STO
		if EA == ZERO || EA == M1
		 WARN null EA ; return FAIL_FAILED
		 // fail can grab warn if thats all we got
	so
	 rejig my use of EXPR to BASE
*/

// #warning "gen_e1_yacc.tab.hh" ...
#include "gen_e1_yacc.tab.hh"
// #warning "gen_e1_yacc.tab.hh" DONE

 struct EXPR {
  virtual ~EXPR() {}
  virtual void printer(int indent) = 0;
 };

namespace EXPRS {
 struct EXPR_lhs_rhs : public EXPR {
	EXPR * lhs;
	EXPR * rhs;
	int op;
	str1 name;
 //	str1 CMNT; // or preparsed ...

	// const char * name;
	EXPR_lhs_rhs()
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
	EXPR_lhs_rhs * E = new EXPR_lhs_rhs();
	E->name = name;
	return E;
}
EXPR * E2( EXPR * lhs_, int op_, EXPR * rhs_ )
{
	EXPR_lhs_rhs * E = new EXPR_lhs_rhs();
	E->lhs = lhs_;
	E->rhs = rhs_;
	E->op = op_;
	return E;
}
EXPR * mk_E_id( const char * name_ ) {
	EXPR_lhs_rhs * E = new EXPR_lhs_rhs();
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
	return E2( lhs, PUNCT_STAR, rhs );
}
EXPR * E_power_E( EXPR * lhs,  EXPR * rhs ) {
	return E2( lhs, PUNCT_CARET, rhs );
}

}; // namespace

// SYNTAX OK // was union field name not type
// typedef ::EXPRS::EXPR EXPR_t2;
		using namespace EXPRS;
// SYNTAX OK // was union field name not type
// typedef EXPR EXPR_t;
#endif
