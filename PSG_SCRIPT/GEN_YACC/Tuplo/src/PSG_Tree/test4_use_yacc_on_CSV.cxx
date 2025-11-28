// #include "e1.tab.hh"

#include <stdio.h>
#include <errno.h>
#include "dgb.h"
#include "buffer2.h" // not neded here, but good for GEN
#include "util_buf.h" // basetype avoids IO, stat_file, alloc buff, open file,
#include "YY_PARSE.h" // calls yyparse, move stuff there

#include "STO.h"
// #include "STO/mmap_file.h"
#include "obj_hold.h"
// using namespace PSG;

int main( int argc, char ** argv )
{
	dgb_fork_stderr_to_tcl_text();
	const char * filename = argv[1];
	if(!filename) {
	 //	filename = "../test2.test";
	 //	filename =
	 //	"/usr/share/fonts/type1/urw-base35/Z003-MediumItalic.afm";
		filename = "../test4.test";
		// has lots of KPX and char names
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

	YY_Parse_t PSG("CSV");
	// e1 is used in signals to say which PSG says what
	// YY_Parse_t PSG(filename); // or nick_name_from_filename
	// YY_Parse_t PSG(name_resource); // or ea_expr_as_name // name_from_

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
