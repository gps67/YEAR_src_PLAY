#include "g_obj.h"

#include <gtk/gtk.h>

/*
	COMPILER COMPLAINED:
	 warning #1881: argument must be a constant null pointer value
*/
#define CONST_NULL ((const void *) 0L)

	/*!
		set a g_objects named property to val
	*/
	void g_obj_temp::set_prop(  str0 prop, str0 val )
	{
		// what happens to old stored value ?
		g_object_set( obj, (STR0) prop, (STR0) val, CONST_NULL );
	}

	/*!
		set a g_objects named property to val
	*/
	void g_obj_temp::set_prop( str0 prop, int val )
	{
		g_object_set( obj, (STR0) prop,        val, CONST_NULL );
	}

	/*!
		set a g_objects named property to val
	*/
	void g_obj_temp::set_prop_obj( str0 prop, GObject * val )
	{
		// what happens to old stored value ?
		g_object_set( obj, (STR0) prop,        val, CONST_NULL );
	}

	/*!
		get a g_objects named property - must be a string
	*/
	str0 g_obj_temp::get_prop_str( str0 name )
	{
		char * str = NULL;
		g_object_get( obj, name, &str, CONST_NULL );
		return str;
	}

	/*!
		get a g_objects named property - must be a GObject
	*/
	GObject * g_obj_temp::get_prop_obj( str0 name )
	{
		GObject * ob = NULL;
		g_object_get( obj, name, &ob, CONST_NULL );
		return ob;
	}

	/*!
		get a g_objects named property - must be an int
	*/
	int g_obj_temp::get_prop_int( str0 name )
	{
		glong val;
		g_object_get( obj, name, &val, CONST_NULL );
		return (int) val;
	}

	/*!
		C --> C++ calback, used by set_data( name, data )
	*/
	void C_del_obj_ref( obj_ref * data )
	{
		data->debug_check_pointer();
		if(data) data->ref_decr();
	}

	/*!
		C --> C++ calback, used by set_data( name, data )
	*/
	void C_del_GObject( GObject * data )
	{
		// data is NOT an ob_ref_0 // data->validate_pointer();
		if(data) g_object_unref( data );
	}

	/*!
		set the property, along with its ref_counting destroyer
	*/
	void g_obj_temp::set_data( str0 name, obj_ref * data )
	{
		GDestroyNotify destroyer = (GDestroyNotify) C_del_obj_ref;
		g_object_set_data_full( obj, (STR0) name, data, destroyer );
	}

	/*!
		set the property, along with its ref_counting destroyer
	*/
	void g_obj_temp::set_data( str0 name, GObject * data )
	{
		GDestroyNotify destroyer = (GDestroyNotify) C_del_GObject;
		g_object_set_data_full( obj, (STR0) name, data, destroyer );
	}

	/*!
		manually adjust the wrapped objects reference count
	*/
	void g_obj_temp::ref_more()
	{
		g_object_ref( obj );
	}

	/*!
		manually adjust the wrapped objects reference count -
	*/
	void g_obj_temp::ref_less()
	{
		g_object_unref( obj );
	}

	/*!
		return the G_object type_name
	*/
	const char * g_obj_temp::_G_OBJECT_TYPE_NAME()
	{
		if(!obj) return "NULL_obj";
		return G_OBJECT_TYPE_NAME( obj );
	}

///////////////


g_obj_perm:: // CTOR
g_obj_perm ( GObject * _o )
: g_obj_temp( _o )
{
	if(_o) g_object_ref( _o );
}

g_obj_perm::
~g_obj_perm() // DTOR
{
	if(obj) g_object_unref( obj );
	obj = NULL;
}

g_obj_perm &
g_obj_perm::
operator = ( GObject * _o )
{
	if(_o) g_object_ref( _o );
	if( obj ) g_object_unref( obj );
	obj = _o;
	return * this;
}
