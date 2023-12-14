#include "YY_PARSE.h"
#include "EXPRS_AFM.h"
#include "EXPRS_PRINTER.h"
#include "EXPRS_RETVAL.h"

#include "buffer2.h"

// OUTSIDE NAMESPACE

	using namespace EXPRS;
	// class EXPR :: api_function
	// ok = BUILD_EXPR_item IDX CTXT 
	// ok = TREE_ITEM " SCRIPT " IDX_locn CTXT 
	// ITEM.print_to( PRINT_HEAD )
	//  sweeps over Tree // clone_sweep_ready // 
	// into_tree // from_tree // tree_from ... // PSG_ARGV

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


 /*!
 	When YACC encounters STEP_from_DATA_io( we_feed_lex )
	it drives its own FSM LOCN-step-LOCN
	each step calls $$ = CSR.EXPR_branch_result(ARGS)

	This is a VTBL_CALL mostly fixed to TREE_BUILD add_leaf add_twig
	Those functions, sub-item-sub-type.fns, grow the PSG_SAYS Tree
	They allocate new tree_twigs_and_leaves in bunched streams // few delete
	Each when new was added to an index list ITEM_found_at( new_tuplo )
	That permits a check for unreachable items, from_malloc_to_RETVAL

		RETVAL_builder_t & TREE = get_RETVAL_builder(); // aka CSR
	
	That enables local #define CSR TREE; // local dialect over code

		RETVAL_tree_builder_csr CSR = get_RETVAL_builder();
	
	CSR provides a MALLOC service, and STO, using SEGMENT [s] and SIGNAL [s]
	
		CSR.mk_LHS_OP_RHS_CMNT( LHS, OP, RHS, CMNT ); // SIMPLES
		CSR_mk_LIST_as_queue( ARGS ); 
		CSR_un_verse( " MMAP_temp_area_upto_close " ) // plus close()
		CSR_close( "~ MMAP_temp_area_upto_close " ) // poss refcount

		USED: build tables into MEM2 then copy over to STO as NBYTES
		FAST: unused_items skipped space_recailable 
		NOTE: leak central says very low loss anyway ROM lean
		NOTE: leak central says MEM2 makes it easy in ROM_GEN

		CSR.mk_LHS_OP_RHS_CMNT( LHS, OP, RHS, CMNT ); // SIMPLES
		CSR.mk_POOL_ITEM_NAME( CSR_POOL, CSR_POOL_ITEM, "Name" );
		CSR.mk_Name( SPELLING_POOL, "Name" );
		// OPTION parse "Name" for SUBLEX for "any.str"
	
	When YACC is done we have PSG of better labelled TUPLES + texts

		SYMBOL PROCESSING TOKEN FACTORY
		a_few_types LOOKUP KEY1 // or more ARGS
		MATCH_TUPLES rules points case bools
	
	An alternative EXPR * RETVAL = $$ might be

		u32_hilo ea_expr_as_u32_in_STO = EXPR.EA_EXPR_i32;
	
	Some fool wants i32 not u32

		i32_hilo _t // all the same

		TYPE_TRAIT
		_is_WORD
		_is_PTR
		_is_IDX
		_is_EXPR
		_is_SIGN
		_is_EDGE
		_is_NAME
		_is_VECT
	
	Choice u8_u24 -or- u8_u8 // second uses first ...

		SEGMENT[ 0xFF ]

		CSR accumulates it's SEGMENT
		Various SUB- SGMT's get SPLICED_MERGED into SEGMENT

		ROM occupies one SEGMENT any number of SGMT
		TEMP SEGMENT_SGMT_TEMP CT_COPY_of_PREBUILT // _component

		ROM allocates u4 16 Files of u24_16M
		ROM allocates u4 SGMT
		ROM allocates u6 64 subzones of 256 (25% of total)

		CTXT decodes the u8, via simple OBJECT * REACHABLE_[u8]

 */
  // virtual
  EXPR * EXPR:: EXPR_branch_result() // wierd fold OFFS , EA_EXPR _32 as PTR_64
  {
 	RETVAL_builder_t & TREE = get_RETVAL_builder(); // aka CSR
	TREE.expr_tree = this; // already allocated, set "CSR" or other
	INFO("RETVAL get this to the call of yyparse");
	print_to_NULL();
	INFO("print_to NULL sweep complete ");
	return TREE.expr_tree;
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

// struct RETVAL_builder_t;
//  RETVAL_builder_t & get_RETVAL_builder();


 // everything (mostly) in EXPRS is a subclass of EXPR
 // EXPR base is outside the namespace

 // Tuples start here, or stay in PAIR
 // soon SEQ <--> STRUCT

 #if 0
 RETVAL_builder 
 add twig to PSGs perceived input tree
 ref REF_TWIG_t = EXPR_FOUND( treenode_id ) // CODE_POINT
 twig OPCODE LHS RHS CMNT //
  u16_hilo OPCODE = CODE_POINT/ROM4/LOCN_opcode
  u16_hilo LHS = DATA_STO_idx_expr // u8_pool u8_item // LOCN_t & u16_item //
  u16_hilo RHS = DATA_STO_idx_expr // u8_pool u8_item // LOCN_t & u16_item //
  u16_hilo CMNT = DATA_STO_idx_expr // u8_pool u8_item // STR_LOCN_t & u16_item //
  // that puts u16_item POOL = CTR_LOCN_t.POOL_for_(" u16_item ") // [256] [256]ZZ
 #endif

 struct EXPR_name : public EXPR {
 // "Name"
	str1 name;

 	EXPR_name( const char * _name ) 
	: name( _name )
	{
	}
	operator STR0() { return name; }
	void print_to( PRINTER * printer)
	{
		// uncalled
		printer -> ind_indent();
		if(0) {
		 printer -> print_STR( name );
		 printer -> out.print( " (EXPR_name) " );
		 printer -> out.print( "\n" );
		} else {
		 printer -> out.print( " EXPR_name( " );
		 printer -> print_STR( name );
		 printer -> out.print( " ) // CMNT\n" );
		}
	}
 };

 struct EXPR_rhs : public EXPR {
	EXPR * rhs;
	int op;

 //	str1 name; // NO NAME
 //	str1 CMNT; // or preparsed ...

 //	OPERATOR might be !exp exp! or (exp)

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

