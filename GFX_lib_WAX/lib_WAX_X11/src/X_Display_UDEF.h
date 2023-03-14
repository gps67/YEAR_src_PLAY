#ifndef X_Display_UDEF_H
#define X_Display_UDEF_H

#include "X_Display.h" // _Root

namespace WAX {

struct X_Display_UDEF : public X_Display
{
	// should throw if the display is not openable
	X_Display_UDEF( const char * display_name )
	: X_Display( display_name )
	{
	}

	/*!
	*/
	void test1();
};

}; // namespace WAX
#endif


