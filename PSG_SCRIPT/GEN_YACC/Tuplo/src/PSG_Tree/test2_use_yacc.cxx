// #include "e1.tab.hh"

#include <stdio.h>
#include <errno.h>
#include "dgb.h"
#include "buffer2.h" // not neded here, but good for GEN
#include "util_buf.h" // basetype avoids IO, stat_file, alloc buff, open file,
#include "Y_PARSE.h" // calls yyparse, move stuff there

#if 0
extern void yyrestart ( FILE *input_file  );

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern "C" {
	// LEXERINPUT.cc is G++ compiled so is not extern C
}
 YY_BUFFER_STATE yy_scan_bytes( const char * buff, int nbytes ); // api_direct
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
#endif

// int main(){
//     char string[] = "String to be parsed.";
//     YY_BUFFER_STATE buffer = yy_scan_string(string);
//     yyparse();
//     yy_delete_buffer(buffer);
//     return 0;
// }

// not void

// yy_scan_bytes says _string _buffer _bytes
// _buffer needs EOB char in it YY_END_OF_BUFFER_CHAR
// _string calls _bytes
// _bytes  returns YY_BUFFER_STATE

// can put this in libs_apps without a yyparse ???

/*
	PSG has been generated compiled and linked to NEARBY
	PSG is BUILTIN as BISON_CHUNKS_FLEX_CODE

	The actions are in the YACC action
	Mostly add_LEAF_to_TREE 

		TREE is EXPR_CALC_SCRIPT_MODULE
		ITEM is CODE_DATA_SOURCE_CTOR
	
	The idea is to serialise input STREAM with added TOKENS
	Build an input tree (SPIN) 
	Build an input tree (SPIN) branch
	Serialise branch to STO
	build a STO machine from parsed SPEC // +- g++ // +- JIT

*/

/*
	YY_INPUT_STREAM_IO

	load_and_parse_filename
	REPL_via_TUI_Bench
*/


#include "STO.h"
// #include "STO/mmap_file.h"
#include "obj_hold.h"

int main( int argc, char ** argv )
{
	const char * filename = argv[1];
	if(!filename) {
		filename = "../test2.test";
	}
	INFO("Parsing %s", filename );

	INFO("TODO %s", "rename to dgb_radical_prints_VIEW" );
	//  dgb_ knows STRUCT layout (itself writes out from own src)
	//  dgb  activated by dbg_PASS_FAIL // location pulse-point
	//  dgb  MATCH expressions in comments // parts of VIEW


	buffer2 text;
	int K_max = 300; // 8 is a lot for me

	// TODO get K_max from USERS.CONFIG
	// ~/VFS/.config/appname/user.cfg
	// QUOTA 300_K expect 8K x 100 _page_pair 
	// ROM CODE unimited = preloaded or SITE loaded or RISK_CODE

	Y_Parse_t PSG("e1");
	// e1 is used in signals to say which PSG says what
	// Y_Parse_t PSG(filename); // or nick_name_from_filename
	// Y_Parse_t PSG(name_resource); // or ea_expr_as_name // name_from_

	// The PSG is hard coded
	// There are a load of CACHE KNOWN STATES in the FSM tables
	// eg START_PARSE_AT_ROM_LOCN // GOTO in need of a TYPE_SPEC
	// root_pos in rom PREBUILT TOKEN

	// one interesting HACK is to give yy_get_lex() the PERMIT token
	// As the first thing in the grammar TOP -token-> ROOT_NODE
	// The RULE CODE is ^TOKEN ACTION { psg.INIT_TOKEN_ARGS
	//  ARGS = get_ARGS_from_TOKEN // carries PERMIT sess_tags
	//  ARGS = VECTOR [ N ] of ARG = ANY_OF ... TUPLE( opcode, name, vale 

	if(!PSG.buf_load_and_parse( text, filename, K_max  ))
	  { FAIL_FAILED(); return errno; }

	return true;
}

// FORGOT - an example of (unused)
extern void yyrestart ( FILE *input_file  );
int main_yyrestart( int argc, char ** argv )
{
	// eg starting lex + yacc over FILE filename
	const char * filename = argv[1];
	if(!filename) {
		filename = "../test2.test";
		INFO("Parsing %s", filename );
	}
	FILE * IN = fopen( filename, "r" );
	if(!IN) {
		FAIL("fopen(%s)", filename );
		return errno;
	}

	yyrestart ( IN );

// NB yyrestart above sets the buffer, this rolls over it
// now needs added parameter
//	yyparse();
	return 0;
}
