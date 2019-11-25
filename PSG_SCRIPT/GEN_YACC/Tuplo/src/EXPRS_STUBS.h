#ifndef EXPRS_STUBS_H
#define EXPRS_STUBS_H


namespace EXPRS {
 struct PRINTER;
 // struct EXPR;
};

 // outside of namespace - because of YACC - BASE CLASS
 struct EXPR {
  virtual ~EXPR() {}
  virtual void print_to( EXPRS::PRINTER * printer) = 0;
  virtual void print_to_NULL(); // create a dummy printer, use copy out, drop
	  bool detect_at_top(EXPRS::PRINTER*printer);
 };

namespace EXPRS {
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

