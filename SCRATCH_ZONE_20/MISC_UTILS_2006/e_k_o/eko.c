#include <stdio.h>

int main( int argc, char ** argv )
{
	const char * sep = "";
	int i;
	for( i=0; i< argc; i++ )
	{
		printf("%s%s", sep, argv[i] );
		sep = " ";
	}
	printf("\n");
	return 0;
}
