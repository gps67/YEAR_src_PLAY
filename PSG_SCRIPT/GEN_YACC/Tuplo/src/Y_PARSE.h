#ifndef Y_PARSE_H
#define Y_PARSE_H

/*
	Y_Parse _t
	 named PSG
	 mux_in_different_dialects // eg ARGV no comma
	 currently running over ... TEXT ... lines_of_lines ...

	This is flawed - it should not matter that yyparse does not exist yet
	in test1 no actual parsing happens, it is all generating
	Probably switching to not-a-class-stic might fix it
*/

#include "EXPRS_STUBS.h"
#include "str1.h"

// namespace ??
namespace YY {
	// NOT YET
}; // namespace

/*	USERS of PARSER
	run it though this
*/
 struct Y_Parse_t {
 	str1 Name;
	Y_Parse_t( str0 _Name )
	: Name( _Name ) // entire filename or item id or any helpful default
	{
		// Now you can access the library of PSG code
		// You might even login by making the right enquiries
		// Plus you get plain data parameters ARGV style OBJV
		// STRING NUMBER OBJECT_with_added_access_filter TUPLO ARGS
	}

	int call_yyparse();
	int ret_from_yyparse; // 0==PASS 1==FAIL 2==ENOMEM


	bool buf_yy_parse( blk1 & text ); // 

  	bool buf_load_and_parse( // load file into buffer, then call_yyparse()
	   blk1 & text,			// load text keep keep text // not MMAP
	   const char * filename,	// filename expect to load entire file
	   int K_max			// RULES is RULES
	);
 };

using namespace YY;

extern	int yyparse( Y_Parse_t & parser );
extern	void yyerror( Y_Parse_t & parser, const char * msg );
extern	bool gen_yyparse_parameter( buffer2 & out);


#endif


