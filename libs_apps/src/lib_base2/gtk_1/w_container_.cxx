
#include "w_container_.h"

#include "w_widget0.h"
#include "obj_hold.h"
// #include "w_window.h"
#include "obj_list.h"
#include "dgb.h"

#define DBG0 if(0)

/*!
	constructor - default
*/
w_container_::w_container_()
{
}

/*!
	destructor - default -- ? -- call remove_all() ??
*/
w_container_::~w_container_()
{
	/*
		This might be a red herring - when the derived class
		object gets as far as here, the VTBL might have been
		replaced with one for this class, and thats empty
	*/
	// remove_all();
}

void w_container_::w_set( GtkWidget * sub )
{
	/*
		this is just a bit NAFF, I dont like it that much

		in particular is over_shadows w_widget::w_set()
		(neither of this should be virtual)

		the problem is to register the widget for deletion
		automatically, a bit more.

		alternative1: use ref_incr on the GtkWidget (everywhere)

		alternative2: call connect_event_destroyed(); yourself
		(or on the child object ...)

		alternative3: set a flag and do this in the base class
	*/
	if(w) disconnect_event_destroyed();
	w_widget0::w_set( sub );
	if(sub) connect_event_destroyed();
}

/*!
	return the underlying GTK container object
*/
GtkContainer * w_container_::Container()
{
	if(!w) {
		e_print("Container() w is NULL");
		debug_track_pointer();
		gdb_break_point();
	}
	if(! GTK_CONTAINER(w) ) {
		e_print("Container() got NULL");
		debug_track_pointer();
	}
	return GTK_CONTAINER(w);
}

/*!
	PRIVATE IMPLEMENTATION: add is the container version of pack

	Derived classes will probably do something more interesting
*/
void w_container_::add( GtkWidget * sub)
{
	// action depends on THIS type (vbox, bin, ... )
	gtk_container_add( Container(), sub );
}

/*!
	The subclass holds one or a list of widgets. When deleting one,
	this function comes in useful, as common code.

	Note that sub_var is a var, possibly fround from a search.
	This code clears that var. (caller would then
	delete slot from list).

		gtk_container_remove( parent, child );
*/
void w_container_::remove_sub( obj_hold<w_widget0> & sub_var )
{
	/*
		Allow the parent object to call this,
		even when there is no sub (eg new objects never filled,
		but remove_all() clears every slot)
	*/
	if(!sub_var)
	{
		DBG0
		{
			e_print( "remove_sub(NULL) (USUALLY OK) from ");
			debug_track_pointer();
		}
		return;
	}

	/*
		The C++ sub-object is deleted AFTER the gtk widget,
		so keep a ref alive.

			BUG in copy constructor - doesnt clear old_ptr to NULL
			obj_hold<w_widget0> sub; sub = sub_var;
			so double step the constructor, assignment
	*/
	obj_hold<w_widget0> sub;
	sub = sub_var;

	/*
		splice NULL into the holder to prevent any recursive action
	*/
	sub_var = NULL;


	/*
		sub exists, but what about its GObject widget?

		There are a few objects that do late_init,
		and might still be NULL.

		Occasionally a widget can be cleared down, empty,
		but I'd rather that was done by the holder, (which
		would use this function).

		Try to detect errors where some destroy() callback
		"helpfully" sets w to NULL, but hasnt removed it
		from the container.

		Nested containers might already have their (own)
		'w' s taken away ...
	*/
	if(!sub->w)
	{
		/*
			log this situation
		*/
		e_print( "remove_sub()          PARENT ");
		debug_track_pointer();
		e_print( "remove_sub() NULL w in CHILD ");
		sub->debug_track_pointer();

		/*
			fake the same sequence as normal dismantle
			(this never happens, so it is correct)
		*/
		sub->call_destroy();
		sub = NULL; // del child c++
		/*
			go no further
		*/
		return;
	}

	if(!w)
	{
	/*
		this (subs parent) should NOT have a NULL w
		Without it we cannot call remove( sub->w )

		THIS HAPPENS A LOT --
			seems that some widgets (container/notebook)
			arent deleting themselves, but are catching
			destroy AFTER gtk has deleted the parent widget

		IT SHOULD HAPPEN -
			gtk destroys its widget then calls event_destroyed()
			and entire subtree's of containers all go, so this
			gets called with null w,
	*/
		 if(0)
		 {
			/*
				log this situation
			*/
			e_print( "remove_sub() NULL w in PARENT ");
			debug_track_pointer();
			e_print( "remove_sub()            CHILD ");
			sub->debug_track_pointer();

			/*
				breakpoint IF gdb is active
			*/
			gdb_break_point();
		 }

		/*
			do some damage limitation ?
		sub->w-set_NULL();
			OR NOT BECAUSE call_destroy() needs to do this
		*/
		/*
			gtk calls event_destroyed() after w is invalid,
			so try doing the same from here
		*/
		// sub->event_destroyed();
		STEP("calling sub->call_destroy()");
		sub->call_destroy();
		sub = NULL; // del child c++
		/*
			go no further
		*/
		return;
	}

	/*
		normal case -- sub exists and has a w
	*/
	if(1)
	{
		/*
			print something - normal case
		*/
		e_print( "remove_sub() PARENT ");
		debug_track_pointer();
		e_print( "remove_sub() CHILD  ");
		sub->debug_track_pointer();
		if(0) e_print("TYPE %s/%s\n",
			G_OBJECT_TYPE_NAME( w ),
			G_OBJECT_TYPE_NAME( sub->w )
		);
	}

	/*
		remove sub->w from this w

		Child widgets of sub->w should disappear from the gtk side,
		probably calling event_destroy() to tell C++.

		Each would dismantle() an already dismantled tree

		sub->outer_packable()  is usually sub->w
	*/
	GtkWidget * WID = sub->outer_packable();
	if(WID!=sub->w) {
		WARN("sub->outer_packable() != sub->w %p %p",
		WID, sub->w );
	}
	remove( WID ); // gtk_container_remove
	// sub->w = NULL; // see comment

	/*
		take sub->w away from sub

		this really should be done by w_widget
		receiving the xxxxx signal from gtk
		particularly when w!=outer_packable
	*/

	sub = NULL;	// delete c++ wrapper 
}

