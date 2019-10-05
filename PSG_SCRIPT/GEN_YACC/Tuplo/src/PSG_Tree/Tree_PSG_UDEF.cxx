
#include "Tree_PSG_UDEF.h"
#include "Tree_PSG_LEX_PUNCT_NAME.h"


bool Tree_PSG_UDEF::print_list(
	LEX_TOKEN_GROUP &  POOL,
	buffer2 & out
) {
	int n = POOL.LIST_Token.N();
	for( int i =0; i<n; i++ ) {
		LEX_TOKEN_DECL * tok = POOL.LIST_Token[ i ];
		out.put( "## DEFINE ## " );
		out.put( POOL.PFX );
		out.put( '_' );
		out.put( tok-> Name );
		out.put( " -> " );
		out.put( '"' );
		out.put( tok-> Value );
		out.put( '"' );
		out.put( '\n' );
	};
}

bool Tree_PSG_UDEF:: build_tree() {
	POOL_PUNCT.add_PUNCT( "==" );
	POOL_PUNCT.add_PUNCT( "!=" ); // alias NOT SHRIEK BANG EXCLAIM
	POOL_PUNCT.add_PUNCT( "++" ); // alias NOT SHRIEK BANG EXCLAIM
	POOL_PUNCT.add_PUNCT( "_EQUAL_EQUAL", "==" );
	POOL_PUNCT.add_PUNCT( "_NOT_EQUAL",   "!=" );
	POOL_PUNCT.add_PUNCT( "_CARET",       "^" );

	POOL_RW.add_RW( "if" );

	return true;
	// return FAIL("unwritten");
}

bool Tree_PSG_UDEF:: print_tree_as_files( ) {
	buffer2 out;
	print_list( POOL_PUNCT, out );
	print_list( POOL_RW, out );
	print_list( POOL_LEX, out );
	e_print( "%s", (STR0) out );
	return FAIL("unwritten");
}
