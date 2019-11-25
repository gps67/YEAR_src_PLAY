
// IDK - WUT ?
// #include "STO/sto_ref.h" // soon to be ROM_counted client needs ACCESS to ROM
// #include "sto_ref.h" // avoid root names like "object" eg "sto_obj"
// DIALECT USAGE LEXICON PHRASE // { TEXT }
// // #include "STO/sto_obj.h"
// // USAGE a sto is an obj GC over FILE
// // USAGE STO & obj ( ARGS ); //_t // _t is a VIEW TIME decoration
// // USAGE OBJ & obj ( ARGS ); //_t // SUBLEXX /*excess detail */  OBJ_t //
//
// DATA_VIEW //
// OR // #include "STO.h" // import module STUBS
// IDK - WUT ?

// COPY_ONE adds this first line(s)
// MERGES into adjustable-code-generator // m4 or something
// TRACKS actual process_USED_RULE

/*
	STO aims for PAGE_4K alignment

		SHIFT_POINT_12
		MASK 0x0FFF
		CODE R >> 12 PAGE_of_OFFS
		CODE R << 12 OFFS_of_PAGE
		// twisters of first_users_of_name_ get free mapping 
		// PAGE_VIEW // SCRIPT_VIEW 
		CODE R & MASK // alignment check 0=OK & VAL=OFFS
		CODE R & !MASK // POINT from SHIFT_POINT


		/ * --
 These are junction boxes
 Bench creates them
 User uses them for some semantic payload
 Start by creating a LOCN here = HERE = TREE.HERE.building_LIST_of_branches
 FRAME HERE {
 	STO LIST of TREE_branch
	 alias "VIEW_CODE"
	  item "PAGE_VIEW"
	  item "SCRIPT_VIEW"
	  item "STO_PAGE_VIEW" // 4K MMAP file
	  item "ROM_PAGE_VIEW" // 4K MMAP file

	 / *
	 	This LIST uses aliases "VIEW_CODE"
		because it is about generating the CODE needed by VIEW_CODE

		MMAP_PAGE_FILE // not yet a VIEW, it is the STO_SPEC

		ROM_PAGE_VIEW is the preferred viewer for RAM_PAGE_VIEW
		Our debugger, DATA_VIEW is for ROM with annotations in STO_2

		SCRIPT_VIEW is both DATA and RT_SNAP_of_running_script
		 running_script
		 .stack
		 .globals
		 .thread_var_pool_v

	 * /
 }
*/

// TODO // FILTER for C++ that permits nested comments // eg if 0 //

/*
	FRAME for STO that uses aligned PAGE_4K

	MMAP will detect next first use
	 bool on_first_MMAP_in( ARGS ) {
	 // this page is one of huge chunk, grow buffer
	 // so file usage increases with time + sto used
	 // MMAP in a new NULL_DATA page
	 // call the USER can INIT page function
	 // which is this one
	 //
	 // CODE FOR MMAP( args ) 
	 }
*/
#include "sto_4k.h"
