#ifndef SPIN_STRING_H
#define SPIN_STRING_H

#include "spin_py_mod.h"
#ifdef USING_PY_STRINGS
/*
	this module disappears 
*/
/*
	Is it of the special type?
	(though this is called when it SHOULD be!)
*/
// #define is_SPIN_string( obj ) 0
#define is_SPIN_string( obj ) PyString_Check( obj )

#else // using SPIN_STRING types

/*
	this can be made more efficient (8 bytes in u32)
	by storing the string as part of the object
	and the flags as the first byte of the len
	with added byte ordering portability
	(later eh)
	Another way is to encode the STRING within the string
	(and then add another sub-type)
*/

struct SPIN_string
{
	PyObject_HEAD
	int flags;
	int len;
	char * str;
};

#define SPIN_string_flags_8859 	0

SPIN_string * new_SPIN_string( PyObject * string ); 
SPIN_string * new_SPIN_string( int flags, int sz, char * str );

PyObject * type_SPIN_string( void ); 

int is_SPIN_string( PyObject * obj );
#endif

#endif
