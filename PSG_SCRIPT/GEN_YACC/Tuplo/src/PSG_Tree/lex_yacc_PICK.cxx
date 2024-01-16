#include "lex_yacc_PICK.h"
#include "lex_yacc.h"
#include "dgb.h"
#include "lex_yacc_UDEF.h"
#include "lex_yacc_UDEF_AFM.h"
#include "lex_yacc_UDEF_CALC.h"
#include "lex_yacc_UDEF_JSON.h"

using namespace PSG;

// PICK BASE CXX code for lex_yacc_UDEF(_BASE,_FILE)
// in practice FILE will invoke _BASE
// or use PREFIX REQUIRED then FILTERED ALIAS "lex_yacc_UDEF_CALC_e1"
// BASE CALC // NAME e1

lex_yacc_UDEF * PSG::mk_lex_yacc_UDEF( STR0 _PSG_Name_BASE, STR0 _PSG_Name_UDEF )
{
	str0 PSG_Name_BASE = (str0)_PSG_Name_BASE;
	str0 PSG_Name_UDEF = (str0)_PSG_Name_UDEF;
	lex_yacc_UDEF * _PSG = NULL;

	if( PSG_Name_BASE == "AFM" ) {
		_PSG = new  lex_yacc_UDEF_AFM();
	} else

	if(( PSG_Name_BASE == "e1" )  
	 ||( PSG_Name_BASE == "CALC" )) {
		_PSG = new  lex_yacc_UDEF_CALC();
		/* ASIDE VFS from API 'if(LANG=="CALC")...' */
#if 0

		PSG_NAME is one of " AFM e1 CALC CXX ARGV_via_PSG _via_NAME PSG"

		# that PICK from LIST also has PICK from IDENT # DIALECT += EXPR
		# DIALECT += { a1_a2_star a1 a2 } SPELLING("%s") cident99 ALIAS ITEM
		# VAR_POOL("VAR_NAME") VAR_POOL_api get_NAME_from_EA_ITEM
		# COMPILE "VAR_NAME" as "VAR_NAME_%s"
		# SCRIPT PERMIT "DECL VAR_NAME ..." # += CODE_POINT XPOS
		# a TOKEN and an XPOS # OFTEN XPOS == BOLN # on_OPEN
		# a SCRIPT_IF "if( CSR == "BOLN" ) ... # claim HERE is BOLN"

		# OK above BOLN" "is  errpor,
		# even when if 0
		# because # PARSER needs to read each # line
		# and IT finds Q2 uneven

		# COMPILE # GEN "{ DECL EA_BOLN == EA_HERE }" #
		# GROUP SCRIPT_PROVIDES_GROUP_NAME "%s" must match cident99 

		# here "CALC" is the name of the PSG for CXX_SCRIPT_EXPR
		# CALC += CXX_SCRIPT_EXPR # LIBR_local
		# PSG += CXX_SCRIPT_EXPR # LIBR_local PSG_CXX
		# CXX += PSG_CXX


			CXX_
			_IDENT
			_VAR_NAME
			_FIELD_NAME
			_NAME
			_IDEN

		# CXX += PSG_CXX

			SCRIPT_is_CXX

			 PHRASE_BOOK
			 LEXICON
			 VAR_POOL
			 TYPE_SPEC

			 TYPE_SPEC("%s") // TYPE_SPEC == VAR_NAME == "VAR_NAME"
			 TYPE_SPEC("%s") // TYPE_SPEC == VAR_NAME == "TYPE_SPEC"
			 TYPE_SPEC("%s") // TYPE_SPEC == ALIAS_NAME == "TYPE_SPEC"

		# WHEN SCRIPT is CT_RT 

			KNOW STEP_id_CXX_STEP
			EG STEP_id_ARGV_STEP _id == "OFFS_in_FILE" # XPOS OFFS
			EG STEP_id_API_STEP

		# WHEN PARSER on SCRIPT is CT_RT

			CT_RT COMPILE SCRIPT

		# simpy writing that ARGV made it a SCRIPT STEP A_LINE an_ARGV

			CT_RT COMPILE SCRIPT STEP == "XPOS_OFFS"

		# ARGV is almost a CXX SCRIPT_LINE_ARGV

			::module::FILE::XPOS::ARGV

		# ADDR_EA_EXPR # 

			DECL typename varname 
			"i32 ea_expr"
			"i32 ea_expr = %s"
			"i32 ea_expr == %s"
			"i32 ea_expr == '%s'" // OPTIONAL mechanism of Q2_Q1

			STREAM "%s_%s" "${NAME}" "%04x"
			// DETECT MATCH "${%s}" NAME == "NAME" 
			// DETECT MATCH "${%s}" NAME == "{NAME}" 
			// DETECT MATCH "${%s}" NAME == "{%s}" NAME
			// STRICT CHECK NAME_is_CIDENT99 // PSG_Name_UDEF and USAGE
			// MATCH "04x" as TOKEN("04x") VALUE() PROVIDE %s
			// MATCH NAME_x%04x // use SP as LEX_EDGE pre Q2 AUTO
			// AUTO Q2 "%04X" // ACTUAL CURR VAL is_UPPER_HEX_QUAD
			// HEX_QUAD_0xFFFF
			// HEX_QUAD_0xF0FF // API_u24_PAYLOAD API_CPU_REG
			// BYTE_B BYTE_A i16_BITFIELD i24_BITFIELD _i16 _i32
			// BITFIELD_name i8_opcode i24
			// when SCRIPT exceeds i32 pre_compile GETTERS ACTS
			// u8_OPCODE // u24_PAYLOAD // u24_BITFIELD 

			ITEM.subfield

			EA == OFFS_of_BOLN_in_FILE // XPOS_OFFS //
			EA == OFFS_in_FILE // XPOS_in_LINE // PARSED_CIDENT99

			EA == ARGV[XPOS_idx] // [i8[0[i8 WRAPPED_onto_ZERO
			## thats the FULL flag N==0 ## but cant have N==0 or can

			"%s_x%04x" "VARNAME_xFFFF" // AUTO_ALIAS PARSE
			"%s_x%04X" VAR_NAME VAR_idx // ida == EA // prove MATCH
			"%s_x%04X" "VARNAME_xFFFF" // AUTO_EDIT FMT_UPPER_HEX
			"%s" 
			"%04X" 
			"VARNAME"
			"_xFFFF" FFFF == EDAF

				// then match EDAF when used in SCRIPT
				// using SPARSE_TABLE_on_ZERO // SIGNED_IDX //
				// USAGE [-128 [ -127 [ 0 [ +127 [ +128 [ N [ u16
				// N == LIMIT_is_one_more_than_highest_in_use
				// N == Next // P2 == NEXT //
				// N == N // the SIGNED_PLUS_SIDE _ONLY
				// VAL == N // VAL == i16_in_CPU_register
				// if SIGNED < 0 // check negative arrays work
				// if SIGNED < 0 // CT calc EA_EXPR
				// if SIGNED < 0 // RT call XPOS //
				// PARSABLE XPOS is "MENTION _xFF"
			
	// if SIGNED < 0 // check negative arrays work

			// on_GOTO _on _GOTO
			// on_CALL _on _CODE

			int mpos = _xFFFF.int_val

				GETTER is used by GEN with TRACE_GEN_on_MINI_MACHINE
				MINI_MACHINE is a COMPILER

					set it up
					run it
					build tracks as TREE of EXPRS used
					allocate OBJECTS to TRAY[slot]
					stream SEGMENT as MINUS_SEGMENT

					PRE_REGISTER ITEM // PRE_CRIME //
					idx = N ++ // ALLOC //

				PUTTER
				PUTTER_of_BITFIELD

					on_OVERFLOW _assigned_BITFIELD_VALUE
					on_EXTRA_RANGE different packet with VAL

						i48_PAYLOAD = NEWVAL;

						i64_LOHI =from i16_in_WORD

					on_EXTRA_RANGE

					OPTION inline GETTERS API
					OPTION inline API

					# THE CODE IS THE IDENTIFIER #
					# using XPOS_PARSED not STR0_TOKENISED
					# using XPOS == OFFS # in_FILE #

					Routing the API calls thorugh functions
					provide BIND_VAR_TO_CALL
					provide GETTERS_PUT_API
					provide CACHE provides COMPILE PRE_

					PROVIDE "{ }"



		#endif
	} else

	if ( PSG_Name_BASE == "CALC" ) {
		// WARN("CODE ERR") // RETVAL " expeced the FILTER to work "
		// WARN("CODE ERR") // RETVAL STR0
		// TEXT("DECL CALC an INST of CALC_t") // 
		// LANG("DECL_IDEN") // IDEN_as_EA_LABEL //
		// IDEN("%s") // CIDENT99("%s") // DEFAULT_NULL is_CIDENT("%s")
		// TEST MATCH "cident99" // BENCH_VAR_on_SESSION 
		// VAR_NAME cident97 // PICK "cident98" FILTER "cident99"
		// PICK THIS_PARSING_CSR // CALC_STR_is_VAR_NAME_is_THIS //
		// AUTO get AUTO_GEN AVAIL // CODE_LINE == "ARGV"
		// CODE_POINT_DATA PTR == CODE_LINE // .str.get() // "%s" //
		// DATA_POINT OBJ_t OBJ 
		// INTO("CODE ERR") // RETVAL " expeced the FILTER to work "
		_PSG = new  lex_yacc_UDEF_CALC();

	} else
	if ( PSG_Name_BASE == "JSON" ) {
		INFO("making JSON");
		_PSG = new  lex_yacc_UDEF_JSON();
	}
	else {
		FAIL("did not recognise PSG_Name_BASE %s", (STR0) PSG_Name_BASE);
	}
	return _PSG;
}



