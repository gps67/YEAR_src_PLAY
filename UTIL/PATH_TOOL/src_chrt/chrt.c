
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
#define DIR "/tools/i686/_build/_go/ccs"
// #define DIR "/tools/i686/_build/build_root"

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

typedef int bool;
#define true 1
#define false 0

bool str_match( const char * lhs, const char * rhs )
{
	if(!lhs) {
		if(!rhs) return true; // BOTH NULL
		return false;
	}
	if(!rhs) { return false; }

	return 0==strcmp( lhs, rhs );
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
        uid_t uid;
        gid_t gid;
        uid_t euid;
        gid_t egid;
        char * dir;
        char * cmd_i;
        char * cmd_p;
        char * cmd_a;
        char * cmd_pg_ctl;
        char * cmd_link;

        char * argv_a[] = {
                "pg_id",
                0
        };

        char * argv_i[] = {
                "initdb",
                "-D",   TOOLS_var_Postgres,
                0
        };

        char * argv_p[] = {
                "postgres",
                "-D",   TOOLS_var_Postgres,
                0
        };

        char * argv_start[] = {
                "pg_ctl",
                "-D",   TOOLS_var_Postgres,
                "start",
                0
        };

        char * argv_stop[] = {
                "pg_ctl",
                "-D",   TOOLS_var_Postgres,
                "stop",
                0
        };

        gid = getgid();
        uid = getuid();
        eprint(2,"CURRENT uid=%d, gid=%d PERMS FROM PROCESS\n", (int) uid, (int) gid );

        egid = getegid();
        euid = geteuid();
        eprint(2,"EFFECTIVE euid=%d, egid=%d PERMS FROM FILE\n", (int) euid, (int) egid );

        dir = DIR;
//      dir = "/";
        cmd_p =      TOOLS_sql "/bin/postgres";
        cmd_i =      TOOLS_sql "/bin/initdb";
        cmd_a =      TOOLS_sql "/bin/pg_id";
        cmd_pg_ctl = TOOLS_sql "/bin/pg_ctl";

        /*
                what umask is used for this ?
                owner = postgres?
        */

        cmd_link =
                "[ -r " TMP_FIFO " ] || ln -s "
                DIR TMP_FIFO
                " " TMP_FIFO;

//      my_system( cmd_link );

// change dir to be within the tree
        if(  chdir(dir) ) perror_exit( "chdir" );


// change user ID to ROOT

        // become root again - to do the chroot
        // only works if program started by root user
        // LURK we can do this now - exec removes that ability?

	int uid_chroot = 0;
        eprint(3,"# CALLING # setuid(uid_chroot=%d) # to be able to chroot\n",uid_chroot);
        if( setuid(uid_chroot) ) perror_exit( "setuid" );
 if(0) {
        eprint(3,"# CALLING # setgid(%d)\n - to keep that group!",egid);
        if( setgid(egid) ) perror_exit( "setgid" );
 }

	if(0) my_system("id");
	// my_debug_shell();

// chroot to the tree
	// inevitably means must be root, but might be ACL'd to another user)
	//
        eprint(3,"# CALL # chroot(%s)\n", CHROOT_DIR);
        if( chroot(CHROOT_DIR) ) perror_exit( "chroot" );

	if(0) my_debug_shell();

// switch back to the group/user (having done the chroot)
        // group first ((HPUX uses egrp euid anyway??))
        // eprint(2,"requesting euid=%d, egid=%d\n", (int) euid, (int) egid );
        eprint(2,"# CALLING # setuid(uid=%d)\n", (int) uid, (int) egid );
        if( setuid(uid) ) perror_exit( "setuid" );

// the actual purpose is a shell ... or 
	if(0) my_debug_shell();


	argv ++; // dumb but OK
	const char * filename = argv[0];
	execve( filename, argv, envp );
        perror_exit( "execve()" );

//      envp = 0; /* blocks IFS attack on non-bash shells */

/*
        archeology - these were compiled, the binary kept then edited out!
        This is a Q+D demo of how to start doing this, not a polished result.
        You would parametise which is called, and also control the ENVP used.
        You did have to be root, though to get this far.
*/
//	my_debug_shell();
//      execve( cmd_pg_ctl, argv_stop, envp );
//      execve( cmd_p, argv_p, envp );
//      perror( "Still Here!! exec was a non starter" );
        return errno;
}


