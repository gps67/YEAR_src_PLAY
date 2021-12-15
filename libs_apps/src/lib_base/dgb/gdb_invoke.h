#ifndef gdb_invoke_H
#define gdb_invoke_H

//! You can switch gdb_ breakpoints off (eg from a menu)
extern bool gdb_off;

//! a function that stops with a gdb breakpoint (x86 only)
extern void gdb_break_point();

//! The gdb_invoke needs a progname
extern const char * progname_argv0;

//! The name of the executable (varies with symb link)
extern const char * progname_name;

//! called by the signal handler (eg split this into POST::post_file.cxx)
extern void gdb_fatal_error_handler(int signo);

//! set progname_argv0 // DONT setup a sigaction default
void set_prog_name( const char * progname );

//! set progname_argv0 and setup a sigaction default
void gdb_sigaction( const char * progname );

//! start gdb with/out the -w gui flag
void gdb_invoke( bool usegui = true );

//! is gdb running? Simple test
extern bool gdb_is_running();



#endif
