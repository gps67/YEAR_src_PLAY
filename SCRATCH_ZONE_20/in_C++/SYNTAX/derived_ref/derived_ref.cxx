#include "stdio.h"

// this is fine, so why is X_Pixamp complaining derived from X_Display_Surface
// OK turns out ... use of X_STUBS.h ... no relation specified
// was fixed by including X_Pixmap.h which clarified base class

struct base_t {
	const char * name;

	base_t()
	{
	}
};

struct derived_t : public base_t {

	derived_t()
	{
	}
};

bool a_func( base_t & base )
{
	printf("a_func");
	return true;
}

int main()
{
	derived_t derived;
	a_func( derived );
	return 0;
}
