
#include "EXPRS_PRINTER.h"

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

	bool detect_at_top(PRINTER*printer)
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
 	 void print_to( PRINTER * printer)
	 {
		buffer2 & out = printer->buff();
		// print INDENT "+"
		printer->ind_indent();
		out.print("[ OP ] %s\n", str_of_token(op) );

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
		buffer2 & out = printer->buff();

		if(lhs) {
			printer->print_EXPR_indented( lhs );
		} else {
			WARN("UNEXPECTED NULL");
		}

		// print INDENT "+"
		printer->ind_indent();
		out.print("%s [OP]\n", str_of_token(op) );

		if(rhs) {
			printer->print_EXPR_indented( rhs );
		} else {
			WARN("UNEXPECTED NULL");
		}
	}
}; // STRUCT struct EXPR_lhs_rhs : public EXPR {}
}; // NAMESPACE
