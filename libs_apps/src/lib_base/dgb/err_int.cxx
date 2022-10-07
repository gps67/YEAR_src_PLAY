#include "err_int.h"
#include "dgb.h"
#include "fd_hold.h"

#ifdef WIN32
// 
#else
#include "netdb.h"
#endif


void err_int_t:: PRINT1( buffer2 & buff )
{
	// caller would clear buffer before call
	buff.print( "%s errno(err %d) == %s - %s ## %s",
		(STR0) e_zone,
		err,
		(STR0) e_name,
		(STR0) e_str,
		(STR0) e_obj_name
	);
}
const char * err_int_t:: PRINT1_str()
{
	// LURK not thread sensitive, maybe not safe
	static buffer2 buff;
	buff.clear();
	PRINT1( buff );
	return (STR0) buff;
}

void err_int_t:: clear()
{
	// if(!this) gdb_invoke();
	if(!err) {
		e_print("## %p clear() error - OK no previous error \n", this);
		return;
	}
	buffer2 buff;
	PRINT1( buff );
	clear_quietly();
	// INFO("%s", buff );
	e_print("# ---- # CLEAR() # %s\n", (STR0) buff );
}

void err_int_t:: clear_quietly()
{
	// gdb_invoke() for no good reason - debug early use maybe // de-loop
	static bool can_call = true;
	can_call = false;
	if( can_call ) {
		can_call = false;
		gdb_invoke(); // crashes - maybe loop // YEP //
	}

	if(!err) return;
	err = 0;	// DWORD on WIN32
	e_name.clear();
	e_zone.clear();
	e_str.clear();
	e_obj_name.clear();
}

err_int_t:: ~err_int_t()
{
	// AUTO // clear_quietly();
	err = 0;
}

err_int_t:: err_int_t()
{
	// DONE // clear_quietly();
	err = 0; // NOT DONE ON STACK!!
}

void err_int_t:: set_to( err_int_t & rhs )
{
//	* this = rhs;
	err = rhs.err;

	e_name.set( rhs.e_name );
	e_zone.set( rhs.e_zone );
	e_str.set( rhs.e_str );
	e_obj_name.set(  rhs.e_obj_name );

return;

	// hmmm seems the buffer2 code needs a bit more ...
	// blk1 operator= declared private is a good stopgap
	// but stops the unreachable from compiling :-(

 //	e_name = rhs.e_name;
 //	e_zone = rhs.  e_zone;
 //	e_str = rhs.  e_str;
 //	e_obj_name = rhs.  e_obj_name;
}

// these return ERROR status, so slightly inverted logic
// TRUE = error state ERROR is_error()
// FALSE = error state NO-ERRO is_error()

/*
	REMOVE THIS FUNCTION - it cannot exist!

	The application need to define this,
	to correctly poll the subsystems in use (and linked in)
*/
bool err_int_t:: get_error_X() // also checks GLIB SSL ...
{
	WARN("This does not make sense, not really");
	if( get_OS_error() ) return true; // FOUND
//
//	THIS DOES NOT LOOK GOOD - maybe call funcs from relevent context
//	
//	if( get_SSL_error() ) return true; // FOUND
	return false;
}

bool err_int_t:: get_OS_error() // OS not GTK not SSL nor ...
{
#ifdef WIN32
	DWORD WIN32_err  = GetLastError();
	set_to_WIN32_error( WIN32_err );
#else
	set_to_UNIX_errno( errno );
#endif
	// if(err == 11) gdb_invoke();
	return is_error();
}


bool err_int_t:: zap_OS_error() // OS not GTK not SSL nor ...
{
#ifdef WIN32
	SetLastError(0);
#else
	if(errno) e_print("# ~~~~ # clearing errno, was %d \n", errno );
	errno = 0;
#endif
	return is_error();
}

bool err_int_t:: eat_OS_error() // OS not GTK not SSL nor ...
{
	get_OS_error();
	zap_OS_error();
	return is_error();
}

bool err_int_t:: eat_DNS_error() // OS not GTK not SSL nor ...
{
#ifdef WIN32
	eat_OS_error();
#else
	err = h_errno;
	e_str = hstrerror(h_errno);
	h_errno = 0;
	zap_OS_error();
#endif
	e_zone = "DNS";
	return is_error();
}

void err_int_t:: set_obj_one( obj_ref * obj )
{
	if(obj)
		e_obj_name = obj->debug_get_obj_name();
	else
		e_obj_name = "NULL";
}

/*!
	This cant be inlined
	It is intended to defeat GCC knowing that a reference cannot be null
	when it might actually be null. So call a fn
*/
bool is_NULL_fn( caddr_t PTR ) {
	return PTR;
}

