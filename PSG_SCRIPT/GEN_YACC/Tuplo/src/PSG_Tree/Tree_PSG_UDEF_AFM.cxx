
#include "Tree_PSG_UDEF_AFM.h"
// #include "Tree_PSG_LEX_PUNCT_NAME.h"

bool Tree_PSG_UDEF_AFM:: build_tree_lex() {
	if(!add_lex_for_AFM()) return FAIL_FAILED();
	INFO("NOT bool add_yacc_for_C_EXPR()");
	return true;
}
bool Tree_PSG_UDEF_AFM:: build_tree_yacc() {
	if(!add_yacc_for_AFM()) return FAIL_FAILED();
	// bool add_yacc_for_C_EXPR();
	INFO("NOT bool add_yacc_for_C_EXPR()");
	return true;
}


bool Tree_PSG_UDEF_AFM:: add_lex_for_AFM() {

	/*
		INVOKE { "e1"
		 main() uses ARGV to set GEN PSG = PSG_AFM 
		 main() uses ARGV to set GEN PSG = PSG_CALC
		 TOKEN PSG_name // VAR = STR0("AFM") // TOKENISED_to_u64_expr
		 // PSG.name == "AFM" // comment moves inside "TAG // CMNT"
		 // PSG.name == "TUPLO // binary_text" // binary in text // xFF
		 // PSG.name == "CALC" // 
		 // PSG.name == "EXPR" // 
		 // PSG.name == "SCRIPT" // 
		}

		so the TECH zone, for everything todo with GEN PSG for SCRIPT
		so the TECH zone, and for EXPR
		so the TECH zone, and for AFM

		FONT.AFM { SAYS }
		 add_DATA_for_AFM
	int	  glyph_width [ char_glyph_idx ] // ASCII_PLUS
	obj *	  glyph_info( chr ) // _idx
		  FILE_LINE_t PHRASE_FOUND = EXPR_FOUND = FACT_FOUND
		  
	*/

#warning new code here is where you add C priority left right options

	INFO("STEP");
	return FAIL("UNWRITTEN");

// https://en.cppreference.com/w/c/language/operator_precedence
// tightest binding lowest priority
#define L2R "L"
#define R2L "R"


	POOL_PUNCT.add_PUNCT( "++",  L2R "1", "arith post_incr"  );  
//	POOL_PUNCT.add_PUNCT( "++",  L2R "2", "arith prefix_incr"  );  
	POOL_PUNCT.add_PUNCT( "--",  L2R "1", "arith postfix_decr"  );  
//	POOL_PUNCT.add_PUNCT( "--",  L2R "2", "arith prefix_decr"  );  

	POOL_PUNCT.add_PUNCT( "()",  R2L "1", "code PARENTH"  );  
	POOL_PUNCT.add_PUNCT( "[]",  R2L "1", "code ARRAY"  );  
	POOL_PUNCT.add_PUNCT(  ".",  R2L "1", "code dot_field"  );  
	POOL_PUNCT.add_PUNCT( "->",  R2L "1", "code PTR_star_dotFIELD"  );  

	// ABSENT // COMPUND LITERAL

//	POOL_PUNCT.add_PUNCT(  "+",  L2R "2", "arith unary_plus"  ); 
//	POOL_PUNCT.add_PUNCT(  "-",  L2R "2", "arith unary_minus"  ); 
	POOL_PUNCT.add_PUNCT(  "!",  L2R "2", "logic NOT"  ); 
	POOL_PUNCT.add_PUNCT(  "~",  L2R "2", "bitwise NOT"  ); 
	// ABSENT // TYPE CAST
	// ABSENT // AMBIG
//	POOL_PUNCT.add_PUNCT(  "*",  L2R "2, "code STAR_deref_PTR" ); // AMBIG 
//	POOL_PUNCT.add_PUNCT(  "&",  R2L "2", "code ADDR_OF"  ); // AMBIG // 
	// ABSENT // sizeof
	// ABSENT // _Alignof

	POOL_PUNCT.add_PUNCT(  "*",  L2R "3", "arith times"  ); 
	POOL_PUNCT.add_PUNCT(  "/",  L2R "3", "arith divide"  ); 
	POOL_PUNCT.add_PUNCT(  "%",  L2R "3", "arith remainder"  ); 

	POOL_PUNCT.add_PUNCT(  "+",  L2R "4", "arith plus"  ); 
	POOL_PUNCT.add_PUNCT(  "-",  L2R "4", "arith minus"  ); 

	POOL_PUNCT.add_PUNCT( "<<",  L2R "5", "bitwise shift_left"  ); 
	POOL_PUNCT.add_PUNCT( ">>",  L2R "5", "bitwise shift_right"  ); 

	POOL_PUNCT.add_PUNCT( "<=",  L2R "6", "cmp less_than_or_equal"  ); 
	POOL_PUNCT.add_PUNCT( ">=",  L2R "6", "cmp greater_than_or_equal"  ); 
	POOL_PUNCT.add_PUNCT(  "<",  L2R "6", "cmp less_than"  ); 
	POOL_PUNCT.add_PUNCT(  ">",  L2R "6", "cmp greather_than"  ); 

	POOL_PUNCT.add_PUNCT( "==",  L2R "7", "cmp EQUAL"  ); 
	POOL_PUNCT.add_PUNCT( "!=",  L2R "7", "cmp NOT_EQUAL"  ); 
	POOL_PUNCT.add_PUNCT( "===",  L2R "7", "cmp js triple EQUAL"  ); 

	POOL_PUNCT.add_PUNCT(  "&",  L2R "8", "bitwise AND"  );  
	POOL_PUNCT.add_PUNCT(  "^",  L2R "9", "bitwise XOR"  );  
	POOL_PUNCT.add_PUNCT(  "|",  L2R "10", "bitwise OR"  );  

	POOL_PUNCT.add_PUNCT( "&&=",  L2R "11", "logic AND_IF" ); 
	POOL_PUNCT.add_PUNCT( "||=",  L2R "12", "logic OR_IF"  );

	POOL_PUNCT.add_PUNCT( "&&",  L2R "11", "logic AND_IF" ); 
	POOL_PUNCT.add_PUNCT( "||",  L2R "12", "logic OR_IF"  );

// SYN	POOL_PUNCT.add_PUNCT( "?:",  R2L "13", "code TERNARY"  );  // lternary condiftion

	POOL_PUNCT.add_PUNCT( "<<<", L2R "14", "bitwise ROTATE"  ); 
	POOL_PUNCT.add_PUNCT( ">>>", L2R "14", "bitwise ROTATE"  ); 

	// CHECK: who says these are R2L not L2R // or is it the =

	POOL_PUNCT.add_PUNCT( "<<=", R2L "14", "bitwise SHIFT"  ); 
	POOL_PUNCT.add_PUNCT( ">>=", R2L "14", "bitwise SHIFT"  ); 
	POOL_PUNCT.add_PUNCT( "<<<=", R2L "14", "bitwise ROTATE"  ); 
	POOL_PUNCT.add_PUNCT( ">>>=", R2L "14", "bitwise ROTATE"  ); 

	POOL_PUNCT.add_PUNCT(  "=",  R2L "14", "code ASSIGN"  ); 
	POOL_PUNCT.add_PUNCT( "+=",  R2L "14", "arith plus"  ); 
	POOL_PUNCT.add_PUNCT( "-=",  R2L "14", "arith minus"  ); 
	POOL_PUNCT.add_PUNCT( "*=",  R2L "14", "arith times"  ); 
	POOL_PUNCT.add_PUNCT( "/=",  R2L "14", "arith divide"  ); 
	POOL_PUNCT.add_PUNCT( "%=",  R2L "14", "arith remainder"  ); 
	POOL_PUNCT.add_PUNCT( "&=",  R2L "14", "bitwise AND"  ); // bitwise
	POOL_PUNCT.add_PUNCT( "^=",  R2L "14", "bitwise XOR"  ); // bitwise
	POOL_PUNCT.add_PUNCT( "|=",  R2L "14", "bitwise OR"  ); // bitwise
 
	POOL_PUNCT.add_PUNCT(  ",",  R2L "15", "code comma"  ); // comma


	// ASCII order -vs- PRIORITY ? 
	// not obviosly, so ... meh

	POOL_PUNCT.add_PUNCT( "(" );
	POOL_PUNCT.add_PUNCT( ")" );
	POOL_PUNCT.add_PUNCT( "{" );
	POOL_PUNCT.add_PUNCT( "}" );

//	POOL_PUNCT.add_PUNCT( "." );

	POOL_PUNCT.add_PUNCT( "//" );
	POOL_PUNCT.add_PUNCT( "/*" );
	POOL_PUNCT.add_PUNCT( "*/" );

	// ERROR of DOUBLE add_PUNCT --

	// Reserved Words -- value is self

	POOL_RW.add_RW( "if" );

	// Names of LEX exprs that match values

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
}


///////////////////////////////////////////////////////////////////////////

bool Tree_PSG_UDEF_AFM:: add_yacc_for_AFM() {
	return FAIL("TODO");
}
