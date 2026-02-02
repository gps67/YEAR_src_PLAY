#ifndef TOKENISER_H
#define TOKENISER_H
#include <tcl.h>
#include <string.h>

/*
	Caller / loader usually provides the following function
	which sets this up:
*/

extern int tokeniser_init_restricted( Tcl_Interp * interp_client );
extern int tokeniser_init_trusted( Tcl_Interp * interp_client );


/*
	class tokeniser

	this parses the tcl to bits
	it doesn't re-assemble it

	(its quick to append to strings and compile them)
*/

extern "C" {
Tcl_ObjCmdProc tokeniser_new;
}
//Tcl_ObjCmdProc tokeniser_call;

#define CONST const

struct tokeniser
{
	Tcl_Interp *interp;	// retval
	Tcl_Command tcl_command;
	int must_free;
	Tcl_Parse   parsed;
	const char *	script_str;
	int	script_len;
	const char *	remain_str;
	int	remain_len;
	Tcl_Token * csr1;		// private to conv_token_list WORD
	Tcl_Token * csr1_end_pos;	// private to conv_token_list
/*
	now local
	Tcl_Token * csr2;		// private to conv_token_list PART
	Tcl_Token * csr2_end_pos;	// private to conv_token_list
*/

	tokeniser( Tcl_Interp *interp1 );
	int call( int objc, Tcl_Obj *CONST objv[] );
	~tokeniser( void );

	void set_script( const char * str, int len );
	void set_script( Tcl_Obj * script );
	void do_free( void );

	Tcl_Obj * parse_one_command( int with_gaps = 0 );
	Tcl_Obj * bug_scan_variable_tokens( int n );
	Tcl_Obj * conv_word_list();
	Tcl_Obj * conv_next_word();
	Tcl_Obj * conv_next_token();
	Tcl_Obj * mk_string( const char * s );
	Tcl_Obj * mk_string( const char * s, int len );
	Tcl_Obj * mk_list0( );
	Tcl_Obj * mk_list1( Tcl_Obj * a );
	Tcl_Obj * mk_list2( Tcl_Obj * a, Tcl_Obj * b );
	Tcl_Obj * mk_list3( Tcl_Obj * a, Tcl_Obj * b, Tcl_Obj * c );
	int       mk_list_append( Tcl_Obj * list, Tcl_Obj * item );

	int parse_script( Tcl_Obj * str );
	int parse_command( Tcl_Obj * str );
	int parse_expr( Tcl_Obj * str );
	int parse_var( Tcl_Obj * str );
	int parse_quotes( Tcl_Obj * str );
	int parse_braces( Tcl_Obj * str );

	int retval( Tcl_Obj * ret );
	int retval_error( Tcl_Obj * ret );
	int retval_error( const char * ret );

};
#endif
