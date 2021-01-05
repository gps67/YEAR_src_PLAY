#ifndef sock_addr_unix_H
#define sock_addr_unix_H

#include "sock_addr_base.h"
#include "str0.h"

/*
	sock_addr_UNIX is a wrap on AF_UNIX, named sockets in the filespace
*/
#ifdef WIN32

	/*
		On WIN32 simply blank out the functionality whilst allowing the
		calling code to compile (but dont call it!)
	*/
	struct sockaddr;
	class sock_addr_UNIX : public sock_addr_base
	{
	 public:
		sock_addr_UNIX()
		: sock_addr_base( NULL, 0 )
		{
			throw "sock_addr_UNIX";
		}

		bool set_pathname( str0 pathname )
		{
			return false;
		}

		str0 get_pathname()
		{
			return "";
		}
	};

#else // not WIN32 so use real code

// #include <memory.h> // memset

// #include <sys/socket.h>
#include <sys/un.h>
// #include <arpa/inet.h>
// #include <netdb.h> // gethostbyname

// #include <stdlib.h> // atoi
// #include <stdio.h> // perror



/*!
	sock_addr_unix is the "filename" of a name unix socket

	Its biggest problem, is that you have to manually delete the inode.
	An easy way to see if the node is busy, is to open it as a client!

	AF_UNIX is a "named fifo", usually /var/myapp/myapp.1234 or /tmp/.MyApp.
	only accessible on the local file system, just like a TCP socket,
	but the Address/Port is the pathname, with a few extra features,
	such as the server gets the clients pid, uid, gid.

	USAGE:
		sock_addr_UNIX addr;
		addr.set_pathname( "/tmp/MyApp.fifo" );

		// client
		fd_hold_1 sock;
		sock.open( addr );

		// server
		fd_hold_1 sock;
		sock.open_FIFO_deleting_locking_etc( addr );

	TODO:
		create sock_addr_0 a base class with virtual functions
			sa_FAMILY() // AF_UNIX | IP
			sa_STREAM_TYPE() // why here ??
			sa_ptr
			sa_len
		
	NB SOCK_DGRAM is also possible but SOCK_STREAM is always used (by me)
	SYSLOG uses - DGRAM?

*/
class sock_addr_UNIX : public sock_addr_base
{
	//! The socket address - a standard UNIX thing
	struct sockaddr_un sa;
 public:
	//! constructor - use a debugging pathname?
	sock_addr_UNIX();


	//! set the FIFO from a string which can NOT contain a NUL
	bool set_pathname( str0 pathname );


	//! get the pathname as a CPU byte order int
	str0 get_pathname();

};

#endif // WIN32-or-UNIX
#endif
