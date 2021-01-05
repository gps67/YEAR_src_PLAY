#include "sleep_ms.h"

#ifdef WIN32
#include <windows.h> 
#else
#include <unistd.h> // sleep
#endif
#include <glib.h>

// #define GLIB_NOT_WIN32

#ifdef GLIB_NOT_WIN32

//	GLIB provides sleep with microsecond parameter

	void sleep_us( int us ) {
		g_usleep( us ); //
	}

	void sleep_ms( int ms ) {
		g_usleep( ms * 1000 );
	}

	void sleep_secs( int s ) {
		g_usleep( s * 1000 * 1000 );
	}

#else
#ifdef WIN32

//	WIN32 provides sleep with millisecond parameter

	void sleep_us( int us ) {
		sleep_ms( (600+us) / 1000 ); // APPROXIMATION
	}

	void sleep_ms( int ms ) {
		Sleep( ms ); // cap S
	}

	void sleep_secs( int s ) {
		Sleep( s * 1000 ); // cap S
	}

#else

//	UNIX provides sleep and usleep

	void sleep_us( int us ) {
		usleep( us );
	}
	void sleep_ms( int ms ) {
		usleep( ms * 1000 );
	}
	void sleep_secs( int s ) {
		sleep( s );
	}
#endif
#endif

void sleep_10( int ths ) {
	sleep_ms( ths * 10 );
}
