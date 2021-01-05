#ifndef sock_server_H
#define sock_server_H

#include "sock_addr_ip.h"
#include "sock_addr_unix.h"
#include "fd_selectable.h"
#include "sock_addr_unix.h"

/*
	I'm not sure how to redesign this code.

	Too many virtual functions is too much (maybe).

	I dont do multimple inheritance much, instead I put the functions
	in the base code, or inherit such layers upto the 'base2' layer.

	You can also wait until the stream is opened, THEN take its fd
	elsewhere (ie UNIX/libc has already provided an overloaded fd).

	EG Is this a 'line_based' fd with virtual got_line()?

	EG Is this a TCP read-means-opening or UDP read-means-packet?

	SO: The TCP 'cots' branch of the server is presumed to accept
	connections, and that fd is then used in an app class. The UDP
	branch does this for every packet, so its closer.

*/

/*!
	The server handles NEW connection requests, and creates sessions.

	UDP has no proper session concept, so receives individual packets.

	TCP needs to accept then read
*/
class sock_server_base : public fd_selectable_base
{
 public:
	sock_server_base();
	bool setsockopt_bool( int SocketOption, bool value );
	bool setsockopt_TTL( int ttl );
	bool bind_any( int port );
	bool bind_lo( int port );
	bool bind( sock_addr_ip & spec );
	bool bind( sock_addr_UNIX & spec );
	bool listen( int backlog = 5 ); // TCP not udp
	bool close_listener(const char * msg = NULL);
	void hex_dump( void * buf, int len );
};

// ############################################################################

/*!
	Connection Oriented Transport Stream - TCP and STREAM 

	With TCP the server is readable to indicate a connection request,
	which needs to be accepted.
*/
class sock_server_cots : public sock_server_base
{
 public:
};

/*!
	Connection-less transport service - UDP and DGRAM (not tcp UNIX)


	With UDP, the readability includes the data packet and peerinfo
	(well almost). 
*/
class sock_server_clts : public sock_server_base
{
 public:
};

// ############################################################################

class sock_server_tcp : public sock_server_cots
{
 public:
	sock_server_tcp();
	bool restart( int port = 1514 );
	bool select_says_read();
	virtual void fork_session_ip( /*const*/ sock_addr_ip & spec, int fd2 )=0;
};

// ############################################################################

class sock_server_unix : public sock_server_cots
{
 public:
	sock_server_unix();
	bool restart( str0 FIFO );
	bool select_says_read();
	virtual void fork_session_unix( /*const*/ sock_addr_UNIX & spec, int fd2 )=0;

};


// ############################################################################

class sock_server_udp : public sock_server_clts
{
 public:
	sock_server_udp();
	bool restart( int port = 1514 );
	bool select_says_read();
};
// ############################################################################

class sock_server_unix_dgram : public sock_server_clts
{
 public:
	sock_server_unix_dgram();
	bool restart( str0 FIFO );
	bool select_says_read();

};

// ############################################################################

class sock_server_icmp : public sock_server_clts
{
 public:
	sock_server_icmp();
	bool restart();
	bool select_says_read();
	virtual void icmp_packet_in( char * buff, int len, sock_addr_ip & from );
};
// ############################################################################

#endif
