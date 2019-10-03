
#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

#include "Tree_PSG_UDEF.h"

bool bool_main( int argc, char ** argv ) {
	set_prog_alias( argv[0] ); // or other ?


 if(1) {
 	// looks nice when called properly // announce action
	INFO("GENERATOR  %s", argv[0]); // PAIR || Tuplo_duo || Derived TYPE Combiner = { ... item LHS item RHS ... }
	INFO(" LHS  LEX  %s", argv[1]); // LHS
	INFO(" RHS YACC  %s", argv[2]); // RHS
	// TEST //INFO(argv[3]); // NULL // "(null)" //  // " (null) " // 
 }
 if(argc!=3) {
 	return FAIL("%d of 3 parameters plus ZERO", argc);
 }

	/*	PSG - the SPEC side of the DATA Tree
		PSG_UDEF - user defined (here) // for (purpose)

		 see PSG_UDEF.cxx
	*/

	Tree_PSG_UDEF PSG;
 	PSG.lex_name_lex = argv[1]; // LHS
 	PSG.yacc_name_yacc = argv[2]; // RHS

 if(0) {
	e_print(" PSG.lex_name_lex %s\n", PSG.lex_name_lex );
	e_print(" PSG.yacc_name_yacc %s\n", PSG.yacc_name_yacc );
 }

	if(! PSG.build_tree() ) {
		return FAIL_FAILED();
	}

	if(! PSG.print_tree_as_files() ) {
		return FAIL_FAILED();
	}
	return true;
}

int main( int argc, char ** argv )
{
	if( bool_main( argc, argv ))
	{
		return 0;
	} else {
		if(errno) {
		 //
		 // OUTPUT DIAGNOSTIC -errno // LOOB 0=OK 1=INFO -1=ERROR
		 //
		 // STDERR DIAGNOSTIC
		 //
		 //	Message
		 //
		 // DIAGNOSE {
		 //
		 //	diagnostic_from_ MODULE // topic = Module //
		 //	fact_list_re_topic EXPR_POOL // needs a KDB twist
		 //
		 // }
		 // _EVENT_._STO_ {
		 //
		 //	ALREADY_DONE user_already_notified_by by_dialog_box
		 //	ALREADY_DONE user_already_notified_by by_errout_done
		 //	IT SAYS errno == EACCESS // Permission denied
		 //	IT SAYS errno == E2BIG // CPU_PAUSED_or_SHUTDOWN
		 //	IT SAYS errno == EADDRNOTAVAIL // 100_spellings_of_ENO
		 //	WANT exit_code = E-NO_PERMCCESS
		 //
		 // }
		 // Report_Any_Residual_Error() // flushing buffers on exit {
		 //	FAIL_FAILED(); // first FAIL sets up DIAG_nostic
		 //	Thats a repeat call, but flushes buffers via ...
		 //	So put any code in (wherever FAIL_FAILED() goes)
		 //	/home/gps/YEAR/src/libs_apps/src/lib_base/dgb/dgb.h
		 //	#define FAIL_FAILED() FAIL("FAILED") // false
		 //	goes near dgb.cxx void do_ssl_ERR(); 
		 // }
		 FAIL_FAILED();

		 return errno; // FORGOT ? NEGATE ABS(errno)
		}
		//
		// perror 22
		// # FAIL # 22 # Invalid argument #
		//
		return 22; /* Invalid argument */
		//
		// /* LEX */
		//
	}
} // main
