#ifndef SAR_PARSER_TEMPS_H
#define SAR_PARSER_TEMPS_H

#include <misc.h>
#include "sar_parser_floats.h"
#include "tm_parts.h"

//! Holds temp vars for the parser, and the tbl_collect columns
struct sar_parser_temps : public sar_parser_floats
{

	sar_parser_temps( tbl_collect * _tbl ) 
	: sar_parser_floats( _tbl )
	{
	 /*
		It only makes sense for tbl_collect to be entirely
		dedicated to this parser (and controlled by it),
		but the tbl_collect OUTLIVES this object, and
		the data collected here is ADDED to it, not this.

		Each attribute is based on tbl->create_col( ... )

		Specials: the hostname is not known until the header
		is parsed, and it has to be added to each attribute
		(unless you take the hostname from the ctxt)

		Rethink: during a time-shift - which should not
		happen if TZ is correctly set in both parser and sar -
		the parser will fail, when it could procede with
		two segments. tbl_collect cant hande this, but a
		generic mem_dset_* could
	 */

		time_start = -1;
		time_now = -1;
		dt_now = -1;

	}


// file header
	char hostname[22];		// [20] limited in sscanf
	int dd, mm, yy, year;

// stanza time
	int hour, min, sec;

	static const int grain = 10;
	void align_time( time_t & t );
	void set_time_now();
	void adj_time_now();

 	tm_parts time_conv;
	time_t	time_start;
	time_t	time_now;
	int dt_now;

// cpu
	int cpu;

// disk dev
	char device[20];		// [20] limited in sscanf

};
#endif
