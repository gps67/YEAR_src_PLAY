#include "M_z_N.h"
#include "dgb.h"

bool main_bool( int argc, char ** argv )
{

	IDX_int  V1;
	IDX_int  V2;
	IDX_int  V3;

	L_M_z_N_R_32 
	M_z_N;
	if(!M_z_N.ALLOC_PLUS( V1 )) return FAIL_FAILED();

	return PASS("OK");
	return true;
}

int main( int argc, char ** argv )
{
	if( main_bool( argc, argv ) ) return 0;
	FAIL_FAILED();
	return 1;
}
