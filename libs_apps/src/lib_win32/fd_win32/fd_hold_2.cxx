/*
	fd_hold_2 - TODO - 

		remove FILE * as buffer2 can do that
		rework 'n' from read/write
		rework BOOL
		rework error message - silent on fail ?
		(proc watcher - syslog of errout, e_print also changes)
*/

#include "dgb.h"	// FAIL

// #include <stdio.h>
// #include <memory.h>
// #include <errno.h>

#ifdef WIN32

// #include <windows.h>
// #include <winsock.h>
// int ioctl (int fd, int func, void * data) { return -1; }

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // IPPROTO_ICMP
// #include <arpa/inet.h>
#include <netdb.h> // getprotobyname("icmp")
//
#include <sys/ioctl.h>
#include <sys/un.h>

#endif

#include <unistd.h> // close
// #include <stdlib.h> // atoi
#include <stdarg.h> // va_list
#include <fcntl.h> // O_NONBLOCK
//#include <malloc.h> // free(File)

#include "fd_hold_2.h"
#include "sock_addr_ip.h"
#include "sock_addr_unix.h"
#include "e_print.h"
#include "dgb_fail.h"
#include "buffer1.h"

static inline int get_IPPROTO_ICMP()
{
#if 1
	return IPPROTO_ICMP;
#else
		struct protoent * proto;
		proto = getprotobyname("icmp");
		if(!proto) {
			perr("getprotobyname-icmp unknown protocol");
			return 1;
		}
		return proto->p_proto;
#endif
}



	/*!
		fd_hold closes what you give it, unless you call clear_dont_close()
	*/
	fd_hold_2::~fd_hold_2()
	{
		if( is_open() ) close();
	}

	/*!
		fd_hold is intended to be a permanent field of a structure.
		You can pre-open fd, or call fd_set_socket(fd) later.
	*/
	fd_hold_2::fd_hold_2( int _fd )
	{
		fd = _fd;
		File = NULL;
		File_flush = NULL;
		blocks_badly = false; // most things are OK
	}

	/*!
		Constructor to hold an existing buffered FILE *
	*/
	fd_hold_2::fd_hold_2( FILE * f )
	{
		fd = -1;
		if( f ) fd = fileno( f );
		File = f;
		File_flush = NULL;
	}

	/* INLINE
		bool fd_hold_2::is_open()
		{
			return fd >= 0;
		}

		int fd_hold_2::fd_get()
		{
			return fd;
		}
	*/

	/*!
		Close any existing fd, and set the new one.
	*/
	void fd_hold_2::fd_set( int _fd )
	{
		if( is_open() ) close();
		fd = _fd;
	}

	/*!
		Take fd away from fd_hold, so that it wont be closed
	*/
	int fd_hold_2::fd_steal()
	{
		int fd1 = fd;
		flush();
		fd = -1;
		File = NULL;
		return fd1;
	}

	/*!
		Forget fd so that it wont be closed
	*/
	void fd_hold_2::clear_dont_close()
	{
		flush();
		fd = -1;
		File = NULL;
	}

	/*!
		The inline expect_open is intentionally light, and calls this
		if it fails. This could throw - not sure yet.
	*/
	bool fd_hold_2::expect_open_fn()
	{
		if(is_open()) return true;
		e_print( "expect_open() failed\n" );
		return false;
	}

	bool fd_hold_2::perr( const char * scall )
	{

		buffer1 tn;
		str2 * debug_name_str2 = debug_name_get();
		const char * debug_name_str = "name";
		if( debug_name_str2 )
		{
			debug_name_str = (STR0) debug_name_str2;
		}
		e_print(" # %s %d # (%s) # %s: %m\n",
			type_name(tn),
			fd_get(),
			debug_name_str,
			scall
		);
		return false;
	}

	/*!
		run fcntl, return true and place return int in ret
	*/
	bool fd_hold_2::do_fcntl( int & ret, int F_CMD )
	{
#ifdef WIN32
#warning "WIN32 has no fcntl()"
		return false;
#else
		ret = fcntl( fd, F_CMD );
		if( ret != -1 ) return true;

		// show EBADF for when files are not actually open
		// is_open only checks fd==-1, not much else
		if( !is_open() ) return false; // of course it failed

		const char * CMD = "F_CMD_unknown";
		switch( F_CMD )
		{
		 case F_GETFD: CMD = "F_GETFD"; break;
		 case F_GETFL: CMD = "F_GETFL"; break;
		 case F_GETOWN: CMD = "F_GETOWN"; break;
		 case F_GETSIG: CMD = "F_GETSIG"; break;
		 default: ; break;
		}

		e_print( "# ERROR # fcntl(%d,%s) %m \n", fd, CMD );
		return false;
#endif
	}

	/*!
	*/
	bool fd_hold_2::do_fcntl( int & ret, int F_CMD, long arg )
	{
#ifdef WIN32
		return false;
#else
		ret = fcntl( fd, F_CMD, arg );
		if( ret != -1 ) return true;

		// show EBADF for when files are not actually open
		// is_open only checks fd==-1, not much else
		if( !is_open() ) return false; // of course it failed

		const char * CMD = "F_CMD_unknown";
		switch( F_CMD )
		{
 		 case F_DUPFD: CMD = "F_DUPFD"; break;
		 case F_SETFD: CMD = "F_GETFD"; break;
		 case F_SETFL: CMD = "F_GETFL"; break;
		 case F_SETOWN: CMD = "F_GETOWN"; break;
		 case F_SETSIG: CMD = "F_GETSIG"; break;
		 default: ; break;
		}

		e_print( "# ERROR # fcntl(%d,%s,0x%xl) %m \n", fd, CMD, arg );
		return false;
#endif
	}

	/*!
	*/
	bool fd_hold_2::do_fcntl( int & ret, int F_CMD, struct flock * lock )
	{
		return false;
	}

	/*!
	*/
	bool fd_hold_2::get_close_on_exec( bool & closes )
	{
#ifdef WIN32
		return false;
#else
		int flag;
		if( ! do_fcntl( flag, F_GETFD ) ) return false;
		closes = flag & FD_CLOEXEC;
		return true;
#endif
	}

	/*!
	*/
	bool fd_hold_2::set_close_on_exec( bool closes )
	{
#ifdef WIN32
		return false;
#else
		
		int flags = 0;
		if( closes ) flags = FD_CLOEXEC;
		return do_fcntl( flags, F_SETFD, flags );
#endif
	}


	/*!
		NON-BLOCKING IO

		This cal also be set on the open() call,
		which may also take some time.
		Unfortunately ioctl()s often dont comply,
		as they may take time (/dev/cdrom)
	*/
	bool fd_hold_2::set_non_block()
	{
#ifdef WIN32
#warning	WIN32 -- set_non_block()
		return false;
#else
		int flags = 0;
		if(! do_fcntl( flags, F_GETFL ) ) return false;
		flags |= O_NONBLOCK;
		if(! do_fcntl( flags, F_SETFL, flags ) ) return false;
		return true;
		/*
			now read() returns when no more data available NOW
			and write() returns when no buffer space available
			(code should hold in its own buffer, do its own XOFF)
		*/
#endif
	}

	/*!
		End of NON-BLOCKING IO
	*/
	bool fd_hold_2::set_block()
	{
#ifdef WIN32
#warning	WIN32 -- set_block()
		return false;
#else
		int flags = 0;
		if(! do_fcntl( flags, F_GETFL ) ) return false;
		flags &= ~O_NONBLOCK;
		if(! do_fcntl( flags, F_SETFL, flags ) ) return false;
		return true;
#endif
	}

	/*!
		close the fd, flushing any data
	*/
	bool fd_hold_2::close()
	{
		int errno_old = errno;
		errno = 0;
		flush();
		if( is_open() )
		{
			::close( fd );
			if(errno) perr( "close" );
			errno = errno_old;
		}
		fd = -1;
		if( File ) {
			fclose( File );
			// BAD IDEA // free( File );
			File = NULL;
		}
		File_flush = NULL;
		return true;
	}

	/*!
		Obtain the FILE * for this fd, open from fd if necessary (rw?)
	*/
	FILE * fd_hold_2::file()
	{
		if( !File ) {
			if( !is_open()) {
				fprintf(stderr,"file not open");
				// throw "file not open";
				return NULL;
			}
			// This uses malloc() ?
			File = fdopen( fd, "w+" );
			if(!File) File = fdopen( fd, "r+" );
			if(!File) File = fdopen( fd, "r" );
			if(!File) File = fdopen( fd, "w" );
			if(!File)
				perr( "file()" );
		}
		return File;
	}

	/*!
		simplistic wrap over read()

		SPECIAL WITH ASYNC NDELAY:

			select_says_read() on socket connect
			select_says_read() on pipe connect

		READING UNTIL/AFTER PIPE IS EMPTY:

			 Resource temporarily unavailable

	*/
	int fd_hold_2::read( void * buff, int len )
	{
		// e_print
		int t = ::read( fd, buff, len );
		if( t < 0 )
		{
			e_print("fd_hold_2::read(%d) returned -1 %m\n", fd );
		}
		return t;
	}

	/*!
	*/
	int fd_hold_2::read_quiet( void * buff, int len )
	{
		// e_print
		int t = ::read( fd, buff, len );
		if( t < 0 )
		{
		 if(0) // should still allow some sense ?
			e_print("fd_hold_2::read(%d) returned -1 %m\n", fd );
		}
		return t;
	}

	/*!
		write a buffer1, return nbytes actually written
	*/
	int fd_hold_2::write( const blk1 & buff )
	{
		return write( buff.buff, buff.nbytes_used );
	}

	/*!
		simple wrapper over write - no perror, or bool-truncation
	*/
	int fd_hold_2::write( const void * buff, int len )
	{
		flush();
		if( len <= 0 ) return 0;
		int t = ::write( fd, buff, len );
		if( t < len ) {
			//
			// NONBLOCK validly writes less and returns
			//
		}
		return t;
	}

	/*!
		simple (unused) - maybe add sock_recvfrom() - maybe use blk1 ?
	*/
	int fd_hold_2::sock_recv_from( void * buff, int len, sock_addr_base & addr )
	{
		int flags = 0; // MSG_OOB
		int t = recvfrom(
			fd_get(),
			(char *)buff,
			len,
			flags,
			addr.sa_ptr,	// might still be NULL
			& addr.sa_len	// HMMM, overwrite known len with len
		);
		return t;		// nbytes read
	}

	/*!
		simple (unused) - maybe add sock_recvfrom() - maybe use blk1 ?
	*/
	int fd_hold_2::sock_recv( void * buff, int len )
	{
		int flags = 0; // MSG_OOB
		sockaddr * from = NULL;
		socklen_t fromlen = 0; // protect from overwrite ?
		int t = recvfrom( fd_get(), (char *)buff, len, flags, from, & fromlen );
		return t;
	}

	/*!
		OLD - requires FILE* and non-blocking

		USER SHOULD - vprint to buffer1 then add that to an output queue
	*/
	int fd_hold_2::vprint( const char * fmt, va_list args )
	{
		FILE * f = file();
		File_flush = f; // FLAG that flush would be useful
		int t = vfprintf( f, fmt, args );
		return t;

	/*
		// SYNOPSIS USAGE
		int CLASS2::print( const char * fmt, ... )
		{
			va_list args;
			va_start( args, fmt );
			int t = FD.vprint( fmt, args );
			va_end( args );
			return t;
		}
	*/

	}

	/*!
	*/
	int fd_hold_2::print( const char * fmt, ... )
	{
		va_list args;
		va_start( args, fmt );
		FILE * f = file();
		File_flush = f; // FLAG that flush would be useful
		int t = vfprintf( f, fmt, args );
		va_end( args );
	if(1) flush();
		return t;

	}

	/*!
		calls fflush()

		That flushes any data already written, but doesnt wait
		for any effect, response or potentially buffered data.

		this can trigger SIGPIPE or other write errors

		TODO: conv to bool
	*/
	void fd_hold_2::flush()
	{
		if( File_flush )
		{
			fflush( File_flush );
			File_flush = NULL;
		}
	}

	/*!
		sync_system calls system sync(2)

		That writes all files, all disks, data and inodes.

		On Linux it waits, on SVR4, the first call waits,
		the second (concurrent) request returns immediately.
	*/
	bool fd_hold_2::sync_system()
	{
		fflush(0); // this processes queues
#ifndef WIN32
		::sync();
#endif
		return true;
	}

	/*!
		fsync the files data, and inode
	*/
	bool fd_hold_2::fsync()
	{
		if(!is_open() ) return true;
		if( File_flush ) flush();
#ifdef WIN32
		return true;
#else
		if( ::fsync( fd ) )
		{
			perr("fsync");
			return false;
		}
		else return true;
#endif
	}

	/*!
		fsync the files data, but not necessarily the inode
	*/
	bool fd_hold_2::fdatasync()
	{
		if(!is_open() ) return true;
		if( File_flush ) flush();
#ifdef WIN32
		return true;
#else
		if( ::fdatasync( fd ) )
		{
			perr("fdatasync");
			return false;
		}
		else return true;
#endif
	}

	/*!
		some ioctls are well known to fail (eg CDROM MCN)
		so only report if ioctl failed.
		is_open() is still expected
	*/
	bool fd_hold_2::do_ioctl_can_fail( const char * name, int func, void * data )
	{
		if(!is_open()) {
			e_print("DEVICE NOT OPEN for %s\n", name );
			return false;
		}
#ifndef WIN32
		if (0 == ioctl (fd, func, data) ) return true;
#endif
		return false;
	}


	/*!
		some retval ioctls take no parameter and return a value
		instead of a 0==OK flag
	*/
	bool fd_hold_2::do_ioctl(
		const char * name,
		int func,
		void * data,
		int & retval
	)
	{
		if(!is_open()) {
			e_print("DEVICE NOT OPEN for %s\n", name );
			return false;
		}
#ifdef WIN32
		FAIL("WIN32 has no ioctl debugname=%s\n", name );
		/*
			PUSH about_to_trim_trailing
			PASS data_multi_part_lines
			HERE trim_trailing_nl_when_adding_extra
			OUTPUT_BUFFER COPY_PARSER bool trim_pending; // extra_info
		*/
		return false;
#else
		retval = ioctl( fd, func, data );
		if( retval == -1 ) {
			return FAIL("%d = do_ioctl(%d,%s,DATA)", retval, fd, name );
		} else {
			return true;
		}
#endif
	}

	bool fd_hold_2::do_ioctl_data_back(
		const char * name,
		int func,
		int * data
	)
	{
		int retval = -1;
		if(! do_ioctl( name, func, data, retval )) return FALSE;
		*data = retval;
		return true;
	}

	/*!
	*/
	bool fd_hold_2::do_ioctl( const char * name, int func, void * data )
	{
		if(!is_open()) {
			e_print("DEVICE NOT OPEN for %s\n", name );
			return false;
		}
		int retval = -1;
		bool ok = do_ioctl( name, func, data, retval );
		if(!do_ioctl( name, func, data, retval )) {
			return FAIL("do_ioctl_int_ret() returned false"); // already reported
			// return FAIL(); // already reported
			return false; // already reported
		}
		return ok;
	}

	/*!
		Theres an infinite variety of open calls,
		then more for sockets, pipe options, locks, etc.
		RO and RW is plenty for many devices
	*/
	bool fd_hold_2::open_RO( str0 filename, bool async )
	{
		close();
		int flags = O_RDONLY;
#ifndef WIN32
		if( async) flags |= O_NONBLOCK;
#endif
		fd = ::open( filename, flags );
		if( fd == -1 )
		{
			perr( filename );
			return false;
		}
		return true;
	}

	/*!
		open a new file, or truncate an old one!

		Note that (on unix) the file permissions is rw-rw-rw
		COMBINED WITH umask (if the file is created).
	*/
	bool fd_hold_2::open_RW_CREATE( str0 filename, bool async )
	{
		close();
		int flags = O_RDWR | O_CREAT | O_TRUNC;
#ifndef WIN32
		if( async) flags |= O_NONBLOCK;
#endif
		mode_t mode = 0666;
		fd = ::open( filename, flags, mode );
		if( fd == -1 )
		{
			perr( filename );
			return false;
		}
		return true;
	}

	/*!
		open filename or device for reading and writing

		async means wait for read, write, open, close.
		fcntl always seems to wait on cdroms media change!

		if file doesnt exist ...
	*/
	bool fd_hold_2::open_RW( str0 filename, bool async )
	{
		close();
		int flags = O_RDWR ; 
#ifndef WIN32
		if( async) flags |= O_NONBLOCK;
#endif
		fd = ::open( filename, flags );
		if( fd == -1 )
		{
			perr( filename );
			return false;
		}
		return true;
	}

	/*
		this is repeated in sock .. move here .. there .. split to parts

		fd_hold is really meant to HOLD an opened fd, opening it
		is a bit of a convenience.

		In the same way that open_RO("filename") is useful,
		so are socket openers (clients).
	*/

	/*!
		create a socket and connect to remote socket address sa
	*/
	bool fd_hold_2::open_TCP( struct sockaddr_in* sa, bool async )
	{
		if(! create_socket_tcp() )
			return false;
		if(async) set_non_block();
		if( 0 != ::connect(
			fd,
			(struct sockaddr*) sa,
			sizeof(*sa)
		)) {
			// man connect // wait for socket writable
			switch( errno ) {
#ifndef WIN32
			 case EINPROGRESS: return true;
#endif
			}
			perr("connect");
			return false;
		}
		return true;
	}

	/*!
		create a socket and connect to remote address SPEC
	*/
	bool fd_hold_2::open_TCP( sock_addr_ip & spec, bool async )
	{
		return open_TCP( (struct sockaddr_in*) spec.sa_ptr, async );
	}

