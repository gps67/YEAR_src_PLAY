#ifndef dgb_fail_H
#define dgb_fail_H

#include "misc.h"
#include "ints.h"
#include "dgb.h"

/*!
	bad_bad_base is an exception base class - ie throw + catch
	rad_fail _base # CODE_POINT (* nearby comments *) dgb_locals

	dgb_fail
	dgb_rad_fail
	dgb_radical_fail
	dgb_mild_fail
	dgb_var_tracker_fail
	dgb_var_expr	# _t name # (that means assign default = "EXPR")
			# token "EXPR" set on locn { CTOR_ROM_VAR _EXPR }
			# 

	GOOD
		RETVAL = EXPR = ...
		return OK == "true"; // reserved word RETVAL

		usage "if(!func_name( args ) THEN ELSE CMNT" "func_name" proto 
		 BENCH
		  VIEW {
		  	CODE_POINT += Local_Layout { // FRAME
			 // example of how to call func_name(args) 
			 // upgrade (and check) if(! EXPR ) ... // API_EXPR_CALL
			 // mini_machine_batch += DATA CALLS from SCRIPTS C
			 // mini_machine += _batch // an AUTO_CODE_DIAG
			}
		  	CODE_POINT += sub_positions_in_sample_text_template {
			 // 
			 // upgrade (and check) if(! EXPR ) ... // API_EXPR_CALL
			}
		  }

	BAD
		FALSE_absent_var // aufo_fix defines local lint checks

			close tray failed // was already closed
			file not found // sign_off write_new_file _request
			as_expected, as_was,
			quota minimalist best logfile_quota
			 create an FSM mini_sub
			 ROM += pulse_point
			  code_point
			  data_avail
			  code_for_zone // HERE is the 
			 dgb += new pulse_point( varname ); // item.varname

			 save dgb as "item.dgb" // auto_gain %s item -varname-

	RADICAL
		RAD_error_report_from_API // code_point api_wired dbg_switch

			FILE exceeds 15K // and not in git's known list
			STOP and ask USER // a radical action
			PAUSE and run SCRIPT // better

			CONTINUE
			RETURN RETVAR_t RETVAR; bool 
			RETURN RETVAL_t RETVAL;
			RETURN bool OK; // init TRUE; default ASIS;
			COMPILE SCRIPT calling it with ARGV // CACHE RESULT //
			THROW dgb_rad_fail

	FAIL
		FAIL error_disk_full // MSG += 256 bytes data

			RADICAL says FAIL throw_report 

			  _dgb_

			CODE_POINT_func_locals // VIEW_xFF // FRAME_u8
			DIAG {
			  VAR DIAG PLUS 
			  VAR SPEC invokes DIAG_POOL[( view, opts, bind )]
			   view_zero # may be the only view # optimise out EA
			   // TOKEN DIAG_POOL[ITEM]; //  ITEM = "( view, opts, bind )"
			}
	_dgb_
		module data pool for DATA 
		generated code for SPEC
		compiled code for SCRIPT

	CURRENTLY it ALWAYS throws an exception.

	I disagree with C++ not allowing 'return report_error("message");'
	when the function is void (and report error is also void). So
	I use an underline where the space was after return.

	if( !connection ) return_VOID_dgb_fail( "SQL connection failed" );

	It may also be appropriate for a program to continue,
	eg with return NULL.

	if( !connection) return NULL_dgb_fail( "SQL connect failed" );

	There may be future options for an exception to be handled
	by a pop-up and the user allowed to stay. This particularly applies
	to debugging, when gdb can then see what would have happened,
	possibly with a manually created return value.
*/
class dgb_fail_base : public GRP_lib_base
{
	const char * msg;
 public:
	/*!
		constructor - exception is a single string message

		Option of calling gdb as well as jumping back.
	*/
	dgb_fail_base( const char * _msg = "EXCEPTION" );
	void caught();
	~dgb_fail_base();
};

/*!
	bad mad is an exception type - the same as dgb_fail_base
*/
class dgb_fail : public dgb_fail_base
{
 public:
	dgb_fail( const char * _msg = "EXCEPTION1" );
	~dgb_fail();
};

/*!
	optional exception. User can continue with NULL
*/
extern void * NULL_dgb_fail( const char * msg );

/*!
	optional exception. User can continue with FALSE
*/
extern bool FALSE_dgb_fail( const char * msg );

/*!
	An exception.
	not an optional one.
*/
extern void THROW_dgb_fail( const char * msg );

/*
	will someone please explain why C++ cannot return VOID
	It not being real is not an excuse for it not existing
*/
#define return_VOID_dgb_fail( msg ) { THROW_dgb_fail( msg ); return; }

#endif
