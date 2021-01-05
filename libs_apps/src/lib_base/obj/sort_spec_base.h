#ifndef sort_spec_base_H
#define sort_spec_base_H

#include "is_diff.h"
// #include "tbl_col_spec.h"
#include "obj_list.h"
// #include "tbl_row_inst.h"
// #include "table.h"
#include <typeinfo>

#define VERBOSE if(0)

// obj_list is DEFINED as a set of obj_hold pointers
// so save some ref_incr() calls by extracting those pointers

#define BASE0 obj_ref
class sort_spec_base : public obj_ref
{
 public:
	// Prototype for the do_cmp member function
	typedef IS_DIFF_t (sort_spec_base::* MFN_do_cmp)(
			BASE0 * p1,
			BASE0 * p2
	);                                                  

	sort_spec_base()
	{
	}

/*
	// SAMPLE:
	// heres a worked example of using a FORGED item type
	// It SHOULD be obj_ref * but is actually row_inst
	// The kludge is the cast in get_do_cmp
	// row_inst MUST be derived from obj_ref
	// and ALL items in the list MUST be (at least) of this type
	
	MFN_do_cmp get_do_cmp()
	{
		return (MFN_do_cmp) &sort_spec_base::do_cmp;
	}

	IS_DIFF_t do_cmp( row_inst * p1, row_inst * p2 )
	{
		e_print("base class func\n");
		return IS_SAME;
	}

*/

 virtual
	MFN_do_cmp get_do_cmp() = 0 ;

	// get_do_cmp() is virtual, but do_cmp isnt ;-)
};

#endif
