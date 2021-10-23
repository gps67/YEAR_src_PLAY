#include "TCL_DICT.h"
#include "TCL_HELP.h" // needs <tcl.h>
#include "TCL_LIST.h"

	bool TCL_DICT:: test(  Tcl_Interp * interp )
	{
		int pos;
		TCL_PTR k1;
		TCL_PTR k2;
		TCL_PTR k3;
		TCL_PTR v1;
		TCL_PTR v2;
		TCL_PTR v3;
		TCL_PTR val;

		k1 = Tcl_NewStringObj( "k1", -1 ); // 
		v1 = Tcl_NewStringObj( "one", -1 ); //
		SET( interp, k1, v1 );

		k2 = Tcl_NewStringObj( "k2", -1 ); // 
		v2 = Tcl_NewStringObj( "two", -1 );
		SET( interp, k2, v2 );

		k3 = Tcl_NewStringObj( "k2", -1 ); // 
		v3 = Tcl_NewStringObj( "three", -1 );
		SET( interp, k3, v3 );

		GET( interp, k1, val );
		INFO("DICT[%s] == %s\n", k1.str(), val.str() );

		DEL( interp, k1 );

		if(!GET( interp, k1, val )) return false;

		GET( interp, k1, val );
		INFO("DICT[%s] == %s\n", k1.str(), val.str() );

		return true;
	}

	bool TCL_DICT:: array_get( Tcl_Interp * interp, TCL_LIST & LIST )
	{
		Tcl_DictSearch search;
		Tcl_Obj * KEY;
		Tcl_Obj * VAL;
		int done;
		
		int t = TCL_OK;
		t = Tcl_DictObjFirst(interp,
			  dictPtr(),
			& search,
                        & KEY,
			& VAL,
			& done
		);
		if(t != TCL_OK ) {
			return false;
		}
		while(!done) {
			int forget_pos;
			LIST.ADD( interp, &forget_pos, KEY );
			LIST.ADD( interp, &forget_pos, VAL );
			Tcl_DictObjNext(&search, &KEY, &VAL, &done );
		}
		Tcl_DictObjDone(&search); // release lock
		return true;
	}

	bool TCL_DICT:: array_set( Tcl_Interp * interp, Tcl_Obj * list )
	{
		int pos = 0;
		int N = 0;
		if(TCL_OK!= Tcl_ListObjLength(interp, list, &N )) {
			return false;
		}
		if( (N%2) ) {
			Tcl_AppendResult( interp,
			"MUST BE EVEN NUMBER k1 v1 j2 v2",
			NULL
			);
			return false;
		}

		Tcl_Obj * KEY = NULL;
		Tcl_Obj * VAL = NULL;
		while( pos < N ) {
		 if(TCL_OK != Tcl_ListObjIndex(interp, list, pos, &KEY )) {
			return false;
		 }
		 pos ++;
		 if(TCL_OK != Tcl_ListObjIndex(interp, list, pos, &VAL )) {
			return false;
		 }
		 pos ++;
		 SET( interp, KEY, VAL );

		}

		return true;
	}

