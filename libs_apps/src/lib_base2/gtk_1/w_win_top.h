#ifndef w_win_top_H
#define w_win_top_H

#include "w_window.h"
#include "APPS_stubs.h"
using namespace APPS;

/*!
	a TOPLEVEL window, with an associated APP (possibly an APP subtask).

	see GTK_MAIN_App_Window_Minor, which uses this as the outer frame,
	but also has inner areas, file_sel, 

	The app 
*/
class w_win_top : public w_window
{
	char marker[4];
	TOPAPP_Task_gtk * app;
 public:

	w_win_top( TOPAPP_Task_gtk * _app = NULL );
	~w_win_top();
	void event_destroyed(); // virtual call-back
	void connect_delete_event();
	bool event_delete_event(); // return true to eat the event without exit
};
#endif
