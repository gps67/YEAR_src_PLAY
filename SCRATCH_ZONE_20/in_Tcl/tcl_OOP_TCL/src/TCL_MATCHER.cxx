#include "TCL_MATCHER.h"
#include <string.h>

extern
bool CMP( Tcl_Obj * obj, const char * str )
{
	// this gets string rep of Literal // also anystr
	const char * str_word = Tcl_GetString( obj );
	return 0 == strcmp( str, str_word );
}

//// LITERAL_MATCHER:: MATCHES( Tcl_Obj * obj ) // bool inline 

bool LITERAL_MATCHER:: MATCHES_fn( Tcl_Obj * obj )
{
	if(!obj) {
	INFO("# NULL # LITERAL_MATCHER(%s).MATCHES_fn(NULL) # \n",
	"{ NULL OBJ ERROR }" );
	return false;
	}

	if(!match_one) {
	INFO("# NULL match_one # should be set by CTOR to LEX2 of LEX1\n");
	// see 
	return false;
	}

	if(!match_one->bytes) {
	INFO("# CODE_ERROR_CTOR_FAIL\n");
	// STAY // return false;
	}

	if(!obj->bytes) {
		INFO("# CODE_ERROR_CTOR_FAIL no bytes\n");
		Tcl_GetString( obj );
		// STAY // return false;
	}

if(0)	INFO("# CALL # LITERAL_MATCHER(%s).MATCHES_fn # %s \n",
		Tcl_GetString( match_one ),
		Tcl_GetString( obj ));


	if( obj == match_one ) return true;
//	if( obj == match_two ) return true;
	if( obj == match_two ) {
		INFO("# FAST MATCH # used match_two # %s \n",
			obj->bytes );
		return true;
	}
	if( obj == match_three ) {
		INFO("# FAST MATCH # used match_three # %s \n",
			obj->bytes );
		return true;
	}

	#if 0
	// there should be an ASM for this
	// plain searches though N can be blindingly fast
	// but setup overhead makes it less so
	// plough through NULL values

	// search through DIFFERENT[i] list of PTR
	// if found, just leave "MATCHES=false"

	for( int i = 0; i<N_different; i++ ) {
		Tcl_Obj * ITEM =  differents[i];
		const char * s = NULL;
		if(ITEM)
			s = differents[i]->bytes; // it exists with bytes
		else
			s = "ITEM (NULL)";
			continue;

		if(!s) s = "(NULL)";
		const char * s2 = match_one->bytes;
		if( obj == differents[i] ) {
			INFO("# %s CONFIRMED_NOT '%s' \n", s2, s );
			// add to counters
			return false;
		}
	}
	#endif

	// ASM OPTIM search for PTR in PTRS[ N ]
	// IF idx in lhs[2] or rhs[6]
	/*
		PARSE  UDEF_u8 = u8_SPEC

		match_one is it all in one
		match_two is local ALIAS that LINKS to MATCH_one

		so if it is not LEX1 or LEX2 it might be another LEX2
		so if it is TYPE_NULL, we want to upgrade it

			STR0 UPGRADE to LEX2 to LEX1
			the next call from that PROC 
			will be of TCL_TYPE_NAME UDEF_%2X // u8_SPEC_id

			SPEC_t * ea_spec = SPECS[u8_idx];
			u8_idx = UDEF_NN

		any other user of this system would LINK to LEX1
		
			LEX1 the PRIMARY_SPELLING
			LEX2 SIMILAR_SEPARATE LINK to LEX1

		LEX2 = new LEX2 // second spelling

		eg ALIAS LEX2 -> LEX1
		eg FILTER LEX2 .PTR1 -> LEX1
		eg FILTER LEX2 .PTR2 -> CP_where_this_LEX2_is_DECLARED_MAYBE

		eg FILTER { FILTER }

		   typePtr -> UDEF_NN 

		   PTR1
		   PTR2


	*/

	// obj not found in cached items
	// do the strcmp compare
	// maybe add it to cache 

	// 

#if 0
	// some sanity checks
	if( !(match_one )) { 
		INFO("**** // NULL match_one // set for first time\n");
		match_one = obj; 

		// that's it, but stay for upgrade obj to LEX2
	}

	if( !(match_one && match_one->bytes )) {
		// match_one is supposed to exist with string
		INFO("**** // NULL match_one\n");
		INFO("**** // NULL match_one bytes\n");
		return false;
	} // else OK stay happy
#endif

	// THIS is SPEC_FF xFF // something tokenised ITEM of PAGE_256
	// EA_u8 // MINI_MACHINE item in table[u8]
	// CLASS_LEX1 // IDX // PTR1 == LEX1_as_u32_TOKEN
	// CLASS_LEX2 // IDX // PTR2 == LEX1

	// match_ONE is "GET" or "array_get"
	// do the strcmp compare
	if( CMP( obj, match_one->bytes )) {
	//	print_tcl_obj( obj, "obj that is not LITERAL" );
	//	print_tcl_obj( match_one, "match_one" );
	//
		upgrade_to_LEX2( obj, match_one ); // MORPH_OBJ_TO_TYPE
		print_tcl_obj( obj, "obj_after_upgrade" );

		// match_two is rare, second route to "Literal"
		// hallens every LEX2 outside of proc
		if(!match_two) { // first LEX2
			match_two = obj; // 
			INFO("setting match_two");
			return true;
		}

		if(!match_three) { // another LEX2
			match_three = obj; // 
			INFO("setting match_three");
			return true;
		}

		// match_tree slides over most recent
		// that might fix it or not
		// I consider this a FAIL
		WARN("match_three OVERWRITE\n");
		FAIL("match_three OVERWRITE I consider this a FAIL\n");
		match_three = obj; // 
		
		const char * s = obj->bytes;
		INFO("match_three = %s",s);
		return true;
	} // else strcmp differs

	#if 0
	// add obj to differents[i] overwrite NULL or FULL
	for( int i = 0; i<N_different; i++ ) {
		if( differents[i] ) {
	INFO("keeping differents[%d] = %s \n",
			i, differents[i]->bytes );
		}
		if( !differents[i] ) {
			differents[i] = obj;
	INFO("setting differents[%d] = %s \n",
			i, differents[i]->bytes );
			// add to counters
			return false;
		}
	}
	// no space for another, N_different used
	WARN("differents[N] FULL");
	#endif
	return false;
}

