#ifndef sleep_ms_H
#define sleep_ms_H

// maybe convert these to extern "C" { ... }


	extern "C" void sleep_us( int us );
	extern "C" void sleep_ms( int ms );
	extern "C" void sleep_10( int ths );
	extern "C" void sleep_secs( int s );

#endif
