#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char ** argv )
{

	int dt = 0;
	time_t t2;
	int i;

	// obtain root perms (file owner)
        if( setuid(0) ) perror( "setuid" );
	// being root, and using system() without abs paths means risks ...
	setenv( "PATH", "/usr/sbin:/sbin:/usr/bin:/bin", 1 );

	if(argc != 2 ) {
		errno = 22;
		perror( "single arg DT required, eg adjtime -59" );
		return 22;
	}

	dt = atoi( argv[1] );
	if( !dt ) {
		errno = 22;
		perror( "DT must be an integer" );
		return 22;
	}

	sleep(1);		// align close to edge
	system( "date" );	// print the before value
	sleep(1);		// align closer to second edge
	time( &t2 );		// get, add dt and set time
	t2 += dt;
	if( -1 == stime( &t2 ) ) {
		perror( "stime()" );
		return errno;
	}
	// end of critical 'close-to-edge' zone

	system( "date" );	// print the after value

	// update the hardware clock // LURK /sbin on PATH! // 

	if( system( "hwclock --utc --systohc" ))
	{
		perror( "FAIL: hwclock" );
	} else {
		printf("system clock updated\n" );
	}

	// now loop showing time

	for( i = 1; i < 12; i++ )
	{
		time( &t2 );
		dt = 10 - (t2 % 10);
		sleep( dt );
		// dt = 10;
		system( "date" );
	}
	return 0;
}
