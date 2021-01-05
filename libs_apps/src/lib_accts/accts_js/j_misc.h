#ifndef j_misc_H
#define j_misc_H

#include "j_class_base.h"
#include "j_args.h"
#include "obj_ref_js.h"
#include "jsc_prop_list.h"
#include "tbl_col_spec.h"
#include "buffer2.h"

#if 0
#define PROTO_JS_FUNC( name ) \
	JSBool name(				\
		JSContext *	_cx,		\
		JSObject  *	_instobj,	\
                uns           _argc,		\
                jsval *         _argv,		\
                jsval *         _retvar		\
	)
#define J_ARGS_FUNC( CX ) j_args_func CX( _cx, _instobj, _argc, _argv, _retvar );

#endif

// This covers the REPEATED arg names

#define J_ARGS_FUNC_Dset( CX, _func_name, nargs ) j_args_func_Dset CX( _cx, _argc, _vp, _func_name, nargs );
#define J_ARGS_FUNC_Row( CX, _func_name, nargs ) j_args_func_Row CX( _cx, _argc, _vp, _func_name, nargs );
#define J_ARGS_FUNC_Table( CX, _func_name, nargs ) j_args_func_Table CX( _cx, _argc, _vp, _func_name, nargs );

///////////// jsc_Misc ///////////////////////
namespace ACCTS {
struct j_args_func_Misc : public j_args_func
{
	bool insane;
	str0 func_name;

	j_args_func_Misc(
		JSContext *	_cx,
//		JSObject  *	_instobj,
                uns           _argc,
                jsval *         _vp,
//              jsval *         _argv,
//              jsval *         _retvar,
		str0		_func_name = "Function",
		int		nargs = 0
	)
	: j_args_func( _cx, _argc, _vp )
	, insane( false )
	, func_name( _func_name )
	{
		if(!full_sanity_check())
		{
			insane = true;
			return;
		}
		if( (int)argc != nargs )
		{
			int nargs2 = - nargs;
			if( (nargs2 >= 0 ) && (nargs2 <= (int) argc )) {
				// OK allow more if -ve
			} else {
				ReportError_v(
					"%s expects %d parameters (got %d)",
					(STR0) func_name,
					nargs,
					argc
				);
				insane = true;
			}
		}
		
	}

	//! utility function
	str0 get_single_string_arg(buffer2 & buf)
	{
		return as_string( argv[0], buf );
	}
};

}; // NAMNESPACE

#endif
