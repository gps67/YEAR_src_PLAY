// Copyright (c) 1999 Graham Swallow
// sp_peg_kit.cxx
// SP ParserEventGeneratorKit
// setup the search paths, SP options (not filenames)

#include "ParserEventGeneratorKit.h"
#include "sp_peg_kit.h"
#include "SP_App.h"

#include "Python.h"
#include "app_link.h"
#include "argv_builder.h"

#include "SPIN_type_Check.h"

// FORWARD
extern PyMethodDef SP_peg_kit_methods[];

// FORWARD
static PyObject * kit_setProgramName( SP_peg_kit * self, PyObject *args );

// EXPORTED
SP_peg_kit *
as_SP_peg_kit( PyObject * self )
{
	if ((self)->ob_type == &SP_peg_kit_Type)
		return (SP_peg_kit *) self;
	PyErr_SetString(PyExc_TypeError,"SP_peg_kit expected");
	return NULL;
}

/*!
	simple constructor
*/
SP_peg_kit::SP_peg_kit()
: PY_Object_base( &SP_peg_kit_Type )
{
	kit = new ParserEventGeneratorKit();
	if(!kit) throw "NULL kit";
}

/*!
	simple destructor
*/
SP_peg_kit::~SP_peg_kit()
{
	if( kit ) delete kit;
	kit = NULL;
}

// EXPORTED
PyObject *
new_SP_peg_kit( PyObject *, PyObject * args ) 
{
	PyObject * string1;
        if (!PyArg_ParseTuple(args, (char *)"S", &string1 ))
                return NULL;
 
	SP_peg_kit * self = new SP_peg_kit();
	if(!self) { return NULL; }
	if(!self->kit) {
		Py_XDECREF( self );
                // some error - cant use raise_error - no EH yet
		return NULL; // python will report something
	}

	kit_setProgramName( self, args );

	return (PyObject *) self;
}

static void
SP_peg_kit_dealloc( SP_peg_kit *self )
{
	delete self;
}

static PyObject *
SP_peg_kit_getattr(
        SP_peg_kit * self,
        char * name
)
{
	return Py_FindMethod(SP_peg_kit_methods, (PyObject *)self, name);
}


static PyObject * kit_showOpenEntities(
        SP_peg_kit * self,
        PyObject *args
)
{
        if (!PyArg_ParseTuple(args, (char *)"")) return NULL;
	self->kit->setOption( ParserEventGeneratorKit::showOpenEntities );
	Py_INCREF( Py_None ); return Py_None;
}

static PyObject * kit_outputCommentDecls(
        SP_peg_kit * self,
        PyObject *args
)
{
        if (!PyArg_ParseTuple(args, (char *)"")) return NULL;
	self->kit->setOption( ParserEventGeneratorKit::outputCommentDecls );
	Py_INCREF( Py_None ); return Py_None;
}

static PyObject * kit_outputMarkedSections(
        SP_peg_kit * self,
        PyObject *args
)
{
        if (!PyArg_ParseTuple(args, (char *)"")) return NULL;
	self->kit->setOption( ParserEventGeneratorKit::outputMarkedSections );
	Py_INCREF( Py_None ); return Py_None;
}

static PyObject * kit_outputGeneralEntities(
        SP_peg_kit * self,
        PyObject *args
)
{
        if (!PyArg_ParseTuple(args, (char *)"")) return NULL;
	self->kit->setOption( ParserEventGeneratorKit::outputGeneralEntities );
	Py_INCREF( Py_None ); return Py_None;
}

static PyObject * kit_mapCatalogDocument(
        SP_peg_kit * self,
        PyObject *args
)
{
        if (!PyArg_ParseTuple(args, (char *)"")) return NULL;
	self->kit->setOption( ParserEventGeneratorKit::mapCatalogDocument );
	Py_INCREF( Py_None ); return Py_None;
}

static PyObject * kit_showOpenElements(
        SP_peg_kit * self,
        PyObject *args
)
{
        if (!PyArg_ParseTuple(args, (char *)"")) return NULL;
	self->kit->setOption( ParserEventGeneratorKit::showOpenElements );
	Py_INCREF( Py_None ); return Py_None;
}


// -- with single string parameter

