
#include "sock_addr_unix.h"

/*
	sock_addr_UNIX is a wrap on AF_UNIX, named sockets in the filespace
*/
#ifdef WIN32

#else // not WIN32 so use real code

// #include <memory.h> // memset

// #include <sys/socket.h>
#include <sys/un.h>
// #include <arpa/inet.h>
// #include <netdb.h> // gethostbyname

// #include <stdlib.h> // atoi
// #include <stdio.h> // perror

// #include "str0.h"

#include "dgb.h" // FAIL

	//! constructor - use a debugging pathname?
	sock_addr_UNIX::sock_addr_UNIX()
	: sock_addr_base( (sockaddr*)&sa, sizeof(sa))
	{
		null_out_sa();
		sa.sun_family = AF_UNIX;
		set_pathname( "/tmp/MyApp.fifo" );
	}

	//! set the FIFO from a string which can NOT contain a NUL
	bool sock_addr_UNIX::set_pathname( str0 pathname )
	{
		if( pathname.str_len() > 108 ) {
			FAIL("len %d str %s",
				pathname.str_len(),
				(STR0) pathname
			);
			return false;
		}
		strncpy( sa.sun_path, (STR0)pathname, 108 );
		return true;
	}


	//! get the pathname as a CPU byte order int
	str0 sock_addr_UNIX::get_pathname()
	{
		return sa.sun_path;
	}


#endif // WIN32-or-UNIX
