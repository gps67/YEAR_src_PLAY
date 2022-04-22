#include "dgb.h"
// #include "CTXT_stubs.h"

// YOU CAN USE THIS WITHOUT THE REST OF libs_apps
// YOU MAY STATICALLY LINK IT (.o) WITH YOUR APPLICATION
// AS LONG AS YOU RESPECT THE REST OF THE LGPL
//
// The idea is to get you to use INFO() WARN() FAIL()
// ready to link with libs_apps in an LGPL way
// but not have to find an alternative, for smaller projects
// or things that dont link with the rest of libs_apps
// Simply broadcast any modifications to the source - THIS FILE
// statically link your proprietry blob (or dyn)
// and be happy

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef WIN32
#include "MY_WIN32.h"
#else
#endif
#include <errno.h>
#include <string.h>
#include "gdb_invoke.h" // a conflicting progname system // LURK TODO
#include "dgb.h" // a conflicting progname system // LURK TODO
#include "obj_ref.h" // does work, then string to output
#include "buffer2.h" // does work, then string to output
#include "err_int.h" // does work, then string to output

#define WITH_EPRINT 1

#if WITH_EPRINT
#include "e_print.h"
#endif

#define FMT_3			"# %s # %-5s # %-12s # "
#define FMT_3_2			"# %-5s # %-12s # "
#define FMT_WIN_d_s 		"# WIN32 ERRNO # %d # %s"
#define FMT_UNIX_d_s 		"# LIBC ERRNO # %d # %s"
#define FMT_UNIX_d_s_lf		 FMT_UNIX_d_s "\r\n"
#define FMT_3_s_s_s 		"# %s # %s # %s: "
#define FMT_3_s_s_s_par		"# %s # %s # %s(): "


int V_print( const char * fmt, va_list args )
{
	static buffer1 buf1;
	int old_errno=errno;
#warning LOCK buf1 here
	buf1.clear();

	va_list args2;
	va_copy( args2, args );
	buf1.vprint( fmt, args2 );
	va_end( args2 );

	int t = 0;
#if WITH_EPRINT
	e_print( "%s", (STR0) buf1 );
#else
	t = fprintf( stderr, "%s", (STR0) (str0) buf1 );
#endif

// release LOCK here // or make non static

	fflush(stderr);
	fflush(0);
	// errors WITHIN V_print get overwritten, so double check buf1.vprint
	errno=old_errno;
	return t;
}

int X_printf( const char * fmt, ... )
{
	va_list args;
	va_start(args, fmt);
	int t = V_print( fmt, args );
	va_end(args);

	return t;
}

int complain_depth = 4;

static const char * prog_alias = 0;

// extern
void set_prog_name( const char * progname )
{
	if( progname ) {
		progname_argv0 = progname;
		// point to name.ext part of path/name.ext
		progname_name = strrchr( progname, '/' );
		if( progname_name )
			progname_name++;
		else
			progname_name = progname;
		// set it here, (MAYBE)
		set_prog_alias( progname_name );
	}
}

// extern
void set_prog_alias( const char * s )
{
	if(!s) {
		s = get_prog_alias(); // from dgb.cxx
	}
	if( prog_alias == s ) return;
	if( prog_alias ) free( (void*) prog_alias );
	char * buf = (char*) malloc( strlen(s) + 1 ); // strdup()
	strcpy( buf, s );
	prog_alias = buf;
}

// extern
const char * get_prog_alias()
{
	if(! prog_alias ) set_prog_alias("PROG");
	return prog_alias;
}


bool ends_with_lf( const char * s )
{
	static bool invoked = false;
	if( invoked ) {
		invoked = true;
		gdb_invoke();
	}
	int l = strlen( s );
	if( !l ) return false;
	return s[l-1] == '\n';
}


