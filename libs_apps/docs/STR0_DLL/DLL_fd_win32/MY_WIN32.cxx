#include "MY_WIN32.h"
#ifdef WIN32
#else
#include <unistd.h> // sleep
#endif
#include <glib.h>

#define GLIB_NOT_WIN32

#ifdef GLIB_NOT_WIN32


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

	void sleep_us( int us ) {
		g_usleep( us ); //
	}

	void sleep_ms( int ms ) {
		Sleep( ms ); // cap S
	}

	void sleep_secs( int s ) {
		Sleep( s * 1000 ); // cap S
	}

#else

	void sleep_us( int us ) {
		g_usleep( us ); //
	}

	void sleep_ms( int ms ) {
		sleep( 1 ); // LURK TODO
	}
	void sleep_secs( int s ) {
		sleep( s );
	}
#endif
#endif
