
#include "fd_selectable.h"

// #include <stdio.h>
// #include <memory.h>
#include <errno.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <unistd.h> // close
// #include <stdlib.h> // atoi
// #include <stdarg.h> // va_list
// #include <fcntl.h> // O_NONBLOCK
// #include <sys/ioctl.h>

// #include "fd_hold.h"
// #include <glib.h> 

#include "obj_hold.h"
#include "dgb.h"
#include "buffer2.h"
#include "buffer3.h"

// currently OK
#define IFNOK if(0)


// virtual
str2 * fd_selectable_base::debug_name_get()
{
	buffer1 tn;
	if( debug_name) return debug_name;
	const char * nm = debug_get_obj_name();
	if(!nm) {
		nm = type_name(tn);
	}
	if(!nm) return NULL;
	debug_name = new str2(nm);
	return debug_name;
}

// #define DEBUG_SHOW_NAME(tht) INFO("FD %d # '%s'", tht->fd_get(), (STR0) (str0) tht->debug_name_get() )

#define DEBUG_SHOW_NAME(tht) INFO("FD %d # '%s'", tht->fd_get(), (STR0) (str0) tht->debug_get_obj_name() )

// virtual
void fd_selectable_base::debug_name_set( str2 * name )
{
	debug_name = name;
	// DUPLICATION - for now!
	debug_set_obj_name( (STR0) name );
}

// PLAIN
void fd_selectable_base::debug_name_set( const char * name )
{
	debug_name_set( new str2(name) );
}

bool fd_selectable_base:: gdb_pre_select_says_something() // gdb_trap point
{
	// STEP("HERE");
	return true;
}


/*
	subscribe() is the hook to glib select(). Other systems would
	do something similar. glib has issues, take your pick.

	The "bool" returned by select_says_read(), tells glib to
	continue with that callback. false cancels that.

	select_says_write() is usually called first, indicating that the
	(async open) fd is properly open. Normal response cancels subscription.
	If you write beyond limits, write() will return less than requested,
	(the wrapped write ... does what ... ) you will have to recover and
	re-subscribe.  

	There is still a design bug, where three callbacks are registered
	one deleted, a shutdown called, but the two remain. This has
	been fudged so that the CPU usage is not 100%, but ... subscribe_each()
	Maybe use a dynamic object to hold the callback.

	Maybe also wait_more() wait_less() counters.
*/

// virtual
/*!
	destructor - cancel select's subscription and close fd
*/
fd_selectable_base::~fd_selectable_base()
{
	unsubscribe();
}

/*!
	constructor - from existing fd, or default none
*/
fd_selectable_base::fd_selectable_base( int _fd )
: fd_hold_1( _fd )
{
	init_base_zero();
}

/*!
	C++ initialises zero, this does the same. Presume random
*/
bool fd_selectable_base::init_base_zero()
{
	channel = NULL;
	sel_source1 = 0;	// glib uses 1,2,... not 0
	eof_seen = false;
	seen_connected = false;

#if 1
	// initially (prepare to) subscribe to everything
	// then CAREFULLY track every return value!
	fix1_conditions_previous = (GIOCondition) ( 0 );
	fix1_conditions = (GIOCondition) ( 0
		| G_IO_IN
		| G_IO_OUT
		| G_IO_PRI
		| G_IO_ERR
		| G_IO_HUP
		| G_IO_NVAL
	);
#endif
	return true;
}

/*!
	errors generate a synthetic EOF

	Because it is synthetic, Its not clear if read/write can be done!
	It might simply be that the app is a bit unhappy, and wants to go home.

	The theory, is that if write() fails, it prints a messages,
	so it is not wrong to call it.

	So attempt a progressive shutdown, with sensible half stages.
*/
void fd_selectable_base::eof_synthetic()
{
	if( eof_seen ) return;
	INFO("FD %d '%s' # eof_synthetic()", fd_get(), debug_get_obj_name() );
	fix1_conditions = (GIOCondition) 0; // what about half open?
//	unsubscribe();
	call_eof_input(); // does a flush
	//
	// call_close() -- doesnt exist
	//
	gdb_pre_select_says_something(); // gdb_trap point
	select_says_eof();
	close();
}

// virtual
void fd_selectable_base::zero_read_means_eof()
{
	STEP("This is the genuine eof_input condition");
	gdb_pre_select_says_something(); // gdb_trap point
	call_eof_input();
}

/*!
	The first EOF, real or synthetic
*/
void fd_selectable_base::call_eof_input()
{
	if( eof_seen ) return;
	// we only have eof INPUT not output (calling close if overreaction)
	eof_seen = true;
	seen_connected = true; // puts into a known state ?
	subscribe_IN_off();
	flush();
	gdb_pre_select_says_something(); // gdb_trap point
	select_says_eof_input();
	flush();
	//
	// do not close - only input has closed
}

// virtual
/*!
	EOF after call_eof_input reported

	The input has seen EOF, or an over clever piece of something
	has faked that. If you want to drain output, write a class,
	with a replacement for this close call
*/
void fd_selectable_base::select_says_eof_input()
{
	INFO("FD %d '%s' ## -EOF- ##", fd_get(), debug_get_obj_name() );
	eof_synthetic();
}

// virtual
/*!
	EOF after call_eof_input reported
*/
void fd_selectable_base::select_says_eof()
{
	INFO("FD %d '%s' ## -EOF- ##", fd_get(), debug_get_obj_name() );
	eof_synthetic();
}

