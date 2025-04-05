#ifndef TOPAPP_Holder_gtk_H
#define TOPAPP_Holder_gtk_H
#include "TOPAPP_Holder_glib.h"
namespace APPS {

class TOPAPP_Holder_gtk : public TOPAPP_Holder_glib
{
 public:
	TOPAPP_Holder_gtk( int * p_argc, char *** p_argv );

	bool call_gtk_or_glib_init(int * p_argc, char *** p_argv);
	bool call_glib_init(int * p_argc, char *** p_argv);
	bool call_gtk_init(int * p_argc, char *** p_argv);
	virtual bool Call_gtk_main();

	// TOP_APP = win // DESKTOP holds WIN_TOP _t // support code
	// typedef w_win_top_t w_win_top
	// typedef w_win_t w_win_top_t // DIALECT drop top //

	bool window_attach( w_win_top * win );
	bool window_detach( w_win_top * win );

	// often single TOP DESKTOP PANEL // exit on close 

	bool object_attach( obj_ref0 * obj );
	bool object_detach( obj_ref0 * obj );

};

}; // NS
#endif
