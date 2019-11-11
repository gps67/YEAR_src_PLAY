#include "EXPRS.h"

// OUTSIDE NAMESPACE

  // virtual
  void EXPR:: print_to_NULL() // create a dummy printer, use copy out, drop
  {
  	EXPRS::PRINTER * printer = new EXPRS::PRINTER();
	if(!printer) { FAIL("null printer from CTOR"); return ; }

  	this->print_to( printer);

	e_print("%s\n", (STR0) printer->out );
	delete printer;
  }

// --------------------------

// using namespace EXPRS;
namespace EXPRS {

EXPR * E1( const char * name )		// expr == IDEN_anystr
{
	// using PAIR to hold nothing, definitely NAME used
	EXPR_lhs_rhs * E = new EXPR_lhs_rhs();
	E->name = name;
	return E;
}
EXPR * E2( EXPR * lhs_, int op_, EXPR * rhs_ ) // expr == EXPR op EXPR
{
	EXPR_lhs_rhs * E = new EXPR_lhs_rhs();
	E->lhs = lhs_;
	E->rhs = rhs_;
	E->op = op_;
	return E;
}
EXPR * mk_E_id( const char * name_ ) {	// expr == IDEN_anystr // REPEAT ??
	EXPR_lhs_rhs * E = new EXPR_lhs_rhs();
	E->name = name_;
	return E;
}
EXPR * E1( int op_, EXPR * rhs_ ) { // expr = TOKEN_unary RHS
	EXPR * e_NULL = NULL;
	return E2( e_NULL, op_, rhs_);
}
EXPR * E_plus_E( EXPR * lhs,  EXPR * rhs ) { // EXPR + EXPR
	return E2( lhs, PUNCT_PLUS, rhs );
}
EXPR * E_times_E( EXPR * lhs,  EXPR * rhs ) { // EXPR * EXPR
	return E2( lhs, PUNCT_STAR, rhs );
}
EXPR * E_power_E( EXPR * lhs,  EXPR * rhs ) { // EXPR ^ EXPR
	return E2( lhs, PUNCT_CARET, rhs );
}

}; // namespace

