
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
		if( list[i]->window == w ) {
			return list[i];
		}
	}
#ifndef FIX_to_report_root_window_grabbed_key
	printf("# ERROR 22  A_Map_W::find(%ld) not found \n", w);
	return list[0];
#endif
	return NULL;
}

void A_Map_W::add( X_Window * W )
{
	if( n >= list_n_max ) {
		throw "A_Map_W::add(W) -- list_n_max";
		// e_print("ERRORsa");
		return;
	}
	list[ n++ ] = W;
}

void A_Map_W::del( X_Window * W )
{
	// laters
}

