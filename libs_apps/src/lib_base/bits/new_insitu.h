#ifndef new_insitu_H
#define new_insitu_H

#include "ints.h"
#include <stdio.h>

/*
	In order to call a constructor on pre-allocated mem
	you overload the new operator and call it with
	the C++ new placement constructor syntax

	new ((void *)addr) TYPE()
	new ((void *)addr) TYPE( A,R,G,S )

	g++/libc++ provides such a function, but you need the declaration

	These MACROS make it a bit easier to use
*/

extern void * operator new(size_t sz , void * p) throw();

#define RAW_INIT( addr, TYPE, ARGS ) new ((void *)addr) TYPE ARGS

#define DEL_ITEM( addr, TYPE ) (addr)->TYPE::~TYPE()

#endif
