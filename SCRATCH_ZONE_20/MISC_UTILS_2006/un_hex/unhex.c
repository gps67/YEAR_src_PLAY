#include <stdio.h>

int main()
{
	int i = 0;
	int * p = &i;
	char c1, c2;
	while( 0 < fscanf(stdin, "%x", p ))
	{
		c1 = i >> 8;
		c2 = (char) i;
		if(c1) printf( "%c", c1 );
		printf( "%c", c2 );
	}
	return 0;
}
