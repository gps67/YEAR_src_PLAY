#include "EXPRS.h"
#include "EXPRS_PRINTER.h"

// OUTSIDE NAMESPACE

	using namespace EXPRS;

	bool EXPR:: detect_at_top(EXPRS::PRINTER*printer)
	{
		// detect and act
		if(!printer->indent) {
			return true;
			printf("#### line #### SINGLE TOKEN\n");
			return true;
		}
		return false;
	}

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
// ALL THE EXPR_ classes are part hidden
// --------------------------

namespace EXPRS {


 // everything (mostly) in EXPRS is a subclass of EXPR
 // EXPR base is outside the namespace

 // Tuples start here, or stay in PAIR
 // soon SEQ <--> STRUCT

 struct EXPR_name : public EXPR {
	str1 name;

 	EXPR_name( const char * _name ) 
	: name( _name )
	{
	}
	void print_to( PRINTER * printer)
	{
		// uncalled
		printer -> ind_indent();
		printer -> print_STR( name );
		printer -> out.print( " (EXPR_name) " );
		printer -> out.print( "\n" );
	}
 };

 struct EXPR_rhs : public EXPR {
	EXPR * rhs;
	int op;
 //	str1 name; // NO NAME
 //	str1 CMNT; // or preparsed ...

	// const char * name;
	EXPR_rhs( int TOK_OP, EXPR * rhs_ )
	{
		init_null();
		rhs = rhs_;
		op = TOK_OP;
	}
	void init_null()
	{
		rhs = NULL;
		op = ' ';
		// name = (const char *) NULL;
	}

  // virtual
 	 void print_to( PRINTER * printer)
	 {
		printer->ind_indent();
		printer->print("%s [OP]\n", str_of_token(op) );

		if(rhs) {
			printer->print_EXPR_indented( rhs );
		}
	}
 };

 struct EXPR_lhs_rhs : public EXPR {
	EXPR * lhs;
	EXPR * rhs;
	int op;
//	str1 name;
 //	str1 CMNT; // or preparsed ...

	// const char * name;
	EXPR_lhs_rhs()
	{
		init_null();
	}
	EXPR_lhs_rhs( EXPR * _lhs, int _op, EXPR * _rhs )
	{
		lhs = _lhs;
		rhs = _rhs;
		op = _op;
//		name = (const char *) NULL;
	}
	void init_null()
	{
		lhs = NULL;
		rhs = NULL;
		op = ' ';
//		name = (const char *) NULL;
	}

  // virtual
 	 void print_to( PRINTER * printer)
	 {
		if(lhs) {
			printer->print_EXPR_indented( lhs );
		} else {
			WARN("UNEXPECTED NULL");
		}

		printer->ind_indent();
		printer->print("%s [OP]\n", str_of_token(op) );

		if(rhs) {
			printer->print_EXPR_indented( rhs );
		} else {
			WARN("UNEXPECTED NULL");
		}
	}
}; // STRUCT struct EXPR_lhs_rhs : public EXPR {}
}; // NAMESPACE
 


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
	EXPR_name * E = new EXPR_name( name_ );
	return E;
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
EXPR * E_paren( EXPR * rhs ) { // EXPR ^ EXPR
	EXPR * E = new EXPR_rhs( PUNCT_L_PAR, rhs );
	return E;
	// return E1( PUNCT_L_PAR, rhs );
}

}; // namespace

