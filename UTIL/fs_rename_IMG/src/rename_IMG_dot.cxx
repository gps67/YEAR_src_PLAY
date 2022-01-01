#include "dgb.h"
#include "rename_IMG.h"
#include <unistd.h>

bool rename_IMG_dot(const char * dir)
{
	if( chdir( dir )) {
		return FAIL("chdir %s", dir );
	}
	return FAIL("TODO");
}
