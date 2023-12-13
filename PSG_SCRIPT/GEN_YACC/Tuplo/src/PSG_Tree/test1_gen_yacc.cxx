
#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

#include "STO.h"

#include "lex_yacc_UDEF.h"
#include "lex_yacc_UDEF_AFM.h"
#include "lex_yacc_UDEF_CALC.h"
#include "lex_yacc_UDEF_JSON.h"

using namespace PSG;

#include "Y_PARSE.h" // to provide uncalled thing

bool bool_main( int argc, char ** argv ) {
	dgb_fork_stderr_to_tcl_text();
	set_prog_alias( argv[0] ); // or other ?

 if(argc!=4) {
 	return FAIL("%d - 1 of 3 parameters plus ZERO", argc);
 }

	// ../obj/ gen_ %s PSG_name
	// ../obj/gen_%s
	// ../obj/gen_%s_lex.lex
	// ../obj/gen_%s_yacc.y
	// ../obj/gen_%s_yacc.tab.cc // the PSG code
	// ../obj/gen_%s_yacc.tab.hh // the PSG token pools and union

 	buffer1 PSG_File_left;
	PSG_File_left.print( "%s%s%s",
		argv[1], // ../obj
		argv[2], // gen_
		argv[3]  // e1
	); // left gen e1

	str0 PSG_File = PSG_File_left;
	str0 PSG_Name = argv[3];

 	// looks nice when called properly // announce action
	INFO("GENERATOR  %s", argv[0]); // PAIR || Tuplo_duo || Derived TYPE Combiner = { ... item LHS item RHS ... }
	INFO(" FILE_ %s", (STR0) PSG_File ); // "../obj/gen_AFM"
	INFO(" PSG %s", (STR0) PSG_Name ); // "AFM"

//	INFO(" LHS  LEX  %s", argv[1]); // LHS
//	INFO(" RHS YACC  %s", argv[2]); // RHS
	// TEST //INFO(argv[3]); // NULL // "(null)" //  // " (null) " // 

	/*	PSG - the SPEC side of the DATA Tree
		PSG_UDEF - user defined (here) // for (purpose)

		 see PSG_UDEF.cxx
	*/

	// PSG = Bench.the_thing_we_are_building();
	// lex_yacc_UDEF PSG;
 //	lex_yacc_UDEF_CALC PSG; 
 //	lex_yacc_UDEF * _PSG = new  lex_yacc_UDEF_AFM();
 	lex_yacc_UDEF * _PSG = NULL;
	if( PSG_Name == "AFM" ) {
		_PSG = new  lex_yacc_UDEF_AFM();
	} else
	if(( PSG_Name == "e1" )  
	 ||( PSG_Name == "CALC" )) {
		_PSG = new  lex_yacc_UDEF_CALC();
		/* ASIDE VFS from API 'if(LANG=="CALC")...' */
#if 0

		PSG_NAME is one of " AFM e1 CALC CXX ARGV_via_PSG _via_NAME PSG"

		# that PICK from LIST also has PICK from IDENT # DIALECT += EXPR
		# DIALECT += { a1_a2_star a1 a2 } SPELLING("%s") cident99 ALIAS ITEM
		# VAR_POOL("VAR_NAME") VAR_POOL_api get_NAME_from_EA_ITEM
		# COMPILE "VAR_NAME" as "VAR_NAME_%s"
		# SCRIPT PERMIT "DECL VAR_NAME ..." # += CODE_POINT XPOS
		# a TOKEN and an XPOS # OFTEN XPOS == BOLN # on_OPEN
		# a SCRIPT_IF "if( CSR == "BOLN" ) ... # claim HERE is BOLN"

		# OK above BOLN" "is  errpor,
		# even when if 0
		# because # PARSER needs to read each # line
		# and IT finds Q2 uneven

		# COMPILE # GEN "{ DECL EA_BOLN == EA_HERE }" #
		# GROUP SCRIPT_PROVIDES_GROUP_NAME "%s" must match cident99 

		# here "CALC" is the name of the PSG for CXX_SCRIPT_EXPR
		# CALC += CXX_SCRIPT_EXPR # LIBR_local
		# PSG += CXX_SCRIPT_EXPR # LIBR_local PSG_CXX
		# CXX += PSG_CXX


			CXX_
			_IDENT
			_VAR_NAME
			_FIELD_NAME
			_NAME
			_IDEN

		# CXX += PSG_CXX

			SCRIPT_is_CXX

			 PHRASE_BOOK
			 LEXICON
			 VAR_POOL
			 TYPE_SPEC

			 TYPE_SPEC("%s") // TYPE_SPEC == VAR_NAME == "VAR_NAME"
			 TYPE_SPEC("%s") // TYPE_SPEC == VAR_NAME == "TYPE_SPEC"
			 TYPE_SPEC("%s") // TYPE_SPEC == ALIAS_NAME == "TYPE_SPEC"

		# WHEN SCRIPT is CT_RT 

			KNOW STEP_id_CXX_STEP
			EG STEP_id_ARGV_STEP _id == "OFFS_in_FILE" # XPOS OFFS
			EG STEP_id_API_STEP

		# WHEN PARSER on SCRIPT is CT_RT

			CT_RT COMPILE SCRIPT

		# simpy writing that ARGV made it a SCRIPT STEP A_LINE an_ARGV

			CT_RT COMPILE SCRIPT STEP == "XPOS_OFFS"

		# ARGV is almost a CXX SCRIPT_LINE_ARGV

			::module::FILE::XPOS::ARGV

		# ADDR_EA_EXPR # 

			DECL typename varname 
			"i32 ea_expr"
			"i32 ea_expr = %s"
			"i32 ea_expr == %s"
			"i32 ea_expr == '%s'" // OPTIONAL mechanism of Q2_Q1

			STREAM "%s_%s" "${NAME}" "%04x"
			// DETECT MATCH "${%s}" NAME == "NAME" 
			// DETECT MATCH "${%s}" NAME == "{NAME}" 
			// DETECT MATCH "${%s}" NAME == "{%s}" NAME
			// STRICT CHECK NAME_is_CIDENT99 // UDEF and USAGE
			// MATCH "04x" as TOKEN("04x") VALUE() PROVIDE %s
			// MATCH NAME_x%04x // use SP as LEX_EDGE pre Q2 AUTO
			// AUTO Q2 "%04X" // ACTUAL CURR VAL is_UPPER_HEX_QUAD
			// HEX_QUAD_0xFFFF
			// HEX_QUAD_0xF0FF // API_u24_PAYLOAD API_CPU_REG
			// BYTE_B BYTE_A i16_BITFIELD i24_BITFIELD _i16 _i32
			// BITFIELD_name i8_opcode i24
			// when SCRIPT exceeds i32 pre_compile GETTERS ACTS
			// u8_OPCODE // u24_PAYLOAD // u24_BITFIELD 

			ITEM.subfield

			EA == OFFS_of_BOLN_in_FILE // XPOS_OFFS //
			EA == OFFS_in_FILE // XPOS_in_LINE // PARSED_CIDENT99

			EA == ARGV[XPOS_idx] // [i8[0[i8 WRAPPED_onto_ZERO
			## thats the FULL flag N==0 ## but cant have N==0 or can

			"%s_x%04x" "VARNAME_xFFFF" // AUTO_ALIAS PARSE
			"%s_x%04X" VAR_NAME VAR_idx // ida == EA // prove MATCH
			"%s_x%04X" "VARNAME_xFFFF" // AUTO_EDIT FMT_UPPER_HEX
			"%s" 
			"%04X" 
			"VARNAME"
			"_xFFFF" FFFF == EDAF

				// then match EDAF when used in SCRIPT
				// using SPARSE_TABLE_on_ZERO // SIGNED_IDX //
				// USAGE [-128 [ -127 [ 0 [ +127 [ +128 [ N [ u16
				// N == LIMIT_is_one_more_than_highest_in_use
				// N == Next // P2 == NEXT //
				// N == N // the SIGNED_PLUS_SIDE _ONLY
				// VAL == N // VAL == i16_in_CPU_register
				// if SIGNED < 0 // check negative arrays work
				// if SIGNED < 0 // CT calc EA_EXPR
				// if SIGNED < 0 // RT call XPOS //
				// PARSABLE XPOS is "MENTION _xFF"
			
	// if SIGNED < 0 // check negative arrays work

			// on_GOTO _on _GOTO
			// on_CALL _on _CODE

			int mpos = _xFFFF.int_val

				GETTER is used by GEN with TRACE_GEN_on_MINI_MACHINE
				MINI_MACHINE is a COMPILER

					set it up
					run it
					build tracks as TREE of EXPRS used
					allocate OBJECTS to TRAY[slot]
					stream SEGMENT as MINUS_SEGMENT

					PRE_REGISTER ITEM // PRE_CRIME //
					idx = N ++ // ALLOC //

				PUTTER
				PUTTER_of_BITFIELD

					on_OVERFLOW _assigned_BITFIELD_VALUE
					on_EXTRA_RANGE different packet with VAL

						i48_PAYLOAD = NEWVAL;

						i64_LOHI =from i16_in_WORD

					on_EXTRA_RANGE

					OPTION inline GETTERS API
					OPTION inline API

					# THE CODE IS THE IDENTIFIER #
					# using XPOS_PARSED not STR0_TOKENISED
					# using XPOS == OFFS # in_FILE #

					Routing the API calls thorugh functions
					provide BIND_VAR_TO_CALL
					provide GETTERS_PUT_API
					provide CACHE provides COMPILE PRE_

					PROVIDE "{ }"



		#endif
	} else
	if ( PSG_Name == "CALC" ) {
		// WARN("CODE ERR") // RETVAL " expeced the FILTER to work "
		// WARN("CODE ERR") // RETVAL STR0
		// TEXT("DECL CALC an INST of CALC_t") // 
		// LANG("DECL_IDEN") // IDEN_as_EA_LABEL //
		// IDEN("%s") // CIDENT99("%s") // DEFAULT_NULL is_CIDENT("%s")
		// TEST MATCH "cident99" // BENCH_VAR_on_SESSION 
		// VAR_NAME cident97 // PICK "cident98" FILTER "cident99"
		// PICK THIS_PARSING_CSR // CALC_STR_is_VAR_NAME_is_THIS //
		// AUTO get AUTO_GEN AVAIL // CODE_LINE == "ARGV"
		// CODE_POINT_DATA PTR == CODE_LINE // .str.get() // "%s" //
		// DATA_POINT OBJ_t OBJ 
		// INTO("CODE ERR") // RETVAL " expeced the FILTER to work "
		_PSG = new  lex_yacc_UDEF_CALC();
	} else
	if ( PSG_Name == "JSON" ) {
		_PSG = new  lex_yacc_UDEF_JSON();
	}
	if(!_PSG) {
		return FAIL(
		  "USAGE %s ../obj_gen AFM # got %s wanted AFM e1 CALC JSON XML",
			argv[0],
			(STR0) PSG_Name
		);
	}

 	lex_yacc_UDEF & PSG = *_PSG;

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


 	if(!PSG.set_PSG_name( PSG_Name, PSG_File )) FAIL_FAILED(); // ../obj/gen_ATM
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


