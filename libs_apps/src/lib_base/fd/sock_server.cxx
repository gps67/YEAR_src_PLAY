
#include "sock_server.h"
#include <errno.h>
#include "buffer2.h"
#include "dgb_fail.h"
#include "dgb.h"

#include <unistd.h> // unlink
#include <sys/stat.h> // chmod, 

// ICMP_ECHO    
#ifndef WIN32
#include <netinet/ip_icmp.h>
#endif
   

/*!
	The server handles NEW connection requests, and creates sessions.

	UDP has no proper session concept, so receives individual packets.
*/
	sock_server_base::sock_server_base()
	: fd_selectable_base()
	{
	}


	/*!
		set a boolean socket option
	*/
	bool sock_server_base::setsockopt_bool( int SocketOption, bool value )
	{
		int optval = 0;
		if( value ) optval = 1;
#ifndef WIN32
		socklen_t optlen = sizeof( optval );
		if( setsockopt(
			fd_get(),
			SOL_SOCKET, // typical
			SocketOption,
			&optval,
			optlen
		 )) {
			perr("setsockopt");
			return false;
		}
		return true;
#endif
		return false;
	}

	/*!
		set the IP_TTL socket option
	*/
	bool sock_server_base::setsockopt_TTL( int ttl ) // default 64
	{
		if(1) INFO("setsockopt_TTL(%d)", ttl );
#ifdef WIN32
#warning setsocketopt_TTL() - set Time TO Live on packet
#else
		int SocketOption = IP_TTL;
		socklen_t optlen = sizeof( ttl );
		if( setsockopt(
			fd_get(),
			IPPROTO_IP, // ** CHANGED FROM SOL_SOCKET **
			SocketOption,
			&ttl,
			optlen
		 )) {
			perr("setsockopt_TTL");
			return false;
		}
		return true;
#endif
		return false;
	}

	/*!
		bind socket to port on any/each interface
	*/
	bool sock_server_base::bind_any( int port )
	{
		sock_addr_ip spec;
		spec.set_port_number(port);
		spec.set_addr_any();
		return bind( spec );
	}

	/*!
		bind socket to port on local-loopback interface only (security)
	*/
	bool sock_server_base::bind_lo( int port )
	{
		sock_addr_ip spec;
		spec.set_port_number(port);
		spec.set_addr_number("127.0.0.1");
		return bind( spec );
	}

// above should be IP ?

	/*!
		bind socket to address
	*/
	bool sock_server_base::bind( sock_addr_ip & spec )
	{
		if( ::bind( fd_get(), spec.sa_ptr, spec.sa_len ))
		{
			perr("bind");
			close();
			return false;
		}
		return true;
	}

	/*!
		bind socket to address - UNIX

		should be a common subclass
	*/
	bool sock_server_base::bind( sock_addr_UNIX & spec )
	{
		if( ::bind( fd_get(), spec.sa_ptr, spec.sa_len ))
		{
			perr("bind");
			close();
			return false;
		}
		return true;
	}

	/*!
		debugging - hex_dump buf/len to e_print()
	*/
	void sock_server_base::hex_dump( void * buf, int len )
	{
		buffer2 prt;
		prt.hex_dump_into_8859( buf, len );
		e_print("%s", prt.buff );
	}

	/*!
		switch listening on, with a backlog of upto
		5 connection indicators
	*/
	bool sock_server_base::listen( int backlog ) // TCP not udp
	{
		// int backlog = 5;	// typical
		if( ::listen( fd_get(), backlog ) )
		{
			perr("listen");
			close();
			return false;
		}
		return true;
	}

	bool sock_server_base::close_listener( const char * msg )
	{
		if(eof_seen) return true;
		if( !msg ) {
			msg = "''";
		}
		if( msg ) {
			INFO("FD %d %s %s", fd, debug_get_obj_name(), msg );
		}
		unsubscribe();
		close();
		return true;
	}