#define UNIX_PATH_MAX    108
	/*!
		create a unix socket, and connect to filename (FIFO)

		A unix socket is usually a filename (of a mknod -p) but it can also 
		be an abstract pathname, with an initial NUL and ...

		Unix sockets can pass file descriptors uid pid ...
	*/
	bool fd_hold_2::open_AF_UNIX( str0 filename, bool async ) // client
	{
#ifdef WIN32
		return false;
#else
		if(! create_socket_unix() )
			return false;

		struct sockaddr_un sa;
		sa.sun_family=AF_UNIX;
		// why doesnt this need strdup, max len ?
		// why is this hardcoded in un.h ?
		// man 4 unix - #define UNIX_PATH_MAX    108
		//
		//              struct sockaddr_un {
       		//            sa_family_t sun_family;  /* AF_UNIX */
		//                   char sun_path[UNIX_PATH_MAX]; /* pathname */
       		//       };

		if( filename.str_len() >= UNIX_PATH_MAX )
		{
			errno = 22;
			perr( filename );
			return false;
		}
		strncpy( sa.sun_path, filename, UNIX_PATH_MAX );

		if( async ) set_non_block();

		if( 0 != ::connect(
			fd,
			(struct sockaddr*) & sa,
			sizeof(sa)
		)) {
			// man connect // wait for socket writable
			switch( errno ) {
			 case EINPROGRESS: return true;
			}
			perr( filename );
			// if connect failed, close fd to indicate that
			// multiple address attempts will be slighly slower
			// but most work first time
			close();
			return false;
		}
		return true;
#endif // WIN32
	}


	/*
		these were moved to fd_hold from sock_xxx
		a listener socket is much like a client socket (at this stage)
	*/

	/*!
                fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	*/
	bool fd_hold_2::create_socket_tcp()
	{
		close();
                fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
		if( fd == -1 ) {
			perr("socket");
			return FALSE_dgb_fail( "socket" );
		}
		return true;
	}

	/*!
                fd = socket (AF_INET, SOCK_DGRAM, 0);
	*/
	bool fd_hold_2::create_socket_udp()
	{
		close();
                fd = socket (AF_INET, SOCK_DGRAM, 0); // IPPROTO_UDP);
		if( fd == -1 ) {
			return FALSE_dgb_fail( "socket" );
		}
		return true;
	}

	/*!
		fd=socket( AF_UNIX,SOCK_STREAM, 0 );
	*/
	bool fd_hold_2::create_socket_unix()
	{
		close();
		fd=socket( AF_UNIX, SOCK_STREAM, 0 );
		if( -1 == fd ) {
			perr("socket");
			return false;
		}
		return true;
	}


	/*!
		fd=socket( AF_UNIX,SOCK_DGRAM, 0 );
	*/
	bool fd_hold_2::create_socket_unix_dgram()
	{
		close();
		fd=socket( AF_UNIX, SOCK_DGRAM, 0 );
		if( -1 == fd ) {
			perr("socket");
			return false;
		}
		return true;
	}



	/*!
		fd=socket( AF_UNIX,SOCK_RAW, proto_ent_icmp );
	*/
	bool fd_hold_2::create_socket_icmp()
	{
		close();
		int ip_proto_icmp = get_IPPROTO_ICMP();

		fd=socket( AF_INET, SOCK_RAW, ip_proto_icmp );
		if( -1 == fd ) {
			perr("socket");
			return false;
		}
		return true;
	}