/*!
	This should not be required, as subscribe_each() does more

	However, there is a TODO: deselect and reselect on write

	We are not ALWAYS hungry for read - dont forget XOFF

void fd_selectable_base::ask_select_for_write()
{
	subscribe_on( G_IO_OUT );
	// wrong // subscribe( G_IO_OUT );
}
*/

bool fd_selectable_base::subscribe_set( GIOCondition condition, bool on )
{
	if( on )
	{
		return subscribe_on( condition );
	} else {
		return subscribe_off( condition );
	}
}

/*!
	now that we have a centralised group-watch
	we can test for individual bit conditions, globally

	Prior to fix1, IN OUT OTHERS had 3 watches,
	so that IN and OUT could switch themselves off.

	With other POSIX select/poll mechanisms, the bitset is shared
*/
bool fd_selectable_base::is_subscribed( GIOCondition cond )
{
	return fix1_conditions & cond;
}

/*!
	you can call this, even when already subscribed (single cond only)
*/
bool fd_selectable_base::subscribe_on( GIOCondition condition )
{
	if( !is_subscribed( condition )) {
		resubscribe_fix1_conditions_plus( condition );
	}
	return true;
}

/*!
*/
bool fd_selectable_base::subscribe_off( GIOCondition condition )
{
	if( is_subscribed( condition )) {
		resubscribe_fix1_conditions_less( condition );
	}
	return false;
}


/*!
	Maybe I should add some vars for WHY connection failed
	Maybe they are still in errno

	What about select_says_error ...
*/
//	VIRTUAL
void fd_selectable_base::select_says_connect_failed(err_int_t & err)
{
	// should have been replaced
	WARN("FD %d '%s' # BASE CLASS FUNC;\n", fd_get(), debug_get_obj_name() );
	err.FAIL_REPORT(); // currently donw before call - might be silent though
	// eof_synthetic(); // done after return
}

/*!
	fd is readable for the first time - subclass may do something
	man 7 socket
*/
//	VIRTUAL
void fd_selectable_base::select_says_connected()
{
	// should have been replaced
	WARN("FD %d '%s' # select_says_connected();\n", fd_get(), debug_get_obj_name() );
}

/*!
	fd is readable - subclass must do something
*/
//	virtual bool fd_selectable_base::select_says_read() = 0;

// virtual
/*!
	fd is writable - default cancel write subscription
*/
bool fd_selectable_base::select_says_write()
{
	return subscribe_OUT_off();
}
// virtual
/*!
	...
*/
bool fd_selectable_base::select_says_PRI()
{
	return false; // no more of these thanks
}

bool fd_selectable_base:: call_select_says_error(err_int_t & err)
{
	if(err.no_error()) {
		return FAIL("ERROR - WHAT ERROR");
	}
	if(err.clear_if_EINPROGRESS()) {
		if(1) INFO("EINPROGRESS happens");
		return true;  // stay subscribed
	}


	// user defined can do what it likes ...
	FAIL("FD %d '%s' calling select_says_error(%s)",
		fd_get(),
		debug_get_obj_name(),
		(STR0) err.e_str );
	err.FAIL_REPORT();
	gdb_pre_select_says_something(); // gdb_trap point
	select_says_error(err);
	eof_synthetic(); // repeat call for safety
	return false;
}
// virtual
/*!
	...
*/
bool fd_selectable_base::select_says_error(err_int_t & err)
{
	errno_zero();
	eof_synthetic();
	errno_zero();
	return false;
}

/*!
	The default initial subscription

	glib can only cancel individual watch'es so write is separate
	from read (write is the default socket sucessfully opened sign)

	REWORK: in future subscribe to everything,
	then delete the OUT/IN that you dont want.
	You really should handle those.

	This comes as part of fix1, being a better way of doing things anyway!
	The naff part is the watch, not being unique part of the fd-channel-obj
*/
void fd_selectable_base::subscribe_each( bool sub_in, bool sub_out )
{
	errno_zero();
	INFO("FD %d '%s' # HERE2", fd, debug_get_obj_name() );
	if(sel_source1) {
		// SO subscribe_each() should only ever be called ONCE
		FAIL("sel_source1 is already non-zero");
		return;
	}
	if( !is_open() ) return;
	if(1) {
		// INITIALISE fix1 first subscription
		// this should work
		resubscribe_fix1_conditions_plus(fix1_conditions);
	} else {
	if(1)	subscribe(
		  G_IO_PRI
		| G_IO_ERR
		| G_IO_HUP 
		| G_IO_NVAL );
	if(1)	if( sub_out ) subscribe( G_IO_OUT );
		if( sub_in  ) subscribe( G_IO_IN );
		STEP("G_IO_OUT can cause errno 11 Resource temporarily unavailable");
	}

	errno_zero();
}