//bool LITERAL_MATCHER:: upgrade_to_LEX1( Tcl_Obj * obj ) {
//	// ALIAS UDEF_%$ HERE UDEF_%X NN 
//	// ALIAS %$ HERE %X NN 
//	// TYPE = UDEF_LEX1
//	return true;
//}

bool LITERAL_MATCHER:: SEE_C_upgrade_to_LEX2( Tcl_Obj * obj,  Tcl_Obj * LEX1 ) {
	INFO("upgrade_to_LEX2 %s\n", obj->bytes);

	/*
		DESIGN PATTERN

			upgrade_STR_to_LEX2
			upgrade TEXT_to_KEY_VAL_LEX2_of_TEXT // not Literal
			upgrade_STR_to_PARSED_TREE 
			PARSE("bytes[length]")

		GEN_PRE_CT

			CTOR("OPCODE_STRING")

		OPTIONS

			Literal("opcode")
			TEXT_HELD("{ SCRIPT }")
			TREE_TEXT("{ TOKENISED SCRIPT }")

		static LEX2_SPEC; // a complete TYPE for ALIAS LEX2 to LEX1

		TOKEN
			TCL_TYPE create LEX2 ; return LEX2 ;
	*/
//	static u8_idx_SPEC_LEX2 = // OBJ->typePtr = type_Ptr_LEX2
//	static u8_idx_SPEC_LEX2 = // OBJ->typePtr = type_Ptr_LEX2
	return true; // but not actually done
	return false; // upgrade failed somehow
}

