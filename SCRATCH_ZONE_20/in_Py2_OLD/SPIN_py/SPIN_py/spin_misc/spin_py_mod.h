#ifndef SPIN_PY_MOD_H
#define SPIN_PY_MOD_H

/*
	define global options
	no concrete use

	EDIT: "#define allowing_old_path"
*/

/*
	whilst editing around with new types for text and strings
	switch it off by defining the following:
*/

#define USING_PY_STRINGS

/*
	when using new STRING type, switch tracers on with:

	define STRING_TRACERS // this breaks elem name matching ? noisy?
*/

/*
	text_collector has this as a tracer feature
	text_collector_utf doesn't even ask

	#define TEXTCOLL_double_strike
*/

extern void gdb_break_point(void);


#endif
