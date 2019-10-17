#ifndef gps_MISC_H
#define gps_MISC_H

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0

struct GRP_lib_base { };

//! You can switch gdb_ breakpoints off (eg from a menu)
extern bool gdb_off;

//! a function that stops with a gdb breakpoint (x86 only)
extern void gdb_break_point();

//! The gdb_invoke needs a progname
extern const char * progname_argv0;

//! called by the signal handler (eg split this into POST::post_file.cxx)
extern void gdb_fatal_error_handler(int signo);

//! set progname_argv0
void gdb_progname( const char * progname );

//! set progname_argv0 and setup a sigaction default
void gdb_sigaction( const char * progname );

//! start gdb with/out the -w gui flag
void gdb_invoke( bool usegui );

// define assert(REQ) /**/
#include <assert.h>

extern void sleep_10( int tenths );


#endif
