#ifndef b512_data_H
#define b512_data_H

#include "ints.h"
#include "dgb.h"

struct b512_data_t {
	const static int N512 = 512;
	const static int N8 = 8;
	typedef u8 name8_t[N8];

	// the device / partion AND each sector starts with "fill_2T" NUL
	// you must manually write that with # printf "fill_2T\0"

	name8_t name;

	// each sector has its byte offset

	i64 sector_offset;

	// the time the request was queued
	i64 time_stamp_us; // us since 1970

	const static int fill_sz
	= N512 - sizeof( name ) - sizeof( sector_offset ) - sizeof( time_stamp_us );

	u8 fill[ fill_sz ];

	b512_data_t();

	bool fill_fill();

	static void copy_name( name8_t & name8 );
	static bool comp_name( const name8_t & name8 );
	bool check_name();
	bool check_sector_is( u64 offs );
	bool check_fill(); // might vary 
	bool time_stamp_now();
	i64 time_stamp_get() { return time_stamp_us; }
	bool time_stamp_set(i64 val)
	{
		time_stamp_us = val;
		return true;
	}

};

#endif