static PyObject * kit_includeParam(
        SP_peg_kit * self,
        PyObject *args
)
{
	const char * str = "-unset-";
        if (!PyArg_ParseTuple(args, (char *)"s",&str)) return NULL;
	self->kit->setOption( ParserEventGeneratorKit::includeParam, str );
	Py_INCREF( Py_None ); return Py_None;
}

static PyObject * kit_enableWarning(
        SP_peg_kit * self,
        PyObject *args
)
{
	const char * str = "-unset-";
        if (!PyArg_ParseTuple(args, (char *)"s",&str)) return NULL;
	self->kit->setOption( ParserEventGeneratorKit::enableWarning, str );
	Py_INCREF( Py_None ); return Py_None;
}

static PyObject * kit_addSearchDir(
        SP_peg_kit * self,
        PyObject *args
)
{
	const char * str = "-unset-";
        if (!PyArg_ParseTuple(args, (char *)"s",&str)) return NULL;
	str = strdup( str ); // LURK FIX LEAK - Python deletes calling string
	// and that probably appies to other
	self->kit->setOption( ParserEventGeneratorKit::addSearchDir, str );
	Py_INCREF( Py_None ); return Py_None;
}

static PyObject * kit_addCatalog(
        SP_peg_kit * self,
        PyObject *args
)
{
	const char * str = "-unset-";
        if (!PyArg_ParseTuple(args, (char *)"s",&str)) return NULL;
	str = strdup( str ); // LURK FIX LEAK - Python deletes calling string
	self->kit->setOption( ParserEventGeneratorKit::addCatalog, str );
	Py_INCREF( Py_None ); return Py_None;
}

static PyObject * kit_setProgramName(
        SP_peg_kit * self,
        PyObject *args
)
{
	const char * str = "-unset-";
        if (!PyArg_ParseTuple(args, (char *)"s",&str)) return NULL;
	str = strdup( str ); // LURK FIX LEAK - Python deletes calling string
	self->kit->setProgramName( str );
	Py_INCREF( Py_None ); return Py_None;
}


/*
self->kit->setOption( ParserEventGeneratorKit::includeParam, "SPIN" );
self->kit->setOption( ParserEventGeneratorKit::enableWarning, "all" );
self->kit->setOption( ParserEventGeneratorKit::addSearchDir, "/etc/sgml" );
self->kit->setOption( ParserEventGeneratorKit::addCatalog, "catalog");
*/

/* now in SPIN_top
static PyObject * SPIN_opt(
        PyObject *, // module
        PyObject *args
)
{
        PyObject *obj = 0;
        PyObject *val = 0;
	const char * opt;
        if (!PyArg_ParseTuple(args, (char *)"OsO",
		&obj,
		&opt,
		&val ))
                return NULL;
	// SP_App * spin_code = find_SP_App( obj );
	SP_App * spin_code = 0;
	if(!spin_code) {
		return NULL;
	}
	spin_code->set_SPIN_opt( opt, val );
	Py_INCREF( Py_None );
	return Py_None;
}
*/

/*
	if you dont like the DTD that the document wants,
	or if you only want the title page, call halt.

	EDIT: move this to app_link
*/

/*
kit->setProgramName( "Spinning" );
kit->setOption( ParserEventGeneratorKit::showOpenEntities );
kit->setOption( ParserEventGeneratorKit::showOpenElements );
kit->setOption( ParserEventGeneratorKit::outputCommentDecls );
kit->setOption( ParserEventGeneratorKit::outputMarkedSections );
kit->setOption( ParserEventGeneratorKit::outputGeneralEntities );
kit->setOption( ParserEventGeneratorKit::mapCatalogDocument );
kit->setOption( ParserEventGeneratorKit::includeParam, "SPIN" );
kit->setOption( ParserEventGeneratorKit::enableWarning, "all" );
kit->setOption( ParserEventGeneratorKit::addSearchDir, "/etc/sgml" );
kit->setOption( ParserEventGeneratorKit::addCatalog, "catalog");
...etc...
*/

