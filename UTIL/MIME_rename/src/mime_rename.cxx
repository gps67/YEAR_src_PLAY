#include "dgb.h"
#include "tty_ptmx.h"
#include "str0.h"
#include "fd_selectable.h"
#include <unistd.h> // execve
#include <sys/wait.h> // wait

// TODO // paid in bar snacks // hedgrow staff //
// TODO // RENT SERVICE // office staff //
// TODO // EVENT CONTRACT PAYMENT DELIVERED PLUS("-etc-")
// TODO // EVENT TENT TENT DESK DOOR FLOOR LIGHTS TENT FOOD STALLS ADMIN
// TODO // FREE FEST ES6 ES14_ECMAScript_14 _ECMAScript_14 _ECMAScript_2023
// TODO // FEST ACCTS _per_TENT
// TODO // _per_TENT += _per_TENT_%s SUBLEX_CIDENT_SEGMENT _SLICE 
// TODO // _per_TENT += %s CIDENT SUBLEX a1_plus // a1_a2_star // TENT_NAME //
// ALIAS // SERVICE_NAME // _prints_as_STR0_for_SUBLEX_CIDENT_SUBLEX
// ALIAS // UNIQUE_NAME_IN_VAR_POOL("VAR_POOL") VARNAME("VAR_NAME") // STEP
// STEP // "{ BIND PRE_TREE to TREE_POST }"
// STEP // "{ VAR_NAME %s }" // unanswered %s leaves item_of_hunger // ANON_
// ANON // ANON_NAME("_NAME_") // GEN "x%03X" // compient simple FMT "%03X"
// ANON // ANON_NAME("'{ FMT %03X }") // GEN "x%s" // GUESS as PARSE " % 0 %d X "
// VALUES // COMMON 1 2 3 4 8 %d
// TODO // FEST ACCTS //
// TODO // GOOGLE API calendar // TODO //
// TODO // _ECMAScript_2023 BABEL can reduce to ES6 // 
// TODO or_TypeScript
// TODO or_COMPILED_ES6 // note all LIBR called // suit ES6 from_ES6 _with_LIBR
// TODO // GEN CXX using LIBR // LISTS STRUCTS CAPTURE CLOSURE CODE_CALL
// WASM // TODO // run_WASM to DECODE ES6_with_SUBLEX // reread DINDINGS
// BENCH // CXX to WASM // STR0 from DATA_BYTES // WASM has CXX_("GLOBAL_VAR")
// alternative to WASM, is slightly slower ES6 to to CSET_PARSE_XPOS 
// libs_apps (half of) in WASM_LIBR // TODO //
// 
// CODE_CALL // REGEX_code_MINI_MACHINE CTXT ARGS CODE STEPS CSR STATE
// or simply RTFM and use the ES6 REGEX facilities as the PARSER
// GEN_Parser // of_CXX // of_ES6 // of_ES14 // of_C // _of_JAVA // of_CPP_2023
// RUN_PARSER // PARSE POOL("lo_cap_poss_ident") POOL("CAPS_IDENT")
// PARSED_TREE SCRIPT EXPR EA_VAR EXPR PHRASE SUBLEX_PHRASE
// SUBLEX uses yacc like decode // tends to longest cident _step_is_expr
// VAR_NAME is compressed NAMED VALUE CXX_PHRASE 
// VAR_EXPR VALUE 
// VAR_VALUE "SPELLING" // MATCH STR0 == "SPELLING" // "{ SPELLING }" // 
// 
// TODO // ALIAS machine // MATCH API // TERSE SCRIPT no underline parse argv
// TODO // DTP WORD_LIST // and simple lines of text carrying a few TOKEN POOL API
// TODO // DTP LANG_WORD_LIST PSG_Phrase PSG_sublex // Cap1 -vs- lo_cap_sublex
// EXPLAIN += SUBLEX NEXT STEP MINI_STEP CAP1 Cap1 lo_cap_sublex
// EXPLAIN lo_cap_%s sample_var_name // LOCAP is bultin gentre_by_way_of_sample
// SAMPLE += "LOCAP_t lo_cap" // AUTO_FILTER += "exact match lo_cap stream"
// SAMPLE += "LOCAP_t lo_cap_stream" // ALIAS += HERE ORIG "LOCAP_t"
// KNOW += "AUTO _t" // SAMPLE LOCAP_t "{ LOCAP_t LOCAP }" // CXX TUPLO
// CXX_TUPLO // SCRIPT include_libr "libr_lo_cap_EXPR_xFFFF"
// CXX_TUPLO // SAMPLE "EXPR_xFFFF" // "EXPR_xFFF" //  "EXPR_3X" // IDX //
// CXX_TUPLO // "{ %s %s } SCRIPT LIBR" -matches-"{ SCRIPT LIBR }"
// BUNCH of DIAG EDIT BENCH EDIT DATA SPEC ...
// BUILD BENCH from DOCS by creating OBJ for MENTIONED
// DOCS_FROM_SCRIPT // describe API // AUTO_MK_VAR_CAPS API /* sample */ // 
// DOCS_FROM_SCRIPT // describe API // AUTO_GETTER API /* { api_t api }  */ // 
// BENCH uses API itself to create a VAR_POOL of GETTERS with obj_idx_api
// OBJ_idx_API // DECODE u64 GET u12_2048_t // PACKED BITFIELD "{ NBITS 12 }"
// SUBLES sees "{ NBITS %s }" // CT_NAME = CT_LOOKUP "NBITS" += "_t"
// operator += ( STR0 ) // DIALECT += compressed func arg prototypes += PARSED
// SESS += "_t" // AUTO_ADDS("_t") to "%s_t" // DECL FILTER //
// SUBLEX auto build plausible CXX var_names_from_phrase // of own SCRIPTS
// CT_PRE_BUILD _KEEP VAR_POOL Module_name("%s") AKA Module_name("module")
// ALIAS Module // HINT TILT "Module" -> "module" PICK u4_PICK

//
// "exact match lo_cap stream"

using namespace BASE1;

struct OBJECT_BASE { };

class mime_rename_t : public OBJECT_BASE {
	buffer2 full_path_name_in; // kept original value
	
};

bool MIME_rename_one( const char * pathname, buffer2 & dest )
{
	dest.clear();
	dest.print("(%s)", pathname );
	return true;
}


bool bool_main( int argc , char ** argv, char ** envp )
{
	buffer2 buff;
	argc--; argv++;
	while( argc > 0 ) {
		if( argv[0][0] == '-' ) {
		 if( argv[0][1] == '-' ) {
			e_print("long_opt %s\n", argv[0]);
		 } else {
			e_print("short_opt %s\n", argv[0]);
		 }
			
		} else {
			buff.clear();
			MIME_rename_one( argv[0], buff );
			e_print("dir_or_filename %s -> %s\n", argv[0], (STR0) buff);
		}
		argc--; argv++;
	}
	return true;
}

// extern char ** environ; // also available as a global

int main( int argc , char ** argv, char ** envp )
{
	// without ANY need, ld optimised out base1.so
	INFO("HERE");
	if( bool_main( argc, argv, envp ) ) 
		return 0;
	else {
		if(errno) {
			FAIL_FAILED();
			return errno;
		}
		WARN("FAILED without errno");
		return 1;
	}
}
