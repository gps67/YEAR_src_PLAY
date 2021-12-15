#ifndef fd_hold_H
#define fd_hold_H

#include "obj_ref.h"

#ifdef WIN32

#include <stdio.h>
#include "MY_WIN32.h"
#else

// NO CHANGE just faster no not include the full spec
// #include <stdio.h>
struct _IO_FILE;
typedef struct _IO_FILE FILE;

#endif

// #include <memory.h>
// #include <errno.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <unistd.h> // close
// #include <stdlib.h> // atoi
#include <stdarg.h> // va_list
// #include <fcntl.h> // O_NONBLOCK
// #include <sys/ioctl.h>
#include "blk1.h"


class sock_addr_ip;
class sock_addr_base;

#if 0
/*!
	There are a bunch of conditions possible
	recv returns >0 len == request_len
	recv returns >0 len < request_len
	recv returns 0 EOF
	recv returns -1 errno = ...

	UNIX style, W32 alternatives, libc mingw seems to wrap API

		EINTR		can retry immediately, handle other interrupt
		EAGAIN		retry but not immediately, no data

		EBADF		error - poss fd closed by other code
		ENOTSOCK	error - code error
		ENOTCONN	error - code error

		EINVAL		
		ENOMEM
		EFAULT		error - code error - pointer

	... what happens on LAN failure, or remote reboot, or remote close ?

	recv returns -1 GetLastError =

		WSANOTINITIALISED	- code error
		WSAEFAULT		- code error
		WSAENOTCONN		- code error
		WSAENOTSOCK		- code error
		WSAEOPNOTSUPP		- code error OOB 
		WSAESHUTDOWN		- code error - or after link fail ?
		WSAEMSGSIZE		- code error indicating MTU > buffer
		WSAEINVAL		- code error

		WSAENETDOWN		- link fail
		WSAENETRESET		- link fail
		WSAECONNABORTED		- link fail
		WSAETIMEDOUT		- link fail

		WSAECONNRESET		- link fail - remote closed

		WSAEINTR		- unused facility
		WSAEINPROGRESS		- unused facility or code error

		WSAEWOULDBLOCK		- EAGAIN


	// texts copied from MS man page for WSA-recv()
	{

		WSANOTINITIALISED

			A successful WSAStartup call must occur before
			using this function.

		WSAENETDOWN

			The network subsystem has failed.

		WSAEFAULT

			The buf parameter is not completely contained
			in a valid part of the user address space.

		WSAENOTCONN

			The socket is not connected.

		WSAEINTR

			The (blocking) call was canceled through
			WSACancelBlockingCall.

		WSAEINPROGRESS

			A blocking Windows Sockets 1.1 call is in
			progress, or the service provider is still
			processing a callback function.

		WSAENETRESET

			For a connection-oriented socket, this error
			indicates that the connection has been broken due
			to keep-alive activity detecting a failure while
			the operation was in progress. For a datagram
			socket, this error indicates that the time to
			live has expired.

		WSAENOTSOCK

			The descriptor is not a socket.

		WSAEOPNOTSUPP

			MSG_OOB was specified, but the socket is not
			stream-style such as type SOCK_STREAM, OOB data
			is not supported in the communication domain
			associated with this socket, or the socket is
			unidirectional and supports only send operations.

		WSAESHUTDOWN

			The socket has been shut down; it is not possible
			to receive on a socket after shutdown has been
			invoked with how set to SD_RECEIVE or SD_BOTH.

		WSAEWOULDBLOCK

			The socket is marked as nonblocking and the
			receive operation would block.

		WSAEMSGSIZE

			The message was too large to fit into the
			specified buffer and was truncated.

		WSAEINVAL

			The socket has not been bound with bind, or
			an unknown flag was specified, or MSG_OOB was
			specified for a socket with SO_OOBINLINE enabled
			or (for byte stream sockets only) len was zero
			or negative.

		WSAECONNABORTED

			The virtual circuit was terminated due to
			a time-out or other failure. The application
			should close the socket as it is no longer usable.

		WSAETIMEDOUT

			The connection has been dropped because of
			a network failure or because the peer system
			failed to respond.

		WSAECONNRESET

			The virtual circuit was reset by the remote
			side executing a hard or abortive close. The
			application should close the socket as it is no
			longer usable. On a UPD-datagram socket this error
			would indicate that a previous send operation
			resulted in an ICMP "Port Unreachable" message.

	}
 
*/
enum ret_read_t
{
	ret_read_CHECKWORLD_OOB, // OOB out of band data is available ("PRI")
	ret_read_CHECKWORLD_EINTR, // EINTR 
	ret_read_CHECKWORLD_EINTR, // EINTR 
};
#endif

