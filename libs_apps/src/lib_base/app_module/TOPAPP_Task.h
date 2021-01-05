#ifndef TOPAPP_Task_H
#define TOPAPP_Task_H

#include "obj_ref_js.h"
#include "obj_list.h"
#include "APPS_stubs.h"
namespace APPS {


/*!
	All tasks will have a TOP object (later not GLIB nor GTK)
*/
class TOPAPP_Task : public obj_ref_js
{
	TOPAPP_Holder * m_a_h; // weak up-link
 public:
	friend class TOPAPP_Holder;

	~TOPAPP_Task();
	TOPAPP_Task( TOPAPP_Holder * _m_a_h );

	bool check_m_a_h();
	TOPAPP_Holder * holder() { return m_a_h; }

	void clear_holder() { m_a_h = NULL; }
};

}; // NS
#endif

