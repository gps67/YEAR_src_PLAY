
#include "argv_builder.h"	
#include "malloc.h"	

argv_builder::argv_builder(PyObject * obj)
{
	// LURK -- obj must outlive the argv (and the contents not change)
	argc = 0;
	argv = 0;
	int i;
	if(!obj) return;

	argc = PyObject_Length( obj );
	if (argc < 1) {
		argc = 0;
		return;
	}
	argv = (char **) malloc(sizeof(argv[0])*argc);
	for(i = 0; i < argc; i++) {
		argv[i] = 0;
	}
	for(i=0; i<argc; i++) {
		PyObject * item = PySequence_GetItem( obj, i );
		if(!item) {
			argc = 0; // LURK exit leaking memory
			return;
		}
		// does this do a malloc ?
		// Objects/stringobject.c says NO!
		// it shares with the object itself
		argv[i] = PyString_AsString( item );
		if(!argv[i]) {
			argc = 0; // LURK exit leaking memory
			return;
		}
	}
	holder = obj;
	Py_INCREF( holder );
}
argv_builder::~argv_builder()
{
	for(int i = 0; i < argc; i++) {
		if(argv[i])
			; // NOT MALLOCED 
		argv[i] = 0;
	}
	if(argv) free(argv);
	Py_XDECREF( holder );
	holder = 0;
	argv = 0;
	argc = 0;
}

/*
	argv_builder is a utility to get round a hassle
*/

