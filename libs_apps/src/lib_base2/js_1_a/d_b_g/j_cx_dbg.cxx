#include "j_cx_dbg.h"
#define UINT32_MAX 0xFFFFffff
#include "jsdbgapi.h"
// #include "stdio.h"
// #include "jscntxt.h"

#ifndef DEBUG
#warning DEBUG should be defined for logfp to exist 
#endif

using namespace JSSM;

bool j_cx_dbg:: set_logfp_stderr()
{
	// fflush(0);
	// guess what, they have taken this away
	// just after I added it
	// OLD // cx -> logfp = stderr;
	bool ok = !!JS_SetDebugMode(cx, true );
	if(!ok) FAIL("JS_SetDebugMode()");

	return true;
}

