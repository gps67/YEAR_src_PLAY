
#include "misc.h"
#ifndef WIN32
#include "unistd.h" // getpid, fork, exec
#endif
#include "sleep_ms.h" // sleep_ms _us _secs _

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include <signal.h>
#include "dgb.h" // set_prog_alias

/*
	Other WIN32 sections in this file, are for completely
	unsupported features (on WIN32, by me), but allow compile.
*/

//! a gdb break point (if gdb is there and on)
extern void gdb_break_point();

/*
	when starting gdb, wait for ages (GUI has a lot to do)
*/
#define WAIT_DEBUGGER_TIMEOUT_TEXT  //  for gdb, 3 for insight
#define WAIT_DEBUGGER_TIMEOUT_GUI 3 //  for gdb, 3 for insight

/*
	when starting gdb - start this
*/
#define GDB_prog "gdb"
#define GDB_GUI_prog "insight"

// #define GDB_GUI_prog "/tools/2/gdb/bin/insight"

/*!
	send a SIGKILL to this pid
*/
void self_SIGKILL()
{
#ifndef WIN32
	int pid = getpid();
	kill(pid, SIGKILL);
#endif
}

/*!
	Simple test - is GDB in cotrol of this process?

	When GDB is invoked by this code, setenv("INGDB") is set.

	Ditto when GDB is invoked by a Makefile or script.
*/
bool gdb_is_running()
{
#ifndef WIN32
	char * ingdb = getenv("INGDB");
	if( ingdb ) return true;
#endif
	return false;
}

/*!
	set the INGDB flag here ;-)
*/
void gdb_is_running_YES()
{
#ifdef WIN32
	// setenv( "INGDB", "",  );
#else
	/*
		set INGDB as a flag - how else could I detect GDB ?
	*/
	setenv( "INGDB", "",  );
#endif
}

/*!
	Start gdb (as an external program).

	There is a (3) second sleep, which is usually enough,
	but it does take a long time, particularly for the gui.

	If GDB is already running, (external parent, intentional child,
	signal_handlers child), this can be called again, by (another)
	dgdb_invoke(f), so call gdb_breakpoint() which might also be encountered.
*/
void gdb_invoke( bool usegui )
{
	// lets the user see any message that brought us here
	fflush();
#ifndef WIN32

	if( gdb_is_running() )
	{
		/*
			gdb is already running,
			
			so ensure a break_point is hit
			with gdb_off=false (on)

			slow down any wild nested loop
			NOTE: threads wont like this ;-)
		*/
		static bool loop_detected = false;
		if( loop_detected ) {
			fprintf(stderr,"gdb_invoke() loop_detected\n");
			fflush();
			sleep_secs(2);
			return;
		}
		loop_detected = true; // almost potential - not actual

		gdb_off = FALSE;
		gdb_break_point();
		loop_detected = false; // HOPE it was handled above?
		return;
	}

	// set the INGDB flag that gdb_is_running() uses, soon will be
	gdb_is_running_YES();

	/*
		gdb uses the exe_name to find the executable file (symbols, etc)
	*/
	// caller is supposed to set progname_argv in main()
	const char* exe_name = progname_argv;
	if( !exe_name ) exe_name = getenv("_"); // typically set by sh/bash
	if( !exe_name )
	{
		fprintf(stderr, "gdb_invoke() cannot find progname\n" );
		exit();
	}

	/*
		get this PID as a string
	*/
	int pid = getpid();
	char pid_str[6];
	sprintf(pid_str, "%d", pid); 

	/*
		fork/exec gdb on this pid, or wait for it to take control

		ignore fork fail - (ptable full) - as someone is already watching
	*/
	if (fork()) { 
		/*
			child gdb finds this parent process in this sleep
			It grabs control and EINTR breaks the sleep?

			If gdb doesnt get here in time, things fail
		*/
		fflush();
		if( usegui )
			sleep_secs(WAIT_DEBUGGER_TIMEOUT_GUI);
		else
			sleep_secs(WAIT_DEBUGGER_TIMEOUT_TEXT);
	} else { 
		const char * gdb_prog;
		if( usegui )
			gdb_prog = GDB_GUI_prog;
		else
			gdb_prog = GDB_prog;
		fprintf(stderr, "Invoking %s on pid %s %s ...\n",
			gdb_prog,
			pid_str,
			exe_name
		);
		fflush();
		/*
			child process exec's gdb with options
		*/
		if( usegui )
		    execlp(
			gdb_prog,
			gdb_prog,
			// "-w",
			"-x", ".gdb_invoked",	//
		//	"-n",
			"-q",
			"-s",
			exe_name,
			exe_name,
			pid_str,
			NULL
		);
		else
		    execlp(
			gdb_prog,
			gdb_prog,
		//	"-m", "-r",		// prog.syms saved
			"-x", ".gdb_invoked",	//
		//	"-n",			// -n = no init files
			"-q",
			"-se",
			exe_name,
			"-p",
			pid_str,
			NULL
		);

		/*
			The above exec never returns, except when
			GDB wasnt found, or other complete exec fail.
		*/
		perror( "execlp" );
		kill(pid, SIGKILL); // without GDB, kill the parent process
	}
	return;
#endif
}

