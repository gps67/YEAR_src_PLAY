#include "X_Drawable_Surface.h"
#include "X_Display.h"

using namespace WAX;

#if 0
	/*!
		create a GC for this drawable
	*/
	GC
	X_Drawable_Surface::
	CreateGC()
	{
		if(!drawable) {
			WARN("ZERO drawable");
			DEBUG_print_stack();
			return 0;
		}
		unsigned long valuemask = 0;
		XGCValues * values = NULL;
		return ::XCreateGC( slow_get_display(), drawable, valuemask, values );
	}
#endif

	Display *
	X_Drawable_Surface::
	slow_get_display() // could declare as inline then include this code
	{
		// CHECK NULL // UNSET // PROMISE // on_demand_question_attr_sto
		// RETVAL_is_EXPR "{ T * GET_fn() { SCRIPT } }"
		if(!disp) {
			FAIL("NULL disp"); // continue with NULL reported
			// possible LATE_LAZY_EVAL on_GOT VARNAME GOT API
			// continue with NULL reported
			// ansent OBJ which might probably return NULL 
			// OBJ_t * PTR = NULL //
			return NULL;
		}
		return disp->display;
	}
