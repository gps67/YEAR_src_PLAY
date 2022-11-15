#include "dgb.h"
#include "tty_ptmx.h"
#include "str0.h"
#include <unistd.h> // execve
#include <sys/wait.h> // wait

// TODO // ecit with childs exit code

using namespace BASE1;

class tty_wrap_t { public:
	tty_ptmx_t tty_ptmx;
	int argc;
	char ** argv;
	char ** envp;
	int child_pid;
	int child_exit_code;
	bool tee;
	bool tee_a;
	str0 tee_file;

 // CTOR
 	tty_wrap_t() {
		child_pid = -1;
		child_exit_code = 22;
		envp = NULL;
		tee = false;
		tee_a = false;
	}

	bool parse_args( int _argc , char ** _argv, char ** _envp )
	{
		envp = _envp;
		argc = _argc;
		argv = _argv;

		int arg_i = 1; // skip arg[0]
		while( arg_i < argc ) { // or break
			str0 arg = argv[arg_i];

			if( arg == "--tee_a" ) {
				arg_i++;
				tee_file = argv[arg_i];
				if( arg_i >= argc ) {
					return FAIL("--tee_a filename # missing filename");
				}
				arg_i++;
				tee_a = true;
				PASS("--tee_a '%s'", (STR0) tee_file );
				continue;
			}

			if( arg == "--tee" ) {
				arg_i++;
				tee_file = argv[arg_i];
				if( arg_i >= argc ) {
					return FAIL("--tee filename # missing filename");
				}
				arg_i++;
				PASS("--tee '%s'", (STR0) tee_file );
				tee = true;
				continue;
			}

			if( arg.starts_with("-") ) {
				return FAIL("opt '%s' not recognised", (STR0) arg );
			}

			break; // should leave argv
		}
		argc = _argc - arg_i;
		argv = _argv + arg_i;

		if( argc <= 0 ) {
			return FAIL("absent CMD ARGS");
		}
		return true;
	}

	bool exec_child() {

		INFO("calling execve '%s' ...", argv[0] );
		// tty_wrap already done dup2(0|1|2)
		int t = execvpe( argv[0], argv, envp );
		if( t == -1 ) {
			return FAIL("execve '%s' ...", argv[0] );
		}
		return FAIL("execve '%s' ... returned %d", argv[0], t );
	}

};

bool bool_main( int argc , char ** argv, char ** envp )
{
	tty_wrap_t tty_wrap;

	if(! tty_wrap.parse_args( argc, argv, envp ) )
		return FAIL_FAILED();

	// REDO LIBR // stderr to stay the same //

	int pid_child = tty_wrap.tty_ptmx.fork_vtty_old();
	if( pid_child == -1 ) return FAIL_FAILED();
	if( pid_child == 0 ) { // CHILD
		tty_wrap.exec_child();
		return FAIL("THIS exec SHOULD NEVER RETURN");
	}
	// PARENT //
	// READ tty_wrap.tty_ptmx.fd_master // copy OUT to OUT
	// EOF tty_wrap.tty_ptmx.fd_master // look at vwait
	// READ STDIN - copy IN
	// WRITE tty_wrap.tty_ptmx.fd_master // copy IN to IN

	sleep(1);

	int exit_child = 0;
	int options = WNOHANG;
	u8 buff[1024];
	int n = read( tty_wrap.tty_ptmx.fd_master, buff, 1024 );
	write( 1, buff, n );
	waitpid( pid_child, &exit_child, options );
	INFO("waitpid exit_child %d", exit_child );

	return PASS("OK");
	return true;
}

