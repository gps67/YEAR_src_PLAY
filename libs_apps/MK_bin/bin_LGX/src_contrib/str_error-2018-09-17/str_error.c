#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int bool;
#define false 0
#define true 1;

bool str_is( const char * lhs, const char * rhs ) {
	return strcmp( lhs, rhs ) == 0;
}

bool str_as_int( const char * str, int * val ) {
	* val = 0;
	int v = atoi( str );
	if(v < 0) {
		* val = -v;
		return true;
	}
	if(v > 0) {
		* val = v;
		return true;
	}
	while( *str && (*str<=0x20)) {
		str ++;
	}
	if( '0' == *str ) {
		// accept actual 0 was found
		* val = 0;
		return true;
	}
	return false;
}

int main( int argc, char ** argv ) {

	const char * argv_0 = argv[0];
	argv++;
	argc--;

	if(!argc) { // it was 1 which was progname // now absent
	  printf("# %s usage error - expected errno message not nothing\n",
  	    argv_0
	  );
	  return 22;
	}

	int err = 0;
	if( str_as_int(*argv, & err )) {
		argv++;
		argc--;
	} else {
		err = 22;
	}

	if( err ) 
		printf("# FAIL # %d #", err );
	else
		printf("# PASS # %d #", err );

	printf(" %s #", strerror( err ) );
	while( argc > 0 ) {
		printf(" %s", *argv );
		argv++;
		argc--;
	}
	printf("\n");
	return err;
}
