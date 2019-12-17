
#include "Tree_PSG_UDEF.h"
// #include "Tree_PSG_LEX_PUNCT_NAME.h"


bool Tree_PSG_UDEF:: add_yacc_for_C_EXPR() {
	return true;
}

bool Tree_PSG_UDEF:: add_lex_for_C_EXPR() {

	POOL_PUNCT.add_PUNCT( "<<=" ); 
	POOL_PUNCT.add_PUNCT( ">>=" ); 

	POOL_PUNCT.add_PUNCT( "==" );
	POOL_PUNCT.add_PUNCT( "!=" ); 
	POOL_PUNCT.add_PUNCT( "<=" ); 
	POOL_PUNCT.add_PUNCT( ">=" ); 
	POOL_PUNCT.add_PUNCT( "<<" ); 
	POOL_PUNCT.add_PUNCT( ">>" ); 
	POOL_PUNCT.add_PUNCT( "++" ); 

// ASCII order -vs- PRIORITY ? 
// not obviosly, so ... meh

	/*
	POOL_PUNCT.add_PUNCT( "<" ); 
	POOL_PUNCT.add_PUNCT( ">" ); 
	POOL_PUNCT.add_PUNCT( "=" ); 
	POOL_PUNCT.add_PUNCT( "(" ); 
	POOL_PUNCT.add_PUNCT( ")" ); 
	POOL_PUNCT.add_PUNCT( "{" ); 
	POOL_PUNCT.add_PUNCT( "}" ); 
	POOL_PUNCT.add_PUNCT( "." ); 
	POOL_PUNCT.add_PUNCT( "+" ); 
	POOL_PUNCT.add_PUNCT( "-" ); 
	POOL_PUNCT.add_PUNCT( "*" ); 
	POOL_PUNCT.add_PUNCT( "/" ); 
	POOL_PUNCT.add_PUNCT( "^" ); 
	POOL_PUNCT.add_PUNCT( "&" ); 
	POOL_PUNCT.add_PUNCT( "%" ); 
	POOL_PUNCT.add_PUNCT( "|" ); 
	POOL_PUNCT.add_PUNCT( "~" ); 
	*/

	POOL_PUNCT.add_PUNCT( "*" ); 
	POOL_PUNCT.add_PUNCT( "+" ); 
	POOL_PUNCT.add_PUNCT( "^" ); 

	POOL_PUNCT.add_PUNCT( "<" ); 
	POOL_PUNCT.add_PUNCT( ">" ); 
	POOL_PUNCT.add_PUNCT( "=" ); 
	POOL_PUNCT.add_PUNCT( "(" ); 
	POOL_PUNCT.add_PUNCT( ")" ); 
	POOL_PUNCT.add_PUNCT( "{" ); 
	POOL_PUNCT.add_PUNCT( "}" ); 
	POOL_PUNCT.add_PUNCT( "." ); 
	POOL_PUNCT.add_PUNCT( "-" ); 
	POOL_PUNCT.add_PUNCT( "/" ); 
	POOL_PUNCT.add_PUNCT( "&" ); 
	POOL_PUNCT.add_PUNCT( "%" ); 
	POOL_PUNCT.add_PUNCT( "|" ); 
	POOL_PUNCT.add_PUNCT( "~" ); 

// ERROR of DOUBLE add_PUNCT --

	POOL_RW.add_RW( "if" );

	POOL_LEX.add_LEX( "EOLN" );
	POOL_LEX.add_LEX( "IDENTIFIER" );
	POOL_LEX.add_LEX( "WS" );
	POOL_LEX.add_LEX( "DOUBLE" );
	POOL_LEX.add_LEX( "INTEGER" );

	// PSG // VALUE Value_type value_as_str value_as_binary // except BLOB
	// PSG // NAME // portal to DECLARATION 
	// PSG // NAME // macro over USAGE
	// PSG // NAME // bound to NAMED_ITEM

	// SEQ(" if ( EB ) SN1 [ else SN2 ] ")

	// OR collect TOKENS as they are used
	// expr brings collection of match like exprs, like C++

	return true;
	// return FAIL("unwritten");
}

