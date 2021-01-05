
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <curses.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "rad_1.h"
#include "sleep_ms.h"

static rad_1 rad;

/*!
	Print 0,1,2,3,4,5 stars. 4 is max, 5 is excess
*/
char * stars4( double f )
{
	f *= 4;
	f += 0.5;
	int i = (int) f;
	if( i<0) i=0;
	if( i>5) i=5;
	static char buff[10];
	for( int j=0; j<i; j++ )
	{
		buff[j]='*';
	}
	buff[i] = 0;
	return buff;
}

//! Scan through all frequencies, listening and showing sig-strength
int main()
{
	if( !rad.open( "/dev/radio" )) return errno;
	rad.unmute();
	int lo, hi;
	rad.get_lo_hi( lo, hi );
	for( int f = lo; f<hi; f++ )
	{
		char st ='-';
		char line = '\r';
		rad.freq_set( f );
		float mhz = rad.mhz_from_int( f );
		printf(" %5d %6.2f ", f, mhz );
		fflush(0);
		// sleeping longer allows the signal to settle
		// but not by much, and such readings are full of static
		sleep_us( 10 * 1000 );
		// sleep_us( 100 * 1000 );
		float sig = (double)rad.get_signal() / 32768.0;
		if( sig > 0.25 ) {
			sleep_10( 9 );
			line = '\n';
			sig = (double)rad.get_signal() / 32768.0;
			rad.get_stereo();
		}
		if( sig > 0.50 ) {
			sleep_10( 20 );
			sig = (double)rad.get_signal() / 32768.0;
			rad.get_stereo();
		}
		if( rad.is_stereo()) st = 'S';
		printf(" %5.2f %c %s %c", sig, st, stars4(sig), line );
	}
	rad.mute();
	printf("\n");
}
