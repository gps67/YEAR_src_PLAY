/* spin_elem_info objects */

#include "spin_py_mod.h"
#include "Python.h"
#include "elem_info.h"
#include "str_obj.h"

#include "SPIN_type_Check.h"

#include "dtd_tables.h"

P_Enum_ET::P_Enum_ET( PyObject * any )
: P_Enum( get_cls() )
{
	set_from_any( any );
}

enum_cls * P_Enum_ET::get_cls()
{

	static P_ref<enum_cls> cls;
	if(!cls)
	{
		enum_cls * cls_ = new enum_cls("Element_Type");
		cls_->add_val_name( EL_empty,  "empty"  );
		cls_->add_val_name( EL_pre,    "pre"    );
		cls_->add_val_name( EL_inline, "inline" );
		cls_->add_val_name( EL_phrase, "phrase" );
		cls_->add_val_name( EL_block,  "block"  );

		cls.snatch_this( cls_ );
	}
	return cls.self();
}


P_Enum_ET::P_Enum_ET( Enum_Element_Type et )
: P_Enum( get_cls() )
{
	set_from_int( et );
}



SPIN_elem_info::SPIN_elem_info(
	SPIN_dtd_tables * _info_pool,
	PyObject * name_,
	PyObject * et,
	PyObject * content_type,
	bool element_breaks_word_
)
: PY_Object_base( &SPIN_elem_info_Type )
, info_pool_weak( _info_pool )
, name()
, el_type( et )
, ctype()
, on_element_start()
, on_element_end()
{
	// gdb_invoke(true);
	// info_pool_weak = _info_pool -> weak ; // LURK
	// claim a REF to the LINK but not to the LINKED
	// ABOVE // info_pool_weak.claim_weak(  _info_pool ); 
	name.claim_this( name_ );
	ctype.claim_this( content_type );

	set_on_flag( IS_first_use ); // cleared by ElementStart

	set_defaults_for_et( et );

	// Py_XINCREF( info_pool_weak );

		// add elem_info to the dict

	_info_pool->dict_elem_info.SetItem( name, (PyObject *)this );
}

SPIN_elem_info::~SPIN_elem_info()
{
	// Py_XDECREF( info_pool_weak );
}

void SPIN_elem_info::set_defaults_for_et( PyObject * etname )
{
	el_type.set_from_any( etname );
	set_defaults_for_et( (Enum_Element_Type)(int)el_type );
}

void SPIN_elem_info::set_defaults_for_et( Enum_Element_Type et )
{
	el_type.set_from_int( et );

	bool is_empty = false;
	bool is_pre = false;
	on_flags = 0;
	element_breaks_word = true;

	const char * c_type = NULL;
	switch( (int) el_type )
	{
	case EL_empty:
		c_type = "empty";
		is_empty = true;
	break;
	case EL_pre:
		c_type = "rcdata";
		is_pre = true;
	break;
	case EL_inline:
		element_breaks_word = false;
		c_type = "mixed";
	break;
	case EL_phrase:
		c_type = "mixed";
	break;
	case EL_block:
		c_type = "element"; // often mixed
	break;
	}
	PyObject * ct = PyString_FromString( c_type );
	ctype.snatch_this( ct );
}

/*
	spin_elem_info holds some info (is_pre,element_breaks_word) per
	element so that the SPIN core can process text wrapping without
	any call backs. After that it also holds the NAME of the elem,
	so you can pass elem_info instead of a string.	elem_info also
	provides a pool for other attributes.

	We still have to find the elem from the name, regrettably that
	means creating (then deleting) a PyString. ENDTAG has been made
	more efficient, by comparing the top of stack (requires compare
	CharString with char *). One option would be an internal hash
	table possibly a flat list (from the the DTD) of what is allowed
	within which element (eg ITEM within ITEMIZE).

	Ditto for attribute names!

	Ditto for sdata names

	SPIN_elem_info is the type name (rather than nameObject)

	an elem_info could hold the call-back functions,
	something like
		elem_info.on_start_element # mini script, list of fns?
	
*/


/* --------------------------------------------------------------------- */
//		SPIN	elem_info	TYPE
/* --------------------------------------------------------------------- */

// warning redeclaration 
// here its external
// later its static (unless i resigned and made it extern)

extern PyMethodDef spin_elem_info_methods[]; // forward without problem!

// #define SPIN_elem_info_Check(v)	((v)->ob_type == &SPIN_elem_info_Type)

extern SPIN_elem_info *
as_SPIN_elem_info( PyObject * elem )
{
	/*
		for now this is a type check,
		it could cast different types,
		or name to elem_info (via app_link's dictionary
		but where is app_link)
	*/
	if (elem && SPIN_elem_info_Check( elem )) {
		return (SPIN_elem_info *) elem;
	} else {
		PyErr_SetString(PyExc_TypeError,"SPIN_elem_info expected");
		return (SPIN_elem_info *) NULL;
	}
}

