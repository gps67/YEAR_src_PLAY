#include "TOPAPP_Task.h"
#include "TOPAPP_Holder.h"
#include "dgb_fail.h"
using namespace APPS;

// ==========================================================
/*
	TODO: App_Task = (TODO) rename TOPAPP_Task
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
TOPAPP_Task::TOPAPP_Task( TOPAPP_Holder * _m_a_h )
{
	/*
		The MAIN holder MUST exist before the app
	*/
	m_a_h = _m_a_h;
//	if(!m_a_h) gdb_invoke(true);
	// allow Minor window to pass NULL 
	if( m_a_h) {
		// try for the first being the MAIN Task
		/*
			ERRM ... this is not registering the MAIN app
			MAYBE that has to be done individually
		*/
	//	m_a_h ->app_register( this );
	//	m_a_h -> task_attach( this ); // wait for this ... (need del)
		WARN("LURK NEED task_detach to be called somewhere ...");
	}
	if(!m_a_h) WARN("NULL m_a_h");
}

/*!
	Destructor - 
*/
TOPAPP_Task::~TOPAPP_Task()
{
	e_print("~TOPAPP_Task()\n");
	WARN("DTOR on an object with not a lot of ref_counting");
	if(m_a_h)
	{
		// PROBLEM - when tracking window list - check refcount == 1
//		m_a_h -> app_destroyed();
		m_a_h = NULL;
	}
}

bool TOPAPP_Task::check_m_a_h()
{
	if(m_a_h) return true;
	FAIL("NULL main_app_holder");
	return false;
}


