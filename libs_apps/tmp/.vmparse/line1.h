#ifndef LINE1_H
#define LINE1_H

/*
	note that this is specific to vmstat ...
	other commands would need another level to 
	parse the # command # out of the line

# host57 # 0731 # vmstat 60 1440 # Sat Jul 31 00:00:01 BST 1999
*/

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

// #include <stdlib.h>
// #include <stdio.h>
// #include <time.h>
// #include <errno.h>

#include "misc.h"
#include "tm_parts.h"

struct line1 {
	char hostname[11];
	char MMDD[5];
	// vmstat 60 1440 // vm specific
	int dt;
	int nn;
	// generic
	char tue[4];
	char jan[4];
	int DD;
	int hh;
	int mm;
	int ss;
	char tz[25];
	int year;
	// result
	time_t t0;

	time_t calc_t0();

	int parse_line1( const char * str );

};



#endif
