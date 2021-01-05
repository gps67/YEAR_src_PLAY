#ifndef TOPAPP_Holder_H
#define TOPAPP_Holder_H

#include "obj_ref_js.h"
#include "APPS_stubs.h"

namespace APPS {
class TOPAPP_Holder: public GRP_lib_base // obj_ref_js
{
 public:
	virtual ~TOPAPP_Holder();

	//! hold the main_app (baseclass) and its data
	obj_hold<TOPAPP_Task> main_app;

	virtual bool call_gtk_or_glib_init(int * p_argc, char *** p_argv); // or your main loop owner ...
	virtual bool call_gtk_init(int * p_argc, char *** p_argv); // or your main loop owner ...
	virtual bool call_glib_init(int * p_argc, char *** p_argv); // or your main loop owner ...

	virtual bool call_gtk_or_glib_quit(); // or your main loop owner ...
	virtual bool call_gtk_quit(); // or your main loop owner ...
	virtual bool call_glib_quit(); // or your main loop owner ...

	virtual bool window_attach( w_win_top * window );
	virtual bool window_detach( w_win_top * window );

	void tell_main_app_to_forget_link_to_here();

};
}; // NS
#endif

