#include "Tree_PSG_LEX_TOKEN.h"

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"
using namespace PSG;


 LEX_TOKEN_DECL:: LEX_TOKEN_DECL() {}
 LEX_TOKEN_DECL:: ~LEX_TOKEN_DECL() {}

 LEX_TOKEN_GROUP:: LEX_TOKEN_GROUP() {}
 LEX_TOKEN_GROUP:: ~LEX_TOKEN_GROUP() {}

 /*
 bool
 LEX_TOKEN_GROUP::
 lookup_by_ID( LEX_TOKEN_DECL_idx   TOK_idx, STR0 Name ) {
 lookup_by_ID( LEX_TOKEN_DECL * retval.TOK, CT_LEX_TOKEN_from_Name u32_TOK_idx_expr, STR0 Name ) {
 lookup_by_ID( // retval_TOK // retval_DECL // retval_LOCN_item // retval_EA_in_ROM
  LEX_TOKEN_DECL * retval_TOK,	// retval = TOK + bool OK; // found TOK
  CT_LEX_TOKEN_from_Name u32_TOK_idx_expr, // Name = u32_TOK = CT_lookup("Name")l
  // STR0 Name // commented out, but useful as CHECK
  // LINT CHECK Name from HERE matches Name from HERE
  ) { ... }
 */
 /*
 	lookup _by_ID      # TOK = CT_LEXICON_lookup_by_id # PREBUILT ROM
 	lookup _by_Name    # eg "PSG_rule_1234" eg "PSG_rule_name[ anystr ]"	
 	lookup _by_Value   # SWEEP / ITERATE over data surface, searching for

		BENCH working on { SCRIPT }

		 CT_lookup_by_Name
		 RT_lookup_by_Name
		   _lookup_by_ID
		   _lookup_by_VALUE
		   
		   VALUE = Title Page and Text of { SCRIPT }
		   VALUE = _searching_thing_

		   	it is looking for
			   MATCH [s]
			it has multiple callable
			   EXPR [s]
			it connects AS_IF
			   api_AS_IF_api
			   api_AS_IS_api
			   api_IS_api
			it is carrying
			   DATA [s]
			   eg ARGV[idx] is DATA_EA_expr // and STO
			   eg TUPLE self_bounding_blob_of_morphable_data
			   eg TUPLE = ENTIRE_FILE _Name _Meta _DATA as_a_Tuplo
			   ie TUPLE = vary_TYPE_in_RANGE _PICK_ _ea
			it is carrying
			   SCRIPT [s]
			   [s]
			    COLLECTION of LABELLED ITEMS
			    ITEM & item = COLLECTION[item_key]
			    ITEM * item = ACCESS[item_key]
			    SCRIPT * EXPR_obj = VFS(path_name) // SCRIPT_NAME
			it is asking to RUN own script over Tree_Sweep
			    Tree_SWEEP = line_by_groups_of_lines_edge_to_edge
			    # edge_to_edge # connectors to outside
			    # edge_to_edge # connectors within FSM internally
			    # FSM with Tree rules and other rules # NOT FSM #
			    # left_to_right_usually


			MATCH looks like REXP_expr
			 DIALECT RE_expr // remember it isn't proper  RE

			EXPR looks like
			 EXPR = eval `* EA_DATA` # leaves " and ' avail
			 # VIEW opt # use backquotes # view as ... # 
			 # VIEW as # eval `ZONE`
			 # VIEW as # ` ZONE `
			 # VIEW as # ... other Layout Frames in use or avail

		   VALUE = Title Page and Text of { SCRIPT }
		   VALUE = _searching_thing_
		    PAIR = TRIPLE = CLASS_and_PAIR
		    
		     SPEC { // lookup("EA_SPEC_expr")
		     // lookup MATCH exact
		     // lookup MATCH components_exact // EXPR2=components_exact
		     // lookup MATCH some_combinarions // of EXPR2 in pool in scope
		     // lookup MATCH combinarions_at_least _shortfall
		     // SUBLEX can steer search budget, or gen code
		     // SUBLEX grows from DATA usage, builds LEXICON_POOL

		     // STO suggests CTXT.dialect. is offering {
		     //  SPEC * SPEC = _SPEC_; // EA_in_ROM
		     //  // local variable global Name
		     //  // _SPEC_ = META_SYNTAX _PLACEHOLDER_ // Label
		     //  // EA_in_ROM might be u32_hilo ea; and ea_DECODER
		     //  // AND EXTERNAL CONTEXT //
		     //  //  normal rules say avoid EXTERNAL CONTEXT
		     //  //  normal rules say GO FUNCTIONAL
		     //  //  but this is the FLIP_SIDE
		     //  //  HERE there are no barriers
		     //  //  HERE there are bridges
		     //  //  THIS is connected to THAT via EXPR_SOURCE
		     //  //  HERE there are SCOPE_BOUNDS // fast space
		     //  //  FLEXI-SPEC awards CODE_FAIL points to USER
		     //  //  FLEXI_SPEC -if-used- can approximate things
		     //  //  eg use new_item_name="%s_%4.4X", typename, N++
		     // ...
		     //}

// STO suggests CTXT.dialect. is offering {

	CTXT.loaded_segment_ROM3.CODE_ZONE_base.MODULE_TOK_POOL.STO
	// STO += { SCRIPTS } // LIBR
	// STO += VFS // LIBR += Tree_Of_Files
	// STO += elf_segment // MMAP non_elf segments into someplace
	// STO += CXX_structs // BUILTIN // PREBUILT //
	// STO += EXPR = EA_ITEM(COLLECTION_Name[ary_name] , ary[ ITEM_expr ] )
	// PAIR lhs rhs # POOL_sel ITEM_expr // ITEM_expr_subitems_tuple
	//  POOL ary = COLLECTION_Name[ary_name] // local selector needs DECODER
	//  ITEM_expr

	// where anystr might be
	// KNOWN STR "EA_SPEC_expr"
	// KNOWN TOK // at CT //
	// KNOWN VAL = CACHED STO.ROM3.SEGMENT_STO_ROM3 SUBEXPR
	// SUBEXPR = CODE_POINT + MINI_STATE + 

// STO suggests CTXT.dialect. is offering {

	CTXT.dialect.lookup( anystr )
	// where anystr might be
	// KNOWN STR "EA_SPEC_expr"
	// KNOWN TOK // at CT //
	// KNOWN VAL = CACHED STO.ROM3.SEGMENT_STO_ROM3 SUBEXPR
	// SUBEXPR = CODE_POINT + MINI_STATE + 

			 
			   
 */

 bool
 LEX_TOKEN_GROUP::
 lookup_by_Name( LEX_TOKEN_DECL * TOK, STR0 Name ) {
	
   int n = LIST_Token.N();
   for( int i =0; i<n; i++ ) {
	LEX_TOKEN_DECL * tok = LIST_Token[ i ];
	if( !tok ) return FAIL("NULL ITEM IN LIST");
	
	// STRING Compare -via- 
	if( tok-> Name == Name ) {
		TOK = tok;
		return true;
	}
	return FAIL("GROUP('%s')ITEM('%s')", (STR0) PFX, (STR0) Name );
   }
   return true;
 }

 bool
 LEX_TOKEN_GROUP::
 lookup_by_Value( LEX_TOKEN_DECL * TOK, STR0 Value ) {
	
	int n = LIST_Token.N();
	for( int i =0; i<n; i++ ) {
		LEX_TOKEN_DECL * tok = LIST_Token[ i ];
		if( !tok ) return FAIL("NULL ITEM IN LIST");
		
		// STRING Compare
		if( tok-> Value == Value ) {
			TOK = tok;
			return true;
		}
	}
	return FAIL("GROUP('%s')ITEM('%s')", (STR0) PFX, (STR0) Value );
 }

