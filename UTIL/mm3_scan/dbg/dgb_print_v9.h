#ifndef dgb_PRINT_H
#define dgb_PRINT_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#include "dgb_fluff_v9.h"
#pragma interface

static
	FILE * dgb_out = stdout;

class dgb_print
{
public:

	 dgb_print( void ) 	{ }
	~dgb_print( void ) 	{ }

	void	dgb_print_space( void )
	{
		fprintf( dgb_out, " " );
	}

	void	dgb_print_chr( unsigned char c1 )
	{
		if( c1 < ' ' ) c1='.';
		fprintf( dgb_out, "%c", c1 );
	}

	void	dgb_chr( u8 c1 )  	{ dgb_print_chr( c1 ); }
	void	print_chr( u8 c1 ) 	{ dgb_print_chr( c1 ); }
	void	chr( u8 c1 ) 		{ dgb_print_chr( c1 ); }

	void	dgb_print_str( const char * str )
	{
		fprintf( dgb_out, "%s", str );
	}

	void	dgb_printf( const char * fmt, ... )
	{
		va_list ap;
		va_start(ap, fmt);
		(void) vfprintf( dgb_out, fmt, ap);
		va_end(ap);
	}

	void	dgb_fmt( const char * fmt, ... )
	{
		va_list ap;
		va_start(ap, fmt);
		(void) vfprintf( dgb_out, fmt, ap);
		va_end(ap);
	}

	void	fmt( const char * fmt, ... )
	{
		va_list ap;
		va_start(ap, fmt);
		(void) vfprintf( dgb_out, fmt, ap);
		va_end(ap);
	}

/*
	void	dgb_print_bit( int k )
	void	dgb_print( void )
*/

};

static dgb_print dgb;

#endif
