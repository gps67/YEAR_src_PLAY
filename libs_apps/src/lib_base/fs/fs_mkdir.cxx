#include "fs_mkdir.h"

#include <sys/stat.h>
#include <sys/types.h>

// #include <fcntl.h>
#include <sys/stat.h>

#include "dgb.h"



bool fs_mkdir( const char * dirname )
{
	mode_t mode = 0777; // octal
	if(  ::mkdir( dirname, mode) == 0 ) {
		return true;
	} else {
		return FAIL("%s", dirname );
	}
}

