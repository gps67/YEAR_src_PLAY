
#include "tty_ptmx.h"

#include <unistd.h>	// fork, close, dup2 ...
// //#include <sys/types.h>
// #include <sys/wait.h>	// wait()
#include <sys/ioctl.h>	// TIOCSCTTY
// #include <fcntl.h>	// O_RDWR
// #include <stdio.h>	// sprintf
// #include <stdlib.h>     // system
// #include <errno.h>      // errno

#include "dgb.h" // FAIL

using namespace BASE1;

#define letter1 "pqrstuvwxyz"
#define letter16 "0123456789abcdef"

bool tty_ptmx::open_pair ()
{
	const char * name_master = "/dev/ptmx";
	fd_master = open( name_master, O_RDWR );
	if(fd_master == -1) {
		FAIL("open %s", name_master);
		return false;
	}

	if( 0!=ptsname_r( fd_master, name_slave, sizeof(name_slave) )) {
		return FAIL("ptsname()");
		return false;
	}

	INFO( "# ptsname says '%s'", name_slave );

	if( 0!=grantpt( fd_master )) {
		return FAIL("grantpt()");
		return false;
	}

	if( 0!=unlockpt( fd_master )) {
		return FAIL("unlockpt()");
		return false;
	}

	fd_slave = open( name_slave, O_RDWR );
	if(fd_slave == -1) {
		return FAIL( "OPEN %s", name_slave );
		close( fd_master );
		return false;
	}
	int fd_test = open("/dev/null", O_RDWR);
	INFO("opened PIPE %d TTY %d NULL %d", fd_master, fd_slave, fd_test );
	close(fd_test);
	return true;
}

int tty_ptmx::fork_vtty()
{
	// deprecated for libs_app // kept for luck
	pid = -1;
	if( !open_pair() ) return -1;
	pid = fork();
	switch (pid) {
	case -1:
		FAIL("fork"); // probably no processes table full
		return -1;
	case 0:
		// child process
	//	setsid(); // so that /dev/tty works and signals etc
		// setsid = no need - should get parents /dev/tty
		// man 7 credentials

		// fd_slave possibly closed on exec
		fd_slave = open( name_slave, O_RDWR ); // for setsid 
		if( fd_slave == -1 ) {
			FAIL("open TTY '%s'", name_slave );
			exit( errno );
		}
		close( fd_master );	
	        ioctl (fd_slave, TIOCSCTTY, 0); // man tty_ioctl
		dup2 (fd_slave, 0);
		dup2 (fd_slave, 1);
		dup2 (fd_slave, 2);
		stty_saner( 0 );
		close( fd_slave );	
		
		return 0;
	default:
		// parent process
		close( fd_slave );	
		return pid;
	}
}

bool tty_ptmx::stty_saner( int fd )
{
	return FAIL("TODO");
}

bool BASE1::is_a_tty( int fd )
{
	if( isatty( fd ) ) return true;
	switch( errno ) {
	 case ENOTTY:
	 	errno = 0;
	 	return false;
	 case EBADF:
	 	FAIL("fd %d - not open - %m", fd );
	 	errno = 0;
		return false;
	}
	return FAIL("fd %d - unexpected errno %d -  %m", fd, errno );
}
bool BASE1:: not_a_tty( int fd ) { return !is_a_tty(fd); }

bool BASE1:: need_fd_as_a_tty( int fd )
{
	if( is_a_tty( fd ) ) {
		return PASS("fd %d already a TTY", fd );
	}
	const char * dev_tty = "/dev/tty";
	int flags = O_RDWR; // supposed to be fd_
	int fd_dev_tty = open( dev_tty, flags );
	if(-1 == fd_dev_tty )
		return FAIL("open('/dev/tty')");
	if( dup2( fd_dev_tty, fd ) == -1 ) {
		return FAIL("dup2");
	}
	if( close( fd_dev_tty ) == -1 ) {
		FAIL("close");
	}
	return true;
}
