#ifndef sock_addr_base_H
#define sock_addr_base_H

#ifdef WIN32

#include "winsock.h"
typedef int socklen_t;

#else

// #include <netinet/in.h> // sockaddr_in
#include <sys/socket.h> // socklen_t
// #include <arpa/inet.h>
// #include <netdb.h> // gethostbyname

#endif

// #include <stdlib.h> // atoi
// #include <stdio.h> // perror
#include <memory.h> // memset

#include "ints.h" // GRP_lib_base
// #include "str0.h"

class sock_addr_base : public GRP_lib_base
{
 public:
	struct sockaddr * sa_ptr;
	socklen_t sa_len;

	sock_addr_base( sockaddr * _p, socklen_t _l )
	{
		sa_ptr = _p;
		sa_len = _l;
	}

	//! initialise sa with NUL bytes
	void null_out_sa()
	{
		memset( sa_ptr, '\0', sa_len );
	}

};

#endif