/*
	playing with Pythons API, to assist debug printing
*/

static int
spin_elem_info_print (
        SPIN_elem_info *op,
        FILE *fp,
        int flags )
{
	// LURK this is not re-loadable
	// but at least it names the element
        fprintf(fp, "(ELEM-INFO-");
	PyObject_Print(op->name, fp, 0);
        fprintf(fp, ")");
        return 0;
}


/* --------------------------------------------------------------------- */
//		SPIN	elem_info	new (name, is_pre)
/* --------------------------------------------------------------------- */

/* EXPORTED */
SPIN_elem_info * new_SPIN_elem_info(	// create or edit an elem_info 
	SPIN_dtd_tables * _info_pool,
	PyObject *name,			// using this script name
	PyObject * etype,	// "inline"
	PyObject *ctype,		// rcdata, pcdata, empty, ...
	bool breaks_word		// and these
) {
	if(!SPIN_dtd_tables_Check( _info_pool )) {
		PyErr_SetString(PyExc_TypeError,"new_SPIN_elem_info: SPIN_dtd_tables/dictionary expected");
		return NULL;
	}
#ifndef USING_PY_STRINGS
	if(!is_SPIN_string( name )) {
		PyErr_SetString(PyExc_TypeError,"elem_info name not SPIN_string");
		return NULL;
	}
#else
	if(!PyString_Check( name )) {
		PyErr_SetString(PyExc_TypeError,"elem_info name not a string");
		return NULL;
	}
#endif
	if(!is_SPIN_string( ctype )) {
		PyErr_SetString(PyExc_TypeError,"elem_info ctype not string");
		return NULL;
	}

	SPIN_elem_info *self = _info_pool->look_elem_info( name );

	if(self)  // already exists - reuse same object
	{
		if(SPIN_elem_info_Check(self)) 
			Py_INCREF( self ); // to match new
		else {
			// rare to the point of extinction
			// self exist/s/ed in pool but isnt elem_info !

			// delete it and start again
			_info_pool->dict_elem_info.DelItem( name );
			PyErr_SetString(PyExc_TypeError,"elem_info not builtin type");
			self = NULL;
		}
	}
	if(!self) {
		self = new SPIN_elem_info(
			_info_pool,
			name,
			etype,
			ctype,
			breaks_word
		);
		if (!self) return NULL;

		/* done below
		self->on_flags = 0;
		self->on_element_start = 0;
		self->on_element_end = 0;
		*/

	}

// if(empty) printf("YES - %s is empty\n", PyString_AsString( name ));

	// SetItem - DOES do INCREF
	// GetItem - DOES NOT do DECREF !!
	// Py_DECREF( self ); // to match new
	// spin_elem_info_print ( self, stdout, 0 );
	return self;
}

/*
	new_SPIN_elem_info( ... ) is the only thing exported from here,
	once the object exists, it can find its hidden static functions
*/

/* --------------------------------------------------------------------- */
//		SPIN	elem_info	dealloc, get/set attrs
/* --------------------------------------------------------------------- */

static void
spin_elem_info_dealloc( SPIN_elem_info *self )
{
	delete self;
}

/*
	This is how Python runs its object system
	method calls (name lookup) goes through the field name lookup
	which means they get two hash table lookups! 

	Its horribly slow, compared to C, and its completely loose
	in its late binding lookups. But thats what makes scripting
	so flexible as quick prototypes.
	Note these are types not classes.

	Here we skim off certain attr/field names, and implement
	them as C data (so other code can reach pre-checked data).

*/

static PyObject *
spin_elem_info_getattr(
	SPIN_elem_info *self,
	char *name_ )
{
	str0 name = name_;
	if( name == "el_type" )
	{
		return self->el_type.new_ref();
	}
	if( name == "el_type_str" )
	{
		return self->el_type.self()->strval.new_ref();
	}
	if( name == "is_pre" )
	{
		return PyInt_FromLong( self->IS_pre() );
	}
	if( name == "element_breaks_word" )
	{
		return PyInt_FromLong( self->element_breaks_word );
	}
	if( name == "is_empty" )
	{
		return PyInt_FromLong( self->IS_empty() );
	}
	if( name == "name" )
	{
		return self->name.new_ref() ; 
	}
	if( name == "on_element_start" )
	{
		return self->on_element_start.new_ref();
	}
	if( name == "on_element_end" )
	{
		return self->on_element_end.new_ref();
	}
	if( name == "ON_Element_Start" )
	{
		int mask = ON_Element_Start;
		return PyInt_FromLong( self->get_on_flag( mask ) );
	}
	if( name == "ON_Element_End" )
	{
		int mask = ON_Element_End;
		return PyInt_FromLong( self->get_on_flag( mask ) );
	}
	if( name == "ctype" )
	{
		return self->ctype.new_ref() ; 
	}
	if( name == "info_pool" )
	{
		SPIN_dtd_tables * pool = self->get_dtd_tables();
		Py_INCREF( pool );
		return (PyObject*) pool;
	}
	return self->get_attr_tail( spin_elem_info_methods, name );

}