const char * str_of_G_IO_X( int conditions )
{
	if( conditions == G_IO_IN ) return "G_IO_IN";
	if( conditions == G_IO_OUT ) return "G_IO_OUT";
	if( conditions == G_IO_PRI ) return "G_IO_PRI";
	if( conditions == G_IO_ERR ) return "G_IO_ERR";
	if( conditions == G_IO_HUP ) return "G_IO_HUP"; 
	if( conditions == G_IO_NVAL ) return "G_IO_NVAL";
	int ALL4= 
	  G_IO_PRI
	| G_IO_ERR
	| G_IO_HUP 
	| G_IO_NVAL;
	if( conditions == ALL4 ) return "G_IO_{ PRI ERR HUP NVAL }";
	static buffer2 buf;
	buf.clear();
	buf.print("(%2X){ ", conditions);
	if( conditions & G_IO_IN )  buf.print("%s ", "G_IO_IN");
	if( conditions & G_IO_OUT ) buf.print("%s ", "G_IO_OUT");
	if( conditions & G_IO_PRI ) buf.print("%s ", "G_IO_PRI");
	if( conditions & G_IO_ERR ) buf.print("%s ", "G_IO_ERR");
	if( conditions & G_IO_HUP ) buf.print("%s ", "G_IO_HUP"); 
	if( conditions & G_IO_NVAL ) buf.print("%s ", "G_IO_NVAL");
	buf.print("}");
	
	return (STR0) buf;
}

const char * str_GIOStatus( GIOStatus s )
{
        switch( s ) {
         case G_IO_STATUS_ERROR:  return "G_IO_STATUS_ERROR";
         case G_IO_STATUS_NORMAL: return "G_IO_STATUS_NORMAL";
         case G_IO_STATUS_EOF:   return "G_IO_STATUS_EOF";
         case G_IO_STATUS_AGAIN:  return "G_IO_STATUS_AGAIN";
         default: return "G_IO_STATUS__XXX_";
        }
}

const char * str_GIOCond( GIOCondition cond )
{
        switch( cond ) {
         case G_IO_IN:  return "IN";
         case G_IO_OUT: return "OUT";
         case G_IO_PRI: return "PRI";
         case G_IO_ERR: return "ERR";
         case G_IO_HUP: return "HUP";
         case G_IO_NVAL: return "NVAL";
        default:
         // double loop only really catches unknown single bits
         // alternative is two functions, first calls second (doesnt call first)
         // alternative is to cater for mutiple/single bits (notice, count them)
         static bool double_loop = false;
         if(double_loop) return "G_IO_double_loop";
         double_loop = true;
         int c_rest = cond;
         int c_bit = 1;
         while( c_rest ) {
          if( cond & c_bit ) {
                INFO( "MULTIBIT %s", str_GIOCond( (GIOCondition) c_bit ));
          }
          c_bit <<= 1;
          c_rest >>= 1;
         }
         double_loop = false;
         return "G_IO_xxx";
        }
}

const char * str_GIOCondition( GIOCondition cond )
{
	return str_GIOCond( cond );

        switch( cond ) {
         case G_IO_IN:  return "G_IO_IN";
         case G_IO_OUT: return "G_IO_OUT";
         case G_IO_PRI: return "G_IO_PRI";
         case G_IO_ERR: return "G_IO_ERR";
         case G_IO_HUP: return "G_IO_HUP";
         case G_IO_NVAL: return "G_IO_NVAL";
        default:
         // double loop only really catches unknown single bits
         // alternative is two functions, first calls second (doesnt call first)
         // alternative is to cater for mutiple/single bits (notice, count them)
         static bool double_loop = false;
         if(double_loop) return "G_IO_double_loop";
         double_loop = true;
         int c_rest = cond;
         int c_bit = 1;
         while( c_rest ) {
          if( cond & c_bit ) {
                INFO( "MULTIBIT %s", str_GIOCondition( (GIOCondition) c_bit ));
          }
          c_bit <<= 1;
          c_rest >>= 1;
         }
         double_loop = false;
         return "G_IO_xxx";
        }
}

extern "C" void g_io_channel_win32_set_debug (GIOChannel *channel, gboolean flag);

/*!
	Vector into glib

	On UNIX you can have multiple watch subscriptions.
	Each can be cancelled separately.

	On WIN32 you can only have one (that works),
	so instead of creating lots of new subscriptions,
	this shares one watch, but you can still call this.

	Create these as a watch, or _ADD_ them to the existing watch.
*/
void fd_selectable_base::subscribe(
	int conditions
)
{
	if(sel_source1) {
		FAIL("sel_source1 should be zero - or maybe allow for when fixed");
		desubscribe_fix1();
	} else {
	 if( channel ) {
		// because of _fix1_ channel is not NULL
		// when _fix1_ is properly fixed, channel should be null
	 	if(0) FAIL("FD %d %s channel should be NULL", fd, debug_get_obj_name() );
	 } else {
		INFO("FD %d %s creating channel OK", fd, debug_get_obj_name() );

		// IT IS NOT MY CHOICE TO MAKE HERE
		// JUST BECAUSE I MAKE THIS CHOICE EVERYWHERE
		// maybe write a test check - check_non_block
		// set_non_block(); // if not already ?
		check_non_block(); // if not already ?

		// lets create the first (and only) watch

#ifdef WIN32
		channel = g_io_channel_win32_new_socket( fd_get() );
#	warning "presumed socket and not file"

		// g_io_channel_win32_new_fd()
		// g_io_channel_win32_new_socket()
		if(0) {
			STEP("switching channel debug on");
			g_io_channel_win32_set_debug( channel, true );
		}
#else
		channel = g_io_channel_unix_new( fd_get() );
#endif
#	warning "channel probably not dereferenced properly, it holds ref to this?"
		if(!channel) {
			FAIL("FD %d %s new channel is NULL", fd, debug_get_obj_name() );
			return;
		}
		g_io_channel_ref( channel );
		g_io_channel_set_close_on_unref( channel, true );
		ref_incr(); // per channel that refers back to this ... maybe
		STEP("after ref_incr() becuase glib hosts it" );
		debug_track_pointer();
	 }
	}

	const char * cond_str = str_of_G_IO_X( conditions );

	IFNOK INFO(
		"FD %d '%s' # select_subscribe(%x)\n",
		fd_get(),
		debug_get_obj_name(),
		conditions
	);

	gpointer user_data = this; 
	int id = g_io_add_watch_full(
		channel,
		G_PRIORITY_DEFAULT,
		(GIOCondition) conditions,
		C_select_says,
		user_data,
		C_select_removal
	);
	// ref_incr(); // per id
	INFO("FD %d '%s' # watch %2.2X %s -> %d", fd_get(), debug_get_obj_name(), conditions, cond_str, id );
	sel_source1 = id;
	errno_zero();
}

