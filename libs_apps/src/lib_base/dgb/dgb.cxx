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
#include "dgb_call_stack.h" // man 3 backtrace
	// DECL dgb_print_call_stack(stderr, depth, skip);
#include "obj_ref.h" // does work, then string to output
#include "buffer2.h" // does work, then string to output
#include "err_int.h" // does work, then string to output

#define WITH_EPRINT 1

#if WITH_EPRINT
#include "e_print.h"
#endif

#include "fork_exec_setup.h" // e_print stderr to child stdin
#include "fork_exec_parent.h" // 

#if 0 
// moved to dgb_call_stack.h
#define dgb_FMT_3			"# %s # %-5s # %-12s # "
#define dgb_FMT_3_2			"# %-4s # %-12s # "
#define dgb_FMT_WIN_d_s 		"# WIN32 ERRNO # %d # %s"
#define dgb_FMT_UNIX_d_s 		"# LIBC ERRNO # %d # %s"
#define dgb_FMT_UNIX_d_s_lf		 dgb_FMT_UNIX_d_s "\r\n"
#define dgb_FMT_3_s_s_s 		"# %s # %s # %s: "
#define dgb_FMT_3_s_s_s_par		"# %s # %s # %s(): "
#endif

// extern
bool DEBUG_print_stack( int depth, int skip )
{
	int show = depth;
	return dgb_print_call_stack( stderr, show, skip );
}


#ifdef WIN32
bool dgb_fork_stderr_to_tcl_text()
{
	return FAIL("TODO WIN32");
}
#else
bool dgb_fork_stderr_to_tcl_text()
{
	// call this as line 1 of main()
	// INFO will appear in the Tcl/Tk window
	// gdb -tui then runs a lot smoother
	// no interruptions from INFO
	//
	// TODO // gdb -tui // sees stdout notatty // refuses
	// so /dev/ptm on stdout !!!
	// nb OK for gdb invoked BEFORE this func, 
	// NOT OK for gdb invoked on gdb_invoke() AFTER this func
	// so rerun as mk_gdb_test8 
	// TODO // try dup2 1 2 // if not a tty
	//
	// tcl_text // tcl_less.tcl // is on PATH
	// it reads stdin and writes to tk_text widget
	// that can be edited, copy+paste, 
	// see YEAR_src_PLAY on github
	//	 "/home/gps/YEAR/src/PLAY"
	//	 "/SCRATCH_ZONE_20/in_Tcl_Module"
	//
	fflush(0);
	pipe_to_child
	 pipe_parent_to_child;
	if( !pipe_parent_to_child.open_pipe() ) return FAIL("open_pipe");
	int pid_child = fork(); 
	if( pid_child == -1 )
	{               
		e_print("ALERT fork() failed %m\n");
		return false;
	}               
	if( pid_child == 0 ) // is child
	{       
		// fprintf(stderr,"CHILD from fork\n");
		fflush(0);
		pipe_parent_to_child.as_child(); // sets fd_here to fd_read
		pipe_parent_to_child.dup2(0); // stdin
	//	fork_parent->exec_child();
		const char * filename =
	#if 0
		 "/home/gps/YEAR/src/PLAY"
		 "/SCRATCH_ZONE_20/in_Tcl_Module"
		 "/tcl_less/"
	#endif
		 "tcl_less.tcl";
		char * const argv[] = { (char *) filename, NULL, NULL };
		char ** envp = environ; // global
	//	execve( filename, argv, envp ); // exact filename no PATH search
		execvpe( filename, argv, envp );
		fprintf(stderr, "EXEC %s FAIL %m \n", filename );
		e_print( "ALERT exec() %s %m\n", filename );
		fflush(0);
		_exit( errno );
		return false;
	} else { // is parent so tell OBJ to act as_parent()
		pipe_parent_to_child.as_parent();
		// set process errout fd2 to the pipe
		pipe_parent_to_child.dup2(2); // stderr
		// fd_OBJ.dup(2) // fd2_is_errout_of_process
	}

	INFO("NB you have to press the EXIT button - after reading the closing messages");
	return PASS("PASS FAIL WARN INFO should now be in tcl/tk text window");
}
#endif

int V_print( const char * fmt, va_list args )
{
	// FAIL calls WARN_fn calls X_print calls V_print calls e_print (or stderr)
	// e_print calls e_vprint
	// e_vprint uses a vector which is NULL so it uses printf stderr //
	// static obj_hold<e_print_io_INTERFACE> e_print_io; // e_print.cxx
	// hmm needs a tidy up //
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
	e_print( "%s", (STR0) buf1 ); // 2022 this is in use
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
		X_printf( dgb_FMT_WIN_d_s, (int) err_int.err, (const char *) err_int.e_str );
	}
#else
	if(err_int.is_error()) {
		X_printf( dgb_FMT_UNIX_d_s_lf, errno, (STR0) err_int.e_str );
		if(0) DEBUG_print_CALL_STACK(6,2);
	}
#endif
#endif
}


// DECL // bool dgb_print_call_stack(FILE *out, int depth, int skip );

#if 0
bool DEBUG_print_STACK2(int depth, int skip = 0) {
	WARN("DELETE ME");
	return dgb_print_call_stack(stderr, depth, skip + 1);
}
#endif

bool DEBUG_print_STACK(int depth, int skip )
{
	// lets hope it wasnt a stack overflow
	// one extra layer needed // dgb_print_call_stack(stderr);
	return dgb_print_call_stack(stderr, depth, skip + 1);
}

bool FALSE_print_STACK( int depth, int skip )
{
	return dgb_print_call_stack(stderr, depth, skip + 1);
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
	if( (NOTE[0] == 'I') && (0==strcmp(NOTE, "INFO")) ) {
		ret_val = true;
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
		// DEBUG_print_CALL_STACK2();
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
		X_printf( dgb_FMT_3, get_prog_alias(), "XXXX", __FUNCTION__ );
		X_printf( "demangle_cpp_symbol failed\n" );
	}

if(0)	X_printf( dgb_FMT_3,   get_prog_alias(), NOTE, (STR0) func_name );
else	X_printf( dgb_FMT_3_2,                   NOTE, (STR0) func_name );

	va_list ap;
	va_start(ap, fmt);
	V_print( fmt, ap );
	va_end(ap);

	if( !ends_with_lf( fmt ) ) X_printf( "\r\n" );
	// if( do_err ) do_ERRNO();
	do_ssl_ERR();
	if(do_stack_dump) if(!ret_val) DEBUG_print_stack(); 

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

#ifdef WIN32 // ----------- WIN32 SILENT NOOP ----------------------

// WIN32 backtrack is different // NOOP // SILENT DEFAULT
// proof of optimisation is OK // switch off at RELEASE // NO PRINT SILENT DEFAULT
// APPS using specific versions DEBUG_print_backtrace();

void dgb_print_call_stack(FILE *out, int show, int skip )
{
	// no execinfo.h (it is /usr/include/)
	// so default = do nothing silent 
	// TODO dgb_backtrace_WIN32
}

#else	//	------------- LIBC LINUX ---------------

// LINUX/FOSS specific ? GCC specific ? 
// http://idlebox.net/2008/0901-stacktrace-demangled/cxa_demangle.htt

// #include <execinfo.h>
// #include <cxxabi.h>
// #include "dgb_call_stack.h"
// extern bool dgb_print_call_stack(FILE *out, int show, int skip );

#endif // WIN32

// extern
bool demangle_cpp_symbol( buffer1 & buf2, const char * symb ) {
	return dgb_demangle_cpp_symbol( buf2, symb );
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

	if(0)
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

