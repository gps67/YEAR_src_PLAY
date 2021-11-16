
#include "buffer/misc.h" // gdb_invoke
#include "unistd.h" // usleep

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include <signal.h>

extern void gdb_break_point();

#define WAIT_DEBUGGER_TIMEOUT 30 // 10 for gdb, more for insight
#define GDB_prog "gdb"
#define GDB_w_prog "insight"

void self_SIGKILL()
{
	int pid = getpid();
	kill(pid, SIGKILL);
}

void gdb_invoke( bool usegui )
{
	static bool loop_detected = false;
	fflush(0);
	char * ingdb = getenv("INGDB");
	if( ingdb )
	{
		/*
			hopefully this doesnt call gdb_invoke ...
			If so add
		*/
		if( loop_detected ) return;
		loop_detected = true; // almost potential - not actual
		gdb_break_point();
		loop_detected = false;
		return;
	}

	// caller is supposed to set progname_argv0 in main()
	const char* exe_name = progname_argv0;
	if( !exe_name ) {
		fprintf(stderr,"ERROR exe_name not set - using HARDCODED one\n");
	}
	if( !exe_name ) exe_name = "/tools/2004/py/bin/python"; // HARDCODED
	if( !exe_name ) exe_name = getenv("_"); // typically set by sh/bash
//	if( !exe_name ) exe_name = "a.out"; // last resort ?
	if( !exe_name )
	{
		fprintf(stderr, "gdb_invoke() cannot find progname\n" );
		exit(1);
	}

	int pid = getpid();
	char pid_str[16];
	sprintf(pid_str, "%d", pid); 

	setenv( "INGDB", "1", 1 );

	fflush(0);
	// ignore fork fail - (ptable full) - someone is watching
	int child_pid = fork();
	if( child_pid == -1 ) {
		perror( "fork - gdb" );
		exit errno;
	}
	if ( child_pid ) { 
		/*
			this is the parent
			child gdb finds this parent process in this sleep
		*/
		fprintf(stderr, "usegui == %s\n", usegui? "true" : "false" );
		fprintf(stderr, "parent if watching:   ps -l %d ; cd /proc/%d ; cat status\n", pid, pid );
		fprintf(stderr, "parent sleeping, child pid is %d\n", child_pid );
		fprintf(stderr, "kill -9 %d %d # parent child \n", pid, child_pid );
		fflush(0);
		sleep(WAIT_DEBUGGER_TIMEOUT);
		fprintf(stderr, "sleep done\n" );
		fflush(0);
	} else { 
		fprintf(stderr, "Invoking %s ...\n", GDB_prog );
		fflush(0);
		const char * progname = GDB_prog;
		if( usegui )
		{

		    progname = GDB_w_prog;
		    execlp(
#if 0
	"strace",
	"strace",
	"-f",
#else
			GDB_w_prog,
#endif
			GDB_w_prog,
			// "-w",
			"-q",
			"-s",
			exe_name,
			exe_name,
			pid_str,
			NULL
		);
		} else
		    execlp(
			GDB_prog,
			GDB_prog,
			"-q",
			"-s",
			exe_name,
			exe_name,
			pid_str,
			NULL
		);

		perror( progname );
		kill(pid, SIGKILL);
		
	}
	return;
}

void gdb_fatal_error_handler(int signo)
{ 
	char * ingdb = getenv("INGDB");
	if( ingdb )
	{
		gdb_break_point();
		return;
	}

	static bool done_once = false;
	if( done_once ) {
		fprintf(stderr, "gdb fatal_error_handler - DONE ONCE\n");
		fflush(0);
		sleep(2);
		if( errno ) exit( errno );
		if( signo ) exit( signo );
		exit(1);
		return;
	}
	done_once = true;

	bool gdb_win = false;
	char  buf[200];
	fprintf(stderr, "Loading gdb ... signal %d\n", signo );
	fprintf(stderr, "X/T/W for eXit Text Windows\n" );
	if (fgets(buf, sizeof buf, stdin)) { 
		if( buf[0] == 'W' ) gdb_win = true;
		else 
		if( buf[0] == 'T' ) gdb_win = false;
		else
			self_SIGKILL();
	} else { 
		self_SIGKILL();
	}

	gdb_invoke( gdb_win );
	return;
}

void gdb_progname( const char * progname )
{
	if( progname ) progname_argv0 = progname;
}

void gdb_sigaction( const char * progname )
{
	gdb_progname( progname );
//  return;
	static struct sigaction sa;
	memset( &sa, sizeof(sa), 0 );
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
}

const char * progname_argv0 = NULL;

// bool gdb_off = FALSE;;
bool gdb_off = TRUE;;

/*!
	Insert an (intel) GDB break point here
*/
void gdb_break_point()
{
	if(gdb_off) return;
	fprintf(stderr,"gdb_break_point()\n");
#if     defined (__i386__) && defined (__GNUC__)
{
        __asm__ ("int $03");
}
#endif
}


//! this should be elsewhere
void sleep_10( int tenths )
{
#ifndef WIN32
	while( tenths > 9 ) {
		usleep( 10 * 100 * 1000 );
		tenths -= 10;
	}
	if( tenths < 1 ) return;
	usleep( tenths * 100 * 1000 );
#endif
}

