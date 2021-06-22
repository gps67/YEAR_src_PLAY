#ifndef G_rrr_H
#define G_rrr_H

#ifdef do_it_slow_but_sure
#include <glib.h>
#else
typedef struct _GError GError;
typedef struct _GtkWidget GtkWidget;
#endif

/*!
	G error return variable ((TODO refactor against err_int))
*/
class G_rrr
{
	GtkWidget *a_window;
	GError * rrr; // used as a parameter in a all funcs

	// TODO: // str_const noun1; // recent filename or objectname
	// TODO: // str_const verb1; // recent filename or objectname
	// TODO: // str_const noun2; // recent filename or objectname
	// TODO: // str_const verb2; // recent filename or objectname
	// ALSO: keep a small list of fails, turn into FAIL

	// This class is all about making life simpler
	// with this global variable, but it does have to be initialised
	// so that the error dialog has a SCREEN to go to
	//
	static G_rrr * global_G_rrr;

 public:
	static G_rrr * get_global_G_rrr();

	static GError ** get_global_GError();

	G_rrr( GtkWidget *_window )
	: rrr( 0 )
	{
		// needed for DISPLAY
		set_window( _window );
	}

	bool wants_a_window() {
		return a_window == 0;
	}

	void set_window( GtkWidget *_window );

	GError ** grrr() {
		return & rrr;
	}

	bool only_print_gerror();
	bool dialog_box_error( const char * fmt_s );
	bool is_key_not_found();
	void clear_gerror();
	void clear_gerror_if_any();

	const char * get_message();


};

#endif
