
#include "spawn.h"
#include "dgb.h"
#include "buff_line_for_io.h"
#include "g_error.h"
#include "dgb.h"
#include "MY_WIN32.h"
#include <unistd.h> // close

// (*GChildWatchFunc)
/*!
	CB_child_watch is the C callback that calls C++ cb_child_exit()

	g_lib does the work, except when out process calls TerminateProcess
	on WIN32, the callback doesnt happen. Hence the sibling func
	cb_child_exit_kill, which is called by our own kill process.

	openvpn returns 0 when killed with -9 !!
	openvpn returns 131 when killed with -3 (128+3=131)
	g_spawn return 3 when killed with -3
*/
void CB_child_watch(
	GPid pid,
	gint status,
	gpointer data
)
{
	// this is a C callback function
	Spawn * spawn = (Spawn *) data;		// we know the pointer type
	if(!spawn) {
		FAIL("CB with NULL object");
		return;
	}
	if(!spawn->have_child_pid()) {
		FAIL("CB with NULL child_pid");
		return;
	}
	INFO4( "(%s) child process %d has exited with status %d",
		spawn->dgb_name.get_str(),
		spawn->child_pid,
		status	// zero !!
	);
	spawn -> child_exit_status = status;	// then also split into bits
	spawn -> release_pid_handle();		// never to be called again
	spawn -> cb_child_exit();		// call the virtual function
	// a derived class would act on the information
}

/*!
	Spawn is future child processes
	the exec bit
	and also the connections back to parent
*/
// Spawn::Spawn( app_session_base * _app, const char * _name )
Spawn::Spawn( const char * _name )
: dgb_name( _name )
// , app( _app )
{
	// NULL // working_directory;
	// argv = NULL;
	arg_c = 0;
	envp = NULL;	// keep unchanged
	flags = GSpawnFlags(0); // you set these
	NULL_child_setup = NULL; // UNIX only - fork()
	NULL_user_data = NULL; // UNIX only - fork()
	child_pid = GPid(0);
	child_exit_status = 0;
	standard_input = -1;
	standard_output = -1;
	standard_error = -1;
	child_watch_event_source = 0;
	shutdown_requested = false; // rename should_be_running

	// (GSpawnFlags)
// http://library.gnome.org/devel/glib/stable/glib-Spawning-Processes.html#GSpawnFlags

	// we will use a G_CHILD_WATCH callback on its exit
	or_flag( G_SPAWN_DO_NOT_REAP_CHILD);

	// search for cmd along PATH
	or_flag( G_SPAWN_SEARCH_PATH );

	// this is my class, and it does nothing, ... to the same extent
	or_flag( G_SPAWN_CHILD_INHERITS_STDIN );

	/*
	// this is badly documented - seems to say the same either way!
		By experiment - if used,
		ARGV[0] is used as the thing to exec
		ARGV[1] is used as the NAME of the thing, as if argv[0]
		effectively removing it from the list
		useful for linked things that respond to being named
		but then make it the link anyway
if(0)
	or_flag( G_SPAWN_FILE_AND_ARGV_ZERO );
	*/

	// if caller wants pipes, caller will have to do something
	set_p_standard_all( false );

#ifdef WIN32
# ifdef WIN32_CONSOLE
	// MAYBE
# else
 // actually we cant get rid of OPENVPN console so dont silence it (yet) (ever)
 if(0) {
	or_flag( G_SPAWN_STDOUT_TO_DEV_NULL );
	or_flag( G_SPAWN_STDERR_TO_DEV_NULL );
	or_flag( G_SPAWN_CHILD_INHERITS_STDIN );
 }
# endif
#endif
}

// virtual
Spawn::~Spawn()
{
	shutdown();
}

bool Spawn::release_pid_handle()
{
	if( have_child_pid() ) {
		GPid loopy_child_pid = child_pid;
		clear_child_pid();
		g_spawn_close_pid( loopy_child_pid );

		// if any real can be found, retain PID (or get info now)
		// eg on UNIX several KILLś might be required
		// also SIG USR1 is an option
		// also WINCH is an option
		// for now, kill is kill, not signal
	}
	if( child_watch_event_source ) {
		// ? nothing to do ? unsubscribe events ? ref_decr ?
		child_watch_event_source = 0;
	}
	return true;
}

