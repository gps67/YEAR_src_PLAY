#include "TOPAPP_Task_glib.h"
#include "TOPAPP_Holder.h"
#include "dgb_fail.h"
#include "dgb.h"
using namespace APPS;

// ==========================================================
/*
	TODO: App_Task = (TODO) rename TOPAPP_Task_glib
	TODO: App_Task_List_Holder = (TODO) rename TOPAPP_Task_holder
	TODO: non GUI app

	Holder = Application (metaphor desktop)
	App_Task = Window-Document-Connection

	Note that one application can have several document windows.
	So rename (mentally) Main_App as App_Task

	One of the App's tasks maybe or is the main window,
	and never exit, or if it does, the entire App exits.

	When a window is equated with an App_Task,
	

*/
// ==========================================================


/*!
	a GTK_Main_app is the main app, held by the m_a_h HOLDER
	it can have several windows, or one.

	When the last TOP win is closed, the app exits.

 TODO:
	SUB-TOP windows can exist and be hidded, eg the errors log
	is a top-level-window but doesnt hold the app open

 TODO:
	should the m_a_h pointer be  weak link ?
*/
TOPAPP_Task_glib::TOPAPP_Task_glib( TOPAPP_Holder * _m_a_h )
: TOPAPP_Task( _m_a_h )
{
	/*
		The MAIN holder MUST exist before the app
	*/
	if(!_m_a_h) WARN("NULL m_a_h");
}

/*!
	Destructor - 
*/
TOPAPP_Task_glib::~TOPAPP_Task_glib()
{
	STEP("DTOR=TOPAPP_Task_glib()");
}

/*!
	win has been created, add it to the list
*/
void TOPAPP_Task_glib::window_attach( w_win_top * win )
{
	if(!check_m_a_h()) {
		FAIL("NULL main_app_holder");
		return;
	}
	holder() -> window_attach( win );
}

/*!
	win is being deleted, remove it from the list.
	If thats the last, shutdown the app. (m_a_h does that).
*/
void TOPAPP_Task_glib::window_detach( w_win_top * win )
{
	if(!check_m_a_h()) {
		FAIL("NULL main_app_holder");
		return;
	}
	holder() -> window_detach( win );
}


