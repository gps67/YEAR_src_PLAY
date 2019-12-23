// #include "e1.tab.hh"

#include <stdio.h>
#include <errno.h>
#include "dgb.h"
#include "buffer2.h" // not neded here, but good for GEN
#include "util_buf.h" // basetype avoids IO, stat_file, alloc buff, open file,
#include "Y_PARSE.h" // calls yyparse, move stuff there

#if 0
struct PARSER_t;
extern // "C"
int yyparse(PARSER_t);
#endif
extern void yyrestart ( FILE *input_file  );

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern "C" {
	// LEXERINPUT.cc is G++ compiled so is not extern C
}
 YY_BUFFER_STATE yy_scan_bytes( const char * buff, int nbytes ); // api_direct
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

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


  extern bool buf_load_and_parse( blk1 & , const char * , int  );
  extern bool buf_yy_parse( blk1 & text );
  extern bool buf_yy_parse( blk1 & text ) // returns when done
  {
  	text.trailing_nul(); // noone else can add stuff, please no, NUL needed

	YY_BUFFER_STATE buffer =	// feed YY's LEX buffer with blk1 text
	  yy_scan_bytes(
	   (const char *) text.buff,
	            (int) text.nbytes_used
	  );
	  //
	  // api_direct // loan our buffer to yyin_to_FLEX // single user
	  // that stays in effect, throughout the parsing, EOF and shutdown

	// yy_ loops over input
	// using YY input previously setup
	INFO("yyparse");
#if 0
	int t = yyparse();
#else
	// YY::
	Y_Parse_t parser;
	int t = parser.call_yyparse();
#endif
	// cleans up input extras
	yy_delete_buffer(buffer);

	if(t<=0) {
		return FAIL("yyparse returned %d", t );	
	}
	INFO("yyparse returned %d", t );

	// if FAIL set errno to something
	// TODO("Review errno set here");

	return true;
  }

#include "STO.h"
#include "STO/mmap_file.h"
#include "obj_hold.h"

int main( int argc, char ** argv )
{
	const char * filename = argv[1];
	if(!filename) {
		filename = "../test2.test";
		INFO("Picking %s", filename );
	}

	// obj_hold<STO::mmap_file> map_file;
#if 0
	obj_hold<STO::mmap_file> map_file_a;
	map_file_a = new STO::mmap_file();
#endif
	obj_hold<STO::mmap_file> map_file
	= new STO::mmap_file();

// 	INFO("map_file->test1();");
// 	map_file->test1();

	// WARN("map_file->test_2();");
	// map_file->test_2();

	INFO("load_and_parse(%s)", filename );
	buffer2 text; int K_max = 64;

if(1)
	if(!buf_load_and_parse( text, filename, K_max  ))
	{ FAIL_FAILED(); return errno; }

	return true;
}

extern bool buf_load_and_parse( // load file then parse it, then return
   blk1 & text,
   const char * filename,
   int K_max
) {

	// claim global LOCK here
	// provide store to the Tree Parser HERE
	// FILE_VER.ITEM_idx
	// SEGMENT_DECOER = load_file _text DECODE_ACCES
	// PARSER stopped by a single NUL, so insist, no really

	if(!blk_read_entire_file( text, filename, K_max ))
	{
		return FAIL("LOAD file '%s'", filename );
	}

	// not making it buffer2 code, keeping it UTIL_buffer code
	if(!buf_yy_parse( text ))
	{ 
		FAIL("filename was %s", filename );
		return false;
	}
	PASS("filename was %s", filename );
	return true;
}

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

// UNCALLED
//	yyparse();
	return 0;
}
