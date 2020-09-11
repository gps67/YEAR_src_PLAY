
// The DIR above /tools where chroot will make '/'
#define CHROOT_DIR "/gps_chroot_zero"

// /tools/2000 is the 'prefix' parent like "/usr/local" has "/usr"
#define TOOLS_2000			           "/tools/i686"

// Note that PGSQL puts its main DB outside of the prefix! and /tmp/sock too!
#define TOOLS_var_Postgres	TOOLS_2000 "/var/Postgres"

// Most PGSQL files are in its own group 'prefix'
#define TOOLS_sql			TOOLS_2000 "/sql"

// The socket outside the chroot, must be the same inode as the local one
// This is for the usage as posgresql server within a chroot, other uses not
#define FIFO_NAME ".s.PGSQL.5432"
#define TMP_FIFO "/tmp/" FIFO_NAME

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void perror_exit( const char * str ) {
        int e = errno;
        perror( str );
        exit( e );
}

void tell( const char * str ) {
        fprintf(stdout, "%s\n", str );
        fflush(0);
}

// #define DIR "/tools/"
#define DIR "/tools/i686/_build/_go/util"

int dbg_level = 4;

int eprint( int lvl, const char * fmt, ... ) 
{
	if(lvl > dbg_level) return 1;
	va_list args;
	va_start( args, fmt );
	int i = vfprintf( stderr, fmt, args ); 
	va_end( args );
	return 1;
}

int my_system( const char * cmd )
{
	eprint(2,"# CMD # %s # \n", cmd );
	int t = system( cmd );
	if(t==-1) { eprint(2, "(FORK FAILED)"); }
	if(t==0x7F00) { eprint(2, "(sytem returning 0x7F00 (%X) means /bin/sh MISSING)", t); }
	if(WIFSIGNALED(t)) {
		if(WTERMSIG(t) == SIGINT ) eprint(2,"(SIGINT)"); // break own loop
		else if(WTERMSIG(t) == SIGQUIT ) eprint(2,"(SIGQUIT)"); // break own loop
		else eprint(2,"(SIGXXX-%d)",WTERMSIG(t));
/* wait()
	} else if(!WIFSTOPPED(t)) {
		eprint(2,"(STOPPED-SIGXXX-%d)",WSTOPSIG(t));
 ... CONTINUED...
*/
	} else if(!WIFEXITED(t)) {
		eprint(2, "# (ABNORMAL EXIT) #\n" );
	} else {
		int exitcode = WEXITSTATUS(t);
		if( exitcode ) {
		eprint(2,"# RET # %s # ", cmd );
		eprint(2, " (ret 1x%X exit %d)\n", t, WEXITSTATUS(t) );
		}
	}
	errno = WEXITSTATUS(t);
	return errno;
}

int my_debug_shell()
{
 //	my_system( TOOLS_2000 "/ccs/bin/bash" );
	my_system( "bash" );
//	my_system( "/bin/bash" );
//      my_system( "/bin/ksh" );
}

main( int argc, char ** argv, char ** envp )
{
// change user ID to ROOT

	int uid_chroot = 0;
        eprint(3,"# CALLING # setuid(uid_chroot=%d) # to be able to chmod\n",uid_chroot);
        if( setuid(uid_chroot) ) perror_exit( "setuid" );
	argv[0] = "SUID";
	execve( "./M", argv, envp );
        perror_exit( "exec" );
	my_system( "./M") ;

        return errno;
}


