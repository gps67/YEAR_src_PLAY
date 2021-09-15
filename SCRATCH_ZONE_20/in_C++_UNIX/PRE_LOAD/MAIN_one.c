
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>


int main( int argc, char ** argv )
{
	int t = open( "myfilename", O_RDONLY );
	fprintf( stderr, "got %d from open() \n", t);
	return t ;
}
