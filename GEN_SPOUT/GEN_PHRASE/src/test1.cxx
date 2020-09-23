// # ITEM_CTOR_ARGV_DATA // unix ARGV passes "{ SCRIPT }"
// # WIN32 # LINK to API # Module_API = "{ HERE }" // _DOT_ _dot_zone _by_ANYSTR
// # TODO # ANYSTR # "%s" # _DATA _CODE _DECL _CALL _ARGV_
// # DECL # ARGV_t ARGV = DATA_STREAM("%s" // ANYSTR // ARGV[ [0..[N_WORDS
// # DECL # N_WORDS # default("N") # LIST += "N" // LINK VAR for GETTER SET_VAR
// "dgb.h" debug degub dee_gee_bee # PLUS _dot_h
// # DECL FAIL PASS INFO WARN dgb_PRINTF_via_DTP_verbs # PRINT_VIEW_idx[_idx]
// # TEXT 
// test1 uses argv_decoder to decode and run in debug mode
// test1 creates a UDEF type and runs its CTOR then event "LOG_DONE" "LOG_FAIL"
// 
//
// PRINTED
//
// # PASS  # bool test_EV::test1() # BUILT RUN test1_OK_TELL PASS
// # PASS  # bool EV_test1(EV::argv_decoder&) # OK

// ARGV_DECODER
//
// CTOR ARGV( argc, argv, envp ) // a clutch of vars
// # INFO  # EV::argv_decoder::argv_decoder(int, char**, char**) # CTOR
// # EV::argv_decoder( argc, argv, envp ) // CTOR message
//
// test1(ARGV) 
//
// code for MAIN, create a LOCAL call its CTOR(ARGV) call its test1()	
//
// test1( ARGV ) // CMD_LINE_ONE = ARGV // ARGV_DECODER argc argv envp
// ARGV.getenv("ENV_VARNAME") 
// ARGV.secure_getenv("ENV_VARNAME") // libc has it's list
//
// 

// 
// # DATA # CTOR(" bool EV(ARGV) # OK // %s MESSAGE
// 
// That then creates SCOPE EV::test_EV // CTOR and run it's test1()
//
//	instance
// # DATA # CTOR(" bool test_EV::CTOR() # MESSAGE # INFER_OK_NESS is_OK "PASS"
//
// we then call it's ARGV_TEST1 function
//
//	EVAL ARGV	// maybe it could do this
// # PASS  # bool ARGV.test1() # OK MESSAGE when DONE PASS dgb_INFO
//
//
// eval_argv // ARGV == implied
// 
// 
// # 
// # DATA # CTOR(" bool test_EV::test1() # MESSAGE # INFER_OK_NESS is_OK "PASS"
// #  namespace:: classname
// #  classname CTOR ARGV // ~= ENV
// #  


#include <stdlib.h> // getenv
#include <unistd.h> // write
// #include <string.h>

#include <stdarg.h> // ...

/*
	EV is event
	api_ev is an event as a class with a virtual function
	api_event is also an event with a class a virtual function
	api_event is also an runtime event with _API_
	CODE += UPDATE_ON_VAR_CHANGE 

	INFO(fmt,a,r,g,s ) // DECL_DATA ARGV char ** argv; // UTF8 anystr

	Each api_ev is very lightweight DATA
	CTXT_EVENT_DATA  adds a load of 

	TODO: remove all SPOUT GEN_PS stuff 

	api_event is an EXIST thing
	as it passes a TREE, it instanciateso
	wave_of_event signal propagation tree queue local sto expands by page


*/

#include "dgb.h"
#include "buffer2.h"

#include "util_buf.h" // blk_write_to_file( out, filename )
#include "dir_name_ext.h" // dir_name_ext( filename ) .ext = ; mk_near_path_name

#include "EV_STUBS.h"
#include "EV_argv_decoder.h"

#define IF_NOT if(0) 

using namespace EV;

	// class EV:: api_ev; // signals as events += DATA
	// class EV:: api_event; // signals as events += DATA
	// CACHE has to pre-guess the remotes interpretations SCRIPTS
	// RUNNING an early request list to offer CALLER
	// EVENT = VAR_CHANGE_VALUE
	// EVENT = VAR_CHANGE_SCOPE
	// EVENT = VAR_CHANGE_TRACKER // CONSOLE VIEW OPTIONS