/* -- LOCAL RANT --

	SUBLEX_CODE_POINT {
		DIALECT SUBLEX 
	 // DIALEXT SPEC { SPEC }
	 // SIMPLIFY DIALECT // reverse of ADD_FIELD // VIEW_SIMPLER
		 VAR_SPEC == SPEC // BIND CODE_POINTS near HERE // DIALECT AUTO

	  PLASTIC_VAR_POOL takes 

	  	A B C D

	  PLASTIC_VAR_POOL takes 

	  	LHS { A B C D } RHS

	  PLASTIC_VAR_POOL also takes 

	  	LINE
		LINE
		LINE
	
	  PLASTIC_VAR_POOL also takes 

	  	LINE_LIST_ONE
		 PLUS

		LINE_LIST_TWO
		 PLUS

		...

		PLUS {
		// HERE //
		}
	
	 DIALECT SIMPLIFY

	 	The second form is complex
	 	The first form is directly available and recognised
		PARSER LINE S LINES // easily addable universal S %sS
		FMT LINES == %sS LINE // LINE == "VAR_NAME" // //"ITEM"
		AUTO_VAR create var "LINE_t LINE" // or OTHER
	
	{ FMT x%X }

		FMT is this CODE_POINTS nickname for FMT

		print FMT A R G S // ARGS PLUS in DIALECT("SCRIPT")

		AUTO_GEN PARSE MATCH scanf FMT2 ARGS TEXT { TEXT == "%X% }
		AUTO_GEN from ALIAS " x4X " // u16 as 4 HEX DIGITS "xFFFF"
		MATCH "xFFFF"
		MATCH "xFFFF" { AUTO_ADD_THIS_USAGE }
		MATCH "xFFFF" { MORE { AUTO_ADD_USAGE } PLUS } "
		MATCH "x%X" u16_idx # VIEW _FF_FF_ # SUBLEX checks for name clash
		# SUBLEX "_FF_FF_" 
		# SUBLEX "_FF_FF_"  { VIEW_BA }  u8_BYTE_B u8_BYTE_A "%2X"
		{ SCAN_FMT "%2X" } // expect u8_idx accept i8_idx 
		MATCH "x%X" u48_idx

	MATCH TUPLO

		UNARY
		 TOKEN
		 VALUE
		 OBJECT
		 LOCN
		 CODE_POINT

		BINARY
		  { PAIR }
		  { lhs rhs }
		{ { lhs rhs } CODE_POINT }


		CODE_POINT { SCRIPT }

		TERNARY
		 TUPLO
		  ARGS
		  	{ ARGS } // CODE_POINT // comment is way in HOOK
	
	MATCH TUPLO

		PARSE

	MATCH TUPLO

		{ A R G S } {
			// lhs == " { A R G S } "
			// rhs == __THIS_SCRIPT__
		}

		// silly language AUTO wraps rhs as " { %s } " cident99 == "rhs"o		// MATCH CODE at or near CODE_POINT { cident99 == "STR0" }
		// DIALECT sees IDENT == "STR0" // reminded to MATCH "ANYSTR"
		// DIALECT { IDENT "STR0" } // found in source CODE_POINTS
		// DIALECT { ALIAS "ANYSTR" } // found in source CODE_POINTS
		// CODE_POINT { expr_filename expr_seek }
		// CODE_POINT { OBJ_filename IDX_seek }
		// u48_IDX // u8_idx OPTION // u16_DECODE BYTE_B BYTE_A BA A B 
		// occasional ambiguity AB BA VIEW_as or STORE_as PICK("VIEW_BA")

		{ rhs } { 
		}

		"rhs" { 
		// Q2 PARAMETER_NAME rhs // "%s" // prep_literal_str // Q2_LIBR
		// Q2_LIBR // SUBLEX PARSER Q2 REQUOTE_PRE_QUOTE_FILTER
		// Q2_REQUOTE FILTER IN = { ... } OUT += { ... } EXPRS += POOLS
		// MATCH IDX WELL_KNOWN_LEX // MATCH cident99_or_ANYSTR
		// ANYSTR == { SCRIPT }
		}

		
	MATCH TUPLO

		PARSE

			PLAYFUL PARSE

				NOUNS

				VERBS

				SCRIPTS

				API VIEW

			SCRIPT_EXPR( "FOUND" )
			SCRIPT_EXPR( "SCRIPT_EXPR" )
			SCRIPT_EXPR( "SCRIPT_EXPR(\"%s\")" ) // GEN // FILTER
			MUST be PRE_QUOTE_Q2 // no \" required
			MAY be PRE_QUOTE_Q2 // \" already applied
			MAY be PRE_QUOTE_Q2 // filter into Q2 // adds \"
			MAY be PRE_QUOTE_Q2 // RT_Q2_requote_99 // FILTER API //
	
	TREE WEAVE

		MATCH TUPLO in TEXT // intentionally use TOKENISATION switches
		MATCH TUPLO in ELSEWHERE // TREE GRAPH WEAVE BENCH LOCN
	
	DOC CREATE

		WEAVE links between meaning
		CONNECT points with TOKEN-ISATION
		REVIEW RESTRUCTURE FLOW OVER BRIDGES RE-FACTOR-TEXT

		DOC must be navigable, parse XML, possibly awful source

		LOCN += { SPEC DATA } // CODE_POINT says TWIG

		TWIG { SPEC DATA } // CODE POINT { ... }

		 SPEC == DTD_PLUS _TOKENISER _PHRASE_BOOK _DECLS _SPECS VALUES
		 DATA == " { ARGS } " // TOKENISER says MATCH S


	DOC FLATTEN

		2D view of TUPLO TREE

		 DOCUMENT FRAGMENT
			Headline
		 DOCUMENT FRAGMENT
		 	BLOCK_of_TEXT { PSG TEXT } // PSG == SPEC("OL")
	
	DTD TREE

		simplified XPAT loader

		SUBLEX_XML

			PARSE STR0 STREAM // guaranteed NUL or P0P2 handler
			 check file ends in LF == OK
			 use FILES_of_LINES

			INDEX IDENT in FILE_of_LINES
			 VAR_POOL each declared at CODE_POINT in SCRIPT
			 	{ SCRIPT LINE XPOS }
			 	{ SCRIPT SEEK }
			 	{ SCRIPT NEAR OFFS }
			 	{ SCRIPT NEAR SWEEP }
			 	{ SCRIPT NEAR TOKEN } // ie VALUE is TOKEN
				{ SCRIPT NEAR SEARCH_EXPR ARGS }
				// note shorter forms OPTIMISE MATCH processing
			 PARSER can then parse IDENT from source


	  // This is a LISPY sort of language { func A R G S } { func(ARGS) }
	  // This is a DATA DECL { A R G S } // MATCH figures it out 
	  // ARGS => { A R G S }
	  // ARGS => { ARGS }
	  // ARGS => { ARGV }
	  // ARGS => { OBJV }
	  // ARGS => { %s } // STR0_for_TOKEN // no NULLS within first part of STR0
	  // DATA ALLOC STR0 /* NUL */ /* MORE BYTES */
	  // DIALECT split MORE == CODE_POINT BYTES == DATA == SELF
	  // 
	}

	A R G S {
	 // parsed("ARGS") // name of VAR // ALIAS OBJV // API registered //
	 // TOKEN_from_STR0("ARGS") // varname
	 // TOKEN_from_STR0("ANYSTR") // must have unique MD_CRC // ie PTR2 != PTR1
	 // __ATTR__ TOKENISE THING_from_LOCN // EA_ITEM_NOW stable_id 
	 // __SPEC__ /* of __ATTR__ */ SPEC { SPEC }
	 // DIALEXT SPEC { SPEC }
	 // %s == SPEC {
	 // ZONE == SPEC as VIEW // with NOW and its SESS_VARS // LIBR
	 // }

	 // SYL_A_BLE { SCRIPT HERE } // UNARY "SCRIPT_HERE" // AUTO_VAR

	 // AUTO_VAR { LHS RHS LHS_RHS LHS_RHS_now LHS_RHS_here ITEM NAME 
	 // AUTO_VAR { NAME } // NAME is a SCRIPT // { SCRIPT DATA } // SESS //
	 // DIALECT // ... all of these ...
	 
	 // SIMPLIFY DIALECT // reverse of ADD_FIELD // VIEW_SIMPLER
	 // SUBLEX finds every word a possible EXPR or SUB_EXPR_HINT
	 // SUBLECT expects VAR_NAME ITEM_idx ALIAS EXPR SESS
	 // SUBLECT expects { SCRIPT }
	 // SUBLEX { HOLDER_of_this_TOKEN retro_fits ALL OTHER IMPORTABLE ITEMS }
	 SCRIPT {
	  MODE = DECLS of MODULE items ENUMS TOKENS VARNAMES EXPRS SUBLEX
		 VAR_NAME
		 VAR_SPEC == SPEC // BIND CODE_POINTS near HERE // DIALECT AUTO
		 ITEM_idx
		 ALIAS
		 EXPR
		 SESS
	 }
	 SUBLEX ITEM // NAME // ITEM_NAME // EXPR("ITEM_NAME") // EXPR("EXPR")
	 BOOT_STRAP EXPR
	 BOOT_STRAP %s // TOKENISER("ANYSTR") // FILTER BEWARE_ANYSTR
	 FILTER ANYSTR {
	 	ANYSTR // HIDDEN DETAIL
		NORMALISE_GEN_CODE SCRIPT //
		SCRIPT == "VAR_NAME" // a specific cident FILTER CONFIRMS
		EXPR == "%s" ANYSTR // but check for cident99 // DIALECT // SUBLEX
	 }

	}


   -- LOCAL RANT -- */

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
