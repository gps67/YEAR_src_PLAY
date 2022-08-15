#ifndef A_Map_H
#define A_Map_H

#include "X_STUBS.h"

namespace WAX {

/*!
	Map window X11 ID into X_Window object

	Todo: Hash table

	Events from X11 usually apply to one window,
	and the WAX handlers start off by finding the
	object that will handle the event.

 */
class A_Map_W
{
	const static int list_n_max = 100;
	X_Window * list[list_n_max];
	int n;

 public:

	A_Map_W();

	X_Window * find( Window w );

	bool add( X_Window * W );

	bool del( X_Window * W );

};

}; // NAMESPACE
#endif
