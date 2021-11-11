#ifndef TCL_LIST_H
#define TCL_LIST_H

#include "TCL_STUBS.h"
#include "TCL_REF.h"
#include "TCL_HELP.h" // print_tcl_obj( listPtr() );

#define XX_GET_STRING(objPtr) \
    Tcl_GetString( objPtr )

/*!
	A flat array or vector of Tcl_Obj * items, accessed by int

	Most functions expect [idx]
	except GET_LIST_COPY which removes NULL items
*/
struct TCL_LIST
{
	// we want LIST to be unshared
	// we are the sole owner of LIST
	// we cant give the list to anyone - without copy
//	TCL_REF list;
	TCL_PTR list;

	// cast to ret type; // == list.PTR
	Tcl_Obj * listPtr() { return list; }

	TCL_LIST( Tcl_Interp * _interp )
	: list()
	{
		renew();
	}

	TCL_LIST( Tcl_Obj * obj )
	: list()
	{
		list = obj;
		INFO("setting from obj - debug should test is_a_list");
	}

	void get_from_PTR2( Tcl_Obj * obj )
        {
		list.get_from_PTR2( obj );
        }

	void set_into_PTR2( Tcl_Obj * obj )
        {
		list.set_into_PTR2( obj );
        }


	// eg PTR2 is dict, but for "safety" we have to unwrap TCP_PTR
	// because of this, drop strong type
	TCL_LIST( Tcl_Interp * _interp, Tcl_Obj ** KEPT_ITEM )
	: list()
	{
		list = *KEPT_ITEM;
		if(!KEPT_ITEM) {
			FAIL("NULL KEPT_ITEM");
		}
		if(!list) {
			FAIL("NULL dict");
		}
	}

	~TCL_LIST()
	{
	}

	bool renew() // no Tcl_nterp * required // Tcl is so specific
	{
		// nb a List is really a VECTOR or flat ARRAY of (Tcl_Obj *)
		list = Tcl_NewListObj( 0, NULL );
		return true;
	}

