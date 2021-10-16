#include "TCL_HASH.h"
#include "TCL_HELP.h" // needs <tcl.h>

/*

	Tcl_CreateHashEntry.3
	Tcl_DeleteHashEntry.3
	Tcl_DeleteHashTable.3
	Tcl_FindHashEntry.3
	Tcl_FirstHashEntry.3
	Tcl_GetHashKey.3
	Tcl_GetHashValue.3
	Tcl_HashStats.3
	Tcl_InitCustomHashTable.3
	Tcl_InitHashTable.3
	Tcl_InitObjHashTable.3
	Tcl_NextHashEntry.3
	Tcl_SetHashValue.3

	Tcl_DictObjDone.3
	Tcl_DictObjFirst.3
	Tcl_DictObjGet.3
	Tcl_DictObjNext.3
	Tcl_DictObjPut.3
	Tcl_DictObjPutKeyList.3
	Tcl_DictObjRemove.3
	Tcl_DictObjRemoveKeyList.3
	Tcl_DictObjSize.3
	Tcl_GetEnsembleMappingDict.3
	Tcl_NewDictObj.3
	Tcl_SetEnsembleMappingDict.3

*/


	bool TCL_HASH:: test(  Tcl_Interp * interp )
	{
		int pos;
		TCL_PTR_t k1;
		TCL_PTR_t k2;
		TCL_PTR_t k3;
		TCL_PTR_t v1;
		TCL_PTR_t v2;
		TCL_PTR_t v3;
		TCL_PTR_t val;

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
		fprintf(stderr,"DICT[%s] == %s\n", k1.str(), val.str() );

		DEL( interp, k1 );

		if(!GET( interp, k1, val )) return false;

		GET( interp, k1, val );
		fprintf(stderr,"DICT[%s] == %s\n", k1.str(), val.str() );

		return true;
	}


