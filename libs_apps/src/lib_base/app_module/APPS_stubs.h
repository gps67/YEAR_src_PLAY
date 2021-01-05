#ifndef TOPAPP_stubs_H
#define TOPAPP_stubs_H

/*
	w_win_top is used a lot by APPTOP virtual functions
	when it does not exist, because GTK does not exist
*/
class w_win_top;

namespace APPS {

	class app_top; // is a HOLDER

	class TOPAPP_Task;
	class TOPAPP_Holder;

	class TOPAPP_Task_glib;
	class TOPAPP_Holder_glib;

	class TOPAPP_Task_gtk;
	class TOPAPP_Holder_gtk;

};

#endif