struct ink_rgb
;

/*!
	dgb_EV VAR_HOLD_TRACKER dgb_api
	test_ev_t ev // EV // alias // ADDR_EXPR // EVENT_VIEWER //

	A DEMO A TEST A LIVE TEST //
*/
class test_EV // : public obj_ref
{
 public:
	buffer2 out;

	// UDEF_t item = CTOR uses VARS // add your vars here 
	// ... // EA_HERE = test1.cxx: Line 48

	test_EV()
	{
		out.get_space( 1024 * 32 );
		// CTOR( CTXT ) // CTXT // ...
		// subtype will use same ATTR'S // u8 field_id = this_idx 
		// explains // u3_from_u8_in_32BYTE_fragment
		// WHEN("ON") catch_up_and_make_good_on _ON_
		// so you have 32 1K pages or 32K
		// VIEW show_all_allocs_EV_event
		// VIEW show_("PTR = EVAL(" EXPR ") // PTR_t // Module_USAGE

	}

	bool test1( argv_decoder & ARGS )
	{
		buffer2 buf;
		STR0 prog_name = ARGS.get_prog_name( buf );
		INFO("progname %s", prog_name );
		if(! prog_name ) {
			return FAIL_FAILED();
		}
		STR0 env_HOME = ARGS.get_env( "HOME" );
		if(! env_HOME ) {
			return FAIL_FAILED();
		}
	if(0)	INFO("env_HOME %s", env_HOME );
		PASS("BUILT RUN test1_OK_TELL PASS"); // RETVAL = "OK" // .test1
		// NEAR ADDR .test1 from "test1" %s%d "test" 1 // = N++
		// OPCODE = LHS opcode RHS //
		// AUTO LINK LHS LHS_%s "ea_this" "item" "mini_heap_item" "dot" "here" "anystr" // seriouosly
		return true;
	} // test1
};

//
// self // this // here // _t += _trait // ASPECT = TYPE_TRAIT_t _trait = EXPR
// UDEF_CODE adds itself at _dot_ which auto_adds_it_to_cache
// _N_BYTES_of_UDEF_ // DECL("N_BYTES"); DECL("UDEF_t") HERE PAYLOAD BIT_FIELDS
// DATA PAYLOAD_N_BYTES_of_UDEF_ // DECL("UDEF") PAYLOAD BIT_FIELDS

bool EV_test1( argv_decoder & ARGS )
{
 	test_EV EVER;
	if(!EVER.test1(ARGS)) 
		return FAIL_FAILED();;
	return PASS("OK");
	// KNOW is_OK WITH idx == u8_idx // STEP[u8] from LOCN("LOCN") // DATA
	// DATA PAYLOAD_N_BYTES_of_UDEF_ // DECL("UDEF") PAYLOAD BIT_FIELDS
	// PARSE DIALECT 
}

int main_FAILED() {
	FAIL_FAILED();
	if(errno) return errno;
	return 1;
}

int main( int argc, char ** argv, char ** envp ) {
	// AUTO_VARS // VARS += "argv_decoder" // "argc" "argv" "envp"
	// AUTO_VARS // VARS += "ARGS" // HERE "argc" "argv" "envp"
	// DECODE(" VARS += "ARGS "); // _SESS _CTXT this near_scope
	// KNOW(" ARGV == 'ARGS' "); // _PICK_CHOICE_OF_Q1 a_near_name _similar
	// AUTO_VARS // 
	argv_decoder ARGS( argc, argv, envp ); // "ARGS" == "ARGV" // _prog_name
	// AUTO_VARS // VARS += "argv_decoder" "argc" "argv" "envp"
	// AUTO_LEX VAR_NAMES_USED USED_LIST // on_exec == on_eval // elf_act
	// This C running ARGV of TEXTS and filenames and values via API
	// API = EV // built_into_ARGS_DECODER_EVAL _EXEC _ARGV_
	// ARGV like ARGS
	if(! EV_test1( ARGS ) ) return main_FAILED();
	return 0;
}