// callback
static do_ssl_err * dgb_ssl_err  = NULL;
void set_dgb_ssl_err( do_ssl_err * cb )
{
	STEP("setting callback");
	dgb_ssl_err = cb;
}
/*!
	print out any SSL error codes - removing them all from thread

	libbase does not link with ssl (libbase2 does),
	so we have to call a callback, to code that does, or NULL
*/
void do_ssl_ERR()
{
	if(dgb_ssl_err)
		dgb_ssl_err();
#if 0
	// cant do this here - SSL is not linked
	int err;
	while((err=ERR_get_error()))
	{
		char buf[1024];
		ERR_error_string_n(err,buf,sizeof buf);
		X_printf( "# SSL ERR # %d # %s", (int) err, (const char *) buf );
	}
#endif
}

void do_ERRNO()
{
#ifdef WIN32
#	warning "WIN32 is setting errno every printf with a %p - or something"
	if(errno==9) errno=0; // bad_file_descriptor
#endif
	err_int_t & err_int = BASE1::get_err_int_for_this_thread();
	err_int.eat_OS_error();
	if(err_int.is_error()) {
		err_int.INFO_REPORT();
	}

#if 0
#ifdef WIN32
	if(err_int.is_error()) {
		X_printf( FMT_WIN_d_s, (int) err_int.err, (const char *) err_int.e_str );
	}
#else
	if(err_int.is_error()) {
		X_printf( FMT_UNIX_d_s_lf, errno, (STR0) err_int.e_str );
		if(0) DEBUG_print_stack_trace(6,2);
	}
#endif
#endif
}


void print_trace(FILE *out, int depth, int skip );
void DEBUG_print_stack_trace2(int depth, int skip = 0) {
	print_trace(stderr, depth, skip + 1);
}

void DEBUG_print_stack_trace(int depth, int skip )
{
	// lets hope it wasnt a stack overflow
	// one extra layer needed // print_trace(stderr);
	DEBUG_print_stack_trace2( depth, skip + 1 );
}

bool FALSE_print_stack_trace( int depth, int skip )
{
	DEBUG_print_stack_trace2( depth, skip + 1 );
	return false;
}

// extern
void errno_zero_if_read_dry()
{
	err_int_t & err_int = BASE1::get_err_int_for_this_thread();
	if(!err_int.is_error()) return;
}

/*!
	use macro errno_zero(); whenever you want a clean errno

	called when a function wants to use errno, but finds it already set,
	or any other detection of unreported error. The finder is not guilty,
	just wants a clean slate for its own work.
*/
void errno_zero_fn()
{
	bool summuts_up = false;

	err_int_t & err_int = BASE1::get_err_int_for_this_thread();
	if(err_int.is_error()) {
		summuts_up = true;
		WARN("LOSING PREVIOUS ERROR REPORT");
		err_int.FAIL_REPORT();
		err_int.clear_quietly();
	}

	if(err_int.eat_OS_error()) {
		WARN("LOSING CURRENT ERROR REPORT");
		err_int.FAIL_REPORT();
		err_int.clear_quietly();
	}

	do_ssl_ERR();
	if(err_int.is_error()) {
		summuts_up = true;
		WARN("LOSING CURRENT ERROR REPORT");
		err_int.FAIL_REPORT();
		err_int.clear_quietly();
	}

 //	if(err_int.eat_SSL_error()) {
 //		WARN("LOSING CURRENT ERROR REPORT");
 //		summuts_up = true;
 //		err_int.FAIL_REPORT();
 //		err_int.clear_quietly();
 //	}
	
	//
	// FAIL(msg) will clear all errors ...
	//
	if(!summuts_up) return;
	do_ERRNO();
	INFO("now zero");
}

// extern
bool FAIL_clear_error()
{
	errno_zero();
	return false;
}