/*!
	fd_hold_1 hold a UNIX file handle (0, 1, 2 .. )
	and knows how to do standard operations on it, such
	as setting non-blocking-mode.

	The fd gets closed when the fd_holder is deleted (C++),
	and this is a base class for event driven IO, sockets,
	and lots of things.

	There are some simplistic but useful routines to open files,
	sockets, convert to FILE *, and printf to them. That might not
	make sense with non-blocking-io.

	Some of this is speculative and will change as code is moved
	down from users to here, eg networking

	Alternatively, advanced usage with own opts (popen) might
	take code away, then call that from here ;-) However, it
	is nice to have simple ways of doing standard things

	FUTURE OPTION - it would be nice to give each fd a name,
	so that error messages make some sense
*/
class fd_hold_1 : public obj_ref
{
 protected:
	int fd;
 public:
	FILE * File;
	FILE * File_flush;

	bool blocks_badly;

	bool PRIV_is_socket; // WIN32 file <> WIN32 sock ?? WIN32 pipe 
	// I think that a WIN32 named pipe is probably a sock?
	// so should be an enum?
	// so use sock_recv instead of read (etc)

	fd_hold_1( int _fd = -1 );
	fd_hold_1( FILE * f );
	~fd_hold_1();

	/*!
		has fd been set, (no test on still open)
	*/
	bool is_open()
	{
		return fd >= 0;
	}

	/*!
		has fd been set, (no test on still open)
	*/
	bool is_not_open()
	{
		return fd < 0;
	}

	/*!
		You can letter your code with sanity checks, might throw
	*/
	bool expect_open() {
		if(is_open())
			return true;
		else
			return expect_open_fn();
	}
	bool expect_open_fn();

	//!	if not inline, could throw if not open
	int fd_get()
	{
		return fd;
	}
	void fd_set( int _fd );
	void fd_set_socket( int _fd );
	void fd_set_nonsocket( int _fd );
	int fd_steal();
	void clear_dont_close();

	bool set_non_block();
	bool check_non_block();
	bool set_block();
	bool close();

	void flush();
	//! sync calls fdatasync() to write data to disk (but not inode)
	bool sync() { return fdatasync(); }
	bool sync_system();
	bool fsync();
	bool fdatasync();
	bool do_ioctl( const char * name, int func, void * data );
	bool do_ioctl_can_fail( const char * name, int func, void * data );
	bool do_ioctl_int_ret( const char * name, int func, int * data );
	bool call_ioctl_data_retval( const char * name, int func, void * data, int *retval );
	bool do_fcntl( int & ret, int F_CMD );
	bool do_fcntl( int & ret, int F_CMD, long arg );
	bool do_fcntl( int & ret, int F_CMD, struct flock * lock );

	bool get_close_on_exec( bool &closes );
	bool set_close_on_exec( bool closes );

#ifdef WIN32
	bool ioctlsocket( long cmd, u_long * parg );
	bool ioctlsocket_FIONBIO( bool blocks );
	bool ioctlsocket_FIONBIO_block();
	bool ioctlsocket_FIONBIO_non_block();
	bool ioctlsocket_FIONREAD( u_long & nbytes );
//	bool ioctlsocket_FIONWRITE( u_long & nbytes );
	// see also WSAIoctl - for more WSA stuff
#endif

	int get_SO_ERROR();

	int read( void * buff, int len );
	int read_quiet( void * buff, int len );
	int sock_recv( void * buff, int len );
	int sock_recv_from( void * buff, int len, sock_addr_base & addr );
	int sock_send( void * buff, int len );
	int write( const void * buff, int len );
	int write( const blk1 & buff );
	virtual void zero_read_means_eof();

	FILE * file();
	int vprint( const char * fmt, va_list args );
	int print( const char * fmt, ... );

	bool open_RO( str0 filename, bool async = false );
	bool open_RW( str0 filename, bool async = false );
	bool open_RW_CREATE_modmask( str0 filename, bool async, mode_t modmask );
	bool open_RW_CREATE( str0 filename, bool async = false );
	bool open_TCP( struct sockaddr_in* sa, bool async = false );
	bool open_TCP( sock_addr_ip & spec, bool async = false );
	bool open_AF_UNIX( str0 filename, bool async = false ); // client

	bool open_RO_async( str0 filename ) { return open_RO( filename, true ); }
	bool open_RW_async( str0 filename ) { return open_RW( filename, true ); }
	bool open_TCP_async( struct sockaddr_in* sa ) { return open_TCP( sa, true ); }
	bool open_TCP_async( sock_addr_ip & spec ) { return open_TCP( spec, true ); }
	bool open_AF_UNIX_async( str0 filename ) { return open_AF_UNIX( filename, true ); }

	bool seek_SET_64( u64 pos );
	bool seek_SET_32( u32 pos );
	bool seek_REL_32( i32 pos );
	bool seek_END_32( i32 pos );

/*
	these can be moved to fd_hold
	a listener socket is much like a client socket (at this stage)
*/

	bool create_socket_tcp(); // throws
	bool create_socket_udp(); // throws
	bool create_socket_unix(); // throws
	bool create_socket_unix_dgram(); // throws
	bool create_socket_icmp(); // throws

	bool perr( const char * scall );

};

// EXTRA FUNCTIONS NOT IN CLASS

extern "C" bool CHECK_can_write_file( const char * filename );

#endif
