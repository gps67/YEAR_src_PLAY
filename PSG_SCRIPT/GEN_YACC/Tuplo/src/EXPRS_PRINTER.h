#ifndef EXPR_PRINTER_H
#define EXPR_PRINTER_H

#include "EXPRS_STUBS.h"
#include "buffer2.h"

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

  /*!
  	pass all printf requests to buffer2 out
  */
  void printf ( const char * fmt, ... ) {
	va_list args;
	va_start( args, fmt );
  	out.vprint(false, fmt, args );
  }
  /*!
  	print is like printf - for those who want it
  */
  void print ( const char * fmt, ... ) {
	va_list args;
	va_start( args, fmt );
  	out.vprint(false, fmt, args );
  }

  /*!
  	puts ... no mention of CHARSET
  */
  void print_STR ( const char * str ) {
  	out.print("%s", str );
  }

  /*!
  	output indent worth of indent
  */
  void ind_indent()
  {
  	ind(indent);
  }

  /*!
  	output ind worth of indent
  */
  void ind(int _ind) {
	if(!indent) {
		// zero can still output
		if(0) out.print("#### line #### ind_indent detected \n");
	}

	/* SELF */ printf("ind(%d)##", indent );
	for(int i=0; i<_ind; i++ ) {
		printf("| ");
	}
  }
 };


};
#endif