/*!
	Cancel all subscriptions - buggy whilst glib API learning curve
*/
void fd_selectable_base::unsubscribe()
{

	// this should  only remove the watch not the channel
	desubscribe_fix1();
	// now continue as if fix1 wasnt an issue

	int t = 0;
	if(!channel) return;
	GIOChannel* t_channel = channel;
	channel = NULL;
	gint t_sel_source1 = sel_source1;
	sel_source1 = 0;

	int fd_old = fd_get(); // debug
	const char * NM = debug_get_obj_name();

	// remove what is easier to find
	if( t_sel_source1 ) {
		t = g_source_remove( t_sel_source1 );
		INFO("FD %d '%s' # unsubscribe() - g_source_remove %d %d", fd_old, NM, t_sel_source1, t );
	}

	// remove all other watches registered with *this*
	// obviously other classes dont do their own watches with this
	while( g_source_remove_by_user_data( this ) )
		INFO("FD %d '%s' # Removed source by user data", fd_old, NM );
	/*
		g_source_remove causes a re-entrant call to C_select_removal
	*/
 if(0)
 if(0) {
	g_io_channel_close( t_channel );
	INFO("FD %d '%s' # unsubscribe() - close ", fd_old, NM );
 }
 if(0) {
	t = g_io_channel_shutdown( t_channel, false, NULL  );
	INFO("FD %d '%s' # unsubscribe() - shutdown %d \n", fd_old, NM, t);
 }
 if(1) {
	INFO("FD %d '%s' # unsubscribe() - unref I AM NOT SURE WHAT THE REF COUNT WAS", fd_old, NM );
	INFO("FD %d '%s' # unsubscribe() - unref", fd_old, NM );
	g_io_channel_unref( t_channel );
 }
	debug_track_pointer();
	FAIL("FD %d '%s' # unsubscribe() - why the ref_incr ??????????????", fd_old, NM );
		STEP("before ref_decr() becuase glib hosts it" );
		debug_track_pointer();
		// not sure what happens if zero - stack shoudl hold this?
	ref_decr();
	INFO("FD %d '%s' # unsubscribe()d", fd_old, NM );
	errno_zero();
}

// static

/*!
	glib - unsubscribe has cancelled a specific watch

	The GLIB watch has to effectively hold a virtual refcount on obj,
	because it has a pointer to obj. So when creating a watch,
	add a ref count, when deleting the watch, decref()

	The problem then, is that I want to use ref_counting
	for something useful, not just malloc.

	So: DO NOT SUBSCRIBE and IGNORE OLD POINTER

	Of course this doesnt apply when fix1 is re-subscribing a lot
	only when the program is exiting
*/
void fd_selectable_base::C_select_removal( gpointer data )
{
	INFO("FD -- obj_name '--' # obj == %p might already not be a valid ptr",
		data );
	data = NULL; // its already invalid
	return; // -------------- //

	fd_selectable_base * obj = (fd_selectable_base *) data;
	DEBUG_SHOW_NAME(obj);
	INFO("FD %d '%s' # obj == %p",
		obj->fd_get(),
		obj->debug_get_obj_name(),	
		obj );

	// _fix1 might make this a commmon call

	/*
		cannot determine which 'id' it was unless 'obj'
		is dymanically allocated for each ...
	*/
	// obj->unsubscribe(); // already done ??
	// wait_less()
	// active_count--;
	// if( active_count <= 0 )
	// 	g_main_quit( main_loop );
	// obj->ref_decr();
}


/*!
	_fix1_ combines multiples conditions into _ONE_ watch

	before adding anything the whole thing must be cancelled.
	Initially there is nothing to cancel.
*/
bool fd_selectable_base::desubscribe_fix1()
{
	if( !sel_source1 ) {
		if(!channel) {
			if(0) INFO("!sel_source1 and !channel - first use? OK!");
			return true;
		}
		WARN("CODE ERROR !sel_source1 and channel");
		return false; // NOT OK
	}
	if(0) STEP("calling g_source_remove() (ALL)");
	bool found = g_source_remove( sel_source1 );
	sel_source1 = 0;
	if(!found) {
		FAIL("sel_source1 not found");
		return false;
	}
	return true; // OK
}

bool fd_selectable_base::resubscribe_fix1_conditions()
{
	if( fix1_conditions == fix1_conditions_previous ) {
		INFO("no change %x", fix1_conditions );
		return true;
	}
	desubscribe_fix1();
	if(!is_open()) { // or at least fd is not forgotten and set to -1
		return true;
	}
	subscribe( fix1_conditions );
	fix1_conditions_previous = fix1_conditions;
	return true;
}

