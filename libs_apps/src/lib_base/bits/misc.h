#ifndef gps_MISC_H
#define gps_MISC_H
// this file gives bool/TRUE/FALSE and also a few stdlib things and debug

#if 0

// when C (not C++) use <stdbool.h>
#define USE_STDBOOL

#ifdef USE_STDBOOL
# ifdef __cplusplus
	/* g++ has true/false builtin */
# else
# 	include <stdbool.h>
 	error here when invoked from this c++ project
	ie this file may need more debug work for C -AND- STDBOOL
# endif

#ifdef WIN32
	// already define BOOL // typedef WINBOOL BOOL;
#else
	typedef bool BOOL;
#endif

# ifndef TRUE
#	define TRUE true
#	define FALSE false
# endif

#else
	typedef int BOOL;
# ifndef TRUE
//	glib defines these so put its #include first
#	define TRUE 1
#	define FALSE 0
# endif
#endif

#else

#ifndef TRUE
	// still problematic with C:: gtk 

# 	include <stdbool.h>
#	define TRUE 1
#	define FALSE 0

#endif

#endif


#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

// when things are running OK its better to be silent
// gcc optimised this out (as do most) but the syntax + vars is checked
#ifndef OKfprintf
#define OKfprintf if(0) fprintf
#endif


#include "gdb_invoke.h"

// define assert(REQ) /**/
#include <assert.h>

#endif