	/*
		OPTION keep NULL in LIST # keep IDX in VECT # api_view
		if(setting(" keep NULL in LIST for VECT # api ")) { SCRIPT }

		API here requiires caller to provide prepared storage for VAL VARS

		bool GET_LIST_COPY( Tcl_Interp * interp, int index, TCL_REF & RET_VAR )
		// GET_LIST_COPY_var my_copy_of_var [WHEN] VALUE_t VALUE; // CMNT
		// GET_LIST_COPY_var_api {
			my_copy_of_var
		// PSG	[WHEN] // u64_unix_time_ u64 %s ITEM_t ITEM // PARAMETER API
		// PSG  WHEN_t WHEN; 
		// psg  bool GET_LIST_COPY( Tcl_Interp * interp, int index, TCL_REF & RET_VAR )
		// psg  bool GET_LIST_COPY( interp, index, TCL_REF & RET_VAR )
		// explain index, TCL_REF & RET_VAR // in_SESS
		// in_SESS = in_SESS("SESS") // lookup logic 
		// explain ALIAS in_SESS as SESSION.SESS and SESSION.API
		// explain IDX as SELF in SESS // need at RUNTIME VAR = "%s"

			STR0_t NAME; // CMNT CODE_POINT // HERE // 
			// LOCN // VAL // %X  // %FMT STR0 str_utf8 // DIALECT
			// DIALACT // KNIT STEP LIBR ROM // RUNTIME // SESS

			bool is_NULL == mode_KEEP_IN_LIST_because_VECT
			// that is a bool VAR in API
			// keeping the FULL_VECT idx==idx KNOW ITEM_ID_idx
			// VAR FULL_VECT // VECT[ ( i < N ) ]
			// DECL in_the_line_above " ( i < N ) " // KNOWN TOKEN
			// NAME = " ( i < N ) " // WELL_KNOWN %s #spaces_added

			VALUE_t VALUE; // CMNT

			for( i < N ) // PARSE { VECT_ITEM[idx] } as FOREACH
			ITEM_t ITEM = VECT_ITEM ( idx == i )
			ITEM_t ITEM_t ITEM = array[N] of VECT_ITEM ( idx == i )
			DIALECT NAEM == VECT_NAME
			VOCAB "VECT" == VECT_Name // DIALECT RHYME 
			
			GEN CXX SPELLING VOCAB SCRIPT DECL VAR { DECL_SCRIPT }
			LIBR u64 u64_WORD * PTR = VAR_NAME _OR_EXPR
			LIBR u64 PICK_ITEM = VECT_NAME[IDX]
			LIBR FILTER EA EXPR SAFE_SCRIPT
			STO = SESS.STO // pass the buck, auto create alias pun
			# ie by invent varname("STO") auto_create SESS.STO
			# explain # ALL_ABOUT "SESS.STO" # PSG WELL_KNOWN_EXPR
			EXPR " WELL_KNOWN_EXPR " # PSG DATA_API_VECT # DIALECT
			# DIALECT.NAME == "SESS.STO" # with STO and SPEC
			# ON_DEMAND create API_tokens # eg my_ref_your_ref
			# DECL VAR # VAR_type == "Tcl_Obj * VECT_PTR;
			# DECL VAR # VAR_type == "Tcl_Obj * ITEM_NAME; // IDX
			# IDX = VAR_in_LIST_NAME_IDX_t varname; // idx_varname
			# VAR_NAME = "SESS.STO" DIALECT = "AUTO_DATA_CODE_SAMPLE_VALUE"
			# ALIAS "VALUE"

			GEN CXX "ANYSTR == VALUE_t VALUE = PARSE_VALUE
			GEN CXX u64 VECT_NAME = array[N] of Item_t; // api
			CXX u64 VECT_IDX = array[N] of Item_t; // api script
			CXX u64 VECT_NAME = array[N] of Item_t; // api

		u64 * WORD in WORD_VECT GETTER_GET
		u64 WORD_VECT[u64] + OFFS_WITHIN_STRUCT
		// OPTION SINGLE VALUE DATA
		// OPTION ARRAY of VALUE DATA
		// SESSION setting precompiled API DIALECT "{ PhraseBook }"

		u64 N = N
		u64 * 
		u8 * EA_as_u8_star = N 
		u64 * == u64 PTR_as_u64
		u64_idx // ALLOC idx = N++ // ALLOC WORD[idx] VIA LANG AVAIL API

		ITERATE " ( i< N ) " {
		}

		ITERATE " ( i< N ) " {
			ITEM_t * ITEM = " VECT[ i ] " ;// ARRAY_of_WORDS
			
			LIBR { WORD = ALIAS = " VECT[ i ] " }
			LIBR { WORD = ALIAS = " i " }
			LIBR { WORD = ALIAS = " VARNAME[i] " }

				VARNAME = "%s" 
				VARNAME = "STR0_from_VAR_NAME_VECT"" 
				VARNAME = "ITEM_NAME"" 
			STO GEN CXX STO u32_u32 ARRAY_of_PAIR u8 N
			STO GEN CXX STO

				u32_u32 ARRAY_of_PAIR u8;
				u64 VECT[ u64 N = X ]
				u64 * WORD_PAIR(" u32_u32 PAIR ");

				// LIBR SAYS HERE ARE SOME EXPRS INDEXED
				// SESS SAYS DATA EXPR VARS EVAL CODE RUN
				// CTXT SAYS RUNTIME SESSION elf_segment_var
				// CODE SAYS " TOKEN == ANYSTR "
				// LIBR SAYS " IDX "


				u64 N = u64_COPY_of_VAR_somewhere;
				u64 VARNAME_t VARNAME;
				// of VARIETY # VARNAME = "u16_idx_MAX_N
				// PRE_EXISTING_CODE // %s = 0x%s" %4X

OUR OWN DIALECT "%4X" means " { 0x %4X } " 
  OUR OWN DIALECT 
   PHRASEBOOK
     %4X
    "%4X" means " { 0x %4X } " 
   VIEW
   	"xFFFF" // HINT u16 for this ITEM in POOL in MINI_WORLD
   	"x%4X"

		ITERATE " ( i< N ) " {
		}

		list2 = COPY list1 

			GROW_LIST = WEARY_PRESET
			N = STO.ROM.u64_N // KEEP u16_N as VARNAME
			N = DATA
			N = u64 u16_N; // pun typename into varname %s_%s
			// u16 idx // ITERATOR _ONE // ITERATOR_TWO //
			// STO VECT[i] // u64 i = u16_idx; 
			// STO VECT[i]

		IDENTITY
			// u64 i = u16_idx; //
			// BIT_CONTROL SIGNED_NESS += u64_carrying_u48_payload
			// BIT_CONTROL u64_WORD u64_carrying_u48_payload
			// BIT_CONTROL u64_WORD u64_carrying_u48_u16_lohi
			// opinionated code CHOICE += "u16_lohi"

		DATA_LAYER

			LAYERS += DATA_LAYER MEMORY_with_Layout
			// DIALECT _with_ %s lhs _with_ %s rhs // MEMORY LAYOUT

//		( on elf_segment_copied_into_runtime %s %s )
//		( SCRIPT gets RO API SESS gets RW API )

		EXPR_MATH

			OFFS = IDX * 8 // 8 is for u64_WORD VECT[N]
			BASE = BASE_address // invent on mention
			ADJ  = adj // OPTION pointer to ITEM=PTR or
			ADJ  = adj // " OFFSET of FIELD in STRUCT "
			ADJ  = adj // " OFFSET of %s in STRUCT " FIELD_NAME in MEM
			// OPTION MEM // OPTION NOT ITEM // GEN a_point_b_VAR_NAME
			// OPTION NAME = idx // obj_%4X // %s_%s PFX IDX //
			// OPTION ADJ < u8 // this obj 80 bytes // OFFS < u8
			// OPTION ADJ = COMPUTE u16_in_u64_segment
			ADJ  = adj // OPTION pointer to ITEM=WORD=PTR or
			ADJ  = adj // OPTION pointer to ITEM[N] // like MEM

			EXPR  ITEM = VECT[ idx ] 

			DECODE idx_t == "u64" // ACTUALLY it is "u64_u32"
			// we only deliver [u32] // PLENTY in EA EXPR

		SAFE MODE PERMITS

			ITEM_t = idx that can be used in ITERATION
			ITERATION = " for [0 [i [N ; idx == i ; call SCRIPT
			SCRIPT_AVAIL_DATA

			DATA is FAST_EVAL SCRIPT
			DATA is SCRIPT_DATA_EVAL_CACHE_
			DATA was elf_segment .bss 
			DATA is ROM.DATA // precompiled SCRIPT MATCH CALL
			GEN PARSE "u64_WORD" SUBLEX TOKEN "u64_N" // u64_idx
			GEN PARSE "u64_WORD" SUBLEX TOKEN "u64_idx" // u64_N

		SAFE MODE EA

			EA_EXPR actually followed the SPEC of some ITEM
			Some smaller BITFIELD_ITEM OPTION SHIFTED u48

			u48_payload = CXX u64 u48_payload; // CMNT
			u64_EXPR = CXX u64_EXPR

			u64_WORD = WORD_VECT[u64] // N == u48_N // 
			u64 u48_N = u48_payload // shifted // truncated_N_bits
			u64_idx "{ [0 [idx[ [N }" // PARSE [0[N DIAG
			u64_idx "{ [0 [idx[ [N }" // ALIAS "ITERATOR 0 idx N"

			REQUIRE idx < N

			simple FILTER point CHECK " CHECK idx < N "
			simple FILTER point CHECK " idx < N "
			simple FILTER point CHECK "{ idx < N }"
			simple FILTER PARSE "{ L I S T }" eg  "{ idx < N }"

			DATA DATA from { ARGV[argc] } CODE_FROM "DITTO"
			SPEC DATA from { ARGV[argc] } CODE_FROM "-ditto-"
			SPEC DATA from { ARGV[argc] } CODE_FROM "-see-also-"
			

		ITERATE " ( i< N ) " {
		}

		}

	SO	the best STYLE is array_get with NULL values in VECT[N] as [idx]

		this does require a local RET_VAR to put redult in
		bool GET_LIST_COPY( interp,  index, TCL_REF & RET_VAR )
		
			TCL_REF & RET_VAR 

			TCL_REF   MY_COPY = GET_LIST_COPY( 

			FOREACH item in 


	*/
	bool GET_LIST_COPY( Tcl_Interp * interp, int index, TCL_REF & RET_VAR )
	{
		// this squeezes out the NULL items, so loses obj_id [pos]
		bool OK = true;

		// SAFE_MODE requires me to provide my TCL_VECT = TCL_LIST
		// sole owner of list2 converts into list3 and list of EXPRS
		TCL_LIST list2(interp);

		// CSR = ITERATOR.CSR // item = CSR.item // CURR_ITEM
		TCL_REF item; // ALIAS SESS_CURR_item

		// pos_in_list2 // idx == %s // pos == %s // %s == %s (...)
		int pos; // AKA pos_in_list2 // { my_ref your_ref }

		// get N from DICT
		int N = 0;
		if(!NN( interp, &N )) return false; // RET_VAR probably NULL

		// ITERATE " 0 idx N " BIND " idx " " i " // %s == %s GEN
		for(int i = 0; i<N; i++ ) {
			GET( interp, i, item );

			// OPTION REWRITE to emit NULL PLACEHOLDER _TOKEN
			// item = filter NULL_VALUE_FOR_NULL item
			// item = UPGRADE item
			// item = "{}" // 
			// item = "" // EMPTY STR IS PREFERRED STRVAL of NULL
			// item = "SKIP_VAL" // possibly NULL u64_ZERO
			// rewrite NULL as u64_SKIP_ITEM //
			// didnt know but might have been
			// u64_NULL u64_SKIP u64_CALL // on_event EVAL u64

			// OPTION just a list of valid records LIST_ALL_KEYS
			// OPTION MINI_MACHINE does KEY use over here,
			if(item) {
				// ITERATOR INTO list2 ALLOC = APPEND ADD [idx]
				//  pos = idx = N++; // ALLOC idx //
				//  list2[ pos ] == item
				list2.ADD( interp, &pos, item );
			}
		}
		RET_VAR = list2.list;
		return OK;
	}

