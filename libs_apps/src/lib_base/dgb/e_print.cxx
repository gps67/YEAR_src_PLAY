#include "e_print_io.h"
#include "e_print.h"
#include "obj_hold.h"
#include "g_print.h"
#include "buffer1.h"

#include <stdarg.h>

#include "buffer2.h"

// extern obj_hold<e_print_io_INTERFACE> e_print_io;
/*!
	there is a single output window (or TTY, or ... )
*/
// static
obj_hold<e_print_io_INTERFACE> e_print_io;

/*!
	it uses a buffer - or it has one itself ...

	If you add one to e_print_io, remove it from here
*/
static buffer1 e_print_temp_buffer; // e_print_io -> buff

void eg_C_g_print_io_str( const char * msg )
{
	if( !e_print_io ) {
		// fprintf ... ?
		return;
	}
	e_print_io -> write_UTF8( msg );
}

/*!
*/
void e_perror( const char * fmt, ... )
{
        va_list args;                                                           
        va_start( args, fmt );                                                  
	e_vprint( fmt, args );
        va_end( args );                                                  
	e_print(" %p\n");
}

/*!
*/
void e_print( const char * fmt, ... )
{
        va_list args;                                                           
        va_start( args, fmt );                                                  
	e_vprint( fmt, args );
        va_end( args );                                                         
}

/*!
	THEY ALL COME THROUGH HERE

	Even if you want 8859, you get utf8, and have to translate it

	Need utf8_to_8859 for buffers
*/
void e_vprint( const char * fmt, va_list args )
{
	if(!e_print_io) {
		// COMPLAIN ## debugging needed anyway!
		// fprintf(stderr, "### no e_print_io ### \n" );
	}

	buffer1 e_print_temp_buffer; // e_print_io -> buff
	bool conv_8859_into_utf8 = false; // its already utf8
	e_print_temp_buffer.vprint( conv_8859_into_utf8, fmt, args );
	e_print_temp_buffer.trailing_nul();
	if(e_print_io) {
		e_print_io->write_UTF8( e_print_temp_buffer );
	} else {
		fprintf(stderr, "%s", (STR0) e_print_temp_buffer );
		fflush(0);
	}
	e_print_temp_buffer.clear(); // retain malled block
}                                                                               

void e_print_from_utf8( const char * fmt, ... )
{
	if( !e_print_io ) {
		// COMPLAIN
		return;
	}

	buffer1 e_print_temp_buffer; // e_print_io -> buff
	bool conv_8859_into_utf8 = false; // its already utf8
        va_list args;                                                           
        va_start( args, fmt );                                                  
	e_print_temp_buffer.vprint( conv_8859_into_utf8, fmt, args );
        va_end( args );                                                         
	e_print_io->write_UTF8( e_print_temp_buffer );
	e_print_temp_buffer.clear(); // retain malled block
}                                                                               

void e_print_show()
{
	if(!e_print_io)
	{
		// COMPLAIN
		// e_print_init(); // fall back to console ??
		if(!e_print_io)
			return;
	}
	if(!e_print_io)
		return;
	e_print_io->show_unhide();
}

void e_print_switch_gui_onoff( bool onoff )
{
	if(!e_print_io) return;
	e_print_io-> switch_gui_onoff( onoff );
}

void e_print_switch_gui_onoff_toggle()
{
	if(!e_print_io) return;
	e_print_io-> switch_gui_onoff_toggle();
}

void e_print_char_table()
{
	for( int hi=2; hi<16; hi++ )
	{
		for( int lo=0; lo<16; lo++ )
		{
			int c = hi*16 + lo;
			e_print(" %c", (unsigned char) c );
		}
		e_print("\n");
	}
	e_print("\n");
	for( int hi=2; hi<16; hi++ )
	{
		for( int lo=0; lo<16; lo++ )
		{
			int c = hi*16 + lo;
			e_print(" %2.2x %c", (int)c, (unsigned char) c );
		}
		e_print("\n");
	}
}

// ///////////////////// - move to base2 where gtk_text is available
#if 0

/*!
	e_print should be initialised by APP_HOLDER
	Here grab the g_print() hook too!
*/
void e_print_init()
{
 return;
	/*
		there is a loop, there is also re-init on program exit.
		It exists once ... but not before app init set ...
	*/
	static bool created = false;
	if( created ) {
		// fprintf(stderr,"# G_PRINT_init:loop_guard \n" );
		return;
	}
	created = true; // prevent recursive loop
	// fprintf(stderr,"# G_PRINT_init:start \n" );
	e_print_view = new E_Print_View();

	g_print("Switching Output Channels - from here\n");
	g_set_print_handler( C_g_print_str );
	g_set_printerr_handler( C_g_print_str );
	g_print("Switching Output Channels - to here\n");

	// fprintf(stderr,"# G_PRINT_init:end \n" );
	fflush(0);
	if(0) e_print_char_table(); // test utf8 from 8 bit 8859, this font
}

void e_print_shutdown()
{
	e_print_view = NULL;
}

// void * e_print_get_object( const char * objname )
// {
// 	return NULL;
// }
#endif

// extern
void e_print_hex_dump( void * buff, int len )
{
	buffer2 b2;
	b2.hex_dump_into_8859( buff, len );
	e_print( "%s", (STR0) b2 );
}
