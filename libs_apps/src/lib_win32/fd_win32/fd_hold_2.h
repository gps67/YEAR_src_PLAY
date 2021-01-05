#ifndef fd_hold_H
#define fd_hold_H

#include "obj_ref.h"

#ifdef WIN32

#include <stdio.h>

#else

struct _IO_FILE;
typedef struct _IO_FILE FILE;

#endif

#include <stdarg.h> // va_list
#include "blk1.h"


class sock_addr_ip;
class sock_addr_base;

/*!
	fd_hold_2 hold a UNIX file handle (0, 1, 2 .. )
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
class fd_hold_2 : public obj_ref
{
 protected:
	int fd;
 public:
	FILE * File;
	FILE * File_flush;

	bool blocks_badly;

	fd_hold_2( int _fd = -1 );
	fd_hold_2( FILE * f );
	~fd_hold_2();

	/*!
		has fd been set, (no test on still open)
	*/
	bool is_open()
	{
		return fd >= 0;
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

	/*
		ON WIN32 NEED TO KNOW fd_set_socket(fd) fd_set_nonsocket(fd)

		this is because the WINSOCK DLL uses _something_ in an _INT_
		whilst the LIBC/OS uses _something_else_

		The idea of fd_hold2 is to convey this somehow.
		Ironically, the old fd_hold1 does it sooner!
	*/
	void fd_set( int _fd );
//	void fd_set_socket( int _fd );
//	void fd_set_nonsocket( int _fd );


	int fd_steal();
	void clear_dont_close();

	bool set_non_block();
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
	bool do_ioctl_data_back( const char * name, int func, int * data );
	bool do_ioctl( const char * name, int func, void * data, int & retvar );

	bool do_fcntl( int & ret, int F_CMD );
	bool do_fcntl( int & ret, int F_CMD, long arg );
	bool do_fcntl( int & ret, int F_CMD, struct flock * lock );

	bool get_close_on_exec( bool &closes );
	bool set_close_on_exec( bool closes );

	int read( void * buff, int len );
	int read_quiet( void * buff, int len );
	int sock_recv( void * buff, int len );
	int sock_recv_from( void * buff, int len, sock_addr_base & addr );
	int write( const void * buff, int len );
	int write( const blk1 & buff );

	FILE * file();
	int vprint( const char * fmt, va_list args );
	int print( const char * fmt, ... );

	bool open_RO( str0 filename, bool async = false );
	bool open_RW( str0 filename, bool async = false );
	bool open_RW_CREATE( str0 filename, bool async = false );
	bool open_TCP( struct sockaddr_in* sa, bool async = false );
	bool open_TCP( sock_addr_ip & spec, bool async = false );
	bool open_AF_UNIX( str0 filename, bool async = false ); // client

	bool open_RO_async( str0 filename ) { return open_RO( filename, true ); }
	bool open_RW_async( str0 filename ) { return open_RW( filename, true ); }
	bool open_TCP_async( struct sockaddr_in* sa ) { return open_TCP( sa, true ); }
	bool open_TCP_async( sock_addr_ip & spec ) { return open_TCP( spec, true ); }
	bool open_AF_UNIX_async( str0 filename ) { return open_AF_UNIX( filename, true ); }

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

#endif
