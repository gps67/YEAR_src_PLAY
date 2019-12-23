
#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

#include "STO.h"

#include "Tree_PSG_UDEF.h"
#include "Tree_PSG_CALC.h"

#include "Y_PARSE.h" // to provide uncalled thing

bool bool_main( int argc, char ** argv ) {
	set_prog_alias( argv[0] ); // or other ?


 if(1) {
 	// looks nice when called properly // announce action
	INFO("GENERATOR  %s", argv[0]); // PAIR || Tuplo_duo || Derived TYPE Combiner = { ... item LHS item RHS ... }
	INFO(" PSG name %s", argv[1] );

//	INFO(" LHS  LEX  %s", argv[1]); // LHS
//	INFO(" RHS YACC  %s", argv[2]); // RHS
	// TEST //INFO(argv[3]); // NULL // "(null)" //  // " (null) " // 
 }
 if(argc!=2) {
 	return FAIL("%d of 2 parameters plus ZERO", argc);
 }

	/*	PSG - the SPEC side of the DATA Tree
		PSG_UDEF - user defined (here) // for (purpose)

		 see PSG_UDEF.cxx
	*/

	// PSG = Bench.the_thing_we_are_building();
	// Tree_PSG_UDEF PSG;
	Tree_PSG_CALC PSG; 
	// PSG = the_thing_we_are_building();
	// PSG is held on STACK for duration, then END_SCOPE_DTOR();
	// PSG is coded and loaded PREBUILT
	// PSG.build_tree() ) { ... }
	// PSG.build_tree_code() ) { ... }
	// PSG.build_tree_data() ) { ... }
	// PSG.print_tree_as_files() ) { ... } // for FLEX + BISON
	// PSG.print_tree_as_files() ) { ... } // for V8 // for API
	// PSG.print_code_as_files() ) { ... } // for API
	// PSG.print_data_as_files() ) { ... } // for API
	// PSG.print_api_as_module() ) { ... } // for API
	// PSG.print_api_as_module_component() ) { ... } // INA_POOL
	/* TODO

		RUN filter OVER tree

		ITERATE wave OVER tree

		PREBUILT result = CHUNKS of STO in ROM + DEX

		CALLER might_provide OFFERS api_stack

		 CACHE the remote Behaviour locally
		  let it run a careful subset of builtins 
		  let it EDIT an objects data and VERSION
		  let it respond to EVENT -> SCRIPT // global thread_var_X 
		 RUN dgb the_code 
		  FILTER use test_data to activate DETECTOR_SENSOR
		   OPTION COMPILE FOR THIS CASE
		    transpile the script to C++
		    transpile the script to ES6
		    transpile the script to wasm
		    transpile the script to SCRIPT
		    link_load_ELF_ROM_SEGMENTS
		    link_load_ELF_JIT_SCRIPTS // prepared earlier
		    MMAP_FILE_script_and_STO // SCRIPT_to_ASM_ELF
		    MMAP_FILE_script_and_STO // SCRIPT_to_CPU_x86_64
		    MMAP_FILE_script_and_STO // SCRIPT_to_CPU_arm_64
		    MMAP_FILE_script_and_STO // SCRIPT_to_C_own_vtbl
		    MMAP_FILE_script_and_STO // SCRIPT_to_Py_own_vtbl

		   FILTER clone FROM / INTO clone2
		    ACTIOIN ONEACH {
		     CLONE item into
		    }

	// OK // some math on CLONE src dst
	// Another pass Tree_Broadcast( "FINALISE" ); // DRAIN before close()
	// OK // that may involve one more WRITE_FILE.so and dl_open()
	// dl_open might find a SEGMENT by name // laters maybe //
	// dl_open might find a SYMBOL by name // init_Module_init(something);
	// dl_open might EDIT a SYMBOL=writable_data by name // get_EA_global()
	*/


 	// PSG.set_PSG_name( argv[1] ); // PSG_Name // gen_e1 //
 	if(!PSG.set_PSG_name( argv[1] )) FAIL_FAILED();
	// yes it was called (nut data didn't stick)

	// then it is immediate ACCESS after STORE
 if(1) {
	buffer2 tmp_str;
	INFO(" PSG.lex_name_lex()     %s\n", PSG.lex_name_lex(tmp_str) );
	INFO(" PSG.yacc_name_y()      %s\n", PSG.yacc_name_y(tmp_str) );
	INFO(" PSG.yacc_name_tab_hh() %s\n", PSG.yacc_name_tab_hh(tmp_str) );
 }

	// build the PSG into ROM_CACHE // clone able MEM buffers
	if(! PSG.build_tree() ) {
		return FAIL_FAILED();
	}

	// build the PSG into YACC_RULES_PSG
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
		 //	WANT exit_code = E-NO_PERM E-ACCESS // nor mullets
		 //	KNOW prefer cfg use_own_
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


