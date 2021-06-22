#ifndef spawn_H
#define spawn_H

#include <glib.h>
#include "str0.h"
#include "str1.h"
#include "blk1.h"
#include "buffer2.h"
// #include "str_const.h"
#include "g_rrr.h"
#include <stdarg.h>

//	// (*GChildWatchFunc)
//	void CB_child_watch(
//		GPid pid,
//		gint status,
//		gpointer data
//	)

/*!
	Spawn is future child processes
	the exec bit
	and also the connections back to parent

TODO

	INPUT
		same as parents STDIN (almost concurrent access to same tty)
		pipe from parents feed
		file opened by parent
		null 

	OUTPUT
		same as parents STDOUT
		pipe feeding back to parent
		file opened by parent
		null

	ERROR
		...OUT...

	STDIN
	or_flag( G_SPAWN_CHILD_INHERITS_STDIN );
	or_flag( G_SPAWN_STDOUT_TO_DEV_NULL );
	or_flag( G_SPAWN_STDERR_TO_DEV_NULL );

*/
class Spawn
{
 protected:
	GError ** grrr() {
		return G_rrr::get_global_G_rrr()->grrr();
	}
	virtual bool only_print_gerror()
	{
		return G_rrr::get_global_G_rrr()->only_print_gerror();

	}
 public:
	bool shutdown_requested;
	GPid child_pid;
	int child_exit_status;
	str1 dgb_name;

	str1 working_directory;

	const static int arg_c_max = 30;
	str1 arg[ arg_c_max + 1 ]; // max is beyond NULL
	int arg_c;

	gchar **envp;
	GSpawnFlags flags;
	GSpawnChildSetupFunc NULL_child_setup;
	gpointer NULL_user_data;

	gint standard_input;
	gint standard_output;
	gint standard_error;

	gint * p_standard_input;	
	gint * p_standard_output;	
	gint * p_standard_error;	

	guint child_watch_event_source;
 public:
	Spawn( const char * _name );
	virtual ~Spawn();
	virtual void cb_child_exit();
	virtual void cb_child_exit_kill(); // on win32 kill own does not get cb
	virtual void shutdown();

	void set_p_standard_all( bool on = true ) {
		p_standard_input  = on ? & standard_input : NULL ;
		p_standard_output = on ? & standard_output : NULL ;
		p_standard_error  = on ? & standard_error : NULL ;
	}

	void set_p_standard_input( bool on = true ) {
		p_standard_input  = on ? & standard_input : NULL ;
		if(on) 
			off_flag( G_SPAWN_CHILD_INHERITS_STDIN );
		else
			or_flag( G_SPAWN_CHILD_INHERITS_STDIN );
	}
	void set_p_standard_output( bool on = true ) {
		p_standard_output = on ? & standard_output : NULL ;
	}
	void set_p_standard_error( bool on = true ) {
		p_standard_error  = on ? & standard_error : NULL ;
	}

	void or_flag ( GSpawnFlags flag )
	{
		flags = GSpawnFlags( flags | flag );
	}

	void off_flag ( GSpawnFlags flag )
	{
		flags = GSpawnFlags( flags & ~flag );
	}

	bool have_child_pid()
	{
		return (child_pid !=0);
	}

	void clear_child_pid()
	{
		child_pid = GPid(0);
	}

	void set_watch();
	bool run_async();
	bool release_pid_handle(); // 

	bool argv_reset();
	bool argv_add( const char * s );
	bool argv_add( str0 s );
	bool argv_add( blk1 & b ) { return argv_add( (str0) b); }
	bool argv_add_trailing_NULL();
	bool argv_fmt( const char * fmt, ... );

	bool argv_print_as_line( buffer2 & line );

	bool kill_pid( int status = 0 );
	void close_pipes();

};

#endif
