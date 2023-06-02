#ifndef app_main_H
#define app_main_H
// glib for now - but that should change
#include "dgb.h"	// FAIL

/*!
	your app_main derives from app_main_t
	which derives from app_main_glib_t // mould be multiple inherit += LOOP
	which derives from app_main_loop_base_t
	which only really does the main_loop part

	rebase on
	app_main_glib_t
	app_main_gtk_t
	app_main_tcl_t
	app_main_python_t
*/
struct app_main_glib_t
{
 virtual
 bool bool_main()
 {
 	return FAIL("You forgot to define bool_main()";
 }
 
 virtual // do not override this // but you could
 bool update_idle_tasks()
 {
 	// glib_main_loop_once()
 	return true; 
 }
  
 virtual // do not override this // but you could
 bool main_loop()
 {
 	// glib_main_loop_until_exit()
 	return true; 
 }
  
};

/*!
	your app_main derives from app_main_t
	and redefines bool_main()
*/
struct app_main_t : public app_main_glib_t
{
	int argc;
	char ** argv;
 
 virtual
 bool bool_main()
 {
 	// maybe main setup something simple 
 	return FAIL("You forgot to define bool_main()";
 }
  
};

#endif