// virtual 
void Spawn::shutdown() // nice or nasty ? keep or kill ?
{
	kill_pid(); /*
		// done by kill_pid // release_pid_handle();
	*/
}

void Spawn::set_watch()
{
	if( child_watch_event_source ) {
		INFO2("dgb_name = %s",  dgb_name.get_str() );
		WARN("child_watch_event_source already set");
		return;
	}
	child_watch_event_source = 
		g_child_watch_add( child_pid, CB_child_watch, this );
		INFO4("(%s) g_child_watch_add() has watch_source: %d on %d",
			dgb_name.get_str(),
			child_watch_event_source,
			child_pid
	);
}

void Spawn::argv_reset()
{
	while( arg_c > 0 ) {
		arg_c --;
		arg[arg_c].clear();
	}
	arg_c = 0;
}

bool Spawn::argv_add( const char * s )
{
	// OK // INFO( s );
	if( arg_c >= arg_c_max ) {
		FAIL("arg_c exceeds limit");
		return false;
	}
	arg[arg_c] = s;
	arg_c ++;
	return true;
}
bool Spawn::argv_add( const str_const & s )
{
	// INFO( s ); // loop detect
	return argv_add( (const char *) s.get_str() );
}
bool Spawn::argv_add_trailing_NULL()
{
	// already there!
	return true;
}
bool Spawn::argv_fmt( const char * fmt, ... )
{
	bool ok = false;
        va_list ap;
        va_start( ap, fmt );
	buff_line_writer buf;
	ok = buf.write_vfmt( fmt, ap, false );
        va_end( ap );
	argv_add( buf.get_str() );
	return ok;
}


bool Spawn::run_async()
{
//	RTFM SAYS! gdk_spawn_on_screen_with_pipes()

	buff_line_writer line(NULL);
	const char * fmt = "%s";
	for( int i = 0; i< arg_c; i++ ) {
		line.write_fmt(  fmt, arg[i].str );
		fmt = " %s";
	}
	STEP3("(%s) RUN_ASYNC('%s') ... ", dgb_name.get_str(), line.get_str() );

//	build_argv(); // gchar **argv,
	const guchar *uargv[arg_c + 1];
	for(int i =0; i< arg_c; i++ ) {
		uargv[i] = arg[i].str ;
	}
	uargv[arg_c] = NULL; // guard NULL
	gchar **argv = (gchar **) uargv;

	// there is no argc - its a NULL terminated argv

 if(1) {
	//
	// do this here, but dont expect it in future
	//
	if( flags & G_SPAWN_STDOUT_TO_DEV_NULL )
		p_standard_output = NULL;
	if( flags & G_SPAWN_STDERR_TO_DEV_NULL )
		p_standard_error = NULL;
	if( flags & G_SPAWN_CHILD_INHERITS_STDIN )
		p_standard_input = NULL;
 }

 if(0) {
	//
	// NOW I do want pipes ...
	//
	/*
		I dont want any pipes (yet)
		so tell glib by setting the result pointers to NULL
	*/
	p_standard_input  = NULL;
	p_standard_output = NULL;
	p_standard_error  = NULL;
 }
	shutdown_requested = false;

	bool t = g_spawn_async_with_pipes(
		(const gchar *) working_directory.str,
		argv,
		envp,
		flags,
		NULL_child_setup,
		NULL_user_data,
		& child_pid,
		p_standard_input,
		p_standard_output,
		p_standard_error,
		grrr()
	);
	if( t ) {
		TRACE3("(%s) g_spawn started %d OK",
			dgb_name.get_str(),
			child_pid
		);
	} else {
		//
		// NOT SURE ... FILE NOT FOUND ???
		//
		// ALSO www SAYS: may need time to startup
		//
		TRACE_1("(%s) g_spawn returned FAIL", dgb_name.get_str() ); // or use argv[0]
		/*
			THERE IS NO PID AND NO CHILD WATCH (see set_watch())
			INFO2("PID == %d", child_pid );
			INFO2("child_watch_event_source == %d", child_watch_event_source );
		*/
		only_print_gerror();
		/*
		# GError # domain=2 code=2
		Failed to change to directory 'keys' (No such file or directory) 
		*/
		return false;
	}

	set_watch(); // needs PID
	return true;
}

