
#include "b512_data.h"
#include <string.h> // memset
#include "str0.h"
#include "time_hires.h"

static b512_data_t:: name8_t name_main =
 { 'f', 'i', 'l', 'l', '_', '2', 'T',  0 };

void b512_data_t:: copy_name( name8_t & name8 ) {
	for( int i=0; i < N8; i++ ) 
		name8[i] = name_main[i];
}

bool b512_data_t:: comp_name( const name8_t & name8 ) {
	for( int i=0; i < N8; i++ ) 
	  if( name8[i] != name_main[i] )
	  	return false;
	return true;
}

bool
b512_data_t::
fill_fill()
{
	memset( & fill, 0xFA, fill_sz );
	return true;
}

bool
b512_data_t::
time_stamp_now()
{
	time_hires_t time_hires;
	time_hires.call_get_time();
	time_stamp_us = time_hires.time64;
	return true;
}

b512_data_t::
b512_data_t()
{
	copy_name( name );
	fill_fill();
	sector_offset = 0;
	if( comp_name( name ) ) 
		PASS("match");
	else
		FAIL("mismatach");
}

bool
b512_data_t::
check_name() {
	if(comp_name( name )) {
		return true;
		return PASS("expected '%s' got '%s'",
			(STR0) name_main,
			(STR0) name
		);
	} else {
		return FAIL("expected '%s' got '%s'",
			(STR0) name_main,
			(STR0) name
		);
	}
	return true;
}

bool
b512_data_t::
check_sector_is( u64 offs )
{
	if( sector_offset == offs ) {
		// match 
	} else {
		return FAIL("sector_offset 0x%08llX offs 0x%08llX", 
			sector_offset, offs );
	}
	return true;
}

bool
b512_data_t::
check_fill()
{
	for( int i=0; i<fill_sz; i++ ) {
		if( fill[i] != 0xFA ) {
			return FAIL("expected bytes 0x%2X got 0x%2X at offs %d",
				0xFA, fill[i], i );
		}
	}
	return true;
}