/* THIS IS NOW DELETED - have peg_kit access

#define OPTION(s) (0==strcmp(opt,s))
static PyObject *
xxxSPIN_parser_opt_val(
        PyObject *, // the module
        PyObject *args
)
{
	// EH requests parser option
        PyObject *obj = 0;
	char * opt = 0;
	char * val = (char *) "UNSET";
        if (!PyArg_ParseTuple(args, (char *)"Os|s",
		&obj,
		&opt,
		&val ))
                return NULL;
	ParserEventGeneratorKit * kit = 0; // = find_ParserKit( obj );
	if(!kit) { return NULL; }

 if     (OPTION("showOpenEntities")) 
	kit->setOption( ParserEventGeneratorKit::showOpenEntities );
 else if(OPTION("showOpenElements")) 
	kit->setOption( ParserEventGeneratorKit::showOpenElements );
 else if(OPTION("outputCommentDecls"))
	kit->setOption( ParserEventGeneratorKit::outputCommentDecls );
 else if(OPTION("outputMarkedSections"))
	kit->setOption( ParserEventGeneratorKit::outputMarkedSections );
 else if(OPTION("outputGeneralEntities"))
	kit->setOption( ParserEventGeneratorKit::outputGeneralEntities );
 else if(OPTION("mapCatalogDocument")) // strange effects
	kit->setOption( ParserEventGeneratorKit::mapCatalogDocument );
 else if(OPTION("includeParam"))	// <!ENTITY %SPIN "INCLUDE">
	kit->setOption( ParserEventGeneratorKit::includeParam, val );
 else if(OPTION("enableWarning")) // -wwarning - see nsgmls docs
	kit->setOption( ParserEventGeneratorKit::enableWarning, val );
 else if(OPTION("setProgramName"))
	kit->setProgramName( val );
 else if(OPTION("addSearchDir"))
	kit->setOption( ParserEventGeneratorKit::addSearchDir, val );
 else if(OPTION("addCatalog"))

	kit->setOption( ParserEventGeneratorKit::addCatalog, val );
 else {
	printf("un recognised options %s\n", opt );
	return NULL;
 }
	Py_INCREF( Py_None );
	return Py_None;
}

*/

/* --------------------------------------------------------------------- */
//	SP  peg_kit  Type -- 
/* --------------------------------------------------------------------- */

// static // warning due to previous declaration 
// which HAD to be external for g++ (but not gcc)

PyTypeObject SP_peg_kit_Type = {

	 PyObject_HEAD_INIT(&PyType_Type)
		0,				/*ob_size*/
	 (char *) 	"SP_peg_kit",		/*tp_name*/
			sizeof(	SP_peg_kit ),	/*tp_basicsize*/
		0,				/*tp_itemsize*/
	 /* methods */
	 (destructor)	SP_peg_kit_dealloc,	/*tp_dealloc*/
	 (printfunc)	0,			/*tp_print*/
	 (getattrfunc)	SP_peg_kit_getattr,	/*tp_getattr*/
	 (setattrfunc)	0,			/*tp_setattr*/
		0,				/*tp_compare*/
		0,				/*tp_repr*/
		0,				/*tp_as_number*/
		0,				/*tp_as_sequence*/
		0,				/*tp_as_mapping*/
		0,				/*tp_hash*/
};


/* --------------------------------------------------------------------- */
//		SPIN	elem_info	UNUSED
/* --------------------------------------------------------------------- */

// static
PyMethodDef SP_peg_kit_methods[] = {

 {(char *)"showOpenEntities",     (PyCFunction) kit_showOpenEntities, 1},
 {(char *)"showOpenElements",     (PyCFunction) kit_showOpenElements, 1},
 {(char *)"outputCommentDecls",   (PyCFunction) kit_outputCommentDecls, 1},
 {(char *)"outputMarkedSections", (PyCFunction) kit_outputMarkedSections, 1},
 {(char *)"outputGeneralEntities",(PyCFunction) kit_outputGeneralEntities, 1},
 {(char *)"mapCatalogDocument",   (PyCFunction) kit_mapCatalogDocument, 1},

 {(char *)"setProgramName",       (PyCFunction) kit_setProgramName, 1},

 {(char *)"includeParam",         (PyCFunction) kit_includeParam, 1},
 {(char *)"enableWarning",        (PyCFunction) kit_enableWarning, 1},
 {(char *)"setProgramName",       (PyCFunction) kit_setProgramName, 1},
 {(char *)"addSearchDir",         (PyCFunction) kit_addSearchDir, 1},
 {(char *)"addCatalog",           (PyCFunction) kit_addCatalog, 1},

 {         NULL,		                NULL}		/* sentinel */
};
/* --------------------------------------------------------------------- */