bool fd_selectable_base::resubscribe_fix1_conditions_plus( GIOCondition cond )
{
	INFO("FD %d %s - %s",
		fd_get(),
		debug_get_obj_name(),	
		str_GIOCond( cond )
	);
	fix1_conditions = (GIOCondition) ( fix1_conditions | cond );
	if(!resubscribe_fix1_conditions()) {
		// already reported
	}
	return true; // to indicate OK - which ?
	return false; // to cancel previous subscription
}

bool fd_selectable_base::resubscribe_fix1_conditions_less( GIOCondition cond )
{
	INFO("FD %d %s - %s",
		fd_get(),
		debug_get_obj_name(),	
		str_GIOCond( cond )
	);
	fix1_conditions = (GIOCondition) ( fix1_conditions & ~cond );
	if(!is_open()) return false;
	if(!resubscribe_fix1_conditions()) {
		// already reported
	}
	return false; // to cancel previous subscription
	return true; // to indicate OK - which ?
}


/*!
	This is the _fix1_ first function that glib calls
	
	It isolates single bits of condition,
	and calls C_select_says_fix1_second() - which is original
	
	Without the _fix1_ malarky you would just get glib to call _second,
	but I have not renamed it yet back to the original
*/
gboolean fd_selectable_base::C_select_says(
	GIOChannel *source,
	GIOCondition condition,
	gpointer data
) {
	return C_select_says_fix1_first( source, condition, data );
}
/*!
	glib - shared callback - some work expected TODO here
	
	Return true to get more callbacks like this, falce to cancel subscription
	
	Try to do the most common first
	
	AAAGH: WIN32 only tracks the last most watch added to a channel,
	so one watch must watch everything then de-resubscribe
	see bug 338943 (at least thats my workaround)
	
	HMMM: with _fix1_ maybe write C_select_says_2 which is unchanged
	but only receives ONE condition, and call it from C_select_says_1
	which does the _fix1_ adjustment, and also the decode condition?
	(at least isolate individual bits).

	LURK: WATCH OUT FOR MULTI CONDITIONS, esp when that is itself
	significant (?eg IN + ERR ?)
*/
gboolean fd_selectable_base::C_select_says_fix1_first(
	GIOChannel *source,
	GIOCondition condition,
	gpointer data
)
{
	/*
		Iterate through a list of bits, calling fix1_second

		unfortunately, the order of these must match _fix1_second
		in so much as - it expects that sort of control

		beware of needing multi-bit combinations
	*/
	GIOCondition cond_bit[] = {
		G_IO_PRI,
		G_IO_ERR,
		G_IO_HUP,
		G_IO_NVAL,

		G_IO_IN,
		G_IO_OUT
	};
	int n_cond_bit = N_ARY( cond_bit );

	obj_hold<fd_selectable_base> obj = (fd_selectable_base *) data;
	if(!obj) {
		FAIL("FD -- # obj is NULL" );
		return false; // desubscribe watch
	}

	/*
		errno might be residual from some previous activity
		(equally err_int where it has been noticed but not handled)

		I think that these are almost always discardable, but ..

	*/
	if(0) gdb_invoke(1);
	err_int_t & err2 = BASE1::get_err_int_for_this_thread();
	if( err2.is_error()) {
		
		WARN("PRE - Clearing unresolved error withing libs_apps err_int system!");
		err2.clear();
	}
	err2.eat_OS_error(); // so that I can do reporting, without tripping up
	err2.clear_if_EAGAIN(); // X11 screen update complete - no more messages
	if( err2.is_error()) {
		STEP("PRE - Clearing unresolved OS_errno from GTK or something");
		err2.clear();
	}

	/*
		if you remove ONE condition, you will need to resub the others
		if false is never returned, keep as it, all,
		any_change return false
	*/
	bool any_change = false;
	// NB obj->handler might subscribe to something more
	// so check for that in the source somewhere
	// NB having cancelled _THIS_ watch, return what exactly ?

	// NEWS I AM BORED WITH THIS FALSE MEANS DONT CALL AGAIN
	// MOVING TO subscribe_IN_on() subscribe_OUT_off()
	// The difference is the use of a bitmap of conditions per fd
	// That is not a bad idea anyway

	// DEBUG - print bits out
	buffer2 list;
	int n_found = 0;
	for(int i=0; i< n_cond_bit; i++) {
		GIOCondition cond = cond_bit[i];
		if(!( condition & cond_bit[i] )) continue;
		n_found ++;
		list.print(" %s", (STR0) str_GIOCondition( cond ));
	}
	INFO("fd %d %s errno %d nbits %d %s",
		obj->fd,
		obj->debug_get_obj_name(),
		errno,
		n_found,
		(STR0) list
	);
	// 2 HUP OUT - when no service listening

	for(int i=0; i< n_cond_bit; i++) {
		GIOCondition cond = cond_bit[i];
		if(!( condition & cond_bit[i] )) continue;
		bool again = C_select_says_fix1_second( err2, source, cond, data );

		// clear any errors I have caused
		// err2 is still the global err_int
		if( err2.is_error() ) {
			WARN("%s - Clearing error I have already  detected",
				(STR0) str_GIOCondition( cond ));
			err2.clear();
		}
		if(err2.get_OS_error()) {
		 WARN("%s - Detecting error I had not noticed",
				(STR0) str_GIOCondition( cond ));
		 if(err2.clear_if_read_dry()) {
			WARN("Detecting error I had not noticed");
			; // EAGAIN - Resource Temp Unavailable // left by prev
		 }
		 if(err2.clear_if_EINPROGRESS()){ // CONNECT ...
			// return true; 
		 }
		}

		if(!again) {
			any_change = true; // do not resubscribe to this cond
			obj->subscribe_off( cond );
		//	obj->resubscribe_fix1_conditions_less( cond );
			//
			// option to truncate activity - no
			// if(!obj->is_open()) break;
	 	}
	}
	if( !obj->is_open() ) {
		if( !any_change ) {
			// CODE ERROR DETECT
			any_change = true;
			FAIL("FD %d %s # NOT is_open() but didnt notice any_change",
				obj->fd,
				obj->debug_get_obj_name()
			);
		}
	}
	/*
		Do not leave any residual error indicators ...
	*/
	if(err2.is_error()){ // so that I can do reporting, without tripping up
		WARN("IO loop clearing detected but uncleared error");
		err2.clear();
	}
	err2.eat_OS_error(); // so that I can do reporting, without tripping up
	if(err2.is_error()){ // so that I can do reporting, without tripping up
		WARN("IO loop detecting and clearing error");
		err2.clear();
	}
	if( any_change ) {
		obj->resubscribe_fix1_conditions(); 
		return false; // if watch not already cleared, cancel subscription
	} else {
		return true; // stay subscribed
	}
}

