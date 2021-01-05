#ifndef TZ_H
#define TZ_H

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

// #include <stdlib.h>
// #include <stdio.h>
// #include <time.h>
// #include <errno.h>

#include <string.h>
#include "misc.h"
#include "ints.h"

// #include "dbg.h"

//#define TZ_LOCAL_LURK "GMT0BST"
#define TZ_LOCAL_LURK "GB"
// Gnome thinks Locale is wrong and using GMT


/*!
	class tz, and static var TZ is like getenv(TZ).

	As far as I can see, tm_parts requires TZ to be set to
	get the non-local convertions (set to UTC)
*/
class tz : public GRP_lib_base
{
protected:
public:
	void save(); // undefined - get_TZ() ? restore()?
	static const char * TZ_SAVED; // process original value
	void print_tz_vars() ; // private or for debugging
	tz() ;
	bool set_TZ( const char * str );
	bool set_TZ_saved() { return set_TZ( TZ_SAVED ); }
//	Rework:- 
//	set_TZ_UST() ...
//	set_TZ_LOCAL() ...
//	leave TZ in a bad state, to only change on use
//	bool set_TZ_UST() { return set_TZ( "UST" ); }
};

extern tz TZ; /*
	global variable
*/

#endif