bool err_int_t:: get_NET_error_from_socket( fd_hold & fd )
{
	// I never use NULL as a referenced var
	// but it might happen, and not be all that alien
	// if it does ...
	if(is_NULL_fn((caddr_t)&fd)) gdb_invoke();
	// if(&fd == 0 ) gdb_invoke();
	// not sure if GCC is letting me do this, it is saying warning:
	// the compiler can assume that the address of ‘fd’
	// will never be NULL 
	// [-Waddress]
	// I want a DIALECT test_ref_addr_for_NULL 
	// Or let compiler optimise SSA of one extra layer
	//  fd_t * PTR = & fd; // fd_t == fd_hold // _t added to ALIAS
	// ASM incr // if_ZERO goto "PTR_is_M1" // M1 == -1 // PTR == M1 //
	// KNOW PTR has done { incr }
	// ASM decr // if_ZERO goto "PTR_is_NULL" // M1 == -1 // PTR == M1 //
	// KNOW PTR has done { incr decr } // know PTR is_back_from_glitch
	//  if( -1 == PTR ) { ... incr then dect
	//	goto("PTR_IS_M1")
	//  if( 0 == PTR ) { ... PTR = EA_fd_hold ... } PTR DROP UNDEFINED 0
	//	goto("PTR_IS_NULL")
	// else:
	//	goto("PTR_IS_PTR") // PTR_is_PTR might not be NULL
	// LABEL "PTR_is_PTR"
	// KNOW PTR is not NULL //
	// KNOW PTR is not M1 //
	// KNOW PTR is OBJ_is_fd_hold_is_fd_t //
	// KNOW OBJ is WELL_KNOWN_ALIAS_FOR_id // holders of resources



	if(!fd.is_open()) {
		err = 9; // UNIX EBADF but 22 EINVAL is also good
		e_str = "fd is not open";
		FAIL("fd is_not_open");
		return is_error();
	}
	int e_value = fd.get_SO_ERROR();
/*
	TRICKY
	should fd_hold know about getsockopt SO_ERROR
	should err_int know about ...
#ifdef WIN32
	INFO("WSAGetLastError() says %d", WSAGetLastError() );
#endif

	int e_value = 0;
	socklen_t val_len = sizeof( e_value );
	int t = getsockopt(
		fd_get(),
		SOL_SOCKET,
		SO_ERROR,
		(char *) & e_value,
		& val_len
	);
	INFO( "t = %d errno = %d e_value = %d", t, errno, e_value );
	if( t ) FAIL("getsockopt() failed");

#ifdef WIN32
	INFO("WSAGetLastError() says %d", WSAGetLastError() );
#endif
*/

	if(!e_value) {
		return is_error();
	}
	set_obj_one( &fd );
	set_to_NET_error( e_value );
	return is_error();
}

bool err_int_t:: is_minor() // EWOULDBLOCK EINTR is_not_really_an_error()
{
	return FAIL("TODO");
}

bool err_int_t:: set_to_OS_error( int e ) // ie the strerror function
{
	e_zone = "OS"; // POINTLESS - gets cleared then overwritten
#ifdef WIN32
	return set_to_WIN32_error( e );
#else
	return set_to_UNIX_errno( e );
#endif
	// they both return is_error();
}

/*!
	set err_int_t from UNIX errno (not how it sounds though)

	if e (errno)
*/
bool err_int_t:: set_to_UNIX_errno( int e ) // ie the strerror function
{
	// remove error // set this one incl 0
	// retain existing err, unless there is a new one
	// clear();
	if(err) clear();
	if(e) if(err) clear_quietly();
#ifdef WIN32
	FAIL("THIS IS NOT UNIX");
#else

	if(e) {
		err = e;
		e_zone = "UNIX";
		get_name_of_errno( err, e_name ); // LURK ERRNO%d static varies
#ifdef patient
		... other surrounding code ...
		// forget this, the bad ways are not just easier
		// they are the only ones that actually work
		// stupid GNU uses same prototype as XSI which returns int, why?
		const char * s = strerror_r( err, (char *) P, N );
		... other surrounding code ...
#else
		e_str = strerror( err );
#endif
		e_str.trailing_nul();
	}
	if(0) INFO_REPORT(); // debug myself
#endif
	return is_error();
}

