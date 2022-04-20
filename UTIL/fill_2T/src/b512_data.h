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

	u64 sector_offset;

	const static int fill_sz
	= N512 - sizeof( name ) - sizeof( sector_offset );

	u8 fill[ fill_sz ];

	b512_data_t();

	bool fill_fill();

	static void copy_name( name8_t & name8 );
	static bool comp_name( const name8_t & name8 );

};

#endif
