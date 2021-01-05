#ifndef w_socket_H
#define w_socket_H

#include "w_widget0.h"
#ifdef WIN32
#include "gdk/gdkwin32.h"
#else
#include "gdk/gdkx.h"
#endif

extern "C"
void gtk_socket_steal ( GtkSocket *socket, GdkNativeWindow wid );

/*!
	w_socket is nothing to to with TCP/IP (stupid name really).

	Its a GTK socket, which goes with a PLUG, a swallowed window.

	The idea was to snatch xawtv,  or some other X11 TV
*/
class w_socket : public w_widget0
{
 public:
	/*!
		constructor - 
	*/
	w_socket()
	{
		w = gtk_socket_new();
	}
	void steal( GdkNativeWindow win_id )
	{
		gtk_socket_steal( (GtkSocket *)w, win_id );

	}
};

#endif