#warning unistd to NOT be included here
#include <unistd.h>

/*!
	Without the _fix1_ malarky you would just get glib to call _second,

	ie _second is the original unchanged (maybe) but _first
	is a wrapper around it, needed on WIN32, used on UNIX for similarity

	SO: fix1 should not be mentioned after the title

*/
gboolean fd_selectable_base::C_select_says_fix1_second(
	err_int_t & err1, // errno as found
	GIOChannel *source,
	GIOCondition condition,
	gpointer data
) {
	bool NZ = false; // noise
	/*
		EOF can cause obj to self destruct
		but that cant happen before unsubscribe completes
	*/
	obj_hold<fd_selectable_base> obj = (fd_selectable_base *) data;

	if(!obj) {
		FAIL("FD -- # obj is NULL" );
		return false; // desubscribe ALL
	}

	/*
		TODO - set 'FD %d' as obj one alias
	*/

	/*
		err1 = err2 = NET error code on socket

		nb err1 was not the NET error,
		it was leftover from another errno syscall

		this code is ony correct if err1.e_int is zero
		which happens because any previous errno is HANDLED!
	*/

	// err_int_t err2;
	err_int_t & err2 = BASE1::get_err_int_for_this_thread();;
	err2.get_NET_error_from_socket( *obj );
	if(!err2.is_error()) {
		err2.set_to_OS_error( err1.err );
	}

	if(0) DEBUG_SHOW_NAME(obj);
	const char * NM = obj->debug_get_obj_name();

	int fd = obj->fd_get();

	if(0) {
		const char * str = str_GIOCondition( condition );
		INFO("FD %d '%s' # condition %s", fd, NM, str );
	}

	if(obj->eof_seen) {
		const char * str = str_GIOCondition( condition );
		return FAIL("FD %d '%s' # eof_seen SKIPPING GIO_X '%s'", fd, NM, str );
	}

#if 0
#warning errno_zero() here must be wrong, really
	errno_zero();
#endif

	// IN + ERR + HUP +(Connection Refused)+

	if(condition & G_IO_PRI ) {
		// I dont currently do PRI
		FAIL("FD %d '%s' # C_select_says(PRI)", fd, NM );
		errno_zero();
		// gdb_pre_select_says_something(); // gdb_trap point
		// return obj->select_says_PRI();
		return false; // desubscribe
	}

//	GOT: { ERR + HUP + IN } when no route to PORT (service not running)


	if(condition & G_IO_ERR ) {
#ifdef WIN32
#else
	//	err.eat_OS_error();
	//	err.get_NET_error_from_socket(*obj);
	//	err.is_error();
	//	err.set_obj_one( obj );
		if( err2.clear_if_EINPROGRESS() )
		{
			INFO("FD %d '%s' # C_select_says(ERR) EINPROGRESS absorbed", fd, NM );
			return true;	// continue with further replies
		}
		if(err2.is_error()) {
			err2.FAIL_REPORT();
		} else {
		FAIL("WHAT IS THE NETWORK FAIL REASON DIAGNOSTIC");
		FAIL("WHAT IS THE NETWORK FAIL REASON DIAGNOSTIC");
		FAIL("WHAT IS THE NETWORK FAIL REASON DIAGNOSTIC");
		}
#endif
/*
	http://cr.yp.to/docs/connect.html
	suggests ... lots of things ...
	getpeername() - fails and then read(1)
	recv() - is buggy !!! (so it says)
	getsockopt(,,SO_ERROR,,),  -- looks best
*/
		if(!obj->seen_connected) {
			FAIL("FD %d '%s' calling select_says_connect_failed('%s')", fd, NM, (STR0) err2.e_str );
			obj->gdb_pre_select_says_something(); // gdb_trap point
			obj->select_says_connect_failed(err2);
			obj->eof_synthetic(); // repeat call for safety
			return false;
			// --STAY-- ??
		} else {
			return obj->call_select_says_error(err2);
		}
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR) BUT IT CONTINUES", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		INFO("FD %d '%s' # C_select_says(ERR)", fd, NM );
		return false;
	}

	if(condition & G_IO_HUP ) {
		INFO("FD %d '%s' # C_select_says(HUP)", fd, NM );
		errno_zero();
		obj->eof_synthetic();
		return false;
	}

	if(condition & G_IO_NVAL ) {
		INFO("FD %d '%s' # C_select_says(NVAL)", fd, NM );
		errno_zero();
		obj->eof_synthetic();
		return false;
	}

	if(condition & G_IO_IN ) {
		if(0) INFO("FD %d '%s' # C_select_says(IN)", fd, NM );
		/*
			seen_connected only matters to G_IO_OUT
			on both WIN32 and UNIX
			A plain client socket is connected when writable
			A normal server socket is acceptable when readable
			(but then call its usual select_says_read())
		*/
		if(!obj->seen_connected)
		{
			// SO WHAT
			// can infer is_server_listening_socket
			// but I dont need to do that
		}

		if(NZ) INFO("FD %d '%s' # calling select_says_read()", fd, NM );
		if((fd>=0)&&!obj->is_open()) {
			FALSE_dgb_fail("fd but not is_open");
			// fd is available to the function as a parameter
			// fd usually matches what is stored in the obj
			// this is sanity test
			// it proves that is_open was true BEFORE calling fn
			// nb when fn handles multiple events, it could close!
		}

		// this might be a bit too frequent for sensible debugging
		if(1) obj->gdb_pre_select_says_something(); // gdb_trap point
		bool again = obj->select_says_read();
		if(!obj->is_open()) {
			INFO("FD %d(now %d '%s' # not is_open () happens with listening socket",
				fd, obj->fd, NM );
			if(again) {
				FAIL("CODE error, select_says_read() should return false, after closing fd");
			}
			errno_zero();
			return false; // de subscribe
		}
// if((fd>=0)&&!obj->is_open()) FALSE_dgb_fail("fd but not is_open");
		bool again_bit = obj->is_subscribed(G_IO_IN);
		if( again != again_bit ) {
		 if(obj->is_open())
			FAIL("Code error G_IO_IN again = %d again_bit = %d", again, again_bit );
			again = again_bit;
		}
		errno_zero();
		// ALSO TODO: 
		// tell_select_am_hungry_for_read
		// tell_select_am_not_hungry_for_read // full
		// bool is_already_subscribed_read // or not
		// ditto write
		// always for errors etc
		/*
			an interesting case is where a server accepts an fd
			there is a nested C-select-says-...
			the socket server is readable
			the accepted fd is ... called by me ??
			... scheduled as 
		*/
		if(again)
			if(!obj->is_open() ) {
				FAIL("FD %d %s again but not is_open()", fd, NM );
			if(0)	if((fd>=0)&&!obj->is_open()) FALSE_dgb_fail("fd but not is_open");
				again = obj->subscribe_IN_off();
			}
				
		if(!again) {
			// this is unusual - so ...
			INFO("FD %d %s got FALSE from obj->select_says_read(); // UNUSUAL", fd, NM );
		} else {
			if(NZ) INFO("FD %d %s got TRUE from obj->select_says_read();", fd, NM );
		}
		return again; // can stay for multiple conditions
	}

	if(condition & G_IO_OUT ) {
		if( obj->eof_seen ) return FAIL("eof_seen found here");
		if(!obj->seen_connected)
		{
			if(1) INFO("FD %d '%s' # calling select_says_connected()", fd, NM );
			obj->seen_connected = true;
			obj->gdb_pre_select_says_something(); // gdb_trap point
			obj->select_says_connected();
			errno_zero();
			// stay to do first write, even if above did it anyway
		} else {
			if(NZ) INFO("FD %d '%s' # C_select_says(OUT)", fd, NM );
		}
		if(0) obj->gdb_pre_select_says_something(); // gdb_trap point
		bool more2 = obj->select_says_write();
		errno_zero();
		// it was true, for this to be called, but might have changed
		bool more_bit = obj->is_subscribed( G_IO_OUT );
		if( more2 != more_bit ) {
		 gdb_invoke(true); // DUNNO
		 INFO("FD %d %s more2 %d more_bit %d", fd, NM, (int) more2, (int) more_bit );
		 // more2 overrides more_bit
		 if( more_bit ) {
			FAIL("obj->select_says_write() says false but did not un-subscribe_OUT_off()!!");
			obj->subscribe_OUT_off();
		} else {
			FAIL("obj->select_says_write() says true but did not subscribe_OUT_on()!!");
			obj->subscribe_OUT_on();
		 }
		 more2 = more_bit;
		}
		return more2;
	}

	FAIL("no matched condition");
	errno_zero();
	return false;
}