bool fn_WARN(
	const char * NOTE,
	int depth,
	const char * file,
	const char * func,
	const char * fmt,
	...
)
{
	bool do_stack_dump = false;
	if(!fmt) fmt="(null)";
	bool do_err = false;
	bool ret_val = false; // usually FAIL
	if( (NOTE[0] == 'U') && (0==strcmp(NOTE, "UNFAIL")) ) {
		ret_val = true;
		FAIL_clear_error();
	}
	if( (NOTE[0] == 'P') && (0==strcmp(NOTE, "PASS")) ) {
		ret_val = true;
	}
	if( (NOTE[0] == 'P') && (0==strcmp(NOTE, "PERROR")) ) {
		do_err = true;
	}
	if( (NOTE[0] == 'F') && (0==strcmp(NOTE, "FAIL")) ) {
		do_err = true;
		// FAIL will cause an error line but not a stack DUMP
		// THROW_1() might also be a name
		// DEBUG_print_stack_trace2();
		if(0) do_stack_dump = true;
	}

	// error or not - eat it here - at least OS stuff, which we might use
	
	err_int_t & err_int = BASE1::get_err_int_for_this_thread();
	if(err_int.is_error()) {
		// eat some more anyway ...
		err_int.eat_OS_error();
	} else {
		err_int.eat_OS_error();
	}
	// now errno is zero - the first WARN/TRACE/FAIL eats it
	// that still leaves SSL, GTK, errors etc
	// that protects out recursive use of print trace texts

	if( do_err ) {
		if( do_err ) do_ERRNO();
		if(err_int.is_error()) {
			err_int.INFO_REPORT(); // might be double done anyway
		}
	}
	// use the sleep lines to set breakpoints on ...

	if( depth > complain_depth ) return ret_val; // usually return false

 // not thread safe - but need sensible approach to malloc count
	buffer1 func_name;
	if(!demangle_cpp_symbol( func_name, func )) {
		func_name.set( func );
		X_printf( FMT_3, get_prog_alias(), "XXXX", __FUNCTION__ );
		X_printf( "demangle_cpp_symbol failed\n" );
	}

if(0)	X_printf( FMT_3,   get_prog_alias(), NOTE, (STR0) func_name );
else	X_printf( FMT_3_2,                   NOTE, (STR0) func_name );

	va_list ap;
	va_start(ap, fmt);
	V_print( fmt, ap );
	va_end(ap);

	if( !ends_with_lf( fmt ) ) X_printf( "\r\n" );
	// if( do_err ) do_ERRNO();
	do_ssl_ERR();
	if(do_stack_dump) if(!ret_val) DEBUG_print_stack_trace2(5);

	// set breakpoints here
	if( do_err ) {
		// even for non-OS errors, sleep more
		dgb_sleep_more();
		dgb_sleep_here();
	}
	return ret_val;
}

// ... rest unused ...

bool fn_COMPLAIN1( const char * fmt, ... )
{
	X_printf( "# COMPLAIN-1 # " );

	va_list ap;
	va_start(ap, fmt);
	V_print( fmt, ap );
	va_end(ap);

	if( !ends_with_lf( fmt ) ) X_printf( "\r\n" );
	do_ERRNO();
	return false;
}

bool fn_COMPLAIN2( const char * fmt, ... )
{
	X_printf( "# 2 # " );

	va_list ap;
	va_start(ap, fmt);
	V_print( fmt, ap );
	va_end(ap);

	return false;
}

bool fn_COMPLAIN3( const char * fmt, ... )
{
	X_printf( "# 3 # " );
	va_list ap;
	va_start(ap, fmt);
	V_print( fmt, ap );
	va_end(ap);

	return false;
}

#ifdef WIN32 // ------------------------------------------------

void print_trace(FILE *out, int show, int skip )
{
	// no execinfo.h (it is /usr/include/)
}

#else // LINUX/FOSS specific ? GCC specific ? 
// http://idlebox.net/2008/0901-stacktrace-demangled/cxa_demangle.htt

