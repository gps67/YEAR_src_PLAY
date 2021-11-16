
#include "enum_cls.h"
#include "enum_val.h"

enum_cls::enum_cls( const char * name )
: PY_weak_base(&enum_cls_Type)
// : PY_Object_base(&enum_cls_Type)
, cls_name( name )
, val_unset()
, val_list()
{
	val_unset.snatch_this( new enum_val( this, -1, "ENUM_ERROR_UNSET" ) );
}

enum_val * enum_cls::add_val_name(
	int v,
	const char * str
)
{
	enum_val * val = new enum_val( this, v, str );
	val_list.Append( (PyObject *)val );
	return val;
}

enum_val * enum_cls::find_by_val( int v )
{
	int n = val_list.N();
	for( int i=0; i<n; i++ )
	{
		enum_val * val = (enum_val *) val_list.get( i ); 
		if( (int) * val == v ) return val;
	}
	PyErr_SetString( PyExc_TypeError, "find_by_val(%d)" );
	return NULL;
}


enum_val * enum_cls::find_by_name( PyObject * str )
{
	return find_by_name( PyString_AsString( str ) );
}

enum_val * enum_cls::find_by_name( const char * str )
{
	int n = val_list.N();
	for( int i=0; i<n; i++ )
	{
		enum_val * val = (enum_val *) val_list.get( i ); 
		if( (str0) * val == str ) return val;
	}
	PyErr_SetString( PyExc_TypeError, str );
	return NULL;
}

enum_val * enum_cls::find_by_any( PyObject * v )
{
	// NULL is not allowed
	if(!v)
	{
		PY_Static::TypeError( "enum_cls::find_by_any( NULL )" );
		return NULL;
	}

	// maybe v is already a valid enum_val, of correct class ..
	if( enum_val_Check( v ) )
	{
		enum_val * val = (enum_val *) v;
		enum_cls * cls1 = this;
		enum_cls * cls2 = val->get_enum_cls();
		if( cls1 == cls2 )
		{
			return val;
		}

		/*
			THINGS we dont support (LAZY)
			enum class is subclass of another enum class
			enum class is compattible with another class
			-- get int then use int --
			coul;d maybe print a warning
		*/
		PY_Static::TypeError( cls1->cls_name.get_str() );
		return false;
	}
	// maybe v is "green"
	if( PY_Static::IS_String( v ) )
	{
		return find_by_name( v );
	}
	// maybe v is 3
	if( PY_Static::IS_Int( v ) )
	{
		return find_by_val(  PyInt_AsLong( v ));
	}
	PY_Static::TypeError( "enum_cls::find_by_any( name|val|pair )" );
	return NULL;
}


static void
enum_cls_dealloc( enum_cls * self )
{
        delete self;
}

extern PyMethodDef enum_cls_methods[];

static PyObject * enum_cls_getattr( enum_cls *self, char *name )
{
        return self->get_attr_tail( enum_cls_methods, name );
}

static int enum_cls_setattr( enum_cls *self, char *name, PyObject *v )
{
        return self->set_attr_tail( name, v );
}

PyMethodDef enum_cls_methods[] = {
        {NULL,          NULL}           /* sentinel */
};



PyTypeObject enum_cls_Type = {

         PyObject_HEAD_INIT(&PyType_Type)
                0,				/*ob_size*/
         (char *) "enum_cls",			/*tp_name*/
                sizeof( enum_cls ),		/*tp_basicsize*/
                0,				/*tp_itemsize*/
         /* methods */
         (destructor)   enum_cls_dealloc,	/*tp_dealloc*/
         (printfunc)    0,			/*tp_print*/
         (getattrfunc)  enum_cls_getattr,	/*tp_getattr*/
         (setattrfunc)  enum_cls_setattr,	/*tp_setattr*/
                0,                              /*tp_compare*/
                0,                              /*tp_repr*/
                0,                              /*tp_as_number*/
                0,                              /*tp_as_sequence*/
                0,                              /*tp_as_mapping*/
                0,                              /*tp_hash*/
};


