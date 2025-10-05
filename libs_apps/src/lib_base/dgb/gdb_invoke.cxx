
#include "misc.h"
#ifndef WIN32
#include "unistd.h" // getpid, fork, exec
#endif
#include "sleep_ms.h" // sleep_ms _us _secs _10

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include <signal.h>
#include "dgb.h" // set_prog_alias // LURK TODO
#include "dgb_fail.h" 

static bool was_gdb_fatal_error_handler = false;

// argv and then fork/exec
#include "g_spawn.h"

#include "tty_ptmx.h" // not_a_tty(fd)
using namespace BASE1; // not_a_tty // need_fd_as_a_TTY


/*
	Other WIN32 sections in this file, are for completely
	unsupported features (on WIN32, by me), but allow compile.
*/

//! a gdb break point (if gdb is there and on)
extern void gdb_break_point();

/*
	when starting gdb, wait for ages (GUI has a lot to do)
*/
#define WAIT_DEBUGGER_TIMEOUT_TEXT 5 // 10 for gdb, 30 for insight
// PROPER // #define WAIT_DEBUGGER_TIMEOUT_GUI 30 // 10 for gdb, 30 for insight
// #define WAIT_DEBUGGER_TIMEOUT_GUI 15 // DualCore2G with cache OK
#define WAIT_DEBUGGER_TIMEOUT_GUI  9 // DualCore2G with cache OK

/*
	when starting gdb - start this
*/
#define GDB_prog "gdb"
#define GDB_GUI_prog "insight"

// #define GDB_GUI_prog "/tools/2000/gdb/bin/insight"

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

static bool yes_it_really_is_running = false;

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
	if(yes_it_really_is_running) {
		FAIL("YES BUT NOT ENV(INGDB)");
		return true;
	}
	return false;
}

/*!
	set the INGDB flag here ;-)
*/
void gdb_is_running_YES()
{
	if(!yes_it_really_is_running)
		INFO("Setting yes_it_really_is_running");
	else
		INFO("REPEAT Setting yes_it_really_is_running");
	yes_it_really_is_running = true;
#ifdef WIN32
	// setenv( "INGDB", "1", 1 );
#else
	/*
		set INGDB as a flag - how else could I detect GDB ?
	*/
	setenv( "INGDB", "1", 1 );
#endif
}

/*!
	after the sleep has COMPLETED, this is called
*/
void post_gdb_invoke_catchup_point()
{
	INFO("HERE");
	gdb_break_point();
	// INFO("HERE");
}

//////////////////////////////////

class spawn_gdb : public Spawn
{
	bool use_gui;
	const char * gdb_prog;
 public:
	spawn_gdb( const char * dbgname = "DEBUGGER" )
	: Spawn(dbgname)
	{
		set_use_gui( true );
	}

	bool set_use_gui( bool use )
	{
	 if( use ) {
		use_gui = true;
		gdb_prog = GDB_GUI_prog;
	 } else {
		use_gui = false;
		gdb_prog = GDB_prog;
	 }
	 return true;
	}

	bool argv_add_gdb_prog()
	{
		if(!argv_add( gdb_prog )) return FAIL_FAILED();	
		return true;
	}

	bool argv_add_my_own_exe_name()
	{
		const char* exe_name = progname_argv0;
		if( !exe_name ) exe_name = getenv("_"); // typically set by sh/bash
		if(!argv_add( exe_name )) return FAIL_FAILED();	
		return true;
		return FAIL( "cannot find progname" );
	}

	bool argv_add_p_pid( int pid )
	{
		if(!argv_add( "-p" )) return FAIL_FAILED();
		if(!argv_fmt( "%d", pid )) return FAIL_FAILED();
		return true;
	}

	bool argv_add_x_dotfile( const char * dotfile = NULL )
	{
		if(!dotfile) dotfile = ".gdb_invoked";
		if(!argv_add( "-x" )) return FAIL_FAILED();
		if(!argv_add( dotfile )) return FAIL_FAILED();
		return true;
	}

};

