// Copyright (c) 1998 Graham Swallow
// Based on /usr/src/sp/doc/generic.htm by James Clark
// spin_py_mod.cxx
// the module initialiser, etc

#include "spin_py_mod.h"
#include "SP_App.h"

#include "Python.h"
#include "app_link.h"
#include "argv_builder.h"
#include "sp_peg_kit.h"
#include "spin_top.h"

#include "write_buffer.h"  

#include "SPIN_tree_print/SPIN_tree_print.h"
#include "buffer/misc.h" // gdb_sigaction()


/*
	This file implements the python module,
	it registers a few functions that create objects
	which are the running SP SGML Parser.

	The calling object is a plain python class.
*/

///////////////////////////////////////////////////////////////////////

// slightly more side effects than normal - see the docs
static PyObject *
GDB_INVOKE(
        PyObject *, // the module
        PyObject * args
)
{
	bool use_gui = true;
	gdb_invoke( use_gui );
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////


// slightly more side effects than normal - see the docs
static PyObject *
SPIN_elem_info(
        PyObject *, // the module
        PyObject * args
)
{
        SPIN_dtd_tables *dry = 0;
        PyObject *name = 0;
        PyObject *etype = 0;
        PyObject *ctype = 0;
        int breaks_word = 0;
        if (!PyArg_ParseTuple(args, (char *)"OOOOi",
		&dry,
		&name,
		&etype,
		&ctype,
		&breaks_word
	))
                return NULL;

	if( !Expect_SPIN_dtd_tables(dry) ) return NULL;
	return (PyObject *) new_SPIN_elem_info(
		dry,
		name,
		etype,
		ctype,
		breaks_word
	);
}

static PyObject *
SPIN_elem_obj_type(
        PyObject *, // the module
        PyObject * args )
{
	return type_SPIN_elem_obj();
}

static PyObject *
SPIN_string_type(
        PyObject *, // the module
        PyObject * args )
{
#ifdef USING_PY_STRINGS
	Py_INCREF( & PyString_Type) ; 
	return (PyObject *) & PyString_Type; 
#else
	return type_SPIN_string();
#endif
}

static PyObject *
SPIN_string(
        PyObject *, // the module
        PyObject * args )
{
        PyObject *string = 0;
        if (!PyArg_ParseTuple(args, (char *)"S",
		&string ))
                return NULL;
#ifdef USING_PY_STRINGS
	Py_INCREF( string );
	return( string );
#else
	return (PyObject *) new_SPIN_string( string );
#endif
}

static PyObject *
SPIN_elem_obj(
        PyObject *, // the module
        PyObject * args )
{
        PyObject *eleminfo = 0;
        PyObject *attslist = 0;
        PyObject *datalist = 0;
        if (!PyArg_ParseTuple(args, (char *)"OOO",
		&eleminfo,
		&attslist,
		&datalist ))
                return NULL;
	return (PyObject *) new_SPIN_elem_obj( eleminfo, attslist, datalist );
	/* new_SPIN_elem_obj does all the checking */
}

static PyObject *
SPIN_write_buffer(
        PyObject *, // the module
        PyObject * args )
{
        PyObject *downstream = 0;
	int lo, hi;
        if (!PyArg_ParseTuple(args, (char *)"Oii:SPIN_write_buffer",
		&downstream,
		&lo,
		&hi ))
                return NULL;
	return (PyObject *) new_SPIN_write_buffer( downstream, lo, hi );
	/* new_SPIN_elem_obj does all the checking */
}

 static char doc_GDB_INVOKE[] =
	 "(1) -- gdb_invoke starts gdb";

 static char doc_elem_info[] =
	 "(dry,name,ispre) -- create a new elem_info in dry (eh.elem_info)";
 static char doc_elem_obj[] =
	 "(eleminfo,atts,datalist) -- create a new elem_obj";
 static char doc_SPIN_string[] =
	 "(string) -- create a new string";
 static char doc_new_peg[] =
	 "(progname) -- create a kit that creates Parsers - holds settings";
 static char doc_new_wb[] =
	 "(progname) -- create a write_buffer, on a writable obj, lo, hi ";

static PyMethodDef SpinMethods[] = {
 {(char *) "GDB_INVOKE",  GDB_INVOKE, 1, doc_GDB_INVOKE },
 {(char *) "SPIN_elem_info",  SPIN_elem_info, 1, doc_elem_info },
 {(char *) "SPIN_elem_obj",   SPIN_elem_obj, 1, doc_elem_obj },
 {(char *) "SPIN_elem_obj_type",   SPIN_elem_obj_type, 1, doc_elem_obj },
 {(char *) "SPIN_top",        new_SPIN_top, 1, doc_new_peg },
// to be deleted ...
 {(char *) "SPIN_string_type",   SPIN_string_type, 1, doc_elem_obj },
 {(char *) "SPIN_string",     SPIN_string, 1, doc_SPIN_string },
 {(char *) "SP_ParserEventGeneratorKit",   new_SP_peg_kit, 1, doc_new_peg },
 {(char *) "SPIN_write_buffer",   SPIN_write_buffer, 1, doc_new_wb },
 {(char *) "SPIN_tree_print",   new_SPIN_tree_print, 1, "NEW tree printer" },
 {NULL,      NULL}        
};

extern "C" {
	// this is run when spin_py.so is loaded
	void initspin_py() // import spin_py (from .so) auto runs this
	{
		/* declare the module */
		(void) Py_InitModule((char *) "spin_py", SpinMethods);
		gdb_sigaction( NULL );
	}
} // extern
