#include "Y_PARSE.h"
#include "dgb.h"
#include "util_buf.h"
#include "buffer2.h"

// SOME STUBS for BISON
// see gen_e1_lex.cc

typedef struct yy_buffer_state * YY_BUFFER_STATE;
// WE get a pointer to it, from yy_scan_bytes( our_provided_text_buffer)
// It is a file, its input buffer, yy_buf_pos - yy_ch_buf
// yy_bs_ _lineno _column
// yy_
// IT also does the BUFFER MAGAZINE loading

   extern YY_BUFFER_STATE yy_scan_buffer ( char *base, int size  );
   extern YY_BUFFER_STATE yy_scan_string ( const char *yy_str  );
   extern YY_BUFFER_STATE yy_scan_bytes ( const char *bytes, int len  );

extern void yyrestart ( FILE *input_file  );
// FILE * IN = fopen( filename, "r" ); // check NULL
// yyrestart ( IN );
// yyparse(PSG);

extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
// FREE BISON buffer position holder

#define  YY_END_OF_BUFFER_CHAR 0x00

// YY:: Y_Parse_t CALLS yyparse PROVIDES SELF.TREE // _BUILDER

// https://www.gnu.org/software/bison/manual/html_node/Parser-Function.html
// says ( this ) would work, but for unknown - need *this
// 

using namespace YY;

int Y_Parse_t::
call_yyparse()
{
	ret_from_yyparse = yyparse( * this ); // matches T & V
//	ret_from_yyparse = yyparse( this ); 
 if(0)	INFO("ret_from_yyparse = yyparse( * this ) = %d",
		(int) ret_from_yyparse
	);

 if(1)
	switch (ret_from_yyparse) {
	 case 0: INFO("0 means PASS yyparse"); break;
	 case 1: FAIL("1 means ERROR yyparse"); break;
	 case 2: FAIL("2 means MEMORY ERROR yyparse"); break;
	default: FAIL("%d unknown retval from yyparse", ret_from_yyparse );
	}
	return ret_from_yyparse;

	// CASE 1 should be virtual to get filename, line, col, seek, SEGMENT_ID
	// CASE 1 should report SEGMENT_ID in ROM // 

}

// C called from yyparse(psg)
// see Y_PARSE_gen.cxx:
// "%%parse-param {Y_Parse_t & psg} 

void yyerror( Y_Parse_t & psg, const char * msg )
{
	FAIL("Y_Parse.Name \"%s\", msg \"%s\" ", (STR0) psg.Name, msg );
	FAIL(" get_prog_alias() says %s", (STR0) get_prog_alias() );
	// gdb_invoke(false);
}


// METHODS

  bool Y_Parse_t::
  buf_yy_parse( blk1 & text ) // returns when done
  {
  	text.put_byte(YY_END_OF_BUFFER_CHAR); // 
  	text.put_byte(YY_END_OF_BUFFER_CHAR); // 
  	// PRE_BOOK FULL LOCK on text holder; // just add this change //
  	text.trailing_nul(); // noone else can add stuff, please no, NUL needed
	// LOCK text // SNAP nbytes // KEEP lock until DTOR of link to text
	// KNOW running PARSER over STREAM will cause DELAYS and WAIT_POINTS
	// KEEP LOCK, KEEP cache_copy_RO LOCK, no edits allowed, catchup

	// WE ARE PROVIDING text buffer. The STATE is a malloced struct
	// struct yy_buffer_state ... // in gen_e1_lex.cc

	YY_BUFFER_STATE buffer =	// feed YY's LEX buffer with blk1 text
	#if 1
	  yy_scan_bytes( // does malloc copy slow loop
	#else
	  yy_scan_buffer(
	#endif
	  // bison adds 2 bytes of YY_END_OF_BUFFER_ so we do too size +=2
  //	(const char *) text.buff,
	   (      char *) text.buff,
	            (int) text.nbytes_used
	  );
	  //
	  // api_direct // loan our buffer to yyin_to_FLEX // single user
	  // that stays in effect, throughout the parsing, EOF and shutdown
	  // could set buffer-> ...
	  if(!buffer) {
	  	return FAIL("bad buffer did not end with two NUL bytes");
	  }

#if 0
	//
	// what the original code did - no parameters at all !!
	// yy_scan_bytes must have absorbed them all !
	//
	int t = yyparse();
#else
	// int ret_from_yyparse =
	call_yyparse(); 
#endif
	// cleans up input extras
	// loses BISON own opinions ?
	yy_delete_buffer(buffer); // yy_scan_bytes 

	// return code bool OK
	if(ret_from_yyparse) {
		// "syntax error"
		return FAIL("yyparse returned %d", ret_from_yyparse );	
	}
	// INFO("yyparse returned %d MUST_BE_XERO", ret_from_yyparse ); // 

	// if FAIL set errno to something
	// TODO("Review errno set here");

	return true; // yyparse returned 0
  }

  ///

bool Y_Parse_t::
buf_load_and_parse(		// load file then parse it, then return
   blk1 & text,			// load text from filename
   const char * filename,	// load entire file
   int K_max			// RULES is RULES
) {

	// claim global LOCK here
	// provide store to the Tree Parser HERE
	// FILE_VER.ITEM_idx
	// SEGMENT_DECOER = load_file _text DECODE_ACCES
	// PARSER stopped by a single NUL, so insist, no really

	if(!blk_read_entire_file( text, filename, K_max )) // util_buf.h
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