bool exec_insight_on_pid( int pid ) {

STEP("@");
	// use_gui insight
	spawn_gdb run( "INSIGHT" );
STEP("a");

	if(!run.argv_reset()) return FAIL_FAILED();
	if(!run.argv_add_gdb_prog()) return FAIL_FAILED();
STEP("b");

	run.argv_add( "-q" );		// quiet
	run.argv_add( "-s" );		// -s exe_name = read symbols from
	run.argv_add_my_own_exe_name();
	run.argv_add_p_pid( pid );

	if(!run.run_async()) {
		return FAIL("_FAILED");
	}

	// rewrite with UNIX_EXEC - or with fork folded in
	sleep_secs(WAIT_DEBUGGER_TIMEOUT_TEXT);
	exit(0);



	return FALSE_dgb_fail("THIS NEVER RETURNS?");
}


// dmesg: ptrace of non-child pid 4433 was attempted by: insight (pid 4434)

#ifndef WIN32 // probably LINUX
#include <sys/prctl.h>
#endif
 bool prctl_allow_PTRACE()
 {
#ifndef WIN32
#if 1
	int mypid = getpid();
	int option = PR_SET_PTRACER;
	unsigned long arg2 = mypid;
	unsigned long arg3 = 0;
	unsigned long arg4 = 0;
	unsigned long arg5 = 0;
	int t = prctl( option, arg2, arg3, arg4, arg5 );
	if(t==-1) {
		return FAIL("prct");
	} else {
		return PASS("prct returned %d",t);
	}
#endif
#endif
	return WARN("not implemented");
 }

/*!
	Start gdb (as an external program).

	There is a (30) second sleep, which is usually enough,
	but it does take a long time, particularly for the gui.

	If GDB is already running, (external parent, intentional child,
	signal_handlers child), this can be called again, by (another)
	gdb_invoke(f), so call gdb_breakpoint() which might also be encountered.
*/
void gdb_invoke( bool usegui )
{
	INFO("invoking gdb ... maybe");
	INFO("the point of the FSF and GCC and GDB is that it is already done");
	INFO("not 'an opportunity to build a debugger' but to USE it");
	// lets the user see any message that brought us here
	fflush(0);

	if( gdb_is_running() )
	{
		STEP("DETECTED it is already running");
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
			fflush(0);
			sleep_secs(2);
			return;
		}
		loop_detected = true; // almost potential - not actual

		sleep_secs(4);
		STEP("here comes a breakpoint");
		gdb_off = FALSE;
		gdb_break_point();
		loop_detected = false; // HOPE it was handled above?
		return;
	}

	prctl_allow_PTRACE();

#ifdef WIN32
	FAIL("not invoking gdb from inside on WIN32, start insight first");
