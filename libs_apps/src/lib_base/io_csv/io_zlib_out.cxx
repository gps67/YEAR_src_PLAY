#include <GLOBAL.H>
#include "io_zlib_out.h"
// #include <glib.h>
#include <string.h>

/*!
	destructor - close the fd
*/
io_zlib_out::~io_zlib_out()
{
	close();
}

/*!
	constructor - init null (work done by open())
*/
io_zlib_out::io_zlib_out()
{
	f = NULL;
	f_plain = NULL;
}

/*!
	return true is there hasnt been a disk full error,
	or transmission error, whilst compressing and sending
	the last buffer.
*/
bool io_zlib_out::close()
{
	bool ok = !io_error;
	if(f) {
		gzclose(f);
	}
	f = NULL;
	if(f_plain) {
		fclose(f_plain);
	}
	f_plain = NULL;
	return ok;
}

/*!
	open a zlib disk file (no extension added).
*/
bool io_zlib_out::open( str0 filename, int compression_level )
{
	close();
	io_error = FALSE;
	if( 0< compression_level )
	{
		if( compression_level > 9 ) compression_level = 9;
		char mode[10];
		sprintf( mode, "wb%d", compression_level );
	/*
		wb0 causes the text to not be compressed
		but there is an initial piece of header (junk)
		SO: ensure an initial CRLF?
	*/
		if(1) e_print("# OK # gzopen %s %s\n", (STR0) filename, (STR0) mode );
		f = gzopen( filename, mode );
		if( f ) return TRUE;
		return io_err( "io_zlib_out::open()::gzopen" );
	} else {
		if(1) e_print("# OK # fopen %s wb\n", (STR0) filename );
		f_plain = fopen( filename, "wb" );
		if( f_plain ) return TRUE;
		return io_err( "io_zlib_out::open()::fopen" );
	}
	return io_err( "io_zlib_out::open()" );
}

/*!
	put string - no added CRLF, you do that
*/
bool io_zlib_out::puts( str0 s )
{
	if(f) {
		if( !gzprintf( f, "%s", (STR0) s ) ) {
			if( 0 == s.str_len()) return TRUE;
			return io_err( "gzprintf" );
		}
		return TRUE;
	}
	if( f_plain ) {
		if( EOF == fputs( s, f_plain ) )
			return io_err( "fputs()" );
		return TRUE;
	}
	return io_err( "io_zlib_out::puts -- not open" );
}

/*!
	put a number of spaces - where should all the facy field-formatting helpers go ?
*/
bool io_zlib_out::puts_n_spaces( int nsp )
{
	if( nsp <= 0 ) return TRUE;
	if( f ) {
		gzprintf( f, "%*c", nsp, ' ' );
		return TRUE;
	}
	if( f_plain ) {
		fprintf( f_plain, "%*c", nsp, ' ' );
		return TRUE;
	}
	return io_err( "NOT OPEN" );
}

/*!
	An IO error might be disk-full, TCP/IP error, and would
	only be reported some time after, as in now.

	This virtual function gets called, FALSE gets returned.

	Replace with your own popup-dialog, as this is the discovery
	of the disk-full, not a mild warning.
*/
// VIRUTAL
bool io_zlib_out::io_err( str0 s )
{
	io_error = TRUE;
	if(errno)
	{
		perror( (STR0) s );
	} else {
		e_print("ZLIB IO_ERROR: %d - %s\n", Z_MEM_ERROR, (STR0) s );
	}
	return FALSE; // always return FALSE
}

//