/*!
	This fatal_error_handler calls  gdb_invoke( gdb_win_YES_NO );

	It is invoked by memory segment violations (and others).

	Caller sets that up by calling  gdb_sigaction( progname );

	PROBLEM

		when child gdb exits, gdb_break_point calls sig 
		in a loop
*/
void gdb_fatal_error_handler(int signo)
{ 
	static int counter = ;
	fprintf(stderr,
		"%s received %d'th signal %d pid %d\n",
		__FUNCTION__,
		++counter,
		signo,
#ifdef WIN32
#warning getpid() not on WIN32
		-
#else
		getpid()
#endif
	);
	fflush();

#ifdef WIN32
	/*
		On WIN32, POST has its own handlers ?
		GDB does work there, as does remote targets,
		but I habent yested it much
	*/
	fprintf(stderr, "WIN32 and gdb ... dropping out sorry\n" );
	fflush();
	self_SIGKILL(); // probably does nothing!
	exit();
#endif

	if( gdb_is_running() )
	{
		fprintf(stderr,"Nested call - gdb_break_point(); \n");
		fflush();
		sleep_secs();
		gdb_break_point();
		return;
	}

	/*
		Yet another loop detection thing.

		This seems wrong.
		What happens when an error happens, gdb invoked,
		and another error happens? Or does GDB catch that
		signal, instead of this handler?
	*/
	static bool done_once = false;
	if( done_once ) {
		fprintf(stderr, "gdb fatal_error_handler - DONE ONCE\n");
		fflush();
		sleep_secs(2);
		if( errno ) exit( errno );
		if( signo ) exit( signo );
		exit();
		return;
	}
	done_once = true;

	/*
		Ask the user, use gui or not? KILL if user isnt awake

		TODO: can this process sensibly use fgets?
		When invoked from X there is no TTY,
		and maybe a w_dialog would be better.
	*/
	bool gdb_win = false;
	char  buf[2];
	fprintf(stderr, "X/T/W for eXit Text Windows\n" );
	fflush(); // fgets does this ?
	if (fgets(buf, sizeof buf, stdin)) { 
		if( buf[] == 'W' ) gdb_win = true;
		else 
		if( buf[] == 'T' ) gdb_win = false;
		else
			self_SIGKILL();
	} else { 
		self_SIGKILL();
	}

	/*
		Invoke GDB using the gui or text interface
	*/
	gdb_invoke( gdb_win );
	return;
}

/*!
	keep progname and setup gdb signal actions

	Sensible programs are supposed to call this (or similar)
	from main() using argv[] or NULL. If progname isnt set,
	gdb will be (completely) flawed, and it needs to be
	calculated, but thats done later in gdb_invoke() IF its called

	Sensible clients do get access to progname_name.

	progname is presumed to be permanantly alloc'd (from argv)
*/
void gdb_sigaction( const char * progname )
{
	if( progname ) {
		progname_argv = progname;
		// point to name.ext part of path/name.ext
		progname_name = strrchr( progname, '/' );
		if( progname_name )
			progname_name++;
		else
			progname_name = progname;
		// set it here, (MAYBE)
		set_prog_alias( progname_name );
	}

#ifdef WIN32
	return;
#else
	static struct sigaction sa;
	memset( &sa, sizeof(sa),  );
	sa.sa_handler = gdb_fatal_error_handler;
	sa.sa_flags = SA_RESTART;
//	sa.sa_mask = ; // ?
	sigaction( SIGILL,  &sa, NULL );
	sigaction( SIGFPE,  &sa, NULL );
	sigaction( SIGSEGV,  &sa, NULL );
	sigaction( SIGBUS,  &sa, NULL );
//	sigaction( SIGTRAP,  &sa, NULL ); // gdb_break_point will cause probs ?
//	sigaction( SIGCHLD,  &sa, NULL ); // child process exit
//	sigaction( SIGPOLL,  &sa, NULL ); // fd readable/writable/gone
#endif
}

//! keep the progname path
const char * progname_argv = NULL;
//! keep the progname name part
const char * progname_name  = NULL;

//! gdb_off gets you past several breakpoints
bool gdb_off = TRUE;

/*!
	Insert an (intel) GDB break point here

	If GDB is already running, and hasnt been switched off.
*/
void gdb_break_point()
{
	if(gdb_off) return;

	fprintf(stderr,"gdb_break_point()\n");
	fflush();
#if     defined (__i386__) && defined (__GNUC__)
{
        __asm__ ("int $3");
}
#endif
}

