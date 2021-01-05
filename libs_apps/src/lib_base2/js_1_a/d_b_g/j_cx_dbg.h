#ifndef j_cx_dbg_H
#define j_cx_dbg_H

#include "j_cx.h"


/*!
	the JSSM DBG API is a bit of an unknown
	it uses JSContext cx_obj; when others use opaque pointer *cx
	so it has been moved to a separate file
	whilst I figure out if I want any of it

	Even JSAPI says there are two interfaces, pick one,
	then doesnt really ouline the boundry line.

	The best clues come from:

		shall/js.cpp testing(true) cx->logfp = stderr
*/
namespace JSSM {
 struct j_cx_dbg : public j_cx
 {

	j_cx_dbg( JSContext * _cx )
	: j_cx( _cx )
	{
	}

	j_cx_dbg( j_cx * jcx )
	: j_cx( (jcx ? jcx->cx : NULL) ) // jcx cant be NULL
	{
	}

	bool set_logfp_stderr();
	
 };
};



#endif

