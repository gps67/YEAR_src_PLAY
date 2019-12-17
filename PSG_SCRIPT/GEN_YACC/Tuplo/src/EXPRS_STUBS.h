#ifndef EXPRS_STUBS_H
#define EXPRS_STUBS_H

// EXPRS_STUBS.h
// EXPR must be in global namespace
// EXPR is the base class of things in EXPRS namespace

namespace EXPRS {
 struct PRINTER;
 struct RETVAL_builder_t;
 // struct EXPR;
};

 // outside of namespace - because of YACC - BASE CLASS
 struct EXPR {
  virtual ~EXPR() {}
  virtual void print_to( EXPRS::PRINTER * printer) = 0;
  virtual void print_to_NULL(); // create a dummy printer, use copy out, drop
  virtual void EXPR_branch_result(); // all PSG types, EXPR too
	  bool detect_at_top(EXPRS::PRINTER*printer);
 };

namespace EXPRS {
  // the missing CTXT, Tree, Stack, (not stack though)
  // eg build tree in a specific SEGMENT
  RETVAL_builder_t & get_RETVAL_builder();

  extern EXPR * E0( const char * name );	// expr == IDEN_anystr
  extern EXPR * E1( int op_, EXPR * rhs_ ); // expr == op EXPR
  extern EXPR * E2( EXPR * lhs_, int op_, EXPR * rhs_ ); //expr== EXPR op EXPR
  extern EXPR * mk_E_id( const char * name_ ); //expr== IDEN_anystr // REPEAT ??
  extern EXPR * E1( int op_, EXPR * rhs_ );  // expr = TOKEN_unary RHS
  extern EXPR * E_plus_E( EXPR * lhs,  EXPR * rhs );  // EXPR + EXPR
  extern EXPR * E_times_E( EXPR * lhs,  EXPR * rhs ); // EXPR * EXPR
  extern EXPR * E_power_E( EXPR * lhs,  EXPR * rhs ); // EXPR ^ EXPR
  extern EXPR * E_paren( EXPR * rhs );
}; // namespace


#endif

