// #include "e1.tab.hh"

#include <stdio.h>
#include <errno.h>
#include "dgb.h"
#include "buffer2.h" // not neded here, but good for GEN
#include "util_buf.h" // basetype avoids IO, stat_file, alloc buff, open file,

extern // "C"
int yyparse();
extern void yyrestart ( FILE *input_file  );

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
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


  extern bool buf_yy_parse( blk1 & text )
  {
  	text.trailing_nul();
	YY_BUFFER_STATE buffer =
	  yy_scan_bytes( (const char *) text.buff, (int) text.nbytes_used );
	  //
	  // api_direct // loan our buffer to yyin_to_FLEX // single user
	  // that stays in effect, throughout the parsing, EOF and shutdown

	// yy_ loops over input
	yyparse();

	// if FAIL set errno to something
	// TODO("Review errno set here");

	// cleans up input extras
	yy_delete_buffer(buffer);
	return true;
  }

int main( int argc, char ** argv )
{
	const char * filename = argv[1];
	if(!filename) {
		filename = "../test2.test";
		INFO("Picking %s", filename );
	}

	// claim global LOCK here
	// provide store to the Tree Parser HERE
	// FILE_VER.ITEM_idx
	// SEGMENT_DECOER = load_file _text DECODE_ACCES
	// PARSER stopped by a single NUL, so insist, no really
	buffer2 text; int K_max = 64;
	if(!blk_read_entire_file( text, filename, K_max ))
	{
		FAIL("LOAD file '%s'", filename );
		return errno;
	}

	if(!buf_yy_parse( text ))
	{ 
		FAIL("filename was %s", filename );
		return errno;
	}
	return 0;
}

int main_yyrestart( int argc, char ** argv )
{
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

	yyparse();
	return 0;
}
