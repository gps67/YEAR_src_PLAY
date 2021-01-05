#include "e_print.h"
#include "e_print_io.h"
#include "obj_hold.h"

// ## INCLUDE _PRIV - manually pasted
// 
extern obj_hold<e_print_io_INTERFACE> e_print_io;

#include "obj_ref.h"
#include "blk1.h"
#include "buffer2.h"


#ifndef WITH_GLIB
#define WITH_GLIB
#warning "WITH_GLIB needed to be defined"
#endif

#ifdef WITH_GLIB
#include <glib.h>
#endif


e_print_io_INTERFACE::e_print_io_INTERFACE()
: prefers_8859( false )
{
}

// virtual 
e_print_io_INTERFACE::~ e_print_io_INTERFACE()
{
}


/*!
	write_BYTES() should never be used
	you should know what CSET you have and call that.
	Each implementation should handle all,
	eg by converting 8859 to UTF8 and calling write_UTF8

	Maybe the base class could do that,
	if( prefers_8859 ) ... conv utf8 to 8859 where poss
	else ... conv 8859 to UTF8 (which is reliable)

	but for now ...
*/
// virtual 
bool e_print_io_INTERFACE::write_BYTES( blk1 & blk )
{
	return false;
	return write_ASCII( blk );
}

/*!
	ASCII is common to both 8859 and UTF8 except NUL (CTRL)
*/
// virtual 
bool e_print_io_INTERFACE::write_ASCII( blk1 & blk )
{
	return write_BYTES( blk );
}
/*
	If you write more than ascii through this channel,
	it should generate a warning (counter),
	maybe fix-it-up for you,
	still work for various purposes

	Though you are not supposed to do that,
	ASCII really means ascii.
*/
// virtual 
bool e_print_io_INTERFACE::write_8859( blk1 & blk )
{
	if( prefers_8859 ) {
		return write_BYTES( blk );
	} else {
		// conv blk to utf8 and write that
		// TODO for now just output something
		return write_BYTES( blk );
	}
}
// virtual 
bool e_print_io_INTERFACE::write_UTF8( blk1 & blk )
{
	return write_BYTES( blk );
}
bool e_print_io_INTERFACE::write_UTF8( STR0 str )
{
	blk1 blk;
	blk.set( str );
	return write_UTF8( blk );
}

// virtual 
void e_print_io_INTERFACE::app_prefers_8859( bool latin15 )
{
	// it is the _io_ object that prefers 8859 (or not)
	// the application can ask it to change, for speed
	// or create one in the first place !
	// actually some sort of enum_CSET is needed
	// but for me thats latin15 or utf8, nothing else
	prefers_8859 = latin15;
}

// virtual 
void e_print_io_INTERFACE::shutdown()
{
	// a hook to do something ?
}

void C_e_print_io_str( const char * msg )
{
	if( !e_print_io ) {
		// fprintf ... ?
		return;
	}
	e_print_io -> write_UTF8( msg );
}

bool
e_print_io_INTERFACE::
set_as_glib_print_handler()
{
	buffer1 tn;
	static bool created = false;
	if( created ) {
		return false;
	}
#ifdef WITH_GLIB
	g_print("Changing Output Channels - from here OLD\n");
	g_set_print_handler( C_e_print_io_str );
	g_set_printerr_handler( C_e_print_io_str );
	g_print("Switching Output Channels - to here ---typename %s\n", type_name(tn));
	if(e_print_io) {
		// COMPLAIN;
		// forget about it
	}
	e_print_io = this; //
#else
#error GLIB IS NEEDED FOR fd_selectable, and here too - but not GTK though
#endif
	return true;
}

// virtual 
void e_print_io_INTERFACE::switch_gui_onoff( bool onoff )
{
}

void e_print_io_INTERFACE::switch_gui_onoff_toggle()
{
}
