
#include "Tree_PSG_UDEF.h"
#include "Tree_PSG_LEX_PUNCT_NAME.h"


/*
	The TOKEN = POOL_tok

	Because PFX_ comes from pool
	Because we dont have an uplink
	Because we didn't take "PFX_"

	On the other hand
	u4_tok taken from POOL somewhere

	In a sense, POOL is a list, idx in list ...
	but we have forgotten that ?
*/
bool Tree_PSG_UDEF::print_TOKEN_name_2(
	buffer2 & out,
	LEX_TOKEN_GROUP &  POOL,
	LEX_TOKEN_DECL * tok //  tok = POOL.LIST_Token[ i ];
) {
	//	out.put( "/* LEX_ */ " );
		out.put( POOL.PFX );	// MADNESS HERE
		out.put( '_' );
		out.put( tok-> Name );
		return true;
}


bool Tree_PSG_UDEF::print_list(
	buffer2 & out,
	LEX_TOKEN_GROUP &  POOL
) {
	int n = POOL.LIST_Token.N();
	for( int i =0; i<n; i++ ) {
		LEX_TOKEN_DECL * tok = POOL.LIST_Token[ i ];
		out.put( "## DEFINE ## " );

		print_TOKEN_name_2( out, POOL, tok );

		out.put( " -> " );
		out.put( '"' );
		out.put( tok-> Value );
		out.put( '"' );
		out.put( '\n' );
	};
	// tok out of scape
	// INFO("is this called. value = '%s'", tok->Value );
	// INFO("is this called. out = '%s'", (STR0) out );
	return true;
}

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

	// SEQ(" if ( EB ) SN1 [ else SN2 ] ")

	// OR collect TOKENS as they are used
	// expr brings collection of match like exprs, like C++

	return true;
	// return FAIL("unwritten");
}

bool Tree_PSG_UDEF:: print_tree_as_files( ) {
	buffer2 out;
	out.put("#\n");
	out.put("# LIST PUNCT\n");
	print_list( out, POOL_PUNCT );
	out.put("# LIST RW\n");
	print_list( out, POOL_RW );
	out.put("# LIST LEX\n");
	print_list( out, POOL_LEX );
	// return true; PARAMETER out += added_text
	e_print( "%s", (STR0) out );
	return FAIL("unwritten");
}
