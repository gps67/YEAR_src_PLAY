
#include "dgb_fail.h"
#include "XML_Parse.h"
#include "XML_DOC.h"
namespace XML_Parse {

 /*!
	create a new sp, based on elem_spec, push it

	sp(_old) is already on the stack

	sp(_new) is added to the stack
 */
 void XML_Stack_Base::push(T_elem_spec * elem_spec)
 {
	/*
		normally sp's ref_count is kept by stack
		but whilst creating new ones, use sp_keep
	*/
	obj_hold<XML_Stack_Layer_VTL> sp_keep; // temp keeper

	/*
		now we dont have a new sp, so set it to NULL
	*/
	sp = NULL;

	// try to get a blank sp from the recycle list
	if( stack_recycle.N() ) {
		/*
			demo of obj_hold used to return values

			this new style could be expanded, so that
			sub-functions are given VARIABLES, which
			might have an interpreted type-range.
			obj_hold is typed by C++, not by SPEC
		*/
		sp = stack_recycle.stack_pop( sp_keep );
	}

	// try to get a blank sp from new()
	if(!sp) {
		sp = new XML_Stack_Layer_VTL();
		sp_keep = sp;
		if(!sp) THROW_dgb_fail("new");
	}

	/*
		initialise the stack frame to the specific elem_info
		wherever sp came from (malloc/recycle)
	*/
	sp->re_init( elem_spec );

	// push sp, which keep a ref_count
	stack.stack_push( sp );

	// sp_keep->dtor()
 }

 /*!
	recover sp from the stack

	sp(_curr) is also on the stack (drop it)

	sp(_recover) is copied from the stack
 */
 void XML_Stack_Base::pop()
 {
	// check that stack had push() called ... (not empty)
	int pos = stack.N();
	if(!pos) {
		THROW_dgb_fail("pop() empty stack");
		return;
	}

	obj_hold<XML_Stack_Layer_VTL> sp_keep; // temp keeper

	/*
		the current sp is also on the stack, remove it, delete it
	*/
	sp_keep = sp;
	stack.stack_pop();
	// delete sp, in a recycle sort of way
	if( sp ) {
		sp->recycle();
		stack_recycle.stack_push( sp );
		sp = NULL;
		sp_keep = sp;
	}

	// remove sp from stack
	sp = stack.stack_top();
 }

}; // XML_Parse

