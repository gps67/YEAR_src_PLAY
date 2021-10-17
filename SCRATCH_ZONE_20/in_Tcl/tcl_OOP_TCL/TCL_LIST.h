#ifndef TCL_LIST_H
#define TCL_LIST_H

#include "TCL_PTR.h"

#define XX_GET_STRING(objPtr) \
    Tcl_GetString( objPtr )

struct TCL_LIST
{
	TCL_PTR list;

		// cast to ret type; // == list.PTR
		Tcl_Obj * listPtr() { return list; }

	TCL_LIST( Tcl_Interp * _interp )
	: list()
	{
		list = Tcl_NewListObj( 0, NULL );
	}

	~TCL_LIST()
	{
	}

	bool GET( Tcl_Interp * interp, int index, TCL_PTR & RET_VAR )
	{
		if(!RET_VAR) {
			throw "NULL RET_VAR"; // 
			return false;
		}
		// TCL_PTR is not understood by Tcl_List* so use plain RET_VAL
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
			fprintf(stderr,"GET fail POS = %d\n", index );
		 	// ERROR message already set
			// HMMM leaves OLD_VAL
			return false;
		}
		if( !RET_VAL ) {
			// NULL from Tcl_ListObjIndex means out of bounds
			int N = 0;
			if(TCL_OK !=Tcl_ListObjLength( interp, listPtr(), &N )) {
				fprintf(stderr,"GET error Tcl_ListObjectLength\n" );
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

		fprintf(stderr,"GET [%d] == '%s'\n", index, Tcl_GetString( RET_VAL) );
		return true;
	}

	// we dont need a smart PTR as the ARG, and auto cast works
	bool SET_( Tcl_Interp * interp, int index, TCL_PTR & VAL )
	{
		return SET( interp, index, (Tcl_Obj *) VAL );
	}

	bool SET( Tcl_Interp * interp, int index, Tcl_Obj * VAL )
	{
		const char * str = Tcl_GetString( VAL );
		fprintf(stderr,"SET [%d] = %s \n", index, str );
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

	bool N( Tcl_Interp * interp, int * intPtr )
	{
		if(TCL_OK!=Tcl_ListObjLength( interp, listPtr(), intPtr )) {
			fprintf(stderr,"ADD fail Tcl_ListObjLength\n");
			return false;
		}
		return true;
	}

	bool ADD( Tcl_Interp * interp, int * intPtr, Tcl_Obj * VAL )
	{
		// [0 .. [N
		// ADD places VAL at POS == N_old N++

		if(!N( interp, intPtr )) {
			return false;
		}
		if(TCL_OK !=
		 Tcl_ListObjAppendElement(
		    interp,
		    listPtr(),
		    VAL
		 )) {
			// ERROR message already in interp //
			fprintf(stderr,"ADD fail Tcl_ListObjAppendElement\n");
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
		
		fprintf(stderr,"ADD pos == %d '%s' \n", *intPtr, Tcl_GetString(VAL));
		return true;
	}

	bool test(  Tcl_Interp * interp );


};
#endif