/*!
	adjusts subscription to (this->fd) own output fd
*/
bool fd_selectable_base::write_buffer_to_fd( buffer3 & buff3 )
{
//	INFO("FD %d %s", fd_get(), debug_get_obj_name() );

//	bool caller_should_stay_write_subscribed = true;
//	bool caller_should_NOT_stay_write_subscribed = false;

	// FROM SSL-BUFFER to PLAIN-FD
	//
	// no need to tell buffer it is full, next select_says will notice
	// DO NEED to tell buffer we are hungry, OBJ->subscribe_IN_on()
	// OR need to ask buffer to ask someone to resume filling it
	// circ_buffer can do this, maybe
	// class buffer_watcher {
	// virtual bool buffer_is_empty_and_consumer_is_hungry()
	// virtual bool buffer_is_full_and_producer_is_overflowing()
	// virtual bool buffer_is_emptying()
	// virtual bool buffer_is_filling() // getting full
	// maybe use either as understatement for FULL EMPTY overflow
	// maybe expect extremes to already be known
	// NB circ_used() circ_added() can queue OBJ->CALLBACK or run now
	// because circ_check copy_down has the lock
	//}

	void * buf = (void*) buff3.circ_addr_rd();
	int max = buff3.circ_data_avail();
	// TODO:
	//
	// if( max > max_write_chunk_size ) max = max_write_chunk_size;

	if( max == 0 ) {
		// this is not streams - do not request zero to signal eof
		// if you want that, create a special function for it
		INFO("NO DATA TO WRITE");
		// careless caller gets let off hook, and subs
		dgb_sleep_more();
		return subscribe_OUT_off(); // unsubscribe write - all done
	}

	int len = sock_send( buf, max );
	if( len <= 0 ) {
	//	switch( errno ) {
	//	 case EINTR:
	//	 case WIN32_GetLastError_Would_Block:
	//	 this is why fd_hold needs to know more about strategy
	//	 we need errno for non-error return codes
	//	}
		FAIL("FD %d sock_send(%d) returned %d", fd_get(), max, len );
		WARN("Staying subscribed - presuming not called manually");
		dgb_sleep_more();
		return subscribe_OUT_off(); // unsubscribe write - ERROR?
		return false;
	}

	buff3.circ_used(len);
	INFO("FD %d %s WROTE %d bytes", fd_get(), debug_get_obj_name(), len );
	buff3.dgb_show( "write fd", "buff3" );
	if(errno) FAIL("errno");
	errno_zero();

	// relay->gw_link_handset->read_some();
	max = buff3.circ_data_avail();
	if( max ) {
		return subscribe_OUT_on(); // subscribe write - MAYBE
	} else {
		return subscribe_OUT_off(); // subscribe write - MAYBE
	}
	return true;
}

