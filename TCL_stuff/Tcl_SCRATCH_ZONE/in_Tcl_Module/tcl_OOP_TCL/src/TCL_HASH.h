#if 0
#ifndef TCL_HASH_H
#define TCL_HASH_H

// INITIAL setup need convert from LIST to HASH
// http://npg.dl.ac.uk/MIDAS/manual/ActiveTcl8.5.7.0.290198-html/tcl/TclLib/Hash.htm

#include "TCL_REF.h"
#include "TCL_HELP.h"

struct TCL_HASH
{
	Tcl_HashTable HT;
	Tcl_HashEntry * HT_entry;


	TCL_REF hash;

		// cast to ret type; // == hash.PTR
		Tcl_Obj * hashPtr() { return hash; }

	TCL_HASH( Tcl_Interp * _interp )
	: hash()
	{
		Tcl_InitHashTable( &HT, TCL_STRING_KEYS );
	}

	~TCL_HASH()
	{
	}

//	bool test(  Tcl_Interp * interp );


};
#endif
#endif

