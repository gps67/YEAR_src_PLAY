#include "fd_hold.h"
#include "dgb.h"

bool bool_main(int argc, char ** argv )
{
	obj_hold<fd_hold_1> fd_blk = new fd_hold_1();
	if(!fd_blk->open_RO( "/dev/sdc2" )) return FAIL_FAILED();
	for( int m=0; m <34; m++ ) {
		int k = m * 1024;
	}
	return true;
}

int main(int argc, char ** argv )
{
	if( bool_main( argc, argv ) ) return 0;
	if( errno ) return errno;
	return 1;
}
