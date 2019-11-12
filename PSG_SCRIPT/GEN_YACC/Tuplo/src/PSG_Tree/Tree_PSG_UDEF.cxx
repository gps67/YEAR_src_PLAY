
#include "Tree_PSG_UDEF.h"
#include "Tree_PSG_LEX_PUNCT_NAME.h"


// SEE: RUN_at_CT // to get PREBUILT machine at init_val_one
// SEE: STO_fold_away // elf_segment expands to .bss or MMAP_fd
// MMAP_fd (is usually /dev/zero to get it to give up solo-owned clean pages)
// MMAP_fd = API_LINK VFS simulating ROM_FILE init_1 data then
// MMAP_fd = API_LINK PKT simulating ROM_FILE_DATA_XFER in API_STREAM
// SERVER delivers REQ_VER_WHEN DIR/FILE/DATA + META_DATA
// SERVER filters requests through CHECKER_FILTER
// RELAY filters are either inside or outside, or vetted


bool Tree_PSG_UDEF:: build_tree() {
	if(! build_tree_lex() ) return FAIL_FAILED(); 
	if(! build_tree_yacc() ) return FAIL_FAILED(); 
	return true;
}

// this will befollowed by EVERY lex used by PSG 
// we have to define the longest first, so others later
// maybe automate as PUNCT4 PUNCT3 PUNCT2 PUNCT1
bool Tree_PSG_UDEF:: build_tree_lex() {

	// manually SORT longest first

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