#include <execinfo.h>
#include <cxxabi.h>
void print_trace(FILE *out, int show, int skip )
{
	int max_depth = 100;
	size_t stack_depth;
	void *stack_addrs[max_depth]; // max_depth limit
	char **stack_strings;

	INFO("show = %d skip = %d errno = %d", show, skip, errno );

	if( show < 1 ) show = 10;
	if( skip < 0 ) skip = 0;

	int maxx = show + skip;

	/*
		TODO - keep original stack depth numbering but only print maxx

		That probably means decoding all (100) levels,
		but only printing 5. Then again, you are not supposed to DUMP

		OR use 1 as the nearest to here not nearest to main
		then the math will work OK (ie need to obtain true stack depth)
	*/

	if( maxx > max_depth ) maxx = max_depth;
// if MAIN_TO_TIP and numbering that way
//	if( maxx < max_depth ) max_depth = maxx;

	stack_depth = backtrace(stack_addrs, max_depth);
	stack_strings = backtrace_symbols(stack_addrs, stack_depth);
//	X_printf(out, "# BACKTRACK { # \r\n" );

	const char * NOTE = "STACK";
	X_printf( "# %s # %s # ---------- \n", get_prog_alias(), NOTE );

	// [0] is this function
	// [1] is almost certainly fn_WARN // when we get rid of extra layers
	// [2] is
	// SHOW MAIN to TIP (or TIP to MAIN)

	size_t lo = skip;
	size_t hi1 = lo + show; // 1 after the highest
	if( hi1 > stack_depth) hi1 = stack_depth;

#define MAIN_TO_TIP 0
#if MAIN_TO_TIP
	for (size_t i = lo; i < hi1; i++)
#else
	for (size_t i = hi1-1; i>=lo; i--)
#endif
	{

	//	static const int main_is_3 = 3;
		int undepth = i - skip;
//		int undepth = stack_depth - i - main_is_3;
//		undepth = skip - i ; // negative

//     /nfs/md7/GPS/libs_apps/lib/libbase.so(_Z7fn_WARNPKciS0_S0_S0_z+0x115) [0x2b12c67c12de]

	__attribute__((unused))
		bool found = true; // optimistic
		char * line = stack_strings[i];
		char * left = line;
		char * name = NULL;
		char * P = left;
		P = strchr( left, '(' );
		if( !P ) {
			found=false;
		} else {
			left = P+1; 
			P = strchr( left, '+' );
			if(!P)
				P = strchr( left, ')' );
			if(!P) {
				found=false;
			} else {
				*P = 0; // splat mid line
				name = left;
			}
		}
		// found is genuinely unudes - but what was it - 
		// want to get back to a working insight or other
		char *ret = NULL;
		buffer1 name_buf;
		if( name ) {
			if( demangle_cpp_symbol( name_buf, name ) ) {
				name = (char *)(STR0) name_buf; // use before free
			} else {
				// name stays as undemangleable
			}
		} else {
			// below thinks input name might be line number
		}
#if 0
		int status = -1;
		if(name) {
			ret = abi::__cxa_demangle(name, NULL, NULL, &status );
#warning LEAK this should be free( ret )
		}
		if(ret) name = ret;
#endif


		char func[20];
		sprintf( func, "%d", int(undepth));
		X_printf( FMT_3_s_s_s_par, get_prog_alias(), NOTE, func );
//		X_printf( "# %s # %s # %s(): ", get_prog_alias(), NOTE, func );
		if( name )
			X_printf( "%s\r\n", name );
		else
			X_printf( "%s\r\n", line );

#if 0
		if( name )
		 if(status==0)
			X_printf(out, "C++ %s\n", name);
		 else if(status==-2)
			X_printf(out, "C   %s()\n", name);
		 else
			X_printf(out, "?+? %s()  %d\n", name,status);
		else
			X_printf(out, " -  %s # no symbol\n", line);
//			X_printf(out, " -  %s // file [offs] // try -rdynamic\n", line);

#endif
		if(ret) free(ret);
	}
	X_printf( "# %s # %s # ---------- \n", get_prog_alias(), NOTE );
//	X_printf(out, "# } # \n" );
//	fflush(out);
	free(stack_strings); // malloc()ed by backtrace_symbols
}
#endif // WIN32

