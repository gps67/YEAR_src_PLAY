#ifndef e_print_H
#define e_print_H

#include <stdarg.h>

//!  e_print is g_print to a specific error buffer
extern void e_print( const char * fmt, ... );

//!
extern void e_vprint( const char * fmt, va_list args );

//!
extern void e_perror( const char * fmt, ... );

//!  The target knows its own cset, the source can be utf8 but defaults to 8859
extern void e_print_from_utf8( const char * fmt, ... );

//!  any app can gently bring window to front, or at least unhide
extern void e_print_show();

//!  e_print should be initialised by APP_HOLDER
extern void e_print_init();
extern void e_print_shutdown();

//!
extern void * e_print_get_object( const char * objname );


#endif
