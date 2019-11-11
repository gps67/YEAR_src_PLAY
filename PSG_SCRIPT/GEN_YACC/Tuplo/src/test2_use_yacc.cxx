// #include "e1.tab.hh"

#include <stdio.h>
#include <errno.h>
#include "dgb.h"

extern // "C"
int yyparse();
extern void yyrestart ( FILE *input_file  );

int main( int argc, char ** argv )
{
	const char * filename = argv[1];
	if(!filename) filename = "../test2.test";
	FILE * IN = fopen( filename, "r" );
	if(!IN) {
		FAIL("fopen(%s)", filename );
		return errno;
	}

	yyrestart ( IN );

	yyparse();
	return 0;
}