/*
	upgrade "++" to _PLUS_PLUS and add_PUNCT that
*/

 bool
 LEX_TOKEN_GROUP::
 add_PUNCT(
   STR0 punct,  // "==" ); // STR0 tail,  // "_EQUAL_EQUAL", // 
   STR0 op_info,
   STR0 opcode_desc // == NULL
 ) {
  // buff .put( PFX ); // PRF_PUNCT is added later

  buffer1 buff; // TAIL = _PLUS_PLUS
  buffer1 NAME_of_char; // PLUS
  
  STR0 Z1 = "OK";
  const char * p = punct;
  char sep = 0;
  while( char c = *(char *)p ) {
	p++; // but leave at NUL
	NAME_of_char.clear();
	if( !print_LEX_punct_name( NAME_of_char, c ) ) {
		// not all defined :-)
		return FAIL_FAILED();
	}
	// add it to tail
 //	buff .put_byte( '*' ); // PUNCT_
 //	buff .put_byte( '_' ); // PUNCT_
	if(sep) buff .put_byte( sep ); // PUNCT_
	sep = '_';
	buff .put( NAME_of_char ); // PUNCT_PLUS
  }
  STR0 _PLUS_PLUS = buff; 
  // INFO( "PLUS_PLUS == '%s'", _PLUS_PLUS );
  return add_PUNCT_4( _PLUS_PLUS, punct, op_info, opcode_desc );

 }



