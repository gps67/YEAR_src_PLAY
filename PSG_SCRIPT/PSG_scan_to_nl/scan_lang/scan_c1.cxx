#include "scan_c1.h"
#include "str1.h"
#include "buffer1.h"

#include <stdio.h>

/*!
*/
void scan_c1::get_x_y_start()
{
	LEX.get_x_y( x_start, y_start );
}

/*!
*/
bool scan_c1::scan_file( const u8 * filename )
{
	if(!open_file( filename ))
		return false;
	
	return LEX.scan_file( filename );
}
