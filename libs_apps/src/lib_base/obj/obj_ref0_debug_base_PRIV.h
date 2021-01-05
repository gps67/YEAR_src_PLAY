#ifndef obj_ref0_debug_base_PRIV_H
#define obj_ref0_debug_base_PRIV_H

/*
	everywhere imports obj_ref0, noone needs to know these values

	maybe if some ref_counting needs to be done really quickly
	(inline not a call) ... then put these back
*/


#define DEBUG_OBJ_BIT_ZERO 		 0 // 
#define DEBUG_OBJ_BIT_static		 1 // 
#define DEBUG_OBJ_BIT_CTOR 		 2 // has_been CTOR'd and not DTOR'd
#define DEBUG_OBJ_BIT_DTOR 		 3 // DTOR has been called [done]
#define DEBUG_OBJ_BIT_REFS		 4 // extra debug tracking on REF_COUNTS
#define DEBUG_OBJ_BIT_on_delete		 5 // do ... something ... what ... ?
#define DEBUG_OBJ_BIT_is_widget		 6 // call check_has_ w ..
#define DEBUG_OBJ_BIT_on_idle		 7 // usually PRNT after CTOR on_IDLE
#define DEBUG_OBJ_BIT_track_0		 8 // _LESS_ debug tracking (off)
#define DEBUG_OBJ_BIT_track_1		 9 // extra debug tracking (ONE)
#define DEBUG_OBJ_BIT_track_2		10 // extra debug tracking (TWO)
#define DEBUG_OBJ_BIT_XXXX		11 // extra debug tracking on XXXX

#endif
