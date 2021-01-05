#ifndef TOPAPP_Task_Window_Major_H
#define TOPAPP_Task_Window_Major_H

#include "e_print.h"
#include "APPS_stubs.h"
#include "TOPAPP_Holder_gtk.h"
#include "TOPAPP_Task_Window_Minor.h"

namespace APPS {
/*!
	A Major window is one that TOPAPP_Holder waits to close

	It usually "owns" the application.

	Currently that happens by chance - first is retained
*/
class TOPAPP_Task_Window_Major : public TOPAPP_Task_Window_Minor
{
 public:
        TOPAPP_Task_Window_Major(
                TOPAPP_Holder * _m_a_h,
                const char * w_title,
                int w_width,
                int w_height,
                const char * _filename,
                bool _compress
        )
        : TOPAPP_Task_Window_Minor( _m_a_h, w_title, w_width, w_height, _filename,  _compress )
        {
        }

  virtual
	void pre_fast_exit() {}
};

}; // NS
#endif
