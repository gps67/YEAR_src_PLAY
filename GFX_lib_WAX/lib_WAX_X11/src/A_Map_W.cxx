
#include <stdio.h>

#include "X_STUBS.h"
#include "A_Map_W.h"
#include "X_Window.h"
// #include "e_print.h"

using namespace WAX;

A_Map_W::A_Map_W()
{
	n = 0;
}

X_Window * A_Map_W::find( Window w )
{
	for(int i=0; i<n; i++ )
	{
		if( !list[i] ) continue;
		if( list[i]->drawable == w ) {
			return list[i];
		}
	}
#ifndef FIX_to_report_root_window_grabbed_key
	FAIL("# ERROR 22  A_Map_W::find(%ld) not found \n", w);
	return list[0];
#endif
	return NULL;
}

bool A_Map_W::add( X_Window * W )
{
	if( n >= list_n_max ) {
		throw "A_Map_W::add(W) -- list_n_max";
		// e_print("ERRORsa");
		return FAIL("n > list_n_max");;
	}
	list[ n++ ] = W;
	return true;
}

bool A_Map_W::del( X_Window * W )
{
	Drawable w_id = W->drawable;
	for(int i=0; i<n; i++ )
	{
		if( !list[i] ) continue;
		if( list[i]->drawable == w_id ) {
			list[i] = list[n];
			list[0] = NULL;
			n--;
			return true;
		}
	}
	return FAIL("# ERROR 22  A_Map_W::find(%ld) not found \n", w_id);
}

