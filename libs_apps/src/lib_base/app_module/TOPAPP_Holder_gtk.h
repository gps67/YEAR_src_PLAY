#ifndef TOPAPP_Holder_gtk_H
#define TOPAPP_Holder_gtk_H
#include "TOPAPP_Holder_glib.h"
namespace APPS {

class TOPAPP_Holder_gtk : public TOPAPP_Holder_glib
{
 public:
	TOPAPP_Holder_gtk( int * p_argc, char *** p_argv );
	bool call_gtk_or_glib_init(int * p_argc, char *** p_argv);
	bool call_gtk_init(int * p_argc, char *** p_argv);
	virtual bool Call_gtk_main();

	bool window_attach( w_win_top * win );
	bool window_detach( w_win_top * win );
};

}; // NS
#endif
