#include "file_rename.h"
#include <unistd.h>
#include "dgb.h"


#warning UNIX specific

bool file_unlink( const char * old_name )
{
	if( unlink( old_name ) ) {
		return FAIL("unlink(%s)", old_name );
	}
	return true;
}

bool file_link( const char * old_name, const char * new_name )
{
	if( link( old_name, new_name ) ) {
		return FAIL("link(%s,%s)", old_name, new_name );
	}
	return true;
}

bool file_rename( const char * old_name, const char * new_name )
{
	if( !file_link( old_name, new_name ) )
		return FAIL_FAILED();
	if( !file_unlink( old_name ) )
		return FAIL_FAILED();
	
	return true;
}
