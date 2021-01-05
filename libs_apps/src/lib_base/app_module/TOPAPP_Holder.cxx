#include "TOPAPP_Holder.h"
#include "TOPAPP_Task.h"
#include "dgb_fail.h"
using namespace APPS;

void TOPAPP_Holder::tell_main_app_to_forget_link_to_here()
{
	if(main_app) main_app->clear_holder();
}

TOPAPP_Holder:: 
~TOPAPP_Holder()
{
}


bool TOPAPP_Holder:: window_attach( w_win_top * window )
{
	return FALSE_dgb_fail("non gtk base class");
}

bool TOPAPP_Holder:: window_detach( w_win_top * window )
{
	return FALSE_dgb_fail("non gtk base class");
}

bool TOPAPP_Holder:: call_gtk_or_glib_init(int * p_argc, char *** p_argv)
{
	return FALSE_dgb_fail("non gtk base class");
}

bool TOPAPP_Holder:: call_glib_init(int * p_argc, char *** p_argv)
{
	return FALSE_dgb_fail("non glib base class");
}

bool TOPAPP_Holder:: call_gtk_init(int * p_argc, char *** p_argv)
{
	return FALSE_dgb_fail("non gtk base class");
}


bool TOPAPP_Holder:: call_gtk_or_glib_quit()
{
	return FALSE_dgb_fail("non gtk base class");
}

bool TOPAPP_Holder:: call_glib_quit()
{
	return FALSE_dgb_fail("non glib base class");
}

bool TOPAPP_Holder:: call_gtk_quit()
{
	return FALSE_dgb_fail("non gtk base class");
}

