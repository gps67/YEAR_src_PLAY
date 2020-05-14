#include <stdio.h>

main()
{
	int i = 0;
	int * p = &i;
	char c1, c2;
	while( 0 < fscanf(stdin, "%x", p ))
	{
		c1 = i >> 8;
		c2 = (char) i;
		printf( "%c%c", c1, c2 );
	}
	exit(0);
}
