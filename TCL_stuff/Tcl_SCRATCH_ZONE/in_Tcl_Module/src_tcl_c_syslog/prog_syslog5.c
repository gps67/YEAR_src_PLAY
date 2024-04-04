/*
	syslog5.c
	report an error via the syslog mechanism
	man syslogd
	man syslog
*/

#include <stdlib.h> /* perror */
#include <errno.h> /* errno */

extern 
       void perror(const char *s);



extern 
#ifdef CPP
"C"
// this is C not CXX
#endif
int syslog_main(int argc, char ** argv);

int main(int argc, char ** argv) {
	int t = syslog_main( argc, argv );
	errno = t;
	if(t) perror( argv[0] );
	return t;
}
