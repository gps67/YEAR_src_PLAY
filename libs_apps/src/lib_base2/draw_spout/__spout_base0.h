#ifndef spout_base_0_H
#define spout_base_0_H

#include "spout_STUBS.h"

#include "obj_ref.h"
#include "obj_ref_js.h"
#include "obj_hold.h"

/*
	avoiding the runtime dynamic load - no point, still have DLL linkage
*/

// # THIS IS SHOUT.h
// #include "spoutlib.h"

// ints.h
// struct GRP_lib_base2_spout {}; // may help DOXYGEN group

/*!
	app_lib defines this refcounted class
*/

namespace SPOUT_GEN {

 struct SPOUT_t; 
 // anonymous type // can use as SROUT_t


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

/*!
	lots of classes need the SPOUT_t pointer but not the extra functions
*/
class spout_base_0 : public spout_obj_ref
{
 public:
 	spout_base_0() {}
	SPOUT_t * spout;

	/*!
		destructor
	*/
	~spout_base_0()
	{
		// NO NO NO // SPOUT_delete( spout );
		// see spout_ctxt
		spout = NULL;
	}

	/*!
	*/
	spout_base_0( SPOUT_t * _spout )
	{
		spout = _spout;
	}
};

 class SPOUT_t : public spout_base_0 {

  SPOUT_t() { late_init(); } // late_init_from_ZERO # flag //
  SPOUT_t(STR0 SCRIPT) { late_init(); } // ignore CTOR request Name
  void late_init() { };

 };


}; // NAMESPACE

#endif