bool err_int_t:: set_to_WIN32_error( int e ) // ie the strerror function
{
	if(e) if(err) clear_quietly();
#ifdef WIN32
	if(!e) {
		return is_error();
	}
	err = e;

	DWORD WIN32_err =  e;
	if(WIN32_err) {

		static const unsigned int sz = 400;
		char buf[ sz ];

		// CALLER DOES :
		// DWORD dw = GetLastError(); 
		DWORD dw = WIN32_err;

		DWORD dwFlags = 0
//		|	FORMAT_MESSAGE_ALLOCATE_BUFFER
		|	FORMAT_MESSAGE_FROM_SYSTEM
		|	FORMAT_MESSAGE_IGNORE_INSERTS
		;
		LPCVOID lpSource = NULL;
		DWORD dwMessageId = dw;
		DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
		LPTSTR lpBuffer = (LPTSTR) &buf;
		DWORD nSize = sz;
		va_list *Arguments = NULL;

		int t = FormatMessage(
			dwFlags,		// alloc ? INSERTS ??
			lpSource,		// NULL
			dwMessageId,		// LANG_NEUTRAL
			dwLanguageId,		// GetLastError()
			lpBuffer,		// output buffer
			nSize,			// output buffer[sz]
			Arguments		// NULL
		);

		/*
			FIXUP when errno is not an errno
			(ie it has no message)
		*/
		if(!t) {
		 if(WIN32_err == WSAEWOULDBLOCK) {
		  sprintf( lpBuffer, "%s\n", "WSAEWOULDBLOCK" );
		 } else {
		  sprintf( lpBuffer, "%s\n", "(WIN32.FormatMessage)" );
		 }
		}

		e_zone = "WIN32";
		e_str.set( lpBuffer );

		// FORMAT_MESSAGE_ALLOCATE_BUFFER // LocalFree(lpDisplayBuf);
	}
#else
	FAIL("NOT WIN32");
#endif // WIN32
	return is_error();
}

bool err_int_t:: set_to_NET_error( int e )
{
	if(e) {
		// TODO ... fix
		set_to_OS_error(e); // converts errno e into string // same on w32
		e_zone = "NET";
	}
	return is_error();
}

/////////////// clear_if_CIRCUMSTANCE // returns YES_THAT_CIRCUMSTANCE_MATCHES

/*
	SSL leaves error 11 EAGAIN behind,
	because it reads until read returns -1 DRY
	Other apps probably similarly

	So after read, try this ...
*/
bool err_int_t:: clear_if_read_dry()
{
	if(!err) return false;
	return clear_if_EAGAIN();
}

bool err_int_t:: peek_if_read_dry() // save debugging messages
{
	// COPY+PASTE clear_if_EAGAIN()
	if(!err) return false;
#ifdef WIN32
        if( err == WSAEWOULDBLOCK ) return true;
#else
        if( err == EWOULDBLOCK ) return true;
        if( err == EAGAIN ) return true;
#endif
	return false;
}


bool err_int_t:: clear_if_EAGAIN()
{
	if(!err) return false;
	// TODO
	// if e_zone_1 != e_zone_OS return false; // enum
#ifdef WIN32
	// if( err == TODO )
        if( err == WSAEWOULDBLOCK ) {
                INFO("WSAEWOULDBLOCK %s", PRINT1_str() );
		return true;
	}
#else
        if( err == EWOULDBLOCK ) {
                INFO("EWOULDBLOCK (alias EAGAIN) %s", PRINT1_str() );
		// gdb_invoke();
		clear_quietly();
		return true;
        }

        if( err == EAGAIN ) {
                INFO("EAGAIN %s", PRINT1_str() );
		clear_quietly();
		return true;
        }
#endif
	return false;
}

/*!
	after read, try this ...
*/
bool err_int_t:: clear_if_EINTR()
{
#ifdef WIN32
	// if( err == TODO )
#else
	if( err == EINTR ) {
                clear_quietly();
		INFO("EINTR");
		return true;
	}
#endif
	return false;
}

/*
	TODO convert e_zone to an enum
	TODO match zone.err == OS_UNIX.EINPROGRESS
	ie dont match SSL error 11 or other ...
*/
bool err_int_t:: clear_if_EINPROGRESS()
{
	if(!err) return false; // no match
#ifdef WIN32
	if(err == WSAEWOULDBLOCK) {
		clear_quietly();
		STEP("yes WSAEWOULDBLOCK");
		return true; // MATCHED
	}
#else
	if(err == EINPROGRESS) { // 113
		clear_quietly();
		STEP("yes EINPROGRESS");
		if(errno == EINPROGRESS) {
			errno = 0;
			STEP("NEEDED TO CLEAR ERRNO!");
		}
		return true; // MATCHED
	}
#endif
	return false; // NO MATCH
}

bool  err_int_t:: INFO_REPORT(const char * FN)
{
	INFO("errno %d", errno );
	if(!FN) FN = "(INFO_REPORT)";
	if(no_error()) return INFO("%p NO ACTUAL ERROR",this);

	INFO("%s %s", FN, PRINT1_str() );
	return false;
}

bool  err_int_t:: FAIL_REPORT(const char * FN)
{
	return INFO_REPORT(FN);
}


////////////////// IF DEF WITH SSL /////////////////

// move this to base
const char * err_int_t:: e_str_tail()
{
	if(!e_str) return "'_'";
	const char * p = strrchr( (STR0) e_str, ':' );
	if(p) p++; else p = (STR0) e_str;
	return p;
}
