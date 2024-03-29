
#include "lex_yacc_UDEF_AFM.h"
// #include "lex_yacc_LEX_PUNCT_NAME.h"
using namespace PSG;

bool lex_yacc_UDEF_AFM:: build_tree_lex() {
	if(!add_lex_for_AFM()) return FAIL_FAILED();
	INFO("AFM");
	return true;
}
bool lex_yacc_UDEF_AFM:: build_tree_yacc() {
	opt_yywrap = false;
	if(!add_yacc_for_AFM()) return FAIL_FAILED();
	// bool add_yacc_for_C_EXPR();
	INFO("AFM");
	return true;
}


bool lex_yacc_UDEF_AFM:: add_lex_for_AFM() {

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

	POOL_RW.add_RW( "StartFontMetrics" );

	// PARSER is ARGV of WORDS // OBJV //

	// provide ACTUAL ";" as "PUNCT"
	// its SPECIAL NAME is not for most PUNCT GLYPHS // MULTI_CHAR
	POOL_PUNCT.add_PUNCT( ";", "L1", "semicolon_EOLN" );

	POOL_RW.add_RW( "Comment" );
	POOL_RW.add_RW( "FontName" );
	POOL_RW.add_RW( "FullName" );
	POOL_RW.add_RW( "FamilyName" );
	POOL_RW.add_RW( "Weight" );
	POOL_RW.add_RW( "ItalicAngle" );
	POOL_RW.add_RW( "IsFixedPitch" );
	POOL_RW.add_RW( "UnderlinePosition" );
	POOL_RW.add_RW( "UnderlineThickness" );
	POOL_RW.add_RW( "Version" );
	POOL_RW.add_RW( "Notice" );
	POOL_RW.add_RW( "EncodingScheme" );
	POOL_RW.add_RW( "FontBBox" );
	POOL_RW.add_RW( "CapHeight" );
	POOL_RW.add_RW( "XHeight" );
	POOL_RW.add_RW( "Descender" );
	POOL_RW.add_RW( "Ascender" );
	POOL_RW.add_RW( "StartCharMetrics" );

	POOL_RW.add_RW( "C" );
	POOL_RW.add_RW( "WX" );
	POOL_RW.add_RW( "N" );
	POOL_RW.add_RW( "B" );


	// Names of LEX exprs that match values

	POOL_LEX.add_LEX( "EOLN" );
	POOL_LEX.add_LEX( "WORD" );
	POOL_LEX.add_LEX( "WORD_TEXT_LIST" ); // KLUDGE of NOT LINE_TAIL
	POOL_LEX.add_LEX( "WS" ); // GAP
	POOL_LEX.add_LEX( "DOUBLE" );
	POOL_LEX.add_LEX( "INTEGER" );
	POOL_LEX.add_LEX( "BOOL" );
//	POOL_LEX.add_LEX( "FLOAT" ); // or AFM is INTS only

	WARN("UNWRITTEN");
	// return FAIL("UNWRITTEN");

	return true;
}


///////////////////////////////////////////////////////////////////////////

bool lex_yacc_UDEF_AFM:: add_yacc_for_AFM() {

	// the original plan was to build TREE_ONE
	// then only print out the parts used (TREE_TWO ?)
	// or add each of these as they are USED in a rule
	// UNION of RET_VAL from LEX and from YACC

	union_field_list.lookup_add( "tokn",     "int tokn" );
	union_field_list.lookup_add( "lex_buff", "const char * lex_buff" );
//	union_field_list.lookup_add( "expr",     "struct EXPR * expr" );
//	union_field_list.lookup_add( "e32",      "u32 e32" );

//	YACC_type_list_1.add_type_rule( "expr", "expr_ident" );
	YACC_type_list_1.add_type_rule( "expr", "expr_ident" );
//	YACC_type_list_1.add_type_rule( "expr", "expr" );
//	YACC_type_list_1.add_type_rule( "expr", "EXPR_line" );
//	YACC_type_list_1.add_type_rule( "expr", "LINES" );


	return true;
	return FAIL("TODO");
}
