
#include "g_rrr.h"
#include <glib.h>
#include <gtk/gtk.h>
#include "dgb.h"
#include <errno.h>

#ifdef WIN32
#include "MY_WIN32.h"
#endif

/*
	This function is ONLY available when the GTK DLL is loaded and init'd

	The glib DLL is used without GTK in base1, with in base2

	This only works because the linker is forgiving about the unused absent
	(it is more visible on WIN32 than on LGX because of lazy loading)

	If it breaks, look for a call to this function,
	then split the class into G_rrr_base, or do like FAIL() does
	to set a pointer and use that, if it is set, by a caller who has
	GTK linked and initialised.

	Better still, this all needs some rework,
	because we are using a global _ANYWAY_ 
	and it should be reduced down to a thread-aware,
	with a better API.
*/

bool G_rrr::dialog_box_error( const char * fmt_s )
{
	g_print( "# ERR # fmt_s = '%s' \r\n", fmt_s );
	only_print_gerror();
	if(!a_window) return FALSE;
	const char * msg = "rrr_error is NULL! non-glib error?";
	if( rrr ) {
		msg = rrr->message;
	} else {
		g_print( "# ERR # errno = %d\r\n", errno );
		g_print( "# ERR # fmt_s = '%s' \r\n", fmt_s );
		// msg = ...
	}
	g_print( "# ERR # %s\r\n", msg );
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new (
		GTK_WINDOW (a_window),
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_CLOSE,
		fmt_s,
		 // "Failed to load an image: %s",
		msg
	);
	clear_gerror_if_any();
//
	if(0)
		g_signal_connect (
			dialog,
			"response",
			G_CALLBACK (gtk_widget_destroy),
			NULL
		);
	
	gtk_widget_show (dialog);
	return FALSE;
}
