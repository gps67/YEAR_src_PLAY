#ifndef TCL_LIST_H
#define TCL_LIST_H

#include "TCL_STUBS.h"
#include "TCL_REF.h"
#include "TCL_HELP.h" // print_tcl_obj( listPtr() );
namespace TCL {

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
	// PLUS // we will LOCK_EDIT_STEP_STEP_UNLOCK // _PAUSE _SHARE_LOCK //
	// SHARE_LOCK means they will give it on demand or on GUARANTEE MOST ALL
	// SHARE LOCK CLOCK // TICK_TOCK every STEP // SPLICE_POINT_in_STREAM
	// STREAM_SPLICE // WORD_PAIR is STREAM_GRAIN OPTION // _ONE _TWO //
	// WORD_PAIR // EA_EXPR of ITEM in HEAP // DECODE_ONE_TWO //
	//
	// list
	// list - Tcl_Obj * list - that is itself a LIST[N] of ITEM
	// list


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
	//
	// CTOR EA_EA_SELF // NO_ARGS // release CACHES previous EA_KEPT
	//
	// for speed CACHE leaves all vars as they were
	// soon enough new value prompts release old value
	// 
	// NOT SURE how this is invoked ?
	// SEE (BENCH gets helpful here)
	//
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
		Tcl_Size pos; // AKA pos_in_list2 // { my_ref your_ref }

		// get N from DICT
		Tcl_Size N = 0;
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
		Tcl_Size pos;
		Tcl_Size N = 0;
		if(!NN( interp, &N )) return false; // RET_VAR probably NULL
		INFO("NN == %ld", N);
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

		Tcl_Size N1 = 0;
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
			Tcl_Size N = 0;
			if(TCL_OK !=Tcl_ListObjLength( interp, listPtr(), &N )) {
				FAIL("GET error Tcl_ListObjectLength\n" );
			}
			// is a NULL value ever possible ?
			char msg[50];
			snprintf(msg,sizeof(msg)-1,"GET[%d] out of range [0[%ld", index, N );
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

	bool NN( Tcl_Interp * interp, Tcl_Size * EA_NN )
	{
		if(TCL_OK!=Tcl_ListObjLength( interp, listPtr(), EA_NN )) {
			FAIL("ADD fail Tcl_ListObjLength\n");
			return false;
		}
		return true;
	}

	bool ADD( Tcl_Interp * interp, Tcl_Size * EA_NN, Tcl_Obj * VAL )
	{
		// [0 .. [N
		// ADD places VAL at POS == N_old N++

		if(!NN( interp, EA_NN )) {
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
			* EA_NN = -1;
			return false;

	/*
			Tcl_AppendResult( interp,
                        (char *) msg,
                        (char *) NULL
			);
			return TCL_ERROR;
	*/
		}
		
		PASS("ADD pos == %ld '%s' \n", *EA_NN, Tcl_GetString(VAL));
		return true;
	}

/* TODO
	https://www.tcl-lang.org/man/tcl9.0/TclLib/ListObj.html

	int Tcl_ListObjAppendList(interp, listPtr, elemListPtr)
	int Tcl_ListObjAppendElement(interp, listPtr, objPtr)
	Tcl_Obj * Tcl_NewListObj(objc, objv)
	          Tcl_SetListObj(objPtr, objc, objv)
	int Tcl_ListObjGetElements(interp, listPtr, objcPtr, objvPtr)
	int Tcl_ListObjLength(interp, listPtr, lengthPtr)
	int Tcl_ListObjIndex(interp, listPtr, index, objPtrPtr)
	int Tcl_ListObjReplace(interp, listPtr, first, count, objc, objv)

*/

	bool test(  Tcl_Interp * interp );
/* TODO

	MIMIC LIST to be done by this class ::TCL::LIST
	It provides the API for VIEW // API_ANGLE // ASIF_api // _GET_AS_LIST
*/

	int LIST_Append_List(Tcl_Interp * interp, Tcl_Obj * tail_list)
	{
	 return Tcl_ListObjAppendList(interp, listPtr(), tail_list);
	}

	int LIST_Append_Element(Tcl_Interp * interp, Tcl_Obj * item)
	{
	 return Tcl_ListObjAppendElement(interp, listPtr(), item);
	}

	Tcl_Obj * LIST_NewListObj(Tcl_Size objc, Tcl_Obj * objv[]) 
	{
	 return Tcl_NewListObj(objc, objv) ;
	}

	Tcl_Obj * Tcl_SetListObj(Tcl_Obj * objPtr, Tcl_Obj * objc, Tcl_Obj * objv)
	{
	 return Tcl_SetListObj(objPtr, objc, objv);
	}

	int LIST_GetElements(Tcl_Interp * interp, Tcl_Size * EA_objc, Tcl_Obj ** EA_objv[] )
	{
	 return Tcl_ListObjGetElements(interp, listPtr(), EA_objc, EA_objv );
	}

	int LIST_Length(Tcl_Interp * interp, Tcl_Size * lengthPtr)
	{
	 return Tcl_ListObjLength(interp, listPtr(), lengthPtr);
	}

	int LIST_Index(Tcl_Interp * interp, Tcl_Size index, Tcl_Obj ** objPtrPtr)
	{
	 return Tcl_ListObjIndex(interp, listPtr(), index, objPtrPtr);
	}

	int LIST_Replace(
		Tcl_Interp * interp,
		Tcl_Size first,  // first to delete
		Tcl_Size count,	 // count to delete
		Tcl_Size objc,	 // count to add
		Tcl_Obj * objv[] // data to add
	) {
	 return Tcl_ListObjReplace(interp, listPtr(), first, count, objc, objv);
	}


};
}; // namespace
#endif

