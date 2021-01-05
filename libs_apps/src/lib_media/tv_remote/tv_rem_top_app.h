
#ifndef tv_rem_top_app_H
#define tv_rem_top_app_H

// #include "obj_hold.h"
#include "TOPAPP_Task_Window_Major.h"
using namespace APPS;

namespace TV_REM {
class tv_panel;


/*!
	a tv_panel application created by tv_rem::main
*/

class tv_rem_top_app : public TOPAPP_Task_Window_Major
{
 public:

	~tv_rem_top_app();
	tv_rem_top_app( TOPAPP_Holder_gtk * holder, const char * first_file );

 /*
	THE DATA OBJECT
 */

	// tv_panel held by vbox.keeper
	obj_hold<tv_panel> tvpanel;

/*
	Standard file open/save hooks

	tv_rem currently is hardcoded
//	bool file_open( str2 * filename );
//	bool file_save( str2 * filename, bool compressed );
*/

};
};
#endif
