#define CHROOT_DIR "/home/gps/chroot_one"
#define tools_var_Postgres "/tools/i586/var/Postgres"
#define tools_sql "/tools/i586/sql"
#define FIFO ".s.PGSQL.5432"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

void perror_exit( const char * str ) {
        int e = errno;
        perror( str );
        exit( e );
}

void tell( const char * str ) {
        fprintf(stdout, "%s\n", str );
        fflush(0);
}

#define DIR "/tools/"
#define var_Postgres "/tools/i586/var/Postgres"

main( int argc, char ** argv, char ** envp )
{
        uid_t uid;
        gid_t gid;
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
                "-D",   var_Postgres,
                0
        };

        char * argv_p[] = {
                "postgres",
                "-D",   var_Postgres,
                0
        };

        char * argv_start[] = {
                "pg_ctl",
                "-D",   var_Postgres,
                "start",
                0
        };

        char * argv_stop[] = {
                "pg_ctl",
                "-D",   var_Postgres,
                "stop",
                0
        };

        gid = getegid();
        uid = geteuid();
        dir = DIR;
//      dir = "/";
        cmd_p =      "/tools/i586/sql/bin/postgres";
        cmd_i =      "/tools/i586/sql/bin/initdb";
        cmd_a =      "/tools/i586/sql/bin/pg_id";
        cmd_pg_ctl = "/tools/i586/sql/bin/pg_ctl";

        /*
                what umask is used for this ?
                owner = postgres?
        */

#define FIFO ".s.PGSQL.5432"

        cmd_link =
                "[ -r /tmp/" FIFO " ] || ln -s "
                DIR "/tmp/" FIFO
                " /tmp/" FIFO;

        system( cmd_link );
        // change dir to be within the tree
        if(  chdir(dir) ) perror_exit( "chdir" );
        // become root again - to do the chroot
        // only works if program started by root user
        // LURK if we can do this now - does exec remove that ability?
        if( setuid(0) ) perror_exit( "setuid" );
// debug
//      system( "/bin/ksh" );
        // chroot to the tree
        if( chroot(dir) ) perror_exit( "chroot" );
// debug
//      system( "/bin/ksh" );
	  system( "/tools/i586/ccs/bin/bash" );
        // switch back to the group/user
        // group first ((HPUX uses egrp euid anyway??))
        if( setgid(gid) ) perror_exit( "setgid" );
        if( setuid(uid) ) perror_exit( "setuid" );
// debug
//      system( "/bin/ksh" );
//      system( "/bin/ksh" );

//      envp = 0; /* blocks IFS attack on non-bash shells */

/*
        archeology - these were compiled, the binary kept then edited out!
        This is a Q+D demo of how to start doing this, not a polished result.
        You would parametise which is called, and also control the ENVP used.
        You did have to be root, though to get this far.
*/
        system( "/bin/ksh" );
//      execve( cmd_pg_ctl, argv_stop, envp );
//      execve( cmd_p, argv_p, envp );
//      perror( "Still Here!! exec was a non starter" );
        return errno;
}