/*!
	PRIVATE IMPLEMENTATION:

	This is 'gtk_container_remove' which most containers use
*/
void w_container_::remove( GtkWidget * sub )
{
	if( ! sub ) {
		e_print("remove(NULL) from ");
		debug_track_pointer();
		return;
	}
	if( !w ) {
		e_print("remove(sub) but w == NULL ");
		debug_track_pointer();
		return;
	}
	/*
		removing sub from this-w
		will delete children of w on the gtk side
	*/
	gtk_container_remove( Container(), sub );
	if(1) e_print("removed\n");
}

/*!
	surrounding gap
*/
int w_container_::get_border_width()
{
#ifdef WITH_GTK2
	return gtk_container_get_border_width( Container() );
#else
	return get_int_prop( "border_width" );
#endif
}

/*!
	plain wrap for gtk_container
*/
void w_container_::set_border_width(int wide)
{
	gtk_container_set_border_width( Container(), wide );
}

/*!
	Derived classes should implement this, to remove each
	child widget. If w is valid call gtk_remove, if not
	call event_destroyed on each retained.

	This might cause the sub-tree to be deleted, then
	event_destroyed() to be called on deleted things.

	Its also called by destructor - but that doesnt make sense, see it.
*/
void w_container_::remove_all()
{
	FAIL("child class should implement this");
}

/*!
	Sub-classes can use this, and implement remove_all

	I think this happens when GTK has already deleted this
	container (w), and feels like telling C++
*/
void w_container_::event_destroyed()
{	
	remove_all();
}

/////////////////////////////////////////////////////////////////////

/*!
	destructor
*/
w_container_1::~w_container_1()
{
	remove_all();
}

/*!
	constructor - default plain - need an actual subclass to set w
*/
w_container_1::w_container_1()
{
}

/*!
	Pack the single child into this 'bin' container
*/
void w_container_1::pack_solo( w_widget0 * sub )
{
	if(!sub) { FAIL("NULL sub"); return; }
	// the gtk widget should not outlive the w_widget0 that holds it
	// so add then packed - ie the OLD thing that _WAS_ packed is deleted
	add( sub->w );		// gtk_container_add - not much more
	packed( sub );		// remove_all() then set var first?
	sub->show();		// pack implies show (for me)
}

/*!
	Caller has already used some other function to pack and show,
	but needs to set sub_kept properly
*/
void w_container_1::packed( w_widget0 * sub )
{
	remove_all();
	sub_kept = sub;
}

/*!
	clear down the single child held in the sub_kept var
*/
void w_container_1::remove_all()
{
	if( sub_kept ) remove_sub( sub_kept );
	return;
}

/*!
	return the child widget
*/
w_widget0 * w_container_1::get_sub_kept()
{
	return sub_kept;
}

/////////////////////////////////////////////////////////////////////

/*!
	destructor - delete all children in list
*/
w_container_N::~w_container_N()
{
	remove_all();
}

/*!
	constructor - subclass needs to allocate a w frame
*/
w_container_N::w_container_N()
{
}

/*!
	derived class did the packing,
	but wants this base to add it to the list
*/
void w_container_N::packed( w_widget0 * sub )
{
	keeper.append( sub );
}

/*!
	The w object defines how add() works, so calling
	this should do it all
*/
void w_container_N::pack( w_widget0 * sub )
{
	add( sub->w );		// gtk_container_add
	sub->show();
	packed( sub );
}

/*!
	search for widget in list and remove it, with GTK removal too
*/
void w_container_N::remove( w_widget0 * sub )
{
	// nothing to do
	if(!sub) return;

	// search for item
	int pos = 0;
	if(! keeper.find_ptr( sub, pos ) )
	{
		e_print("w_container_N::remove(w0) -- not found\n");
		e_print("PARENT ");
		debug_track_pointer();
		e_print("CHILD  ");
		sub->debug_track_pointer();
		gdb_break_point();
		return;
	}

	// remove it
	remove_sub( keeper[pos] );
}

/*!
	call remove_sub() on each item in list (which becomes NULL)
*/
void w_container_N::remove_all()
{
	int n = keeper.N();
	if(0) {
		e_print("remove_all() %d from ", n );
		debug_track_pointer();
	}
	for( int i = n-1; i>=0; i-- )
	{
		// remove(  keeper[i] );
		remove_sub(  keeper[i] );
	}
	keeper.clear();
}

