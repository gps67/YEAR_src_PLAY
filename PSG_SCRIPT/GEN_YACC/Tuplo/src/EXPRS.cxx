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
// I need to GEN foreach NEWITEM * TreeNode_TYPE_04
// 04 // C++ Style fragments of code
// new TreeNode_LOCN for a PSG::EXPRS_TREE::
// match EXPR[s] s=idx = EA_EXPR( HEAP, ITEM ); // HEAP=EXPRS ITEM=expr _.idx
// MORPH_EXPR_suffix( HEAP, ITEM ); // HEAP=EXPRS // ITEM=expr _.idx
// MATCH // HERE // no meta CMNT but can guess(attr, here)
// MATCH // HEAP=EXPRS // binding that happens with the LINK_LOAD_ROM_ITEM
// MATCH // HEAP=EXPRS // connect var, set LEX_DIALECT as my_ref_your_ref
// foreach in list of new TreeNode Types
// CODE_UP LOCN( "E0( name )"
// CODE_UP LOCN( "E1( op_code, rhs )" ) // SPEC_LINK( "E1( lhs, op_code )" )
// CODE_UP LOCN( "E1( op_code, data )" ) // SPEC_LINK( "E1( lhs, op_code )" )
// CODE_UP LOCN( "E1( lhs, op_code rhs )" ) // SPEC_LINK // seq on input line
// CODE_UP LOCN( "E2( lhs, op_code rhs )" ) // LAYOUT_COLLECT code_point

// using namespace EXPRS;
namespace EXPRS {

EXPR * E0( const char * name )		// expr == IDEN_anystr
{
	EXPR_name * E = new EXPR_name( name );
	return E;
}
EXPR * E1( int op_, EXPR * rhs_ ) // expr = TOKEN_unary RHS
{
	EXPR_rhs * E = new EXPR_rhs( op_, rhs_);
	return E;
}
EXPR * E2( EXPR * lhs_, int op_, EXPR * rhs_ ) // expr == EXPR op EXPR
{
	EXPR_lhs_rhs * E = new EXPR_lhs_rhs( lhs_, op_, rhs_ );
	return E;
}
EXPR * mk_E_id( const char * name_ ) {	// expr == IDEN_anystr // REPEAT ??
	return E0( name_ );
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

