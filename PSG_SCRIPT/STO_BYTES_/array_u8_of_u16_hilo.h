#ifndef array_u8_of_u16_hilo_H
#define array_u8_of_u16_hilo_H

#include "STO_i16_hilo.h"
// #include "STO_i32_hilo.h"

/*
	TUPLE n = SPEC.get_N
	SPEC = u16_hilo_SPEC dereferenced
	ARGS[ u4 ] // from SPEC_OPCODE embedded parameter || SPEC.info

	Maybe func uses u16 to access it's local vars, EXPR's, MACHINE STEPS
	All these u16_hilo are PROBABLY also for a shared CTXT FRAME
	same SEGMENT each FUNC_LOCN // uses 1 locn, plue each ITEM
	near addr is u8 // ITEM_t & ITEM = locn[ u8_item }

	 U8_OPCODE u8_SEGMENT u8_locn u8_item
	 U4_OPCODE u8_SEGMENT u8_locn u12_item // SEGMENT export limit1
	 u8_SEGMENT u24_XXX_STEPS {
	 // XXX
	 	u8_ZONE
		u8_POOL
		u8_ITEM
	 // or
	 	u8_POOL
		u8_ITEM
		u8_EXPR // action, MACHINE STEP, CALL_STEP, u8_STEP
	 // or
	 	u8_TYPE
		u8_ITEM
		u8_CALL // with u8 as a parameter if thats useful
	 }

	 When we already have a SPEC for the parent node
	 AND we already have a DATA for the same loaded active node
	 there may be something we can get from that

	 	POOL [s] = local_scope = DATA._POOL_TABLE_ONE;
		// we know TABLE is a TABLE // using C++ type flow
		POOL_t & pool = local_scope[ u8_POOL_idx ]

		ITEM_t & item = pool.lookup_TEM_idx[ u8_ITEM ];

		RETVAL = item.STEP_EXPR( u8_EXPR );
		//
		// OK RETVAL is vague ATM
		// add STACK of FUNC then THREADS
		//
	
	BUT
		that reduces range down to u16 points of 256 action/vals
		that requires first u8_SEGMENT to be sharper FOCUS CTXT
		BUT ... clutch of NEARBY objs in CPU or STACK
	
	ALSO
		Each POOL can have a complete TYPE_OF_ITEM[ u8 ] == 
		== INDIVIDUALLY_SET_ CURR_== "tagged_Object_V4"
		== COLLECTIVELY_SET_ KNOW == "tagged_Object_V4"
		== COLLECTIVELY_SET_ KNOW == "page_of_TUPLO_XX"
		== COLLECTIVELY_SET_ LINK == decode_tag_table / [u8]
		// SPEC_32 = decode_tag_table[ u8_idx ]
	
	XYZ_MACHINE
		exports STEP[256] from ROM
		exports VARS[256] from STO
		exports VARS[256] from MEM // cache hardwired?

		imports EA_EXPR_32 from ROM // STEPS not OFFS SEGMENT_XYZ
		imports EA_EXPR_32 from ROM // OFFS not STEPS OFFX_idx

		uses u8_u8_from_u16_hilo 
		DECODE u8_u8 from CTXT.LOCN.CSR
	

	CSR_MACHINE
		API via its own 64K world
		can have STACK (unlimited depth)
		can have VARS EXPR IMPORTS TABLES STEPS NAME [s]
		eg of a STEP
		eg var V2 = V1.add_node( ARGS ) // auto-graft steps
		eg var V2 = V1.add_leaf( ARGS ) // 
		eg var LOCN_SPEC = get_SPEC( ARGS )

		We make the first 128 STEPS with u7_idx // QUICK
		We make the next 32K steps with u15_idx  // NEARBY

			[0[128	 N==128 == u7
			[128[256 N==128 == u7 u8 == u15 = 128 * 256 == 32K

		This permits extra XXX STEP opcode, for common phrases
		Keeps NEARBY range of IDX into SEGMENT
		Keeps POSSIBLE the FAR addressing into items above 128

		The selected 128 are not in the 32K, but might be
			16 global CPU_MACHINE register (not cpu tho)
			16 local 16 vars
			16 local 16 plus plus += EXPR L OP R // func line
			16 local 16 plus
		EACH 

*/



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
	// ZERO edge between HEAD and TAIL
	// crossing that LINE
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
