#ifndef spout_obj_ref_H
#define spout_obj_ref_H

#include "spout_STUBS.h"

/*
	we then go on to include _js stuff 
	which might be slow
*/

#include "obj_ref.h"
#include "obj_ref_js.h"
#include "obj_hold.h"

namespace SPOUT_GEN {

/*!
	A base class so that doxygen looks nicer

	_js means that JS is going to call SPOUT ARGV
	_js could be generic for _script
	_js for those that need it ?
	_py for those that need it
	_py when nonpy is avaiable ?
	_tcl PEER  <=--=> UDEF_refounted PEER

	frame CALL as

	 OBJ1 = get_obj( u32_WORD1 ) // u64_WORD_1
	 OBJ2 = get_obj( u32_WORD2 ) // u64_WORD_2

	 Target ARM_64 x4 + ARM_64_slow x2 # 32 registers u16_u48 options

	 u32 gives the u64 limits when u16_CODE u16_DATA
	  u16_u8_u8

	 That limits space to 64K objects (opcode tells us)
	 That limits space to 64K BYTES (u3 gives u19 64x8=256 K)
	 That limits space to POOL = from OPCODE 
	 Item_t * item = POOL[ u16_idx ].as_item();// type cast in func
	 That opens up 

*/
class spout_obj_ref : public obj_ref_js
{
 public:
	// external
};

/*!
	A base class so that doxygen looks nicer
*/
struct spout_root : public GRP_lib_base2_spout
{
 public:
};

}; // NAMESPACE

#endif
