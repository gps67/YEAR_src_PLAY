#ifndef LINE1_H
#define LINE1_H

#include "misc.h"
// #include "tm_parts.h"

/*!
	The ascii data collector wraps vmstat with an initial line1.

	This class parses that line to get hostname, t0, dt, (nn)

	# host57 # 0731 # vmstat 60 1440 # Sat Jul 31 00:00:01 BST 1999
*/
struct vmstat_wrap_line1 : public GRP_sar_parse
{
	char hostname[11];
	char MMDD[5];
	// vmstat 60 1440 // vm specific
	int dt;
	int nn;
	char tue[4];
	char jan[4];
	int DD;
	int hh;
	int mm;
	int ss;
	char tz[25];
	int year;
	time_t t0;

	time_t calc_t0();

	bool parse_line1( const char * str );

};



#endif
