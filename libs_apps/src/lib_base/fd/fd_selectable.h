#ifndef fd_selectable_H
#define fd_selectable_H

// #include <stdio.h>
// #include <memory.h>
// #include <errno.h>
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

#include "fd_hold.h"
#include <glib.h> 
#include "str2.h"
#include "obj_hold.h"
#include "err_int.h"

struct buffer3;

/*!
	fd_selectable_base is an fd_hold_1 with added event driven IO hooks.
	This version uses glib, but any select/poll could be spliced in.

	Whenever select() says the fd is readable, it calls select_says_read(),
	(a virtual function) it also detects eof and error.

	Its written for glib, but should be transferrable to direct UNIX
	(or other) without far too much work.

	It uses virtual functions for most tasks, as this is a reasonable
	cost for minimal overhead. If your app is usually line-based,
	see fd_sel_line, or fd_sel_http.

	TODO: There is no write buffer, for when fd (non-blocking) refuses to
	accept more. That requires an XOFF/XON style mechanism, and some
	additional buffering. select_says_write(...) would then drain the
	backlog, and restart output generation.

	TODO: the glib implementation works, but is a bit thin.

	This class is usually derived by fd_sel_line, or other.
*/
class fd_selectable_base : public fd_hold_1
{
 private:
	void select_says_connect_failed(); // removed ?
 protected:
	bool eof_seen; // or abrupting error
 public:
	GIOChannel* channel;

	gint sel_source1; // errm ...

//	bool XOFF_write; // additional mask
//	bool XOFF_read; // additional mask
	//// WANT_read; // deduce all the time
	// TODO: XOFF_sent_released_unacked_etc_pending

	// PRIVATE only to be called just before returning false to cancel old
	// actually, now I want the downstream write side to call
	// here->subscribe_read_more(); // because data is now available
	// and it can make good use of fix1_conditions
	// maybe move into obj flags (about time)
	//
	GIOCondition fix1_conditions; // its a fix
	GIOCondition fix1_conditions_previous; // its a fix
	bool desubscribe_fix1();
	bool resubscribe_fix1_conditions_less( GIOCondition cond );
	bool resubscribe_fix1_conditions_plus( GIOCondition cond );
	bool resubscribe_fix1_conditions(); 

	bool is_subscribed( GIOCondition cond );
	
//	void retry_write_set( bool on );

	bool seen_connected; // man 7 socket - on first select says read

	/*
		TODO - rework against debug_obj_name - and debug_track_pointer
	*/
	obj_hold<str2> debug_name; // used for debugging
        //! get the debugging name
	virtual str2 * debug_name_get(); //  { return debug_name; }
        //! set the debugging name
        virtual void debug_name_set( str2 * name ); //  { debug_name = name; }
        void debug_name_set( const char * name ); //  { debug_name = name; }

	virtual ~fd_selectable_base();

	fd_selectable_base( int _fd = -1 );
	bool init_base_zero();

	virtual void eof_synthetic();
	virtual void call_eof_input(); // what exactly is this - macro?
	virtual void zero_read_means_eof();
	virtual void select_says_connect_failed(err_int_t & err); // ASYNC - 
	virtual void select_says_connected(); // first select_says_read()
	virtual void select_says_eof_input();	// OK it wasnt select ...
	virtual void select_says_eof();	// OK it wasnt select ...
//	virtual void select_says_eof_output(); // this never happens, VOLANTEER
	virtual bool select_says_read() = 0;
	virtual bool select_says_write();
	virtual bool select_says_PRI();
//	virtual bool select_says_error();
	virtual bool select_says_error(err_int_t &err);

	/*
		call the virtual functions
		maybe add some dgb wrapping
		maybe add some unsubscribe logic
		maybe obj->FLAGS can be used ...
	*/
	bool gdb_pre_select_says_something(); // gdb_trap point
	void call_eof_synthetic();
	void call_call_eof_input(); // what exactly is this - macro?
	void call_zero_read_means_eof();
	void call_select_says_connect_failed(err_int_t & err); // ASYNC - 
	void call_select_says_connected(); // first select_says_read()
	void call_select_says_eof_input();	// OK it wasnt select ...
	void call_select_says_eof();	// OK it wasnt select ...
//	void call_select_says_eof_output(); // this never happens, VOLANTEER
	bool call_call_select_says_read();
	bool call_select_says_write();
	bool call_select_says_PRI();
//	bool call_select_says_error();
	bool call_select_says_error(err_int_t &err);

// dont use these any more
	void subscribe_each( bool sub_in = true, bool sub_out = true );
	void subscribe_in()
	{
		subscribe_each( true, false );
	}
	void subscribe_out() // see "ask_select_for_write"
	{
		subscribe_each( false, true );
	}

	// incrementally set one condition on or off
	// bool is always true for on, false for off
	// return subscribe_IN_off(); // back to select_says_read
	bool subscribe_set( GIOCondition condition, bool maybe ); // calls _off
	bool subscribe_on( GIOCondition condition );
	bool subscribe_off( GIOCondition condition );
	bool subscribe_OUT_on() { return subscribe_on( G_IO_OUT ); }
	bool subscribe_OUT_off() { return subscribe_off( G_IO_OUT ); }
	bool subscribe_OUT_set(bool more) { return subscribe_set( G_IO_OUT, more ); }
	bool subscribe_IN_on() { return subscribe_on( G_IO_IN ); }
	bool subscribe_IN_off() { return subscribe_off( G_IO_IN ); }
	bool subscribe_IN_set(bool more) { return subscribe_set( G_IO_IN, more ); }
	bool is_subscribed_IN() { return is_subscribed( G_IO_IN ); }
	bool is_subscribed_OUT() { return is_subscribed( G_IO_OUT ); }

	void subscribe(
		int conditions
		= G_IO_IN
		| G_IO_OUT
		| G_IO_PRI
		| G_IO_ERR
		| G_IO_HUP
		| G_IO_NVAL
	);
	void unsubscribe();

 static
	void C_select_removal( gpointer data );
 static
	gboolean C_select_says(
		GIOChannel *source,
		GIOCondition condition,
		gpointer data
	);
 static
	gboolean C_select_says_fix1_first(
		GIOChannel *source,
		GIOCondition condition,
		gpointer data
	);
 static
	gboolean C_select_says_fix1_second(
		err_int_t & err1, // errno as found
		GIOChannel *source,
		GIOCondition condition,
		gpointer data
	);

	bool write_buffer_to_fd( buffer3 & buff3 );
	bool read_fd_to_buffer( buffer3 & buff3 );
};


#endif