// ############################################################################

	// COTS select_says_read --> accept + fork_session

	/*!
		When a bound/listening socket is readable,
		a connection request is happening, so accept it
	*/
	bool sock_server_tcp::select_says_read()
	{
		if(0) STEP("select says read - so do_accept()");
		// the callers address
		sock_addr_ip spec_cli;
		socklen_t len = spec_cli.sa_len;
		// accept using the specificly typed client address
		int fd2 = ::accept( fd_get(), spec_cli.sa_ptr, &len );
		if(fd2<0)
		{
			perr("accept");
			return false;
		}
		fork_session_ip( spec_cli, fd2 );
		// listener might have closed - single use listener (or error)
		if(!is_open()) return false;
		return true; // accept more
	}

	/*!
		When a bound/listening socket is readable,
		a connection request is happening, so accept it
	*/
	bool sock_server_unix::select_says_read()
	{
		if(0) STEP("select says read - so do_accept()");
		// the callers address
		sock_addr_UNIX spec_cli;
		socklen_t len = spec_cli.sa_len;
		// accept using the specificly typed client address
		int fd2 = ::accept( fd_get(), spec_cli.sa_ptr, &len );
		if(fd2<0)
		{
			perr("accept");
			return false;
		}
		fork_session_unix( spec_cli, fd2 );
		if(!is_open()) return false;
		return true;
	}

// ############################################################################

	/*!
		constructor - noop
	*/
	sock_server_udp::sock_server_udp()
	: sock_server_clts()
	{
	}

	/*!
		start/restart UDP server on port on any interface
	*/
	bool sock_server_udp::restart( int port )
	{
		// fd_hold provides create socket
		if(!create_socket_udp()) return false;

		if(!setsockopt_bool( SO_REUSEADDR, true )) {
			// continue
		}

		if(!bind_any(port)) return false;

/*
		if(!listen()) return false;
*/
		set_non_block();
		subscribe_each(); // to select
		return true;
	}

	/*!
		udp server is different from tcp server, it doesnt
		accept and 'fork' new sessions, the one socket is used,
		(A sub-class would duplicate and override this func).

		Obtain the callers address (which might be faked) and
		hex_dump the packet.
	*/
	bool sock_server_udp::select_says_read()
	{
		if(1) STEP("UDP - readable");
		int len = 2048;
		char buf[len];
		unsigned int flags = 0;
#ifndef WIN32
		flags |=   MSG_NOSIGNAL;
#endif
		// struct sockaddr from;
		// sock_addr_ip CURRENTLY presumes IP not AF_UNIX or other
		sock_addr_ip from;
		socklen_t fromlen = from.sa_len;
		
		int t = recvfrom(
			fd_get(),
			buf,
			len,
			flags,
			from.sa_ptr,
			&fromlen
		);
		if( t < 0 )
		{
			FAIL("# ERROR %d recvfrom(%d)", t, fd_get() );
			perr("recvfrom(UDP)");
			return true; // dont delete selector
			return false;
		}
		e_print("UDP packet from %s:%d", (STR0) from.get_addr_number(), from.get_port_number() );
		hex_dump( buf, t );

		if(!is_open()) return false;
		return true; //
	}

// ############################################################################

	/*!
		constructor - noop
	*/
	sock_server_tcp::sock_server_tcp()
	: sock_server_cots()
	{
	}

	/*!
		start a TCP listener on port (any interface)

		programs would only use a different function to set
		specific interfaces, as fork_session() is the leverage point.
	*/
	bool sock_server_tcp::restart( int port )
	{
		create_socket_tcp();

		if(!setsockopt_bool( SO_REUSEADDR, true )) {
			// continue
		}

		if(!bind_any(port)) return false;

		if(!listen()) return false;
		set_non_block();
		subscribe_each(); // to select
		return true;
	}


// ############################################################################


	/*!
		constructor - noop
	*/
	sock_server_unix::sock_server_unix()
	: sock_server_cots()
	{
	}

	/*!
		start a UNIX listener on named FIFO

		programs would only use a different function to set
		specific interfaces, as fork_session() is the leverage point.
	*/
	bool sock_server_unix::restart( str0 FIFO )
	{
		create_socket_unix();

		if(!setsockopt_bool( SO_REUSEADDR, true )) {
			// continue
		}

		sock_addr_UNIX spec;
		spec.set_pathname(FIFO);

		unlink( FIFO );
		if(!bind( spec )) return false;
		chmod( FIFO, 0666);

		if(!listen()) return false;

		set_non_block();
		subscribe_each(); // to select
		return true;
	}

