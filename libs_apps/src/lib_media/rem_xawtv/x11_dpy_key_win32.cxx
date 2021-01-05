#include "x11_dpy.h"
#include "dgb_fail.h"

bool x11_dpy::lookup_modifier_mask_by_name( const char * _mod, int & var ){
	return false;
}
bool x11_dpy::Grab_Key_ROOT (
	const char * str_key, 
	const char * str_mod
) {
	return false;
}
bool x11_dpy::Grab_Key_ROOT_ITERS (
	const char * str_key, 
	const char * str_mod
) {
	return false;
}
bool x11_dpy::Grab_Key_ROOT (
	int keycode, 
	int modifiers
) {
	return false;
}
bool x11_dpy::Grab_Key (
	Window grab_window,
	int keycode, 
	int modifiers
) {
	return false;
}

// V-SIMILAR

fd_x11::~fd_x11()
{
	e_print("# DTOR # fd_x11::~fd_x11()\n" );
}


fd_x11::fd_x11()
{
	// simple default CTOR in need of late_init( dpy )
}

fd_x11::fd_x11( x11_dpy * _dpy )
{
	late_init( _dpy );
}
void fd_x11::late_init( x11_dpy * _dpy )
{
}

bool fd_x11::select_says_read()
{
	return false;
}

bool x11_dpy::select_says_read()
{
	return false;
}

void x11_dpy::flush()
{
}

bool  x11_dpy::lookup_Key_by_name(
	const char * keyname, 
	int & keysym,
	int & keycode 
){
	return false;
}

void x11_dpy::test1()
{
}
