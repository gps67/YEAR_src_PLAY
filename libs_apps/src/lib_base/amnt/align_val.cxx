#include "align_val.h"
#include "dgb.h"
// #include "gtkenums.h"
#ifdef WIN_GTK2
#include "gtk/gtkenums.h"
#else
#define GTK_JUSTIFY_LEFT	0
#define GTK_JUSTIFY_RIGHT	1
#define GTK_JUSTIFY_CENTER	2
#endif

using namespace BASE1;

// GtkJustification
int align_val::as_gtk_justification()
{
#ifndef WIN_GTK2
	FAIL("GTK was not available at compile time");
#endif
	switch( val ) {
	 case align_none:   return GTK_JUSTIFY_LEFT;
	 case align_left:   return GTK_JUSTIFY_LEFT;
	 case align_right:  return GTK_JUSTIFY_RIGHT;
	 case align_center: return GTK_JUSTIFY_CENTER;
	}
	return GTK_JUSTIFY_LEFT;
}

//! convert name to val (and assign)
bool align_val::set_by_name( str0 name )
{
	if( 0==strcmp( name, "left" ) )
	{
		set_align_left();
		return true;
	} else
	if( 0==strcmp( name, "right" ) )
	{
		set_align_right();
		return true;
	} else
	if( 0==strcmp( name, "center" ) )
	{
		set_align_center();
		return true;
	} else
		return false;
}

str0 align_val::get_val_name()
{
	switch( val ) {
	 case align_none:   return "left";
	 case align_left:   return "left";
	 case align_right:  return "right";
	 case align_center: return "center";
	}
	return "left";
}
