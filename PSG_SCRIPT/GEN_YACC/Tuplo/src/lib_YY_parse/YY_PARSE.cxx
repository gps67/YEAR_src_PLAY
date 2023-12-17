#include "YY_PARSE.h"
#include "dgb.h"
#include "util_buf.h"
#include "buffer2.h"

// SOME STUBS for BISON
// see gen_e1_lex.cc


// STUBS only needed within this file
struct yy_buffer_state;

extern yy_buffer_state * yy_scan_buffer ( char *base, int size  );
extern yy_buffer_state * yy_scan_string ( const char *yy_str  );
extern yy_buffer_state * yy_scan_bytes ( const char *bytes, int len  );

extern void yyrestart ( FILE *input_file  );
// FILE * IN = fopen( filename, "r" ); // check NULL
// yyrestart ( IN );
// yyparse(PSG);

extern void yy_delete_buffer(yy_buffer_state * buffer);
// FREE BISON buffer position holder

// we KNOW the NUL will be there 
// it's OK if it is not // P0P2 instread of STR0
// thats what this parser brings, STR0_in_P0P2

#define  YY_END_OF_BUFFER_CHAR 0x00

// YY:: YY_Parse_t CALLS yyparse PROVIDES SELF.TREE // _BUILDER

// https://www.gnu.org/software/bison/manual/html_node/Parser-Function.html
// says ( this ) would work, but for unknown - need *this
// 

using namespace YY;

YY_Parse_t::
YY_Parse_t()
{
	INFO("CTOR");
}

YY_Parse_t::
~YY_Parse_t()
{
	INFO("DTOR");
}

int YY_Parse_t::
call_yyparse()
{
	ret_from_yyparse = yyparse( * this ); // matches T & V
//	ret_from_yyparse = yyparse( this ); 

 if(1)
	switch (ret_from_yyparse) {
	 case 0: INFO("0 means PASS yyparse"); break;
	 case 1: FAIL("1 means ERROR yyparse"); break;
	 case 2: FAIL("2 means MEMORY ERROR yyparse"); break;
	default: FAIL("%d unknown retval from yyparse", ret_from_yyparse );
	}

	return ret_from_yyparse; // OK0 //

	// CASE 1 should be virtual to get filename, line, col, seek, SEGMENT_ID
	// CASE 1 should report SEGMENT_ID in ROM // 

}

// C called from yyparse(psg)
// see Y_PARSE_gen.cxx:
// "%%parse-param {YY_Parse_t & psg} 

void yyerror( YY_Parse_t & psg, const char * msg )
{
	FAIL("Y_Parse.Name \"%s\", msg \"%s\" ", (STR0) psg.Name, msg );
	FAIL(" get_prog_alias() says %s", (STR0) get_prog_alias() );
	FAIL(" where is XPOS CPOS YPOS FILE LOCN %s ", (STR0) "csr_idx" );
	// gdb_invoke(false);
}


// METHODS

  bool YY_Parse_t::
  buf_yy_parse( blk1 & text ) // returns when done
  {
  	// YOU have already allocated buffer2 with loaded text and soon PADD
	// We are not using MMAP we are using pre_know malloc and mem_copy
	// We can use P0P2
	// We can use STR0
	// We can use BUFF // stream mode 
	// We can use FILE // pre_loaded_FILE // ASCII latin1 cryllic utf8 CSET
	// We can use XPOS // seek i24 in i8_EXPR // PICK_OPCODE[i8]

  	// YOU have already loaded text
	// we ALWAYS append a few NUL bytes
	// which might be what MMAP makes easy
	// alloc PAGES from HEAP of available malloc and free

  	// here we are polluting the STREAM with helpful noise EOLN NUL 
	// our own code avoids the need for the external check, provides own
	// EOLN NUL EOF // then this added stuff // ?? ?? NUL EOF
	// this also messes up MMAP // unless it is lenient COW PAGE_PLUS
	// HAPPENS before end of PAGE write to area plus skids
	// HAPPENS after end of PAGE create an extra PAGE for this NL_NUL_EOF
	// HAPPENS after end of PAGE create an extra PAGE for this NUL_EOF
	// HAPPENS after end of PAGE create an extra PAGE for this EOF CHECK
	// so we load the entire file in one go // i24_world //

	// CHECK in FILTER // PAGE in RANGE("VALID_RANGE") // of COMPILED PAGE_id
	// template and manually requested 
  	text.put_byte(YY_END_OF_BUFFER_CHAR); // 
  	text.put_byte(YY_END_OF_BUFFER_CHAR); // 
  	// PRE_BOOK FULL LOCK on text holder; // just add this change //
  	text.trailing_nul(); // noone else can add stuff, please no, NUL needed
	// LOCK text // SNAP nbytes // KEEP lock until DTOR of link to text
	// KNOW running PARSER over STREAM will cause DELAYS and WAIT_POINTS
	// KEEP LOCK, KEEP cache_copy_RO LOCK, no edits allowed, catchup

	// WE ARE PROVIDING text buffer. The STATE is a malloced struct
	// struct yy_buffer_state ... // in gen_e1_lex.cc

	yy_buffer_state * buffer =	// feed YY's LEX buffer with blk1 text
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
	int OK0 = 0;
	if( OK0 == call_yyparse() ) {
		// OK 
	} else {
	}
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

bool YY_Parse_t::
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
