#ifndef sock_addr_ip_H
#define sock_addr_ip_H

#include "sock_addr_base.h"
#ifndef WIN32
#include <netinet/in.h> // sockaddr_in
#endif
#include "str0.h"

/*
	sock_addr_ip is the ADDRESS of one end of an IP connection, or listener.

	Either you provide the parameters, and open the connection,
	or you accept a connection, and read the parameters of the
	other end.

	sock_addr_ip (for IP) is the IP address and IP port numbers,
	both of which can have names.

	IP numbers arrive as strings, and then need swapping to/from
	network-byte-order. Outside of this class, ints are CPU order.

	TODO: sort out the difference between sockaddr and sockaddr_in

	TODO: connect this class with AUTH_CHECK_ADDR

	There are several methods for converting names to numbers (and back),
	and different class varients will want to do things differently.
	The initial objective is to clarify the parameters required.

	TODO: use the correct functions that use /etc/nsswitch and
	the methods/order specified there. PLUS: other overrides.

	As wells as IP addresses, there are UNIX and XXX connections.
	This class presumes IP.

	Secure apps will want to decide when to use numbers,
	aliases from /etc/hosts or DNS (and write a bit more code).

	USAGE:
		sock_addr_ip addr;
		addr.set_addr_name( "www.python.org" );
		addr.set_port_number( "80" );

		fd_hold_1 sock;
		sock.open( addr );

*/
class sock_addr_ip : public sock_addr_base
{
	struct sockaddr_in sa;
 public:
	//! constructor - init sane
	sock_addr_ip();

	//! set the network byte ordered port number from a cpu ordered int
	bool set_port_number( int port );

	//! set the port from a string which is a number
	bool set_port_number( str0 port_str );

	//! set the port from its name string (in the "udp" or "tcp" domain)
	bool set_port_name_proto( str0 port_name, str0 proto );

	//! lookup the port name from /etc/services
	bool set_port_name_tcp( str0 port_name );

	//! lookup the port name from /etc/services
	bool set_port_name_udp( str0 port_name );

	//! set the addr to 0 - so that bind will listen to all interfaces
	bool set_addr_any();

	//! set the addr to "127.0.0.1" or other dotted string
	bool set_addr_number( str0 a_b_c_d );

	//! get the addr as a string, eg "127.0.0.1"
	str0 get_addr_number();


	//! (TEMP STATIC STR) ftp needs an address to call back as a string
	str0 ftp_addr_str();

	//! get addr/port as a string for use in ftp PORT command "a,b,c,d,H,l
	str0 ftp_port_str();

	/*!
		This is strangly rare - the addr comes from the connected
		ctrl line, the port from the unconnected listener.
	*/
	str0 ftp_addr_port_str();

	//! get the port as a CPU byte order int
	int get_port_number();

	//! eg set_addr_name( "www.Information-Cascade.co.uk" );
	bool set_addr_name( str0 host );

	//! get addr and port from an existing socket
	bool get_socket_name_IP( int fd );

};

#endif

