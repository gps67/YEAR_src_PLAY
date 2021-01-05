/*
	time_u.h

	Instead of signed time_t, use unsigned time_u for abs times

	REASON: Jan 10 2004 is when time_t is 0x40-00-00-00

	The average of two times will be negative, unless you take care!

		T_mid = ( T_left + T_right ) / 2

	even
		unsigned long 
		T_mid = ( T_left + T_right ) / 2

	fails, because the unsigned convertion is done AFTER the shift!

		T_mid = unsigned( T_left + T_right ) / 2


	I hit this problem searching for T_mid when isdst changes,
	but ANY BINARY SEARCH with a SIGNED int key index will fail here.

*/