#else

	// set the INGDB flag that gdb_is_running() uses, soon will be
	gdb_is_running_YES();

	// also stop at the next gdb_break_point() in the code
	// needed to allow Continue to get the stack !!
	gdb_off = FALSE;

	/*
		gdb uses the exe_name to find the executable file (symbols, etc)
	*/
	// caller is supposed to set progname_argv0 in main()
	const char* exe_name = progname_argv0;
	// $_ == "/usr/bin/make" // which is what bash last saw
	if( !exe_name ) exe_name = getenv("_"); // typically set by sh/bash
	if( !exe_name )
	{
		fprintf(stderr, "gdb_invoke() cannot find progname\n" );
		exit(1);
	}

	/*
		get this PID as a string
	*/
	int pid = getpid();
	char pid_str[16];
	sprintf(pid_str, "%d", pid); 

	INFO("pid %d exe_name %s", pid, exe_name );


	fflush(0); // fflush before fork - just in case
	/*
		fork/exec gdb on this pid, or wait for it to take control

		fork returns -1 on error
		fork returns 0 on child (getpid)
		fork returns PID on parent (of child)

		ignore fork fail - (ptable full) - as someone is already watching
	*/
	int child_pid = fork();
	if (child_pid == -1) { 
		FAIL("fork() failed");
		sleep_secs(2);
		return;
	}
	if (child_pid) { 
		/*
			This is the parent process.
			Sleep until the child gdb process comes here.

			child gdb finds this parent process in this sleep
			It grabs control and EINTR breaks the sleep?
			(That might explain the shallow stack)

			If gdb doesnt get here in time, things fail

			FOR NO GOOD REASON
			intel cc and insight find a very shallow stack
			also N fails
			also F fails
			also insight is a very sick orphan
			the alternatives are (somewhat naff)
		*/
		if( usegui )
			sleep_secs(WAIT_DEBUGGER_TIMEOUT_GUI);
		else
			sleep_secs(WAIT_DEBUGGER_TIMEOUT_TEXT);
		post_gdb_invoke_catchup_point();

	} else { 
		// this is the child process - exec gdb
		if(0) {
		 /*
			the newer experimental gdb invoke via arg builder
			there is an argv_builder and exec code in (nearby)
			that is not experimental, it works well
			this might be, but its not rocket science, just unused
			the parameters are still here
			sort them out first ?
		 */
		 if( usegui ) {
			exec_insight_on_pid( pid );
			FALSE_dgb_fail("exec never returns");
		 }
		}

		const char * gdb_prog;
		if( usegui )
			gdb_prog = GDB_GUI_prog;
		else
			gdb_prog = GDB_prog;

		INFO( "Invoking %s on pid %s %s ...\n",
			gdb_prog,
			pid_str,
			exe_name
		);
		/*
			child process exec's gdb with options

			THIS ONLY MAKES SENSE FOR VOLANTEER gdb_invoke()
			NOT FOR SIG CATCHING
		*/
		{
		 WARN("no finish command");
		 was_gdb_fatal_error_handler = true;
		}
		if( usegui ) {
		 if(was_gdb_fatal_error_handler) {
			// do not 'finish' a faulting program
			// maybe rework this as the new argv builder
		    INFO("execlp form 1");
		    execlp(
			gdb_prog,
			gdb_prog,
			"-x", ".gdb_invoked",	// was ignored, but .gdbinit ok
			"-q",			// quiet intro banners
			"-s", exe_name,		// read symbols from file
			exe_name,
			pid_str,
			NULL
		    );

		 } else {

			INFO( "Invoking %s on pid %s %s ...",
				gdb_prog,
				pid_str,
				exe_name
			);
		    
		    INFO("execlp form 2");
		    execlp(
			gdb_prog,
			gdb_prog,
			"-x", ".gdb_invoked",	// was ignored, but .gdbinit ok
 // no luck with this here in this form -- ignored all subsequent args!
 //			"--eval-command=b post_gdb_invoke_catchup_point()",
		// OK but NOR
		//	"--eval-command=f",		// finish
	//		"--eval-command=f",		// finish
			// "-w",
		//	"-n",			// no .gdb_init +/- others?
			"-q",			// quiet intro banners
 		// NO	"--eval-command=break post_gdb_invoke_catchup_point",
 		// maybe	"--eval-command=continue",
		/*
		  YES 
			it ran this before the GUI became visible
 			"--eval-command=finish",
		*/
	/*
 			"--eval-command=finish",
 			"--eval-command=finish",
 			"--eval-command=finish",
 			"--eval-command=finish",
			"-s", exe_name,		// read symbols from file
	*/
			exe_name,
			pid_str,
			NULL
		);
		 }
		}
		else {
		    INFO("GDB still complains that 'make' is not 'myapp.exe' load symbols");
		    INFO("gdb_prog %s", gdb_prog ); // gdb
		    INFO("exe_name %s", exe_name ); // /usr/bin/make
		    // -tui // demands that stderr is a tty // tcl_less.tcl
		    //
		    need_fd_as_a_tty(1);
		    need_fd_as_a_tty(2);
		    if( not_a_tty( 0 )) {
		    	FAIL("TODO make stdin a TTY");
		    }
		    if( not_a_tty( 1 )) {
		    	FAIL("TODO make stdout a TTY");
		    }
		    if( not_a_tty( 2 )) {
		    	FAIL("TODO make stderr a TTY");
		    }
		    // is this a relic from a cut+paste typo - three opts?
		    execlp(
			gdb_prog,
			gdb_prog,
			"-tui",
		//	"-m", "-r",		// prog.syms saved
			"-x", ".gdb_invoked",	//
		//	"-n",			// -n = no init files
			"-q",
			"-se",			// errm -se=file // both
			exe_name,
			"-p",
			pid_str,
			NULL
		     );
		}

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

		when child gdb exits, gdb_break_point calls sig 11
		in a loop
*/
void gdb_fatal_error_handler(int signo)
{ 
	was_gdb_fatal_error_handler = true;
	static int counter = 0;
	fprintf(stderr,
		"%s received %d'th signal %d pid %d\n",
		__FUNCTION__,
		++counter,
		signo,
#ifdef WIN32
#warning getpid() not on WIN32
		-1
#else
		getpid()
#endif
	);
	fflush(0);

#ifdef WIN32
	/*
		On WIN32, POST has its own handlers ?
		GDB does work there, as does remote targets,
		but I habent yested it much
	*/
	fprintf(stderr, "WIN32 and gdb ... dropping out sorry\n" );
	fflush(0);
	self_SIGKILL(); // probably does nothing!
	exit(1);
#endif

	if( gdb_is_running() )
	{
		fprintf(stderr,"Nested call - gdb_break_point(); \n");
		fflush(0);
		sleep_secs(5);
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
		fflush(0);
		sleep_secs(2);
		if( errno ) exit( errno );
		if( signo ) exit( signo );
		exit(1);
		return;
	}
	done_once = true;

	/*
		Ask the user, use gui or not? KILL if user isnt awake

		TODO: can this process sensibly use fgets?
		When invoked from X11 there is no TTY,
		and maybe a w_dialog would be better.
	*/
	bool gdb_win = false;
	char  buf[200];
	fprintf(stderr, "X/T/W for eXit Text Windows\n" );
	fflush(0); // fgets does this ?
	if (fgets(buf, sizeof buf, stdin)) { 
		if( buf[0] == 'W' ) gdb_win = true;
		else 
		if( buf[0] == 'T' ) gdb_win = false;
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
	from main() using argv[0] or NULL. If progname isnt set,
	gdb will be (completely) flawed, and it needs to be
	calculated, but thats done later in gdb_invoke() IF its called

	Sensible clients do get access to progname_name.

	progname is presumed to be permanantly alloc'd (from argv)
*/
void gdb_sigaction( const char * progname )
{
	set_prog_name( progname );
 /*
 	if( progname ) {
		progname_argv0 = progname;
		// point to name.ext part of path/name.ext
		progname_name = strrchr( progname, '/' );
		if( progname_name )
			progname_name++;
		else
			progname_name = progname;
		// set it here, (MAYBE)
		set_prog_alias( progname_name );
	}
 */

#ifdef WIN32
	return;
#else
	static struct sigaction sa;
	memset( &sa, 0, sizeof(sa) );
	sa.sa_handler = gdb_fatal_error_handler;
	sa.sa_flags = SA_RESTART;
//	sa.sa_mask = 0; // ?
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
const char * progname_argv0 = NULL;
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
	if(gdb_off) {
		STEP("true == gdb_off - so no pause");
		return;
	}

#if     defined (__i386__) && defined (__GNUC__)
{
	STEP("gdb_break_point() __i386__");
        __asm__ ("int $03");
	INFO("OK");
}
#elif     defined (__amd64__) && defined (__GNUC__)
{
	STEP("gdb_break_point() __amd64__");
        __asm__ ("int $03");
	INFO("OK");
}
#elif defined (__GNUC__)
{
	STEP("gdb_break_point() NOT KNOWN ARCHITECTURE");
        __asm__ ("int $03");
	INFO("OK");
}
#else
	FAIL("gdb_break_point() NOT GCC ARCHITECTURE");
#endif
}

