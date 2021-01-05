#include "jsc_Stats_Module.h"
#include "stats_sysinfo.h"
#include "proc_partitions.h"

#define IFDBG if(0)

///////////// jsStats  ///////////////////////

/*
.../src/sar_tests/js_stats/jsc_Stats_Module.cxx(17):
error: argument of type
"JSBool={JSIntn={int}} (*)(JSContext *, JSObject *, uintN={JSUintn={unsigned int}}, jsval={uint64={JSUint64={uint64_t={unsigned long}}}} *, jsval={uint64={JSUint64={uint64_t={unsigned long}}}} *)"
is incompatible with parameter of type
"JSNative"

Yes they changed the parameters so much, but left the names the same.
I want to get off this vehicle, thankyou.
Next stop.
Just as soon as I can, frankly, it would have been quicker to do so initially.

*/


jsc_Stats_Module::
jsc_Stats_Module( JS1 * js1 )
: j_class_plus( "Stats_Module", js1, true )
{

	Def_Function(
		"partitions",
		f_partitions,
		0		// number of args
	);

	Def_Function(
		"sysinfo",
		f_sysinfo,
		0		// number of args
	);

}


/*!
	There should only be zero/one instance of Stats, (like a module)
	but it is created in the script as new Stats()

	JS has already created the (accts) JSObject,
	so upgrade it and give the JSObject a few functions.
*/
//  VIRTUAL
JSBool jsc_Stats_Module::new_Thing( j_args_newfunc * args )
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
	The Stats module can create any number of different DSET CLASSES,
	(by loading them from files).

	A DSET is a class, because each has a different set of tables,
	which appear as top level properties (as dset.VAT not dset.table.VAT)

	It gives me some practice with class-attributes, but POTENTIALLY
	clashes table names against function names, so they have to have
	'odd' names or __names() Ditto for table._row_keys()

	An object could also have varient fields, but using fixed prop id's
	That means the tables cannot be renumbered here or in the C++ side
*/
//  STATIC
JSBool jsc_Stats_Module:: f_sysinfo(
	JSContext *	cx,
	uns           argc,
	jsval *         vp
)
{
	// The MODULE object ("Stats")
	j_cx_obj_peer CX_ACCT( cx, argc, vp );
//	j_cx_obj_peer CX_ACCT( cx, module_obj );
	if(!CX_ACCT.full_sanity_check()) return JS_FALSE;

	// get filename
	if( argc != 0 )
	{
		return CX_ACCT.ReportError( "f_sysinfo() - no parameters allowed" );
	}
	// str0 filename = CX_ACCT.as_string( argv[0] );


//	str0 classname; // defaults NULL
//	jsc_Dset * jsc = jsc_Dset::mk_jsc_Dset( classname, CX_ACCT.get_js1(), dset ); // NEVER DELETED!

//	// wrap C++ dset in a JS_Object, and set that as a return JSVAL
//	*retvar = OBJECT_TO_JSVAL( jsc->mk_js_self( dset ) );

	main_stats_sysinfo();

	return JS_TRUE;
}


/*!
	A second function that runs the parser for /proc/partitions

	Theres (currently) no return value.
	presumably the internal machine forwards the received data to (...)
	with other functions to configure the internal machine, send now, etc
*/
//  STATIC
JSBool jsc_Stats_Module:: f_partitions(
	JSContext *	cx,
	uns           argc,
	jsval *         vp
)
{
	// The MODULE object ("Stats")
//	j_cx_obj_peer CX_ACCT( cx, module_obj );
	j_cx_obj_peer CX_ACCT( cx, argc, vp );
	if(!CX_ACCT.full_sanity_check()) return JS_FALSE;

	// get filename
	if( argc != 0 )
	{
		return CX_ACCT.ReportError( "f_partitions() - no parameters allowed" );
	}
	// str0 filename = CX_ACCT.as_string( argv[0] );


//	str0 classname; // defaults NULL
//	jsc_Dset * jsc = jsc_Dset::mk_jsc_Dset( classname, CX_ACCT.get_js1(), dset ); // NEVER DELETED!

//	// wrap C++ dset in a JS_Object, and set that as a return JSVAL
//	*retvar = OBJECT_TO_JSVAL( jsc->mk_js_self( dset ) );

	main_proc_partitions();

	return JS_TRUE;
}


