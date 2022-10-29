
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>


int main( int argc, char ** argv )
{
	// open accepts 2 or 3 args
	// if we dont provide it, the stack provides some random
	int mode = 04755; // suid and 755
	int t = open( "myfilename", O_RDONLY, mode );
	fprintf( stderr, "%s: got %d from open() \n", argv[0], t);
	return 0 ;
}