	bool array_get( Tcl_Interp * interp, TCL_REF & RET_VAR ) // auto &
	{
		// this squeezes out the NULL items, so loses obj_id [pos]
		bool OK = true;
		TCL_LIST list2(interp);
		TCL_REF item;
		int pos;
		int N = 0;
		if(!NN( interp, &N )) return false; // RET_VAR probably NULL
		INFO("NN == %d", N);
		for(int i = 0; i<N; i++ ) {
			GET( interp, i, item );
			if(item) {
				list2.ADD( interp, &pos, Tcl_NewIntObj(i) );
				list2.ADD( interp, &pos, item );
			}
		}
		RET_VAR = list2.list;
		return OK;
	}

	bool array_set( Tcl_Interp * interp, Tcl_Obj *  pairs_list )
	{
		// this squeezes out the NULL items, so loses obj_id [pos]
		bool OK = true;

		int N1 = 0;
		if(TCL_OK!=Tcl_ListObjLength(interp, pairs_list, &N1 )) {
		       return false;
		}
		if(N1 % 2) {
			printf("# FAILS not even N1\n");
		       return false;
		}

		int pos1 = 0;
		while( pos1 < N1 ) {

			Tcl_Obj * KEY_INT = 0;
			Tcl_Obj * VAL = 0;


		       if( TCL_OK !=
		       Tcl_ListObjIndex(interp, pairs_list, pos1, &KEY_INT )) {
			       return false;
		       }

		       pos1 ++;

		       if( TCL_OK !=
		       Tcl_ListObjIndex(interp, pairs_list, pos1, &VAL )) {
			       return false;
		       }

		       pos1 ++;

			int pos2;
			if(TCL_OK!=Tcl_GetIntFromObj(interp, KEY_INT, &pos2 )) {
				return false;
			}

			if(!SET( interp, pos2, VAL )) return false;

		}

		return OK;
	}

