#ifndef g_obj_H
#define g_obj_H

#include "str0.h"
#include "obj_ref.h"

/*
	Avoid the full include, users will need to get the full declarations
	from other sources. Some probs with #include <gobject.h> not complete
	#include <gtk/gtk.k>
*/
struct _GObject;
typedef struct _GObject GObject;


/*
	g_obj_temp uses GObject refcounting, perm adds obj_ref<>

	a wrapper on a g_object, to assist with properties and soon.

	There are two main varieties - temp assistance and perm wrappers.
	The memory overhead makes it less interesting to have permanent
	wrappers.

	The w_GTK class branch didnt use this - maybe add it under if it fits.
	That means that there is a lot of duplication, when a widget could
	be based on a g_obj (as it is in Gtk).

*/

/*!
	There should be a sub-base for a refcounting baseclass,
	of g_adjustment (and others), but this isnt it.

	g_obj_temp is a TEMP cast, which you then use to access
	its functions.  A tool to work on GObjects, set properties, etc

	As a TEMP, it doesnt OWN the object (no ref-counting),
	and C++ can create more copies - no destructor effect!
*/
class g_obj_temp : public GRP_lib_base1_glib
{
 protected:
	GObject * obj;
 public:
	g_obj_temp( GObject * _obj )
	{
		obj = _obj;
	}
	g_obj_temp & operator=( GObject * _o )
	{
		obj = _o;
		return *this;
	}
	GObject * OBJ() { return obj; }
	operator bool() { return (bool) obj; }
	operator GObject *() { return obj; }

	void set_prop(  str0 prop, str0 val );
	void set_prop( str0 prop, int val );
	void set_prop_obj( str0 prop, GObject * val );

	str0 get_prop_str( str0 name );
	GObject * get_prop_obj( str0 name );
	int get_prop_int( str0 name );

	void set_data( str0 name, obj_ref * data );
	void set_data( str0 name, GObject * data );

	void ref_more(); // vs incr/decr of obj_ref
	void ref_less();

	const char * _G_OBJECT_TYPE_NAME();

};

/*!
	THIS CLASS now USED

	2010-10-12 HT_List_Item.h
*/


class g_obj_perm : public g_obj_temp
{
 public:
	~g_obj_perm();
	g_obj_perm( GObject * _o );
	g_obj_perm & operator=( GObject * _o );
};


#endif
