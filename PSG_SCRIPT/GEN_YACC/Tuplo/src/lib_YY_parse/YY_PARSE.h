#ifndef YY_PARSE_H
#define YY_PARSE_H

/*
	YY_Parse _t
	 named PSG

	 run YY_Parse over TEXT passing SELF as only ARG into BISON and FLEX
	 that builds a TREE, held by SELF

	 Grammar is FIXED - how to set prefix per Grammar ?

*/

#include "EXPRS_STUBS.h"
#include "str1.h"

// namespace ??
namespace YY { /* NOT YET */ }; // namespace

/*	USERS of PARSER
	run it though this
	SELF is parameter to invoked functions
	OPTIONS 4 parameters but all in ONE
*/
 struct YY_Parse_t {

 	YY_Parse_t();
 	virtual ~YY_Parse_t();

	// debugging Name //
 	// this PARSER is called "PARSER" // "YY_Parse" "_t"
 	// this PARSER is called "FILENAME" // "TEXT_to_DATA.txt" // SCRIPT
 	// this PARSER is called "PHRASENAME" // "parse_PHRASE" // even in ROM
	// local copy of external STR0 // COMPILER moves to ROM.SEGMENT.AREA
 	str1 Name;

	YY_Parse_t( str0 _Name ); // name is PSG Name eg "AFM"

	int call_yyparse();
	int ret_from_yyparse; // 0==PASS 1==FAIL 2==ENOMEM==recursive.loop

//	virtual bool buf_append_NL_NUL_EOF();

	bool buf_yy_parse( blk1 & text ); // 

  	bool buf_load_and_parse( // load file into buffer, then call_yyparse()
	   blk1 & text,			// load text keep keep text // not MMAP
	   const char * filename,	// filename expect to load entire file
	   int K_max			// RULES is RULES
	);
 };

using namespace YY;

// because of %parse-param
// %parse-param {YY_Parse_t & psg} // PARAMETER // NB REF AMP
// yy..( parser )


extern	int yyparse( YY_Parse_t & parser );
// call parser.parse()
extern	void yyerror( YY_Parse_t & parser, const char * msg );
// call parser.yy_error( msg )

extern	bool gen_yyparse_parameter( buffer2 & out, STR0 decl = 0 );
extern	bool gen_yyparse_prefix( buffer2 & out, STR0 pfx_ = 0 );


#endif