static int
spin_elem_info_setattr(
	SPIN_elem_info *self,
	char *name_,
	PyObject *v )
{
	str0 name = name_;
	if( name == "element_breaks_word" )
	{
		int breaks_word = PyInt_AsLong( v ); // no error detected
		self->element_breaks_word = breaks_word;
		return 0;
	}
	if( name == "el_type" )
	{
		if( self->el_type.set_from_any( v ) )
			return 0;
		else
			return -1;
	}

	if( name == "ctype" )
	{
		if(!PyString_Check( v )) {
			return -1;
		}
		self->ctype.claim_this( v );
		return 0;
	}
	if( name == "name" )
	{
		// it should be illegal to change an elem_info's name!
		if(!PyString_Check( v )) {
			return -1;
		}
		self->name.claim_this( v );
		return 0;
	}
	if( name == "ON_Element_Start" )
	{
		int mask = PyInt_AsLong( v ); // no error detected
		if( mask ) mask = ON_Element_Start;
		if( mask ) {
			self->set_on_flag( mask );
		} else {
			self->clr_on_flag( mask );
		}
		return 0;
	}
	if( name == "ON_Element_End" )
	{
		int mask = PyInt_AsLong( v ); // no error detected
		if( mask ) mask = ON_Element_End;
		if( mask ) {
			self->set_on_flag( mask );
		} else {
			self->clr_on_flag( mask );
		}
		return 0;
	}

	if( name == "on_element_start" )
	{
		if( v == Py_None ) {
			self->on_element_start.release();
			return 0;
		}
		if(!PyCallable_Check( v )) {
			PyErr_SetString(PyExc_TypeError,
				"parameter must be callable" );
			return -1;
		}
		self->on_element_start.claim_this( v );
		return 0;
	}

	if( name == "on_element_end" )
	{
		if( v == Py_None ) {
			self->on_element_end.release();
			return 0;
		}
		if(!PyCallable_Check( v )) {
			PyErr_SetString(PyExc_TypeError,
				"parameter must be callable" );
			return -1;
		}
		self->on_element_end.claim_this( v );
		return 0;
	}

	return self->set_attr_tail( name, v );
}
/* --------------------------------------------------------------------- */
//	SPIN  elem_info  Type -- 
/* --------------------------------------------------------------------- */

// static // warning due to previous declaration 
// which HAD to be external for g++ (but not gcc)

PyTypeObject SPIN_elem_info_Type = {

	 PyObject_HEAD_INIT(&PyType_Type)
		0,				/*ob_size*/
	 (char *) "spin_elem_info",		/*tp_name*/
		sizeof(	SPIN_elem_info ),	/*tp_basicsize*/
		0,				/*tp_itemsize*/
	 /* methods */
	 (destructor)	spin_elem_info_dealloc, /*tp_dealloc*/
	 (printfunc)	spin_elem_info_print,	/*tp_print*/
	 (getattrfunc)	spin_elem_info_getattr, /*tp_getattr*/
	 (setattrfunc)	spin_elem_info_setattr, /*tp_setattr*/
		0,				/*tp_compare*/
		0,				/*tp_repr*/
		0,				/*tp_as_number*/
		0,				/*tp_as_sequence*/
		0,				/*tp_as_mapping*/
		0,				/*tp_hash*/
};


/* --------------------------------------------------------------------- */
//		SPIN	elem_info	PRIVATE get isp_pre direct
/* --------------------------------------------------------------------- */

// EXPORTED
int spin_elem_info_is_pre( PyObject *self )
{
	if(SPIN_elem_info_Check(self))
		return ((SPIN_elem_info *)self) -> IS_pre();

	PyErr_SetString(PyExc_TypeError,"SPIN_elem_info expected");
	// vret(0); // report it
	return 0;
}

/* --------------------------------------------------------------------- */
//		SPIN	elem_info	UNUSED
/* --------------------------------------------------------------------- */

/*
	obj.method( self, args ) (other than the standard ones)

	I couldnt think of any functions I want from elem_info objects
	but having a template will help later
*/

static PyObject *
spin_elem_info_hello(
	SPIN_elem_info *,
	PyObject *args )
{
	PyObject * string;
	printf("spin_elem_info_hello()\n");
	if (!PyArg_ParseTuple(args, (char *)""))
		return NULL;
	string = PyString_FromString("hello");
	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef spin_elem_info_methods[] = {
	{(char *)"hello",	(PyCFunction)spin_elem_info_hello,	1},
	{NULL,		NULL}		/* sentinel */
};

/* --------------------------------------------------------------------- */
//		SPIN	elem_info	MODULE functions
/* --------------------------------------------------------------------- */


/*
	NEW elem_info has to be careful, since the element is also
	registered in a dictionary. Any scheme is applicable, here is one.
*/
