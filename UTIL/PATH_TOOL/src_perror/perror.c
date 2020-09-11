#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main( int argc, char ** argv)
{
	int e = 22;
	const char * msg = argv[0];
	argv++;
	argc--;
	if( argc == 0 ) {
		errno = e;
		perror( msg );
		return e;
	}
	e = atoi( * argv );
	argv++;
	argc--;

	if( * argv ) msg = *argv;

	errno = e;
	perror( msg );
	return e;
}