// VIRTUAL
void Spawn::cb_child_exit()
{
	// child_pid // no-longer available, maybe add //  gint child_pid_was;
	INFO3("(%s) CHILD PROCESS HAS EXITED WITH STATUS %d",
		dgb_name.get_str(),
		child_exit_status
	);
}

/*!
	cb_callback child_exit due to kill

	One windows version doesnt report child exit, if you kill it yourself.
	Another does. We need to know, and also delete the callbacks.
*/
// VIRTUAL
void Spawn::cb_child_exit_kill()
{
	INFO2("(%s) CHILD PROCESS IS BEING KILLED - PRESUME TO EXIT", dgb_name.get_str());
	release_pid_handle();		// never to be called again
	cb_child_exit();
}


/*!
	who knows what this does

	I need to know what signal to send to openvpn
	I need to know what WIN32 does
	I need to know what glib does

	maybe unused status is the KILL parameter?

	SIGKILL is a bit abrupt -9
*/
bool Spawn::kill_pid( int unusedstatus )
{
	shutdown_requested = true;
	if( !have_child_pid() ) {
		if(0) INFO2("OK %s child_pid is NULL", dgb_name.get_str() );
		return true;
	}

#ifdef WIN32
	TRACE3("(%s) killing with status (not signal) = %d", dgb_name.get_str(), unusedstatus );
	if (!TerminateProcess( child_pid, unusedstatus)) {
		PERROR("TerminateProcess( ... ) failed");
	}
	child_exit_status = 1;
	release_pid_handle();		// never to be called again
	cb_child_exit_kill();
	close_pipes();
#else
/* kill -l
 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL
 5) SIGTRAP	 6) SIGABRT	 7) SIGBUS	 8) SIGFPE
 9) SIGKILL	10) SIGUSR1	11) SIGSEGV	12) SIGUSR2
13) SIGPIPE	14) SIGALRM	15) SIGTERM	16) SIGSTKFLT
17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU
25) SIGXFSZ	26) SIGVTALRM	27) SIGPROF	28) SIGWINCH
29) SIGIO	30) SIGPWR	31) SIGSYS	34) SIGRTMIN
35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3	38) SIGRTMIN+4
39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
43) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12
47) SIGRTMIN+13	48) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14
51) SIGRTMAX-13	52) SIGRTMAX-12	53) SIGRTMAX-11	54) SIGRTMAX-10
55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7	58) SIGRTMAX-6
59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
63) SIGRTMAX-1	64) SIGRTMAX	
*/

	unsigned int SIG = SIGTERM;
	SIG = SIGKILL;

	TRACE4( "(%s) killing %d with signal (not status) = %d",
		dgb_name.get_str(),
		child_pid,
		SIG
	);

	if (kill( child_pid, SIG)) {
		PERROR2("(%s) kill() failed", dgb_name.get_str());
	}
#endif
	return release_pid_handle();
	// I did not get any notification - so synthesise one ?
	// it does make sense - would only retry somthing, due to ignorance
	return true;
}

void Spawn::close_pipes()
{
	// g_spawn opens pipes and places the values - if PTR wasnt NULL ...AND...
	// so close them now
	if( standard_input >= 0 ) {
		close( standard_input );
		standard_input = -1;
	}
	if( standard_output >= 0 ) {
		close( standard_output );
		standard_output = -1;
	}
	if( standard_error >= 0 ) {
		close( standard_error );
		standard_error = -1;
	}
	
}
