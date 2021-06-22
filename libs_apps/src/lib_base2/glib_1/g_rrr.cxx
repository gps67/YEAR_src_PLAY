
#include "g_rrr.h"
#include <glib.h>
// #include <gtk/gtk.h>
#include "dgb.h"
#include <errno.h>

#ifdef WIN32
#include "MY_WIN32.h"
#endif

// static
G_rrr * G_rrr::global_G_rrr = NULL;

G_rrr * G_rrr::get_global_G_rrr()
{
#warning lurk TODO spin lock here
	if(!global_G_rrr)
	{
		WARN("NOT REMEMBERING/RETAINING A WINDOW");
		global_G_rrr = new G_rrr(NULL);
		if(!global_G_rrr) {
			FAIL("NULL global_G_rr");
			// THROW ?
		}
	}
	if(global_G_rrr->rrr) {
		// maybe get_global_G_rrr is _ONLY_ called at the right times
		// and could notice unhandled errors
		WARN("uncleared message %s", global_G_rrr->rrr->message);
	}
	return global_G_rrr;
}

GError ** G_rrr::get_global_GError()
{
	G_rrr * G = get_global_G_rrr();
	if(!G) return NULL;
	return & G->rrr;
}

const char * G_rrr::get_message()
{
	if(!rrr) {
		WARN("no GError");
		return "(NO-ERROR-MESSAGE)";
	}
	return rrr->message;
	// LURK caller must still clear message
	// maybe add a guard to get_global_GError()
}

bool G_rrr::only_print_gerror()
{
	if(!rrr) {
		if(errno) {
			FAIL("errno but not GError");
			return false;
		}
#ifdef WIN32
		else if( GetLastError() ) {
			FAIL("WIN32 error but not GError");
			return false;
		}
#endif
		FAIL("neither errno nor GError");
		return false;
	}

	g_print( "# GError->message # %s\r\n", rrr->message );
	clear_gerror();
	return FALSE;
}

void G_rrr::clear_gerror()
{
	if( rrr ) {
		g_error_free (rrr);
		rrr = NULL;
	}
}
void G_rrr::clear_gerror_if_any()
{
	if( rrr ) {
		if( rrr->message == (char *)(-1) ) {
			// -1 set by who ?
		} else {
			INFO("rrr->message = %s", rrr->message );
		}
		clear_gerror();
	}
}

void G_rrr::set_window( GtkWidget *_window )
{
	if( !_window ) return; // pointless // and not usable as clear()
	if( a_window ) return; // repeat call of any kind
	INFO("%p", _window );
	a_window = _window;
	if(!global_G_rrr) {
		INFO("Setting global_G_rrr to this %p", this );
		// this is not obj_ref0 // debug_track_pointer();
		global_G_rrr = this;
		return; // know this has a_window
	}
	// sort of a late init // second init first time without window
	if(global_G_rrr->wants_a_window()) {
		global_G_rrr->set_window(_window );
	}
}

/*!
	each module might want to add a snippet to test for common codes
	here KEY_FILE + KEY_NOT_FOUND is checked for in several places.
*/
bool G_rrr::is_key_not_found() {
	if(!rrr) return false;
	if(!( rrr -> code == G_KEY_FILE_ERROR_KEY_NOT_FOUND )) return false;
	if(!( rrr -> domain == G_FILE_ERROR )) return false;
	return true;
}

/*
# ONLY WHEN DIALOG BOX #
/usr/lib/gio/modules/libgvfsdbus.so: wrong ELF class: ELFCLASS64
Failed to load module: /usr/lib/gio/modules/libgvfsdbus.so
/usr/lib/gio/modules/libgioremote-volume-monitor.so: wrong ELF class: ELFCLASS64
Failed to load module: /usr/lib/gio/modules/libgioremote-volume-monitor.so
/usr/lib/gio/modules/libgiogconf.so: wrong ELF class: ELFCLASS64
Failed to load module: /usr/lib/gio/modules/libgiogconf.so

I think that this is i386 gtk loading AMD64 
It probably wont happen on a pure system,
but it will happen on a LOT of hybrid - dual installation systems


*/