	bool GET( Tcl_Interp * interp, int index, TCL_REF & RET_VAR )
	{
	 if(0)	if(!RET_VAR) { // VAR is not null but VAL or VAR is NULL
			FAIL( "NULL RET_VAR" ); // 
			throw "NULL RET_VAR"; // 
			return false;
		}

		// TCL_REF is not understood by Tcl_List* so use plain RET_VAL
		Tcl_Obj * RET_VAL = NULL;
		if(TCL_OK !=
		 Tcl_ListObjIndex(
		    interp,
		    listPtr(),
		    index,
		  & RET_VAL
		 )) {
		 	// ERROR only on not-a-list
			// index out of range returns TCL_OK and NULL
			FAIL("Tcl_ListObjIndex() POS = %d\n", index );
		 	// ERROR message already set
			// HMMM leaves OLD_VAL
			return false;
		}
		if( !RET_VAL ) {
			// NULL from Tcl_ListObjIndex means out of bounds
			int N = 0;
			if(TCL_OK !=Tcl_ListObjLength( interp, listPtr(), &N )) {
				FAIL("GET error Tcl_ListObjectLength\n" );
			}
			// is a NULL value ever possible ?
			char msg[50];
			snprintf(msg,sizeof(msg)-1,"GET[%d] out of range [0[%d", index, N );
		       Tcl_AppendResult( interp,
                        (char *) msg,
                        (char *) NULL
			);
			return TCL_ERROR;

		}
		//
		// now do the Incr(newval) and Decr(oldval)
		RET_VAR = RET_VAL;

		PASS("[%d] == '%s'\n", index, Tcl_GetString( RET_VAL) );
		return true;
	}

