/*
	syslog5.c
	report an error via the syslog mechanism
	man syslogd
	man syslog
*/

#include <stdlib.h> /* perror */
#include <errno.h> /* errno */


extern 
#ifdef CPP
"C"
#endif
int syslog_main(int argc, char ** argv);

int main(int argc, char ** argv) {
	int t = syslog_main( argc, argv );
	errno = t;
	if(t) perror( argv[0] );
	return t;
}
