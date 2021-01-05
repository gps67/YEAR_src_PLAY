#ifndef spout_base_0_H
#define spout_base_0_H

#include "spout_STUBS.h"

#include "obj_ref.h"
#include "obj_ref_js.h"
#include "obj_hold.h"

/*
*/

/*!
	SPOUT_GEN namespace == all of SPOUT
*/

namespace SPOUT_GEN {

  /*!
	  A base class so that doxygen looks nicer (not obj_ref)
  */
  // ints.h // struct GRP_lib_base2_spout : public GRP_lib_base2 {};


  /*!
	  A base class so that doxygen looks nicer
	  NB is _js so that scripts can use any of these
  */
  class spout_obj_ref : public obj_ref_js
  {
   public:
	  // external
  };

  /*!
	  lots of classes need the SPOUT pointer but not the extra functions
  */
  class spout_base_0 : public spout_obj_ref
  {
   public:
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


}; // NAMESPACE

#endif
