#include "png_one.h"
#include "dgb.h"

#include "buffer2.h"
#include <stdlib.h>

bool bool_main( int argc, char ** argv ) {

	const char * filename_1 = "/tmp/filename_1.png";
	const char * filename_2 = "/tmp/filename_2.png";
	const char * filename_3 = "/tmp/filename_3.png";

	filename_1 = "/home/gps/Downloads/sea_ice_vol_zoomed_out.png";


	png_one png;
	if(!png.read_from_file( filename_1 )) return FAIL_FAILED();
	png.draw_nasty_blob(); // test RGBA 
	if(!png.write_to_file( filename_2 )) return FAIL_FAILED();

	buffer2 cmd;
	cmd.print( "mv %s %s ; m %s ; sleep 5; rm %s ",
		filename_2, // mv 2 3
		filename_3,
		filename_3, // view 3
		filename_3 // delete 3
	);
	system( (STR0) cmd );
	return true;
}

int main( int argc, char ** argv )
{
	if( bool_main( argc, argv )) {
		return 0;
	} else {
		if(errno) return errno;
		return 1;
	}
}
