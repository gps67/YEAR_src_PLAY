

#include "scan_c1.h"

int main(int argc, char ** argv)
{
	if(argc!=2)
	{
		printf("USAGE: %s filename\n",argv[0]);
		exit(1);
	}
	scan_c1 f1;
	if( f1.scan_file( (u8*) argv[1]) ) {
		return 0;
	} else
		return 1;
}
/* -eof- */
