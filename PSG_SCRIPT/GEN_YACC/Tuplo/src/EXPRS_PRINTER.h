#ifndef EXPR_PRINTER_H
#define EXPR_PRINTER_H

#include "EXPRS.h"

namespace EXPRS {}

# if 0
 struct EXPR {
  virtual ~EXPR() {}
  virtual void print_to( EXPRS::PRINTER * printer) = 0;
  virtual void print_to_NULL(); // create a dummy printer, use copy out, drop
 };
//typedef EXPRS::EXPR EXPR;
#endif


namespace EXPRS {

 struct PRINTER
 { public:
  buffer2 out;
  int indent;
  virtual ~PRINTER() {
  }
  PRINTER() {
  	indent = 0;
  }

  buffer2 & buff() { return out; }

  void print_EXPR ( EXPR * expr ) 
  {
  	// undecided if always checking is slower than checking always
	// maybe silence this auto-path-completion
	// maybe decide on return true; // from empty branch
	// maybe decide on return false; // from empty branch
  	if(!expr) { WARN("NULL expr"); return; }
	// ACTION = 
	expr -> print_to( this );
  }
  void print_EXPR_indented ( EXPR * expr ) {
  	if(!expr) { WARN("NULL expr"); return; }
	// modified ACTION // let compiler inline above here ?
  	indent ++;
	print_EXPR( expr );
  	indent --;
  }
  void printf ( const char * fmt, ... ) {
	va_list args;
	va_start( args, fmt );
  	out.vprint(false, fmt, args );
  }
  void print ( const char * fmt, ... ) {
	va_list args;
	va_start( args, fmt );
  	out.vprint(false, fmt, args );
  }
  void print_STR ( const char * str ) {
  	out.print("%s", str );
  }

  void ind_indent()
  {
	if(!indent) {
		if(0) out.print("#### line #### ind_indent detected \n");
		// return;
	}
	// zero can still output
  	ind(indent);
  }

  void ind(int _ind) {
	/* SELF */ printf("ind(%d)##", indent );
	for(int i=0; i<_ind; i++ ) {
		printf("| ");
	}
  }
 };


};
#endif

