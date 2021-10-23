#ifndef TCL_DICT_H
#define TCL_DICT_H

// INITIAL setup need convert from LIST to DICT

#include "TCL_STUBS.h"
#include "TCL_PTR.h"
#include "TCL_HELP.h"

struct TCL_DICT
{
	TCL_PTR dict;

		// cast to ret type; // == dict.PTR
		Tcl_Obj * dictPtr() { return dict; }

	TCL_DICT( Tcl_Interp * _interp )
	: dict()
	{
		dict = Tcl_NewDictObj();
	}

	~TCL_DICT()
	{
	}

	bool GET( Tcl_Interp * interp, Tcl_Obj * keyPtr, TCL_PTR & RET_VAR )
	{
		Tcl_Obj * RET_VAL = NULL;
		if(TCL_OK !=
		 Tcl_DictObjGet(
		    interp,
		    dictPtr(),
		    keyPtr,
 (Tcl_Obj **)	    RET_VAR
		 )) {
		 	// ERROR only on not-a-dict
			// keyPtr out of range returns TCL_OK and NULL
			INFO("GET fail POS = %s\n", Tcl_GetString(keyPtr) );
		 	// ERROR message already set
			// HMMM leaves OLD_VAL
			return false;
		}
		if(!RET_VAR) {
			INFO("SILENT GET absent key = '%s'\n", Tcl_GetString(keyPtr) );
			return false;
		}
		return true;
	}

	// we dont need a smart PTR as the ARG, and auto cast works
	bool SET_( Tcl_Interp * interp, Tcl_Obj * keyPtr, TCL_PTR & VAL )
	{
		return SET( interp, keyPtr, (Tcl_Obj *) VAL );
	}

	bool SET( Tcl_Interp * interp, Tcl_Obj * keyPtr, Tcl_Obj * VAL )
	{
		const char * str_key = Tcl_GetString( keyPtr );
		const char * str_val = Tcl_GetString( VAL );
		INFO("SET [%s] = %s \n", str_key, str_val );
		if(TCL_OK !=
		 Tcl_DictObjPut(
		    interp,
		    dictPtr(),	// Tcl_Obj * the_list
		    keyPtr,
		    VAL
		 )) {
			return false;
		}
		return true;
	}

	bool DEL( Tcl_Interp * interp, Tcl_Obj * keyPtr )
	{
	 return TCL_OK ==
		Tcl_DictObjRemove( interp, dictPtr(), keyPtr );
	}

	bool NN( Tcl_Interp * interp, int * intPtr )
	{
	 return TCL_OK ==
		Tcl_DictObjSize( interp, dictPtr(), intPtr );
	}

	bool array_get( Tcl_Interp * interp, TCL_LIST & LIST );
	bool array_set( Tcl_Interp * interp, Tcl_Obj * list );

	bool test(  Tcl_Interp * interp );


};
#endif

