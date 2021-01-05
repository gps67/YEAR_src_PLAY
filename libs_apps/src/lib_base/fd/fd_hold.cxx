/*
	fd_hold_1 - TODO - 

		remove FILE * as buffer2 can do that
		rework 'n' from read/write
		rework BOOL
		rework error message - silent on fail ?
		(proc watcher - syslog of errout, e_print also changes)
*/


#include <stdio.h>
// #include <memory.h>
#include <errno.h>

#ifdef WIN32

#include <winsock.h>
#warning WIN32_has_no_ioctl
//int ioctl (int fd, int func, void * data) { return -1; }

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

#include "fd_hold.h"
#include "sock_addr_ip.h"
#include "sock_addr_unix.h"
#include "dgb_fail.h"
#include "dgb.h"
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
	fd_hold_1::~fd_hold_1()
	{
		if( is_open() ) close();
	}

	/*!
		fd_hold is intended to be a permanent field of a structure.
		You can pre-open fd, or call fd_set_XXX(fd) later.
	*/
	fd_hold_1::fd_hold_1( int _fd )
	{
		fd = _fd;
		File = NULL;
		File_flush = NULL;
		blocks_badly = false; // most things are OK
		PRIV_is_socket = false;
	}

	/*!
		Constructor to hold an existing buffered FILE *
	*/
	fd_hold_1::fd_hold_1( FILE * f )
	{
		fd = -1;
		if( f ) fd = fileno( f );
		File = f;
		File_flush = NULL;
	}

	/* INLINE
		bool fd_hold_1::is_open()
		{
			return fd >= 0;
		}

		int fd_hold_1::fd_get()
		{
			return fd;
		}
	*/

	/*!
		Close any existing fd, and set the new one.
		NB rewrite your code to use fd_set_socket _non
		or better dont accept fd's from strangers
		without guessing is remote or device or STO or
	*/
	void fd_hold_1::fd_set( int _fd )
	{
		if( is_open() ) close();
		FAIL("CODE ERROR need to set PRIV_is_socket");
		PRIV_is_socket = true;
		fd = _fd;
	}

	/*!
		Close any existing fd, and set the new one.
	*/
	void fd_hold_1::fd_set_nonsocket( int _fd )
	{
		if( is_open() ) close();
		PRIV_is_socket = false;
		fd = _fd;
	}

	/*!
		Close any existing fd, and set the new one.
	*/
	void fd_hold_1::fd_set_socket( int _fd )
	{
		if( is_open() ) close();
		PRIV_is_socket = true;
		fd = _fd;
	}

	/*!
		Take fd away from fd_hold, so that it wont be closed
	*/
	int fd_hold_1::fd_steal()
	{
		int fd1 = fd;
		flush();
		fd = -1;
		File = NULL;
		PRIV_is_socket = false;
		return fd1;
	}

	/*!
		Forget fd so that it wont be closed
	*/
	void fd_hold_1::clear_dont_close()
	{
		flush();
		fd = -1;
		File = NULL;
		PRIV_is_socket = false;
	}

	/*!
		The inline expect_open is intentionally light, and calls this
		if it fails. This could throw - not sure yet.
	*/
	bool fd_hold_1::expect_open_fn()
	{
		if(is_open()) return true;
		FAIL( "expect_open() failed '%s'", debug_get_obj_name() );
		return false;
	}

	bool fd_hold_1::perr( const char * scall )
	{
		buffer1 tn;
		FAIL(" # %s %d # (%s) # %s (...errno...)\n",
			type_name(tn),
			fd_get(),
			debug_get_obj_name(),
			scall
		);
		return false;
	}

	/*!
		run fcntl, return true and place return int in ret
	*/
	bool fd_hold_1::do_fcntl( int & ret, int F_CMD )
	{
#ifdef WIN32
#warning "WIN32 has no fcntl()"
		return FAIL("WIN32");
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

		FAIL( "# FD %d '%s' # fcntl(%s)", fd, debug_get_obj_name(), CMD );
		return false;
#endif
	}

	/*!
	*/
	bool fd_hold_1::do_fcntl( int & ret, int F_CMD, long arg )
	{
#ifdef WIN32
		return FAIL("WIN32");
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

		/* F_SETFL can only change
			O_APPEND
			O_ASYNC
			O_DIRECT
			O_NOATIME
			O_NONBLOCK
		 it is already too late for (on open)
			O_RDONLY
			O_WRONLY
			O_RDWR
		 it is already too late for (on open)
			O_CREAT
			O_EXCL
			O_NOCTTY
			O_TRUNC
		*/

		FAIL( "# ERROR # fcntl(%d,%s,0x%xl) %m \n", fd, CMD, arg );
		return false;
#endif
	}

	/*!
	*/
	bool fd_hold_1::do_fcntl( int & ret, int F_CMD, struct flock * lock )
	{
		return FAIL("No support for flock"); // no need yet
	}

	/*!
	*/
	bool fd_hold_1::get_close_on_exec( bool & closes )
	{
#ifdef WIN32
		return FAIL("WIN32");
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
	bool fd_hold_1::set_close_on_exec( bool closes )
	{
#ifdef WIN32
		return FAIL("WIN32");
		return false;
#else
		
		int flags = 0;
		if( closes ) flags = FD_CLOEXEC;
		return do_fcntl( flags, F_SETFD, flags );
#endif
	}


#ifdef WIN32
	const char * WIN32_ioctl_name( long cmd )
	{
	 switch(cmd) {
	  case FIONBIO: return "FIONBIO";
	  case FIONREAD: return "FIONREAD";
//	  case FIONWRITE: return "FIONWRITE";
	  default: {
		FAIL("unknown ioctl %d", (int) cmd );
		return "X_IOCTL_X";
	  }
	 }
	}

	/*!
		log and call WIN32 ioctlsocket (see also WSAIoctl)
	*/
	bool fd_hold_1::ioctlsocket( long cmd, u_long * parg )
	{
		const char * cmd_str = WIN32_ioctl_name( cmd );
		int t = ::ioctlsocket (fd_get(), cmd, parg);
		if( t == 0 ) {
		 INFO("OK from ioctlsocket(fd, %s, arg);", cmd_str);
		 return true;
		} else {
		 FAIL("WSAGetLastError() says %d", WSAGetLastError() );
		 FAIL("FAIL from ioctlsocket(fd, %s, arg);", cmd_str);
		 return false;
		}
	}

	//! set (clear) non-blocking io
	bool fd_hold_1::ioctlsocket_FIONBIO( bool blocks )
	{
		u_long mode = 1;
		if(blocks) mode = 0;
		return ioctlsocket( FIONBIO, &mode);
#warning	WIN32 -- set_non_block() - seen to not be working yet
	}
	bool fd_hold_1::ioctlsocket_FIONBIO_block()
	{
		return ioctlsocket_FIONBIO( true );
	}
	bool fd_hold_1::ioctlsocket_FIONBIO_non_block()
	{
		return ioctlsocket_FIONBIO( false );
	}
	//! number of bytes that can be read
	bool fd_hold_1::ioctlsocket_FIONREAD( u_long & nbytes )
	{
		return ioctlsocket( FIONREAD, & nbytes);
	}
#if 0
	//! number of bytes that can be written
	bool fd_hold_1::ioctlsocket_FIONWRITE( u_long & nbytes )
	{
		return ioctlsocket( FIONWRITE, & nbytes);
	}
#endif
	// see also WSAIoctl - for more WSA stuff
#endif

#warning TODO refactor - err_int_t:: get_NET_error_from_socket( fd_hold & fd )
/*
	err_int is quite good, improve on it, and depend on it
	Make it a parameter for (all) operations (thread!)
	Make it non-virtual? Or OK virtual!
	Make it OK for unlinked libraries (eg SSL not included)
	Make it modular for { PGSQL, SSL, ... }

	In particular note the thread safe extra parameter,
	excpet for larger classes, that want to carry it on fd.
	Then its the shared use of that obj that is not thread safe
*/

	/*
		NB-IO connect - the error code is here ...

		err_int comes here to get this (pick one or other)
	*/
	int fd_hold_1:: get_SO_ERROR()
	{
#ifdef WIN32
// OK ZERO //	INFO("WSAGetLastError() says %d", WSAGetLastError() );
#endif
		int value = 0;
		socklen_t val_len = sizeof( value );
		int t = getsockopt(
			fd_get(),
			SOL_SOCKET,
			SO_ERROR,
			(char *) & value,
			& val_len
		);
		INFO( "t = %d errno = %d value = %d", t, errno, value );
		if( t ) FAIL("getsockopt() failed");
#ifdef WIN32
// OK ZERO //	INFO("WSAGetLastError() says %d", WSAGetLastError() );
// value is fine when used as WIN32 OS error number
#endif
		return value;
	}

	/*!
		NON-BLOCKING IO

		This call also be set on the open() call,
		which may also take some time.
		Unfortunately ioctl()s often dont comply,
		as they may take time (/dev/cdrom)
http://www.on-time.com/rtos-32-docs/rtip-32/reference-manual/socket-api/ioctlsocket.htm
	*/
	bool fd_hold_1::set_non_block()
	{
#ifdef WIN32
		return ioctlsocket_FIONBIO_non_block();
		return FAIL("maybe if I dont do it but glib does ...");
#else
		int flags = 0;
		if(! do_fcntl( flags, F_GETFL ) ) return false;
		INFO( "FD %d %s flags %x", fd_get(), debug_get_obj_name(), flags );
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
		fd is expoected to be in non_block mode
	*/
	bool fd_hold_1::check_non_block()
	{
#ifdef WIN32
		WARN("WIN32"); // probably could be I cant be bothered
		return true;
#else
		int flags = 0;
		if(! do_fcntl( flags, F_GETFL ) ) return false;
		if( flags & O_NONBLOCK ) {
			INFO( "FD %d %s flags %x = PASS", fd_get(), debug_get_obj_name(), flags );
			return true;
		} else {
			INFO( "FD %d %s flags %x", fd_get(), debug_get_obj_name(), flags );
			return FAIL("fd is not set to blocking mode");
		}
		return true;
#endif
	}

	/*!
		End of NON-BLOCKING IO
	*/
	bool fd_hold_1::set_block()
	{
#ifdef WIN32
		return ioctlsocket_FIONBIO_block();
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
	bool fd_hold_1::close()
	{
		// after other errors, close() gets calls for luck
		// before CTOR close() any previous, IF_ANY_PREVIOUS
		// so we need to preserve the older error and ignore this one?
		// maybe // if(errno_old) errno=errno_old;
		int errno_old = errno;
		errno = 0;
		fsync(); // Non-Linux POSIX doesnt fsync on close !!!
	//	flush(); // we expect that fsync() does this FILE* flush
		if( is_open() )
		{
#ifdef WIN32
			WARN("WIN32 socket would need ::closesocket(sock)");
			// also WIN32 should not rely on errno
#endif
			::close( fd );
			if(errno) perr( "close" );
			errno = errno_old;
		}
		fd = -1;
		// I dont do FILE* so interleave differently?
		// eg reopen fd as FILE ??? or get fd from FILE ??
		if( File ) {
			fclose( File );
			// BAD IDEA // free( File );
			File = NULL;
		}
		File_flush = NULL;
		PRIV_is_socket = false;
		return true;
	}

	/*!
		Obtain the FILE * for this fd, open from fd if necessary (rw?)
	*/
	FILE * fd_hold_1::file()
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
	int fd_hold_1::read( void * buff, int len )
	{
		int t = ::read( fd, buff, len );
		if( t == 0 )
		{
			zero_read_means_eof();
		}
		if( t < 0 )
		{
			FAIL("# FD %d '%s' # read returned -1", fd, debug_get_obj_name() );
		}
		return t;
	}

	/*!
	*/
	int fd_hold_1::read_quiet( void * buff, int len )
	{
		int t = ::read( fd, buff, len );
		if( t == 0 )
			zero_read_means_eof();
		if( t < 0 )
		{
		 if(0) // should still allow some sense ?
			FAIL("# FD %d '%s' # read returned -1", fd, debug_get_obj_name() );
		}
		return t;
	}

	/*!
		write a buffer1, return nbytes actually written
	*/
	int fd_hold_1::write( const blk1 & buff )
	{
		return write( buff.buff, buff.nbytes_used );
	}

	/*!
		simple wrapper over write - no perror, or bool-truncation
	*/
	int fd_hold_1::write( const void * buff, int len )
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
	int fd_hold_1::sock_recv_from( void * buff, int len, sock_addr_base & addr )
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

		ERRM WIN32 needs to call recv -NOT- read on sockets
		ERRM that will need a flag "I_AM_SOCKET_NOT_FILE_HANDLE"
		(something to do with WINSOCK -vs- SYSTEM)

		TODO: this is where eof_input() is detected, 
		this absolutely requires non_block()

	*/
	int fd_hold_1::sock_recv( void * buff, int len )
	{
		int flags = 0; // MSG_OOB
		sockaddr * from = NULL;
		socklen_t fromlen = 0; // protect from overwrite ?
		int t = recvfrom( fd_get(), (char *)buff, len, flags, from, & fromlen );
		if(t==0) {
			bool this_is_non_block = true;
			if( this_is_non_block ) {
				zero_read_means_eof();
			}
		}
		return t;
	}

	/*!
		read() and recv() only  return zero on EOF.

		However, it is up to the caller of recv() to know that?
		This compromise, is that zero_read_means_eof() _IS_ called
		but does nothing. The child class, overrides this by calling
			eof_input();

		Reasons:

			caller should know zero means EOF
			caller does not expect simple wrap to do anything
			caller might not be able to call recv! (eg SSL does it)
			caller might have a buffers worth _BEFORE_ eof event
				
	*/
	// VIRTUAL
	void fd_hold_1::zero_read_means_eof()
	{
		// this file uses perr(system_call_name) which calls FAIL
		// I dont get these message, so unused?
		FAIL("ZERO means eof on non blocking - MUST REIMPLEMENT");
		// IE CALL // eof_input();
		// eof_input(); // is not defined in base class
		FAIL("calling close()");
		close();
	}

	/*!
		simple (unused) - maybe add sock_recvfrom() - maybe use blk1 ?

		ERRM WIN32 needs to call recv -NOT- read on sockets
		ERRM that will need a flag "I_AM_SOCKET_NOT_FILE_HANDLE"
		(something to do with WINSOCK -vs- SYSTEM)
	*/
	int fd_hold_1::sock_send( void * buff, int len )
	{
		int flags = 0; // 
		int t = send( fd_get(), (char *)buff, len, flags );
		return t;
	}

	/*!
		OLD - requires FILE* and non-blocking

		USER SHOULD - vprint to buffer1 then add that to an output queue
	*/
	int fd_hold_1::vprint( const char * fmt, va_list args )
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
	int fd_hold_1::print( const char * fmt, ... )
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
	void fd_hold_1::flush()
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

		No need HERE to wait for the disk-writes, 
		Know are requested
		Know system will stay up for ... over next horizon

		Dont call this, call self.sync() => fsync(fd); // with added
	*/
	bool fd_hold_1::sync_system()
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
	bool fd_hold_1::fsync()
	{
		// encourage calling fsync()
		if(!is_open() ) return true;

		// close expects fsync() to call flush() 
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

		bool sync() { return fdatasync(); }
	*/
	bool fd_hold_1::fdatasync()
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
	bool fd_hold_1::do_ioctl_can_fail( const char * name, int func, void * data )
	{
		int retval;
		if(!call_ioctl_data_retval( name, func, data, & retval )) {
			// it failed, it reported
			// we dont complain more than that
			return false;
		}
		return false;
	}

	bool fd_hold_1::call_ioctl_data_retval(
		const char * name,
		int func,
		void * data,
		int * retvar
	) {
		* retvar = -1;
		if(!is_open()) {
			INFO("fd.is_open() == false" );
			return FAIL("%s", name );
		}
#ifdef WIN32
		return FAIL("## NO ioctl on WIN32 ##");
		return false;
#else
		int e0 = ioctl (fd, func, data);
		if( e0 == -1 ) {
			FAIL("ioctl (fd, func, data);");
			INFO("ioctl name = '%s';", name );
			return false;
		}
		// your code here
		return true;
#endif
	}

	/*!
		ioctls return -1 on failure
		many return N bytes or old_value

		all ioctls vectored through the things that reports FAIL
	*/
	bool fd_hold_1::do_ioctl_int_ret(
		const char * name,
		int func,
		int * data
	) {
		int retval = -1;
		if(!call_ioctl_data_retval( name, func, data, & retval ))
			return false;

		// returned in in same bytes that were received ... hmm
		* data = retval;
		// OK
		return true;
	}

	/*!
	*/
	bool fd_hold_1::do_ioctl( const char * name, int func, void * data )
	{
		if(!is_open()) {
			FAIL("# FD %d '%s' DEVICE NOT OPEN for %s", fd, debug_get_obj_name(), name );
			return false;
		}
		int retval_zero;
		if(!call_ioctl_data_retval( name, func, data, & retval_zero ))
			return false;
#ifdef WIN32
		FAIL("## NO ioctl on WIN32 ##");
		return false;
#else
		if (0 == ioctl (fd, func, data) ) return true;
#endif
		FAIL("# FD %d %s do_ioctl(fd,%s,DATA)",
			fd,
			debug_get_obj_name(),
			name
		);
		return false;
	}

	/*!
		Theres an infinite variety of open calls,
		then more for sockets, pipe options, locks, etc.
		RO and RW is plenty for many devices
	*/
	bool fd_hold_1::open_RO( str0 filename, bool async )
	{
		close();
		int flags = O_RDONLY;
#ifdef WIN32
		if(async) WARN("WIN32 ignoring flags on file O_NONBLOCK");
		WARN("WIN32 This defaults to O_TEXT no O_BINARY");
#else
		if(async) flags |= O_NONBLOCK;
#endif

#if 0
		// where the BUGS dwell
		INFO("file (STR0) %s str0 %s # shows str0 STR0 var_args",
		 (STR0) filename,
		        filename
		);
#endif
		fd = ::open( (STR0) filename, flags );
		if( fd == -1 )
		{
			return FAIL( "open %s flags %X", (STR0) filename, flags );
// obj.put_val_for_filetype_ARGS( buffer1 & out ); // BASE class of DATA.api 
// printf "%4X" u16_as_0xFFFF
// printf "FMT=" u16_as_0xFFFF
// printf -as- u16_as_0xFFFF
// - When In PSG_n_SUBLEX - look for " GAP BL+ GAP " // BL = "%4X" // BL = BL
// - Bounce SUBLEX BL==BL // DIFFERENT USAGE PATTERNS += ADD_THIS
// - BIND SUBLEX BL=="%4X" // DIFFERENT SUBLEX USAGE PATTERNS += ADD_THIS
// printf " %s%s%s " LHS OPCODE RHS // lhs LHS MID RHS rhs // MID == L M R
// printf " x%XD_%s " " %s0x"LEX_VAL_0xF_"
// printf " %s " of "LEX_VAL_0xF%4X"
// printf " %s%s " of "LEX_VAL_0xF_" { SCRIPT ... { OPCODE LHS RHS CMNT } ... }
// " %s%s " { SCRIPT ... { OPCODE LHS RHS CMNT } ... }
		}
		INFO( "open '%s' flags %X", (STR0) filename, flags );
		return true;
	}

	bool fd_hold_1::open_RW_CREATE( str0 filename, bool async )
	{
		mode_t mode = 0666;
		return open_RW_CREATE_modmask( filename, async, mode );
	}

	/*!
		open a new file, or truncate an old one!

		Note that (on unix) the file permissions is rw-rw-rw
		COMBINED WITH umask (if the file is created).
	*/
	bool fd_hold_1::open_RW_CREATE_modmask(
		str0 filename,
		bool async,
		mode_t modmask
	)
	{
		close();
		int flags = O_RDWR | O_CREAT | O_TRUNC;
#ifdef WIN32
		flags += O_BINARY; // WIN32 API likes to do CRLF convertion
		// alternative is O_TEXT or nothing
		if( async) {
			return FAIL("WIN32 and O_NONBLOCK");
		//	flags |= O_NONBLOCK;
		}
#else
		if( async) flags |= O_NONBLOCK;
#endif
		fd = ::open( filename, flags, modmask );
		if( fd == -1 )
		{
			return FAIL( "open %s %od %0d", (STR0) filename, flags, modmask );
		}
		return true;
	}

	/*!
		open filename or device for reading and writing

		async means wait for read, write, open, close.
		fcntl always seems to wait on cdroms media change!

		if file doesnt exist ...
	*/
	bool fd_hold_1::open_RW( str0 filename, bool async )
	{
		close();
		int flags = O_RDWR ; 
#ifdef WIN32
		return FAIL("WIN32");
#endif
		if( async) flags |= O_NONBLOCK;
		fd = ::open( filename, flags );
		if( fd == -1 )
		{
			return FAIL( "open %s %od", (STR0) filename, flags );
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
	bool fd_hold_1::open_TCP( struct sockaddr_in* sa, bool async )
	{
		errno_zero();
		if(! create_socket_tcp() )
			return false;
		if(async) set_non_block();
		STEP("Calling Connect");
		errno_zero();
		if( 0 != ::connect(
			fd,
			(struct sockaddr*) sa,
			sizeof(*sa)
		)) {
			// man connect // wait for socket writable
			// get_SO_ERROR
#ifdef WIN32
#warning sort-out-int_err_t usage here - eg parameter for this call chain or fd
			// not thought out here // get_SO_ERROR();
			// switch( errno ) {
			switch( WSAGetLastError() ) {
#else
			switch( errno ) {
#endif
#ifdef WIN32
			 case WSAEWOULDBLOCK:
				// likewise
				INFO("DEFERRED CONNECT");
				errno_drop();
				return true;
#else
			 case EINPROGRESS: 
			/*
			# LIBC ERRNO # 115 # Operation now in progress
				The connect() call has been started,
				but set_non_block() means that we dont wait
				so UNIX replies with EINPROGRESS.

				When the relevent packets arrive,
				the FD will become readable,
				for the first time, which is when
				fd_selectable calls C_select_says which calls

					select_says_connected()

				If "Connection Refused" because no listener,
				it is reported by C_select_says
					G_IO_ERR G_IO_HUP

				NB: I am using libc ::connect not g_socket_connect
				http://library.gnome.org/devel/gio/stable/GSocket.html
				

			*/
				errno_drop();
				return true;
#endif
			}
			FAIL("connect");
			return false;
		}
		errno_zero();
		return true;
	}

	/*!
		create a socket and connect to remote address SPEC
	*/
	bool fd_hold_1::open_TCP( sock_addr_ip & spec, bool async )
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
	bool fd_hold_1::open_AF_UNIX( str0 filename, bool async ) // client
	{
#ifdef WIN32
		return FAIL("WIN32");
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
			return FAIL( "filename.str_len() exceeds UNIX_PATH_MAX %s", (STR0)filename );
		}
		strncpy( sa.sun_path, (STR0) filename, UNIX_PATH_MAX );

		if( async ) set_non_block();

		if( 0 != ::connect(
			fd,
			(struct sockaddr*) & sa,
			sizeof(sa)
		)) {
			// man connect // wait for socket writable
			switch( errno ) {
			 case EINPROGRESS:
				errno_drop();
				return true;
			}
			FAIL( "filename %s", (STR0)filename );
			close();
			return false;
		}
		return true;
#endif // WIN32
	}

bool fd_hold_1::
seek_SET_32( u32 pos )
{
	if(-1 == lseek( fd, pos, SEEK_SET ) )
	{
		return FAIL("seek");
	}
	return true;
}

bool fd_hold_1::
seek_REL_32( i32 pos )
{
	if(-1 == lseek( fd, pos, SEEK_CUR ) )
	{
		return FAIL("seek");
	}
	return true;
}

bool fd_hold_1::
seek_END_32( i32 pos )
{
	if(-1 == lseek( fd, pos, SEEK_END ) )
	{
		return FAIL("seek");
	}
	return true;
}


/*
	file_stat has some sense too 
	it knows about symb links

	maybe rewrite it to use this ...

	It also does stat on the file, not fd


bool fd_hold_1::
fstat( struct stat & st )
{
	if(-1 == fstat( fd, &st ) )
	{
		return FAIL("fstat");
	}
	return true;
}
*/

	/*
		these were moved to fd_hold from sock_xxx
		a listener socket is much like a client socket (at this stage)
	*/

	/*!
                fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	*/
	bool fd_hold_1::create_socket_tcp()
	{
		close();
                fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
		if( fd == -1 ) {
			perr("socket");
			return FALSE_dgb_fail( "socket" );
		}
		PRIV_is_socket = true;
		return true;
	}

	/*!
                fd = socket (AF_INET, SOCK_DGRAM, 0);
	*/
	bool fd_hold_1::create_socket_udp()
	{
		close();
                fd = socket (AF_INET, SOCK_DGRAM, 0); // IPPROTO_UDP);
		if( fd == -1 ) {
			return FALSE_dgb_fail( "socket" );
		}
		PRIV_is_socket = true;
		return true;
	}

	/*!
		fd=socket( AF_UNIX,SOCK_STREAM, 0 );
	*/
	bool fd_hold_1::create_socket_unix()
	{
		close();
		fd=socket( AF_UNIX, SOCK_STREAM, 0 );
		if( -1 == fd ) {
			perr("socket");
			return false;
		}
		PRIV_is_socket = true;
		return true;
	}


	/*!
		fd=socket( AF_UNIX,SOCK_DGRAM, 0 );
	*/
	bool fd_hold_1::create_socket_unix_dgram()
	{
		close();
		fd=socket( AF_UNIX, SOCK_DGRAM, 0 );
		if( -1 == fd ) {
			perr("socket");
			return false;
		}
		PRIV_is_socket = true;
		return true;
	}



	/*!
		fd=socket( AF_UNIX,SOCK_RAW, proto_ent_icmp );
	*/
	bool fd_hold_1::create_socket_icmp()
	{
		close();
		int ip_proto_icmp = get_IPPROTO_ICMP();

		fd=socket( AF_INET, SOCK_RAW, ip_proto_icmp );
		if( -1 == fd ) {
			perr("socket");
			return false;
		}
		PRIV_is_socket = true;
		return true;
	}

/////////////////////////

// extern "C" 
bool CHECK_can_write_file( const char * filename )
{
	// The file will be TOUCHED, as part of the test, probably
	int fd = -1;
	fd = open( filename, O_RDWR|O_CREAT, 0660 );
	if( fd < 0 ) {
		WARN("CANNOT OPEN FILE %s FOR WRITING", filename );
		return false;
	}
	close( fd );
	return true;
}