// ############################################################################


	/*!
		constructor - noop
	*/
	sock_server_unix_dgram::sock_server_unix_dgram()
	: sock_server_clts()
	{
	}

	/*!
		start a UNIX listener on named FIFO

		programs would only use a different function to set
		specific interfaces, as fork_session() is the leverage point.
	*/
	bool sock_server_unix_dgram::restart( str0 FIFO )
	{
		create_socket_unix_dgram();

		if(!setsockopt_bool( SO_REUSEADDR, true )) {
			// continue
		}

		sock_addr_UNIX spec;
		spec.set_pathname(FIFO);

		unlink( FIFO );
		if(!bind( spec )) return false;
		chmod( FIFO, 0666);

		// DGRAM doesnt LISTEN
		// if(!listen()) return false;

		set_non_block();
		subscribe_each(); // to select
		return true;
	}

	bool sock_server_unix_dgram::select_says_read()
	{
		if(0) STEP("DGRAM - readable");
		int len = 2048;
		char buf[len];
		unsigned int flags = 0;
#ifndef WIN32
		flags |=   MSG_NOSIGNAL;
#endif
		// struct sockaddr from;
		// sock_addr_ip CURRENTLY presumes IP not AF_UNIX or other
		sock_addr_ip from;
		socklen_t fromlen = from.sa_len;
		
		int t = recvfrom(
			fd_get(),
			buf,
			len,
			flags,
			from.sa_ptr,
			&fromlen
		);
		if( t < 0 )
		{
			FAIL("FD %d %s # recvfrom()", fd_get(), debug_get_obj_name() );
			perr("recvfrom(UNIX)");
			return true; // dont delete selector
			return false;
		}
		buf[t] = '\0';
		e_print("DGRAM %s", (STR0) buf );
#warning "this is wrong"
		// e_print("UNIX packet from %s:%d", (STR0) from.get_addr_number(), from.get_port_number() );
		// hex_dump( buf, t );
/*
<13>Feb 22 22:51:01 logger: hello 
*/


		return true; //
	}

// ############################################################################

	

	/*!
		constructor - noop
	*/
	sock_server_icmp::sock_server_icmp()
	: sock_server_clts()
	{
	}

	/*!
		start/restart UDP server on port on any interface
	*/
	bool sock_server_icmp::restart()
	{
		// fd_hold provides create socket
		if(!create_socket_icmp()) return false;

#if 1
		if(!setsockopt_bool( SO_REUSEADDR, true )) {
			// continue
		}
#endif

#if 0
		if(!bind_any(port)) return false;
#endif

/*
		if(!listen()) return false;
*/
#if 0
		set_non_block();
#endif
		if(1) STEP("ICMP - non_block and subscribe");
		// gdb_invoke(true);
		set_non_block();
		subscribe_each(); // to select
		return true;
	}

	/*!
		udp server is different from tcp server, it doesnt
		accept and 'fork' new sessions, the one socket is used,
		(A sub-class would duplicate and override this func).

		Obtain the callers address (which might be faked) and
		hex_dump the packet.
	*/
	bool sock_server_icmp::select_says_read()
	{
		if(0) STEP("ICMP - readable");
		int len = 2048;
		char buf[len];
		unsigned int flags = 0;
#ifndef WIN32
		flags |=   MSG_NOSIGNAL;
#endif
		// struct sockaddr from;
		// sock_addr_ip CURRENTLY presumes IP not AF_UNIX or other
		sock_addr_ip from;
		socklen_t fromlen = from.sa_len;
		
		int t = recvfrom(
			fd_get(),
			buf,		// note 2048 is much more than needed
			len,
			flags,
			from.sa_ptr,
			&fromlen
		);
		if( t < 0 )
		{
			FAIL("FD %d %s # recvfrom()", fd_get(), debug_get_obj_name() );
			perr("recvfrom(ICMP)");
			return true; // dont delete selector
			return false;
		}
	//	e_print("ICMP packet from %s:", (STR0) from.get_addr_number() );
		icmp_packet_in( buf, t, from );

		return true; //
	}

	/*
	 */
	void sock_server_icmp::icmp_packet_in( char * buff, int len, sock_addr_ip & from )
	{
		if(1) e_print("ICMP packet from %s:", (STR0) from.get_addr_number() );
#ifdef WIN32
#warning WIN32 - ICMP
#else
		// the packet is an IP one
		struct icmp * hdr = (icmp *) buff;
		buff += 8;
		len -= 8;
		e_print( "%-10s %d", "icmp_type", hdr->icmp_type );
		e_print( "%-10s %d", "icmp_code", hdr->icmp_code );
		e_print( "%-10s %d", "icmp_cksum", hdr->icmp_cksum );
		e_print( "%-10s %d", "icmp_id",  hdr->icmp_id );
#endif

		hex_dump( buff, len );
	}

// ############################################################################
	
