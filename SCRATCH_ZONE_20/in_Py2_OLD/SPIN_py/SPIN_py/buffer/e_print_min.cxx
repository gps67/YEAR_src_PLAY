/*
	e_print( fmt, args, ... ) is like fprintf(stderr, fmt, args, ... )
	g_print is gtk
*/

#include "e_print.h"
#include "buffer1.h"

#include <stdarg.h>
#include <stdio.h>
// #include "GTK_Main_App_Window_Minor.h"
// #include "w_text_view.h"
// #include "w_win_top.h"
// #include "w_vbox.h"

/*!
*/
static buffer1 e_text_buff;

/*!
	problem with top level window - close calls destroy
	but obj_hold holds something
*/

static const bool e_print_view = false;

// class E_Print_View : public GTK_Main_App_Window_Minor


/*!
*/
void C_g_print_str( const char * msg )
{
	if(e_print_view)
	{
	} else {
		fflush(0);
		fprintf(stderr,"%s", msg );
		fflush(0);
	}
}

/*!
*/
void C_g_printerr_str( const char * msg )
{
	if(e_print_view)
	{
	} else {
		fflush(0);
		fprintf(stderr,"%s", msg );
		fflush(0);
	}
}


/*!
*/
void e_perror( const char * fmt, ... )
{
        va_list args;                                                           
        va_start( args, fmt );                                                  
	e_vprint( fmt, args );
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
*/
void e_vprint( const char * fmt, va_list args )
{
/*
	STATIC INITIALISATION before gtk_init
	if( !e_print_view )
		e_print_init();
*/
	if( e_print_view )
	{
	}
	bool print_it_anyway = false;
	print_it_anyway = true;
	if((! e_print_view) || print_it_anyway )
	{
		// should be true - iff output is utf8, which it isnt
		bool conv_8859_into_utf8 = false;
		e_text_buff.vprint( conv_8859_into_utf8, fmt, args );
		e_text_buff.trailing_nul();
		if(0)
			C_g_print_str( (STR0) e_text_buff.buff) ;
		else {
			fflush(0);
			fprintf(stderr,"%s", (STR0) e_text_buff.buff) ;
			fflush(0);
		}
		e_text_buff.clear(); // retain malled block
	}
        va_end( args );                                                         
}                                                                               

void e_print_from_utf8( const char * fmt, ... )
{

        va_list args;                                                           
        va_start( args, fmt );                                                  
	if( e_print_view )
	{
	} else {
		// possible case of convert utf8 to 8859, tty isnt (ignore)
		bool conv_8859_into_utf8 = false;
		e_text_buff.vprint( conv_8859_into_utf8, fmt, args );
		e_text_buff.trailing_nul();
		C_g_print_str( (STR0) e_text_buff.buff) ;
		e_text_buff.clear(); // retain malled block
	}
        va_end( args );                                                         
}                                                                               

void e_print_show()
{
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

/*!
	e_print should be initialised by APP_HOLDER
	Here grab the g_print() hook too!
*/
void e_print_init()
{
	fflush(0);
}

void e_print_shutdown()
{
}

void * e_print_get_object( const char * objname )
{
	return NULL;
}
