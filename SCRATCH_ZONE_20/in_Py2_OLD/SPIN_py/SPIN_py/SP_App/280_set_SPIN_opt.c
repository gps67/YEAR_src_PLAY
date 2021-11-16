#define OPTION(b) (0==strcmp(opt,b))

void SP_App::set_SPIN_opt(const char * opt, PyObject * val )
{
	int int_val = -1;
	if(PyInt_Check( val )) {
		int_val = PyInt_AsLong( val );
	} else {
		raise_error("set_SPIN_opt currently expects an int");
		return;
	}

	// printf("set_SPIN_opt %s %d - request\n", opt, int_val );

	if OPTION("data_trim") {
		// printf("set_SPIN_opt %s %d - OK\n", opt, int_val );
		data_trim = int_val;
	} else if OPTION("space_migration") {
		data_trim = 1;
		set_wrap( int_val );
		// react to CTYPE-element - treat as space, and eat adjacent
		// option of retro eating prev sp
		// options of adding missing space
		// option of detect NOSP
		// currently unused - done by trim_wrap
	} else if OPTION("wrap") {
		// see DtdStart.py for when to call these
		// printf("set_SPIN_opt %s %d - OK\n", opt, int_val );
		set_wrap( int_val );
		// stack.item[stack.sp].wrap = int_val;
	} else if OPTION("auto_load") {
		bool on = false;
		if( int_val ) on = true; // normalise bool
		app.set_auto_load( on );
	} else if OPTION("unknown") {
	} else {
		printf("ERROR set_SPIN_opt %s %d\n", opt, int_val );
		raise_error(opt);
	}
}
