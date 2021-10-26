#ifndef TCL_LIST_H
#define TCL_LIST_H

#include "TCL_STUBS.h"
#include "TCL_REF.h"

#define XX_GET_STRING(objPtr) \
    Tcl_GetString( objPtr )

/*!
	A flat array or vector of Tcl_Obj * items, accessed by int

	Most functions expect [idx]
	except GET_LIST_COPY which removes NULL items
*/
struct TCL_LIST
{
	TCL_REF list;

		// cast to ret type; // == list.PTR
		Tcl_Obj * listPtr() { return list; }

	TCL_LIST( Tcl_Interp * _interp )
	: list()
	{
		renew();
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
		TCL_LIST list2(interp);
		TCL_REF item;
		int pos;
		int N = 0;
		if(!NN( interp, &N )) return false; // RET_VAR probably NULL
		for(int i = 0; i<N; i++ ) {
			GET( interp, i, item );
			if(item) {
				list2.ADD( interp, &pos, item );
			}
		}
		RET_VAR = list2.list;
		return OK;
	}

	bool array_get( Tcl_Interp * interp, int index, TCL_REF & RET_VAR )
	{
		// this squeezes out the NULL items, so loses obj_id [pos]
		bool OK = true;
		TCL_LIST list2(interp);
		TCL_REF item;
		int pos;
		int N = 0;
		if(!NN( interp, &N )) return false; // RET_VAR probably NULL
		for(int i = 0; i<N; i++ ) {
			GET( interp, i, item );
			if(item) {
				list2.ADD( interp, &pos, Tcl_NewIntObj(pos) );
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
		if(!RET_VAR) {
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