	// we dont need a smart PTR as the ARG, and auto cast works
	bool SET_( Tcl_Interp * interp, int index, TCL_REF & VAL )
	{
		return SET( interp, index, (Tcl_Obj *) VAL );
	}

	bool SET( Tcl_Interp * interp, int index, Tcl_Obj * VAL )
	{
		const char * str = Tcl_GetString( VAL );
		INFO("SET [%d] = %s \n", index, str );
		int objc = 1;
		Tcl_Obj * objv[1] = { VAL };
		int first = index;
		int count = 1;
		if(TCL_OK !=
		 // Replace a segment with a segment // of 1
		 Tcl_ListObjReplace(
		    interp,
		    listPtr(),	// Tcl_Obj * the_list
		    first,	// first to delete
		    count,	// count to delete
		    objc,	// count to add
		    objv	// data to add
		 )) {
			// Tcl checks index
			// ERROR message already in interp //
			return false;
		}
		return true;
	}

	bool NN( Tcl_Interp * interp, int * intPtr )
	{
		if(TCL_OK!=Tcl_ListObjLength( interp, listPtr(), intPtr )) {
			FAIL("ADD fail Tcl_ListObjLength\n");
			return false;
		}
		return true;
	}

	bool ADD( Tcl_Interp * interp, int * intPtr, Tcl_Obj * VAL )
	{
		// [0 .. [N
		// ADD places VAL at POS == N_old N++

		if(!NN( interp, intPtr )) {
			return false;
		}
		if(TCL_OK !=
		 Tcl_ListObjAppendElement(
		    interp,
		    listPtr(),
		    VAL
		 )) {
			// ERROR message already in interp //
			FAIL("ADD fail Tcl_ListObjAppendElement\n");
			// set some bad POS for later
			* intPtr = -1;
			return false;

	/*
			Tcl_AppendResult( interp,
                        (char *) msg,
                        (char *) NULL
			);
			return TCL_ERROR;
	*/
		}
		
		PASS("ADD pos == %d '%s' \n", *intPtr, Tcl_GetString(VAL));
		return true;
	}

	bool test(  Tcl_Interp * interp );


};
#endif

