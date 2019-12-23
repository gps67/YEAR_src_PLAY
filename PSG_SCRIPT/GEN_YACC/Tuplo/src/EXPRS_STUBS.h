#ifndef EXPRS_STUBS_H
#define EXPRS_STUBS_H

#include "STO_i32_hilo.h"

	struct buffer2; // not in a namespace ?

// EXPRS_STUBS.h
// EXPR must be in global namespace
// EXPR is the base class of things in EXPRS namespace

namespace EXPRS {
 struct PRINTER;
 struct RETVAL_builder_t;
 // struct EXPR;
};

using namespace STO;

 struct EXPR_u8_u24 {
  //
  u32_hilo u8_u24;	// converting PTR to int, to int

// rework using DEFAULT CTOR rules ?
// default copy ctor copies each field ...

  EXPR_u8_u24 ( const EXPR_u8_u24 & rhs )
  : u8_u24( rhs.u8_u24)
  { }

  // comvert u32 to u32_hilo
  EXPR_u8_u24 ( u32 & val ): u8_u24( val ) {}
  EXPR_u8_u24 ( i32 & val ): u8_u24( val ) {}

  void set( u32 val ) { u8_u24 = val; }
  void set( i32 val ) { u8_u24 = val; }

  u8 get_u8() { return (u32) u8_u24 & 0xFF; } 
  u32 get_u24() { return (u32) u8_u24 >> 8; } 

  u32 set_u8_u24( u8 idx, u32 offs ) {
  	u8_u24 = (offs << 8) + idx;
  	return u8_u24;
 };
 }; // EXPR_u8_u24

 // outside of namespace - because of YACC - BASE CLASS
 struct EXPR {
  virtual ~EXPR() {}
  virtual void print_to( EXPRS::PRINTER * printer) = 0;
  virtual void print_to_NULL(); // create a dummy printer, use copy out, drop
  virtual EXPR * EXPR_branch_result(); // all PSG types, EXPR too
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