// extern
bool demangle_cpp_symbol( buffer1 & buf2, const char * symb )
{
	// CANT use FAIL etc ... because they call this

#ifdef WIN32
	buf.print( "%s()", symb ); // is it a function? eg an exception
	return true;
	
	const char * ret = 0;
#else
#warning LOCK buf here
	static buffer1 buf;
	buf.clear();
	/*
		buf can get itself remalloced which is a problem.
		BY THE CALL TO abi::__cxa_demangle ????
		Setting it excessive is not OK,
		as a larger symbol will realloc the buf
		(though this is what buffer1 does, but ... )
		2049 must be above the 60 that is allocated without malloc

		YES the design of abi::__cxa_demangle is mental.
		I dont want it to malloc buffer, just fill upto limit.
		I dont want to malloc just to feed it, I want buffer1

		SO use a static buffer1, and LOCK this function (somehow)
	*/
	if(!buf.get_space( 2049 )) {
		// CANT CALL FAIL // even though get_space probably did !!
		e_print("#### demangle_cpp_symbol buf.get_space()) failed\n");
		return false;
	}
	int status = -1;
	const char * ret = abi::__cxa_demangle(
		symb,
		(char *) buf.buff,
		(size_t*) & buf.nbytes_alloc,	// cannot avoid malloc/free
		&status
	);
	if( status == 0 ) {
		// copy the static string (at the start of buf) to buf2
		// which is also in the buf
		buf2.print( "%s", ret );
		return true;
	}

	if((status==-2)&&(!ret)) {
		/*
		This is called by fn_WARN which is using symb = prototype
		and not symb = mangled elf name eg:
static int JS::j_class_plus::s_getProperty(JSContext *, JSObject *, long, long *
		so no not complain, and copy symb through
		Adding the () is ... for ... dunno
		*/
		if(0) e_print("#### symb %s not valid under the C++ ABI mangling rules.", symb );
		// do for C what C++ gets
		// the difference between __FUNCTION__ and __PRETTY_FUNCTION__
		// is that NS, class, args and () get added
		// we should also do NAMESPACE prefix stripping
		bool already_as = strchr( symb, ')' );
		if(already_as)
			buf2.print( "%s", (STR0) symb );
		else
			buf2.print( "%s()", (STR0) symb );
		return true;
	}
	if(!ret) e_print("#### ret==NULL status = %d symb = %s \n", status, symb);
	if( status == -2 ) {
	  if(0)	e_print("#### symb %s not valid under the C++ ABI mangling rules.", symb );
		buf2.put_plain( (str0) "()" ); // do for C what C++ gets
		return true;
	}
	if( status == -1 ) { e_print("#### malloc error");  return false; }
	if( status == -3 ) {
	  if(1)	e_print("#### symb '%s' but other parameter error", symb );
		return false;
	}
	e_print("#### return code %d", status );
	return false;
#endif // WIN32/ UNIX
}

static int dgb_mad_count_fail = 0;
static int dgb_mad_count_pass = 0;
static int dgb_mad_count_fade = 0;
static int dgb_mad_count_here = 0;
static int dgb_mad_sleep_for = 1; // if at all
static bool dgb_mad_sleeping = false; // if at all

