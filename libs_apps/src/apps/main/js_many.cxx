/*
#include "amnt_t.h"
#include "date_glib.h"
#include "dset.h"

#include "w_widget0.h"
#include "w_file_sel.h"
#include "top_app.h"
*/

#include "misc.h"

#include "JS1.h"
#include "jsc_Stats_Module.h"
#include "jsc_Accts_Module.h"
#include "jsc_scan_Module.h"

int main(int argc, char ** argv )
{
	gdb_sigaction( argv[0] ); // sets progname_argv0

	bool ok = true;

	const char * filename = "nofilename.js";
	if( argc == 2 ) {
		filename = argv[1];
	}

	JS1 js1;
	js1.Init();

	/*
		The Accts module object is not actually used
		by the calling code, but it is recorded in itself!
		Its constructor adds things to the interpreter,
		and I dont want to call its destructor.

		I do want to SELECTIVELY activate different modules,
		maybe I should call a function that checks its static var
		and never repeats the add, or set lots of bools
	*/
	jsc_Accts_Module * Accts = NULL;
	jsc_Stats_Module * Stats = NULL;
	jsc_scan_Module * Scans = NULL;

	str0 name = progname_name;

	bool js_many = ( name == "js_many" );

	if( js_many 
 	|| ( name == "js_accts" )
	|| ( name == "accts_js" )
	)
	{
		// The C++ object that manages JS 
		Accts = new jsc_Accts_Module( & js1 );
	}

	if( js_many || ( name == "js_stats" ))
	{
		// The C++ object that manages JS 
		Stats = new jsc_Stats_Module( & js1 );
	}

	if( js_many || ( name == "js_scan" ))
	{
		// The C++ object that manages JS 
		Scans = new jsc_scan_Module( & js1 );
		// LURK Scan_Util is in play and accts (the JS part only)
		// the source is in play
		// the object is in accts
		// because its late
	}

	JSScript * script = NULL;

	str0 arg1 = argv[1];
	if( arg1 == "--Script" )
	{
		script = js1.CompileScript( argv[2] );
	} else if( (arg1 == "-") || arg1.is_empty() ) {
		js1.loop_edit_line();
	} else {
		script = js1.CompileFile_hash( argv[1] );
	}

	if( !script ) {
		ok = false;
	}
	if( script ) {
		if(! js1.ExecuteScript( script ) ) ok = false;;
		     js1.DestroyScript( script );
	}
	js1.shutdown();
	// ref_tally.print_leaked_report();
	if( ok )
		return 0;
	else
		return 1;
}

