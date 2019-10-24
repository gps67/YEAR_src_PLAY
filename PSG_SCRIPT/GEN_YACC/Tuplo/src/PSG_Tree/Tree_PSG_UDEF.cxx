
#include "Tree_PSG_UDEF.h"
#include "Tree_PSG_LEX_PUNCT_NAME.h"



bool Tree_PSG_UDEF:: build_tree() {
	POOL_PUNCT.add_PUNCT( "==" );
	POOL_PUNCT.add_PUNCT( "!=" ); 
	POOL_PUNCT.add_PUNCT( "<=" ); 
	POOL_PUNCT.add_PUNCT( ">=" ); 
	POOL_PUNCT.add_PUNCT( "<<" ); 
	POOL_PUNCT.add_PUNCT( ">>" ); 
	POOL_PUNCT.add_PUNCT( "<<=" ); 
	POOL_PUNCT.add_PUNCT( ">>=" ); 
	POOL_PUNCT.add_PUNCT( "++" ); 
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

	POOL_RW.add_RW( "if" );

	POOL_LEX.add_LEX( "EOLN" );
	POOL_LEX.add_LEX( "IDENTIFIER" );
	POOL_LEX.add_LEX( "WS" );
	POOL_LEX.add_LEX( "DOUBLE" );
	POOL_LEX.add_LEX( "INTEGER" );

	// SEQ(" if ( EB ) SN1 [ else SN2 ] ")

	// OR collect TOKENS as they are used
	// expr brings collection of match like exprs, like C++

	return true;
	// return FAIL("unwritten");
}
