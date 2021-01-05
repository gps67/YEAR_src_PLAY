#include "new_insitu.h"
#include <stdio.h>

/*
	WIN32 needed some link option to find this,
	but I cant remember what. ?libgcc?
*/

/*!
	placement new is for when no malloc() is required,
	but it would be nice if it could be inlined out.
*/
void * operator new(size_t sz , void * p) throw()
{
	/*
		used mostly by dyn_array
		converting its space to objects
	*/
	// fprintf(stderr, "placement new()\n");
	return p;
}

