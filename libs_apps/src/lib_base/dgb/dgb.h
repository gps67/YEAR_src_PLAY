#ifndef dgb_H
#define dgb_H

// TODO // __attribute__((__format__ (printf, 1, 2))) // plain printf
// TODO // __attribute__((__format__ (printf, 5, 6))) // fn_WARN(a,b,c,d,fmt,..
// that will check the callers args, 
// https://gcc.gnu.org/onlinedocs/gcc-3.2/gcc/Function-Attributes.html
// __printf__, __scanf__, __strftime__, __strfmon__.
// 5 == position of fmt
// 6 == start of first-to_check // 0 for varargs fprintf

// FLAG # alternate form adds 0x or . or 
// FLAG 0 zero padded
// FLAG - left 
// FLAG (SP) add space before +ve 
// FLAG + add + when +ve
// FLAG ' (UNIX_1) put comma in thousands
// FLAG I (glib2) use fa_IR digits in that locale ??

// FIELD_WIDTH dec // decimal avoid 0 start // 
// FIELD_WIDTH * // *m$ # m-th arg //
// PRECISION . dec // . * // .*m$ // "." means zero // 
// for % d i o u x X // minimum # digits
// for % a A e E f F // significant // maximum // blurry
// LENGTH MODIFIER
// hh
// h
// l long int, 
// ll long long int
// L long double
// z size_t
// t ptrdiff_t
// CONVERSION SPECIFIER %s
// d decimal, padded with 0's,
// i integer, decimal
// 
// u unsigned decimal
// o octal
// x hex
// X HEX
//
// e E -d.ddde-dd
// f F -ddd.ddd // precision == 6
//
// g G double
// a A hex float
// c char // lc wint_t
// s str // ls means wide chars
// p PTR // %#x
// n STO // int * STO = n chars written
// m // strerror(errno) // no parameter // message str
// %% // %




// I keep redefining dgb
// e_print (which is just like printf)
// FAIL, WARN, now with added _fmt,
// FAIL, WARN, now with added __LINE__, __FILE__, __FUNCTION__
// (but to get those extra parameters, you must call FAIL_(fmt,a,b)
// They all return false, so you can return FAIL("FUGAZI");

// YOU CAN USE THIS WITHOUT THE REST OF libs_apps
// YOU MAY STATICALLY LINK IT (.o) WITH YOUR APPLICATION (or dll no probs)
// without it bothering your copywrite of your code
// because if(gone_wrong) return FAIL("explain"); // not a HARPOON
// so the code must exist, you can have it, FAIL WARN INFO PASS e_print dgb
// just dont ask me to extract it into an independent module, one day, laters
// LGPL elsewhere // the rest of libs_apps is LGPL though // Library GPL // was

extern void set_prog_alias( const char * s );
extern const char * get_prog_alias();

typedef void do_ssl_err();
extern void set_dgb_ssl_err( do_ssl_err * cb );

extern int complain_depth;
// calls dgb_stackback print_backtrace // gets STACK each time //
extern bool DEBUG_print_stack( int depth = 30, int skip = 0 ); 
extern bool FALSE_print_stack( int depth = 30, int skip = 0 );

class buffer1;
extern bool demangle_cpp_symbol( buffer1 & buf, const char * symb );

// struct err_int_t;
// #include "CTXT_stubs.h"
struct err_int_t;

extern void errno_zero_fn();
#ifdef WIN32
#define errno_zero() errno_zero_fn();
#define errno_drop() errno_zero_fn();
#else
#define errno_zero() if(errno) errno_zero_fn();
#define errno_drop() if(errno) errno = 0;
#endif
extern void errno_zero_if_read_dry();

extern bool FAIL_clear_error();

extern void dgb_sleep_more(); // maybe I am mad
extern void dgb_sleep_less(); // I am doing non-mad things
extern void dgb_sleep_here(); // here is a bit of both and place to sleep

extern void dgb_sleep_less_less(); // error has been half reported
extern void dgb_sleep_zero(); // error has been reported

