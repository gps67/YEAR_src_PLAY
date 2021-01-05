#include "io_csv_out.h"
#include <stdio.h>
#include <string.h>


io_csv_out::io_csv_out()
: comma( ", ")
{
	boln = TRUE;
//	comma = ", ";
	io_error = FALSE;
	w_residue = 0;
}
io_csv_out::~io_csv_out()
{
	close();
}
bool io_csv_out::close()
{
	if( ! boln ) eoln();
	io_zlib_out::close();
	w_residue = 0;
	boln = TRUE;
	return !io_error;
}
bool io_csv_out::open( str0 filename, bool compressed )
{
	int lev = -1;
	if( compressed ) lev = 9;
	return io_zlib_out::open( filename, lev );
}

bool io_csv_out::put_field( str0 value, align_val align, int width )
{
	bool right = align.val == align_right;
	return put_field( value, right, width );
}

bool io_csv_out::put_field( str0 value, bool right, int width )
{
	if( ! value )
	{
		// e_print("io_csv put_field - NULL value\n" );
		value = "";
	}
	if( boln )
	{
		boln = FALSE;
		// first field MUST start at boln - no right justify!
	} else {
		puts( comma );
		if( w_residue > 0 ) {
			puts_n_spaces( w_residue );
			w_residue = 0;
		}
		puts(" "); // always one
	}
	int w = strlen( value );
	w_residue += width - w;
	if( right && w_residue )
	{
		if( w_residue > 0 ) {
			puts_n_spaces( w_residue );
			w_residue = 0;
		}
	}
	return puts( value );
}

bool io_csv_out::eoln()
{
	if(0) 
		puts( "\n");
	else
		puts( "\r\n");
	boln = TRUE;
	w_residue = 0;
	return TRUE;
}