/*!
	TODO sock_recv() sort of knows its a socket
*/
bool fd_selectable_base::read_fd_to_buffer( buffer3 & buff3 )
{
	// INFO("FD %d %s", fd_get(), debug_get_obj_name() );
//	bool caller_should_stay_read_subscribed = true;
//	bool caller_should_NOT_stay_read_subscribed = false;

        int max = buff3.circ_space_avail();
	if( max == 0 ) {
		buff3.dgb_show( "read_fd_to", "buff3" );
		WARN("FD %d %s - buffer space = %d - FULL", 
			fd_get(),
			debug_get_obj_name(),
			max
		);
		// if you have a second buffer to read into
		// you had better switch IN_on again!
		// Most will watch buffer being drained and do so from there
		return subscribe_IN_off();;
		// not that all apps have a single view of the world
	}
#if 1
	const static int MX1 = 20480; // 1024;
#else
# warning limiting packet size for debugging reasons - data corrupted somewhere
	const static int MX1 = 20; // 1024;
#endif
	if( max > MX1 ) {
		max = MX1;
	}

	void * buf = (void*) buff3.circ_addr_wr();
	// if( PRIV_is_socket ) ;
        int len = sock_recv( buf, max );
	INFO("FD %d %s sock_recv() => %d",
		fd_get(),
		debug_get_obj_name(),
		len
	);
	if( len == 0 ) {
		//
		// done by sock_recv(
		// DONE // this->call_eof_input();
		//
		return subscribe_IN_off();;
		//
	} else if( len < 0 ) {
		FAIL("sock_recv returned %d", len );
		FAIL("sock_recv returned %d - what about non-blocking", len );
		FAIL("sock_recv returned %d - what about non-blocking", len );
		FAIL("sock_recv returned %d - what about non-blocking", len );
		FAIL("sock_recv returned %d - what about non-blocking", len );
		FAIL("sock_recv returned %d - what about non-blocking", len );
		FAIL("sock_recv returned %d - what about non-blocking", len );
		FAIL("sock_recv returned %d - what about non-blocking", len );
		dgb_sleep_more();
		WARN("Staying subscribed");
		//
		// Changed my mind
		// if there is an error, nothing more will be read
		return FAIL("ERROR cancels subscription");
		//
		return subscribe_IN_on();
		//
		// this may be confusing as a half line gets re-parsed
		// but this is a very rare case - 
		// as it should NEVER happen, but ok it did
		// whatever error ... might be calling sock_recv when no input
		// chance your arm - maybe return false, and chancer knows not
	}
	buff3.circ_added( len );
	buff3.dgb_show( "read  fd", "buff3" );

	if( len == max ) {
		// TIDE
		if(1) INFO("input buff3 is now full - other side of app must find out");
	} else {
		if(len > 999 ) INFO("FD is now drained - no backlog");
		//
		// SET A FLAG TO AVOID CALLING SELECT AGAIN (or not)
		//
		// input data was not broken boundry (not by THIS read anyway)
	}

	/*
		COULD check buffer3 for
		leaving zero
		leaving lo tide
		entering hi tide
		entering full

		COULD do that in circ_added
		
	*/
	return subscribe_IN_on();
}



