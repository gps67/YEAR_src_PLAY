
#include "sock_addr_ip.h"
#ifndef WIN32
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#else
#include "MY_WIN32.h"
#endif
#include <stdio.h> // sprintf
// #include <errno.h> // perror
#include "err_int.h"

#include "dgb.h"



	//! constructor - init sane
	sock_addr_ip::sock_addr_ip()
	: sock_addr_base( (sockaddr *)&sa, sizeof(sa))
	{
		null_out_sa();
		sa.sin_family = AF_INET;
		set_addr_number( "127.0.0.1" );
		set_port_number( 80 );
	}

	//! set the network byte ordered port number from a cpu ordered int
	bool sock_addr_ip::set_port_number( int port )
	{
		sa.sin_port = htons(port);
		return true;
	}

	//! set the port from a string which is a number
	bool sock_addr_ip::set_port_number( str0 port_str )
	{
		int port = atoi( (STR0) port_str );
		// disallow zero
		if(!port) return FAIL("bad port number-string '%s'", (STR0) port_str );
		return set_port_number( port );
	}

	//! set the port from its name string (in the "udp" or "tcp" domain)
	bool sock_addr_ip::set_port_name_proto( str0 port_name, str0 proto )
	{
		// allow "2222" as a portname // OK over "9 thing" // atoi
		// any protocol a number is a number
		// beware of byte order by calling sub func
		int port = atoi( (STR0) port_name );
		if( port ) {
			return set_port_number( port );
		}

		// not NIS
		struct servent * entry = getservbyname( (STR0) port_name, (STR0) proto );
		if(!entry) return FAIL("No %s service port '%s'", (STR0) proto, (STR0) port_name );
	// 	sa.sin_port = htons(entry->s_port); // NO // already in nbo
		sa.sin_port = entry->s_port;
		return true;
	}

	//! lookup the port name from /etc/services
	bool sock_addr_ip::set_port_name_tcp( str0 port_name )
	{
		return set_port_name_proto( (STR0) port_name, "tcp" );
	}

	//! lookup the port name from /etc/services
	bool sock_addr_ip::set_port_name_udp( str0 port_name )
	{
		return set_port_name_proto( (STR0) port_name, "udp" );
	}

	//! set the addr to 0 - so that bind will listen to all interfaces
	bool sock_addr_ip::set_addr_any()
	{
		sa.sin_addr.s_addr = htonl( INADDR_ANY );
		return true;
	}

	//! set the addr to "127.0.0.1" or other dotted string
	bool sock_addr_ip::set_addr_number( str0 a_b_c_d )
	{
#ifdef WIN32
	// inet_pton( AF_INET, (STR0) a_b_c_d, & sa4.sin_addr );
		unsigned long addr = inet_addr( (STR0) a_b_c_d );
		if( addr == INADDR_NONE ) return FAIL("inet_addr(s)", (STR0) a_b_c_d);
		// it might not be a FAIL, but fix on fail
		if( addr == INADDR_ANY ) FAIL("inet_addr(s) as INADDR_ANY", (STR0) a_b_c_d);
		sa.sin_addr.s_addr = htonl( addr ); // IP4
		WARN("untested convertion of %s (first is CTOR init)", (STR0) a_b_c_d);
		return true;
#else
		return inet_aton( (STR0) a_b_c_d, & sa.sin_addr );
#endif
	}

	//! get the addr as a string, eg "127.0.0.1"
	str0 sock_addr_ip::get_addr_number()
	{
		return inet_ntoa( sa.sin_addr );
	}


	//! (TEMP STATIC STR) ftp needs an address to call back as a string
	str0 sock_addr_ip::ftp_addr_str()
	{
		unsigned int a, a1, a2, a3, a4;
		a = ntohl( sa.sin_addr.s_addr );
		a4 = a & 0xff; a>>=8;
		a3 = a & 0xff; a>>=8;
		a2 = a & 0xff; a>>=8;
		a1 = a & 0xff; a>>=8;
		static char buff[ 4 * 4 + 10 ];
		sprintf( buff, "%d,%d,%d,%d", a1,a2,a3,a4 );
		INFO("# ADDR # ADDR STR # %s #", buff );
		return buff;
	}


#warning "ftp_port_str() presumes ftp connected over TCP"

	//! get addr/port as a string for use in ftp PORT command "a,b,c,d,H,l
	str0 sock_addr_ip::ftp_port_str()
	{
		unsigned int p, p1, p2;
		p = ntohs( sa.sin_port );
		p2 = p & 0xff; p>>=8;
		p1 = p & 0xff; p>>=8;
		static char buff[ 2 * 4 + 10 ];
		sprintf( buff, "%d,%d", p1,p2 );
		INFO("# ADDR # PORT STR # %s #", buff );
		return buff;
	}

	/*!
		This is strangly rare - the addr comes from the connected
		ctrl line, the port from the unconnected listener.
	*/
	str0 sock_addr_ip::ftp_addr_port_str()
	{
		static char buff[ 6 * 4 + 10 ];
		sprintf( buff, "%s,%s",
			(STR0)ftp_addr_str(),
			(STR0)ftp_port_str()
		);
		INFO("# ADDR # ADDR_PORT STR # %s #", buff );
		return buff;
	}

	//! get the port as a CPU byte order int
	int sock_addr_ip::get_port_number()
	{
		return ntohs( sa.sin_port );
	}

	//! eg set_addr_name( "www.Information-Cascade.co.uk" );
	bool sock_addr_ip::set_addr_name( str0 host )
	{
		INFO("calling gethostbyname(%s)", (STR0) host );
		struct hostent * h = gethostbyname( (STR0) host );
		if(!h) {
			err_int_t & err = BASE1::get_err_int_for_this_thread();
			err.eat_DNS_error();
			// let the MORE DETAIL tag extract this line?
			// or figure out attr-val pairs ("func", "gethostbyname)
			// or figure out attr-val pairs ("host", (STR0)host)
			return FAIL("DNS gethostbyname('%s') - %s",
				(STR0) host,
				(STR0) err.e_str );
			
		}
		PASS("gethostbyname(%s)", (STR0) host );
		memcpy( & sa.sin_addr, h->h_addr_list[0], h->h_length );
		return true;
	}

	//! get addr and port from an existing socket
	bool sock_addr_ip::get_socket_name_IP( int fd )
	{
		/*
			errm - what about different name types
			this shows the need for a common base class
			but in this case it would have to create a
			derived class instance to hold the actual -
			SO it must be of FIXED type TCP or  IP or ..
		*/
		socklen_t len = sa_len;
		int t = getsockname(
			fd, 
			sa_ptr,
			&len
		);
		if( t!=0 )
		{
			FAIL("FD %d # ERROR # getsockname()", fd);
			INFO("# len was/now %d/%d", sa_len, len );
			return false;
		}
		if( sa_len != len )
		{
			WARN("# len was/now %d/%d", sa_len, len );
		}
		return true;
	}


