#ifndef MY_WIN32_H
#define MY_WIN32_H

#ifdef WIN32
// #define WIN32_LEAN_AND_MEAN
// loses lots of socket definitions
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#ifdef WIN32
#define ON_WIN32( CODE ) CODE
#else
#define ON_WIN32( CODE )
#endif

extern void sleep_ms( int ms );
extern void sleep_secs( int s );

#include "dgb.h"


// maybe look in http://www.freepops.org/files/html/win32__compatibility_8h.html



#endif