void dgb_sleep_calc_silent() // maybe I am mad
{
	static bool loopy = false;
	if(loopy) { return; }
	loopy = true;

	if( dgb_mad_count_pass > 100 ) {
		dgb_mad_count_pass = 100;
		dgb_mad_count_fail =   0;
	}

	if( dgb_mad_count_fail < -10 ) {
		if( dgb_mad_count_pass > 10 ) {
			dgb_mad_count_pass --;
			dgb_mad_count_pass --;
			dgb_mad_count_fail ++;
		}
	}

	if( dgb_mad_sleeping ) {
		// only fade whilst sleeping
		// 10 fades is one pass
		// fade is opposite of _here default
		dgb_mad_count_here = 0;
		if( dgb_mad_count_fade > 10 ) {
			dgb_mad_count_fade = 0;
			dgb_mad_count_pass ++;
			dgb_sleep_less();
		}
	}
	// 10 here is one fail extra
	// any other non-default verdict clears the here count
	if( dgb_mad_count_here > 10 ) {
		dgb_mad_count_here = 0;
		dgb_mad_count_fail ++;
	}
	// 5+5 pass is one fail less // or equal ...
	if( dgb_mad_count_pass > 10 ) {
		if( dgb_mad_count_fail > 0) {
			dgb_mad_count_fail --;
			dgb_mad_count_pass -= 5;
		} else {
			// convert +2 pass to -1 fail
			// pass grows
		}
	}
	// subtract one from the other - when 10 reached ?
	// 10 fail is considered mad, but each pass counts as 1 less
	if( dgb_mad_count_fail > 10 ) {
		if( dgb_mad_count_pass > 0 ) {
			dgb_mad_count_pass -- ;
			dgb_mad_count_fail -= 6 ;
		} else {
			// dgb_mad_count_fail -=  dgb_mad_count_pass ;
			// dgb_mad_count_pass =0 ;
		}
	}
	if( dgb_mad_count_pass > 10 ) { // can never happen see above
		if( dgb_mad_count_fail > 0 ) {
			dgb_mad_count_pass -=  dgb_mad_count_fail ;
			dgb_mad_count_fail =0 ;
		}
	}
	// decision
	if( dgb_mad_count_fail > 10 ) {
		if( dgb_mad_sleeping ) {
		} else {
			dgb_mad_sleeping = true;
			STEP("switching SLEEP ON");
		}
		dgb_sleep_here();
	} else {
		if( dgb_mad_sleeping ) {
			STEP("switching SLEEP OFF");
			dgb_mad_sleeping = false;
		} else {
		}
	}
	loopy = false;
}

void dgb_sleep_calc() // maybe I am mad
{
	// seems the problem is in INFO_ calling dgb_sleep_calc ...
	static bool loopy = false;
	if(loopy) { return; }
	loopy = true;

	dgb_sleep_calc_silent(); // maybe I am out of control

	INFO("fail/pass/fade/here = %d/%d/%d/%d",
		dgb_mad_count_fail,
		dgb_mad_count_pass,
		dgb_mad_count_fade,
		dgb_mad_count_here
	);
	loopy = false;
}

void dgb_sleep_more() // maybe I am mad
{
	dgb_mad_count_here = 0;
	dgb_mad_count_fail++;
	dgb_sleep_calc();
}

void dgb_sleep_less() // I am doing non-mad things
{
	dgb_mad_count_here = 0;
	dgb_mad_count_pass++;
//	dgb_mad_count_fail--;
	dgb_mad_count_fade = 0;
	dgb_sleep_calc();
}

void dgb_sleep_less_less() // I am doing non-mad things // less and then more so
{
	dgb_mad_count_here = 0;
	dgb_mad_count_pass++;
	dgb_mad_count_pass++;
	dgb_mad_count_pass++;
	dgb_mad_count_pass++;
//	dgb_mad_count_fail--;
//	dgb_mad_count_fail--;
//	dgb_mad_count_fail--;
//	dgb_mad_count_fail--;
	dgb_mad_count_fade = 0;
	dgb_sleep_calc();
}

void dgb_sleep_zero() // error has been acked - reset to zero
{
	dgb_mad_count_here = 0;
	dgb_mad_count_pass = 0;
	dgb_mad_count_fail = 0;
	dgb_mad_count_fade = 0;

	dgb_sleep_calc();
}

#include "MY_WIN32.h"
void dgb_sleep_here() // here is a bit of both and place to sleep
{
	static bool loopy = false;
	if(loopy) { return; };
	loopy = true;
	dgb_mad_count_here ++;
	dgb_sleep_calc_silent();
#warning sleeping switched OFF
 if(0)
	if( dgb_mad_sleeping ) {
		WARN("sleeping SLEEP %d", dgb_mad_sleep_for );
		sleep_secs( dgb_mad_sleep_for );
	}
	loopy = false;
}

