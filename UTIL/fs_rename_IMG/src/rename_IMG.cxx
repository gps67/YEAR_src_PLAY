#include "dgb.h"
#include "rename_IMG.h"


bool bool_main( int argc, char ** argv ) {

	set_prog_name( argv[0] ); // check this

//	gdb_invoke(false);

	if( argc != 2 ) {
		errno = 22;
		return FAIL("USAGE %s DIR # ", get_prog_alias() );
	}

	if(!rename_IMG_dot(argv[1])) {
		return FAIL_FAILED();
	}



	return true;
}

int main( int argc, char ** argv ) {
	if( bool_main( argc, argv )) {
		PASS("DONE");
		return 0;
	}
	if(errno) 
		return errno;
	WARN("NO ERRNO");
	return 1;
}
