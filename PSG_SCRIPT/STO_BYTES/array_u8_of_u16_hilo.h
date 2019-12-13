#ifndef array_u8_of_u16_hilo_H
#define array_u8_of_u16_hilo_H

#include "STO_i16_hilo.h"
// #include "STO_i32_hilo.h"




namespace STO {

struct u16_hilo : public i16_hilo
{
	// add CTORS and furniture
	// add unsigned cast - or clone the functions
	// could even split to parallel ?
	// decinde on N_alloc N_used flaz ZERO_means_FULL
};

/*!
	not sure, could simply attach a memory block, prefixed with this
	if the compiler adds 255 checks .. rework

	maybe rename array_u16_of_u16

*/
struct array_u8_of_u16
{
	u8 u8_obj_SPEC;	// type selector
	u8 u8_obj_N; // 

	u16_hilo obj_SPEC;
	u16 obj_N; // 0 means 0 or 256 or more
	u16_hilo obj_DATA[255];

	bool ary_get( u16_hilo & hilo, i16 idx ) 
	{
	 if( idx < 0 ) {
		return FAIL("idx %d negative N %d", idx, u8_obj_N );
	 }
	 if(!( idx < obj_N )) {
		return FAIL("idx %d exceeded N %d", idx, u8_obj_N );
	 }
	 // ( idx < obj_N )
	 // idx is in range
	 hilo = obj_DATA[ idx ];
	 return true; // u8 did not overflow u8 
	}

}; // namespace

#endif
