
#include "lex_yacc_UDEF_JSON.h"
// #include "lex_yacc_LEX_PUNCT_NAME.h"
using namespace PSG;

bool lex_yacc_UDEF_JSON:: build_tree_lex() {
	if(!add_lex_for_JSON()) return FAIL_FAILED();
	INFO("JSON");
	return true;
}
bool lex_yacc_UDEF_JSON:: build_tree_yacc() {
	opt_yywrap = false;
	if(!add_yacc_for_JSON()) return FAIL_FAILED();
	// bool add_yacc_for_C_EXPR();
	INFO("JSON");
	return true;
}


bool lex_yacc_UDEF_JSON:: add_lex_for_JSON() {

	/*
		INVOKE { "JSON"
		 main() uses ARGV to set GEN PSG = PSG_AFM 
		 main() uses ARGV to set GEN PSG = PSG_CALC
		 main() uses ARGV to set GEN PSG = PSG_JSON
		 TOKEN PSG_name_of_PROTOTYPE // VAR = STR0("AFM") // first alias
		 TOKEN PSG_name // VAR = STR0("JSON") // TOKENISED_to_u64_expr
		 // PSG.name == "AFM" // comment moves inside "TAG // CMNT"
		 // PSG.name == "TUPLO // binary_text" // binary in text // xFF
		 // PSG.name == "CALC" // 
		 // PSG.name == "EXPR" // 
		 // PSG.name == "SCRIPT" // 
		 // PSG.name == "JSON" // 
		}

		so the TECH zone, for everything todo with GEN PSG for SCRIPT
		so the TECH zone, and for EXPR
		so the TECH zone, and for JSON

		STO.JSON { SAYS }
		 add_DATA_for_JSON
		  PARSED from STO 

		 KEEP OFFS in ROM to track rerun ROM at any point
		  
	*/

#warning new code here is where you add C priority left right options

	POOL_RW.add_RW( "JSON_RESERVED_WORD_none" );

	// Names of LEX exprs that match values

	POOL_LEX.add_LEX( "EOLN" );
	POOL_LEX.add_LEX( "WORD" );
	POOL_LEX.add_LEX( "WS" );
	POOL_LEX.add_LEX( "DOUBLE" );
	POOL_LEX.add_LEX( "INTEGER" );
	POOL_LEX.add_LEX( "BOOL" );
//	POOL_LEX.add_LEX( "FLOAT" );

	WARN("UNWRITTEN");
	// return FAIL("UNWRITTEN");

	return true;
}


///////////////////////////////////////////////////////////////////////////

bool lex_yacc_UDEF_JSON:: add_yacc_for_JSON() {
	return true;
	return FAIL("TODO");
}
