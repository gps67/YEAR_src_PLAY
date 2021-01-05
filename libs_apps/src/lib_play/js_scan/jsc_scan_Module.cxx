#include "jsc_scan_Module.h"
#include "scan_util.h"
#include "buffer1.h"

#define IFDBG if(0)

//copied from Stats - rename scan - mechanically unsound !
///////////// jsscan  ///////////////////////


jsc_scan_Module::
jsc_scan_Module( JS1 * js1 )
: j_class_plus( "Scan_Module", js1, true )
{

	// lots of functions here 
	// ...

	Def_Function(
		// this IS the correct default function
		"scan_into_file",
		f_scan_into_file,
		0		// number of args
	);

	Def_Function(
		// can UK-EMBASSY read PNG - I dont know !
		"scan_into_jpeg",
		f_scan_into_jpeg,
		0		// number of args
	);

}


/*!
	There should only be zero/one instance of scan, (like a module)
	but it is created in the script as new scan()

	JS has already created the (accts) JSObject,
	so upgrade it and give the JSObject a few functions.
*/
//  VIRTUAL
JSBool jsc_scan_Module::new_Thing( j_args_newfunc * args )
{
	// args->set_obj_inst( new no_priv() ); // LEAKED
	set_inst(
		new no_priv(), // module has no C++ instance
		args->js_obj
	);
/*
	I think this showed that it could have been here ?
	JS_DefineFunction(
		args->cx,
		args->js_obj,
		"load_dset",
		f_load_dset,
		1,
		0
	);
*/

	return JS_TRUE;
}


/*!
* 	...
*/
//  STATIC
PROTO_JS_FUNC( jsc_scan_Module:: f_scan_into_file )
{
	// The MODULE object ("scan")
	j_cx_obj_peer CX_ACCT( _cx, _argc, _vp );
	if(!CX_ACCT.full_sanity_check()) return JS_FALSE;

	// get filenames
	if( _argc != 2 )
	{
		return CX_ACCT.ReportError( "f_scan_into_file(filename,comment) - two parameters required (got %d )", _argc );
	}
	buffer1 buf1;
	buffer1 buf2;
	str0 filename = CX_ACCT.as_string( J_ARGV0, buf1 );
	str0 comment = CX_ACCT.as_string( J_ARGV1, buf2 );


	ACCTS::Scan_Util scanner;
	if( scanner.scan_into_file( filename, comment ) )
	{
		// pass
	} else {
		// fail
		return CX_ACCT.ReportError_v(
			"scan_into_file('%s','%s') failed\n",
			(STR0) filename,
			(STR0) comment
		);
	}
	return JS_TRUE;
}





/*!
* 	...
*/
//  STATIC
PROTO_JS_FUNC( jsc_scan_Module:: f_scan_into_jpeg )
{
	// The MODULE object ("scan")
	j_cx_obj_peer CX_ACCT( _cx, _argc, _vp );
	if(!CX_ACCT.full_sanity_check()) return JS_FALSE;

	// get filenames
	if( _argc != 2 )
	{
		return CX_ACCT.ReportError( "f_scan_into_jpeg(filename,comment) - two parameters required (got %d )", _argc );
	}
	buffer1 buf1;
	buffer1 buf2;
	str0 filename = CX_ACCT.as_string( J_ARGV0, buf1 );
	str0 comment = CX_ACCT.as_string( J_ARGV1, buf2 );


	ACCTS::Scan_Util scanner;
	if( scanner.scan_into_jpeg( filename, comment ) )
	{
		// pass
	} else {
		// fail
		return CX_ACCT.ReportError_v(
			"scan_into_jpeg('%s','%s') failed\n",
			(STR0) filename,
			(STR0) comment
		);
	}
	return JS_TRUE;
}




