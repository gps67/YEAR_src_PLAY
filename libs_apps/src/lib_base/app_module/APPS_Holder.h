#ifndef TOPAPP_Holder_H
#define TOPAPP_Holder_H

#include "obj_ref_js.h"
#include "obj_list.h"
#include "APPS_stubs.h"
namespace APPS {

class TOPAPP_Holder
{
	void init0();
 public:
	obj_hold<TOPAPP_Task> main_app;
	obj_list<obj_ref0> OBJECT_keep_list;
	bool close_called;
	bool glib_not_gtk;

	// these bools are ignored, mostly to allow return FAIL()
	virtual bool init_gdb_sigaction(const char * progname );
	virtual bool init_main_loop();
	virtual bool exit_main_loop();
};


}; // NS
#endif
