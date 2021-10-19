#include "TCL_PTR.h"
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
/*
	Hash is more complex

	it allows for multiple types of key
	it does not reuse Tcl_Obj * str;
	it uses const char * str;
*/




