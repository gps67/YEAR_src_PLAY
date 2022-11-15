#ifndef tty_ptmx_H
#define tty_ptmx_H

#if 0
#include <unistd.h>	// fork, close, dup2 ...
//#include <sys/types.h>
#include <sys/wait.h>	// wait()
#include <sys/ioctl.h>	// TIOSCTTY
#include <fcntl.h>	// O_RDWR
#include <stdio.h>	// sprintf
#include <stdlib.h>     // system
#include <errno.h>      // errno
// #include <termios.h>    /* tcgetattr(), struct termios, etc.  */
#endif

namespace BASE1 {
/*!
	Create a pseudo TTY

	Normally the parent sees the "serial-wire" traffic,
	and the child process sees the "TTY" traffic

	/dev/pts/1 is a bit like /dev/tty1 - the app pid sees this
*/
struct tty_ptmx_t
{
	int fd_master;
	int pid;	
	int fork_vtty_old(); // returns smae as fork() -1 childspid or 0
private:
//	// probably from SVR4 days
//	char _DEL_name_master[11];
	char name_slave[21];
	int fd_slave;

	bool open_pair();
	bool stty_saner( int fd );
};

extern bool is_a_tty( int fd );
extern bool not_a_tty( int fd );
extern bool need_fd_as_a_tty( int fd );

}; // namespace
#endif
