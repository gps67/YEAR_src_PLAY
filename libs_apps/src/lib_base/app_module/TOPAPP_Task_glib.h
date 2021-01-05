#ifndef TOPAPP_Task_glib_H
#define TOPAPP_Task_glib_H

#include "TOPAPP_Task.h"

class w_win_top;

// #include "w_widget0.h" // ie gtk
#include "obj_ref.h" //
#include <glib.h>

#include "APPS_stubs.h"
namespace APPS {

//! derive a class from this
/*!
	Many applications are a single main Task.
	Most applications have one window per document per Task
	GUI Tasks will tend to keep things like "current_file_name"
	NET Tasks will tend to have different requirements

	This base class _Task is for lists to have a basic type.
	In particular the TOPAPP_Task_List_Holder will exit
	when all Tasks have exited (or put up a fileselector box).

	The silly name is to leave space for task/threads, others are:

		APP_Task
		APP_Document
		SESS_Task

	Plus room is needed for when one document has a list of sub-tasks,
	possiblty each an FSM with bits in shared queued (different task).

	With a GUI the TASK is the ACTIVITY
	With a BATCH the TASK is the SCRIPT_THREAD
	With a NET the TASK is again a SESSION like a GUI
	With a WWW the TASK includes a single fetch or CGI action or ...

*/
class TOPAPP_Task_glib : public TOPAPP_Task
{
 public:
	/*!
		a GTK_Main_app is the main app,
		it can have several windows, or one.
	*/
	TOPAPP_Task_glib( TOPAPP_Holder * _m_a_h );
	~TOPAPP_Task_glib();
	void window_attach( w_win_top * win );
	void window_detach( w_win_top * win );
};

}; // NS
#endif