// this looks orphaned
enum fn_WARN_tag 
{
	
};

extern bool dgb_fork_stderr_to_tcl_text();

__attribute__((__format__ (printf, 5, 6))) // fn_WARN(a,b,c,d,fmt,..
extern bool fn_WARN(
	const char * note,
	int depth,
	const char * file,
	const char * func,
	const char * fmt,
	...
);
extern bool fn_COMPLAIN1( const char * fmt, ... );
extern bool fn_COMPLAIN2( const char * fmt, ... );
extern bool fn_COMPLAIN3( const char * fmt, ... );

#define COMPLAIN1 fn_COMPLAIN1
#define COMPLAIN2 fn_COMPLAIN2
#define COMPLAIN3 fn_COMPLAIN3

// __func__ is C99
// __PRETTY_FUNCTION__ adds C++ signature with additional () at end!

#define WARN_WARNED() WARN("WARNED") // false
#define FAIL_FAILED() FAIL("FAILED") // false
#define PASS_PASSED() PASS("PASSWD") // true

// ... VARADIC MACROS ...
// ... VARADIC MACROS ... __VA_ARGS__
// ... VARADIC MACROS ...

// TESTING THE SYNTAX - using an otherwwise unsed macro
#if 0
// WIN32 VC suppresses any previous comma, when __VA_ARGS__ is empty
// UNIX/GCC does the correct thing and causes a syntax error if prev comma does
// hence need for separate macros for absent first arg 

#define DIAG_FAIL(...) \
	fn_WARN( "FAIL",   1, __FILE__, __PRETTY_FUNCTION__, (const char *)__VA_ARGS__)
#endif

///////////////// NUMBERLESS SOON //////////////////


#define UNFAIL(...) fn_WARN( "UNFAIL",   1, __FILE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#define FAIL(...)   fn_WARN( "FAIL",   1, __FILE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#define WARN(...)   fn_WARN( "WARN",   2, __FILE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#define INFO(...)   fn_WARN( "INFO",   4, __FILE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#define TODO(...)   fn_WARN( "TODO",   2, __FILE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#define PASS(...)   fn_WARN( "PASS",   2, __FILE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#define STEP(...)   fn_WARN( "STEP",   3, __FILE__, __PRETTY_FUNCTION__, __VA_ARGS__)

#if 0
#define FAIL_(...)   FAIL(__VA_ARGS__)
#define STEP_(...)   STEP(__VA_ARGS__)
#define WARN(...)   WARN(__VA_ARGS__)
#define INFO_(...)   INFO(__VA_ARGS__)
#define TRAC_E(...)  TRACE(__VA_ARGS__)
#define TODO_(...)   TODO(__VA_ARGS__)
#define PASS_(...)   PASS(__VA_ARGS__)

#define TRACE_(...)  STEP(__VA_ARGS__)
#endif


#if 0
// unused ??
#define DUMP(fmt) (FAIL(fmt)&&FALSE_print_stack(5))
#define DUMP_1(fmt,one) (FAIL_1(fmt,one)&&FALSE_print_stack(5))
#define DUMP_2(fmt,one,two) (FAIL_2(fmt,one,two)&&FALSE_print_stack(5))
#define DUMP_3(fmt,one,two,three) (FAIL_3(fmt,one,two,three)&&FALSE_print_stack(5))
#define DUMP_4(fmt,one,two,three,four) (FAIL_4(fmt,one,two,three,four)&&FALSE_print_stack(5))
#define DUMP_5(fmt,one,two,three,four,five) (FAIL_5(fmt,one,two,three,four,five)&&FALSE_print_stack(5))
#endif


#ifdef WITH_GTK
// WIN32 auto.exe uses dgb.h but not dgb_fail.h
// WIN32 auto.exe uses fprint but not e_print (not g_print)
// WITH_GTK is not the correct flag, but ...
#include "dgb_fail.h"
#else
// including anyway ATM
#include "dgb_fail.h"
#endif

#endif

