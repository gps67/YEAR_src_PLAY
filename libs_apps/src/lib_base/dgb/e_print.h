#ifndef e_print_H
#define e_print_H

// class g_text_buffer;

#include <stdarg.h>
#include "obj_hold.h"

class e_print_io_INTERFACE;
extern e_print_io_INTERFACE * new_e_print_stderr();
// see include_PRIV // manually pasted
// 
// extern obj_hold<e_print_io_INTERFACE> e_print_io;

// extern bool install_e_print_io( e_print_io_INTERFACE & io );

//!  e_print is g_print to a specific error buffer
extern void e_print( const char * fmt, ... );


//!  The target knows its own cset, the source can be utf8 but defaults to 8859
extern void e_print_from_8859( const char * fmt, ... );

//!  The target knows its own cset, the source can be utf8 but defaults to 8859
extern void e_print_from_utf8( const char * fmt, ... );


//! debugging thing - eg hex dump TCP/IP PKT header fragment to e_print
extern void e_print_hex_dump( void * buff, int len );

//!
extern void e_vprint( const char * fmt, va_list args );

//!
extern void e_perror( const char * fmt, ... );

//!  any app can gently bring window to front, or at least unhide
extern void e_print_show();

//!
extern void e_print_switch_gui_onoff( bool onoff );
extern void e_print_switch_gui_onoff_toggle();

//!  e_print should be initialised by APP_HOLDER
// extern void e_print_init();
// class TOPAPP_Holder; // probably in a namespace
#include "APPS_stubs.h"


// provided in libbase2/gtk_text
extern void e_print_init_gtk_text(APPS::TOPAPP_Holder * holder);
extern void e_print_init_console(); // provided in libbase2/gtk_text
extern void e_print_shutdown();

//!
extern void * e_print_get_object( const char * objname );


#endif
