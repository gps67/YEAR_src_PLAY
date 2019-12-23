
#include "Tree_PSG_UDEF.h"
// #include "Tree_PSG_LEX_PUNCT_NAME.h"


bool Tree_PSG_UDEF:: add_yacc_for_C_EXPR() {
	return true;
}

bool Tree_PSG_UDEF:: add_lex_for_C_EXPR() {

#warning new code here is where you add C priority left right options


// https://en.cppreference.com/w/c/language/operator_precedence
// tightest binding lowest priority
#define L2R "L"
#define R2L "R"


	POOL_PUNCT.add_PUNCT( "++", L2R "1" );  // postfix incremental
//	POOL_PUNCT.add_PUNCT( "++", L2R "2" );  // prefix incremental
	POOL_PUNCT.add_PUNCT( "--", L2R "1" );  // postfix decr
//	POOL_PUNCT.add_PUNCT( "--", L2R "2" );  // prefix decr

	POOL_PUNCT.add_PUNCT( "()", R2L "1" );  // NOT sure if operator PUNCT
	POOL_PUNCT.add_PUNCT( "[]", R2L "1" );  // NOT sure if operator PUNCT
	POOL_PUNCT.add_PUNCT(  ".", R2L "1" );  
	POOL_PUNCT.add_PUNCT( "->", R2L "1" );  

	// ABSENT // COMPUND LITERAL

//	POOL_PUNCT.add_PUNCT( "+", L2R "2" ); // AMBIG // UNARY PLUS
//	POOL_PUNCT.add_PUNCT( "-", L2R "2" ); // AMBIG // UNARY MINUS
	POOL_PUNCT.add_PUNCT( "!", L2R "2" ); // logical NOT
	POOL_PUNCT.add_PUNCT( "~", L2R "2" ); // bitwise NOT
	// ABSENT // TYPE CAST
	// ABSENT // AMBIG
//	POOL_PUNCT.add_PUNCT( "*", L2R "2" ); // AMBIG // DEREF
//	POOL_PUNCT.add_PUNCT( "&", R2L "2" ); // AMBIG // ADDR_of 
	// ABSENT // sizeof
	// ABSENT // _Alignof

	POOL_PUNCT.add_PUNCT( "*", L2R "3" ); // times
	POOL_PUNCT.add_PUNCT( "/", L2R "3" ); // divide
	POOL_PUNCT.add_PUNCT( "%", L2R "3" ); // remainder

	POOL_PUNCT.add_PUNCT( "+", L2R "4" ); // PLUS
	POOL_PUNCT.add_PUNCT( "-", L2R "4" ); // MINUS

	POOL_PUNCT.add_PUNCT( "<<", L2R "5" ); 
	POOL_PUNCT.add_PUNCT( ">>", L2R "5" ); 

	POOL_PUNCT.add_PUNCT( "<=", L2R "6" ); 
	POOL_PUNCT.add_PUNCT( ">=", L2R "6" ); 
	POOL_PUNCT.add_PUNCT( "<", L2R "6" ); 
	POOL_PUNCT.add_PUNCT( ">", L2R "6" ); 

	POOL_PUNCT.add_PUNCT( "==", L2R "7" ); 
	POOL_PUNCT.add_PUNCT( "!=", L2R "7" ); 

	POOL_PUNCT.add_PUNCT( "&", L2R "8" );  // bitwise AND
	POOL_PUNCT.add_PUNCT( "^", L2R "9" );  // bitwise XOR
	POOL_PUNCT.add_PUNCT( "|", L2R "10" );  // bitwise OR

	POOL_PUNCT.add_PUNCT( "&&", L2R "11" );  // logic AND_IF
	POOL_PUNCT.add_PUNCT( "!|", L2R "12" );  // logic OR_IF

// SYN	POOL_PUNCT.add_PUNCT( "?:", R2L "13" );  // lternary condiftion

	POOL_PUNCT.add_PUNCT( "<<=", R2L "14" ); 
	POOL_PUNCT.add_PUNCT( ">>=", R2L "14" ); 
	POOL_PUNCT.add_PUNCT( "=", R2L "14" ); // Assignment
	POOL_PUNCT.add_PUNCT( "+=", R2L "14" ); 
	POOL_PUNCT.add_PUNCT( "-=", R2L "14" ); 
	POOL_PUNCT.add_PUNCT( "*=", R2L "14" ); 
	POOL_PUNCT.add_PUNCT( "/=", R2L "14" ); 
	POOL_PUNCT.add_PUNCT( "%=", R2L "14" ); 
	POOL_PUNCT.add_PUNCT( "&=", R2L "14" ); // bitwise
	POOL_PUNCT.add_PUNCT( "^=", R2L "14" ); // bitwise
	POOL_PUNCT.add_PUNCT( "|=", R2L "14" ); // bitwise

	POOL_PUNCT.add_PUNCT( ",", R2L "15" ); // comma




// ASCII order -vs- PRIORITY ? 
// not obviosly, so ... meh

	POOL_PUNCT.add_PUNCT( "(" );
	POOL_PUNCT.add_PUNCT( ")" );
	POOL_PUNCT.add_PUNCT( "{" );
	POOL_PUNCT.add_PUNCT( "}" );

//	POOL_PUNCT.add_PUNCT( "." );

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

