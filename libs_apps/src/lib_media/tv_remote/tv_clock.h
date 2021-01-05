#ifndef tv_clock_H
#define tv_clock_H

#include "w_vbox.h"
#include "w_label.h"
#include "tm_parts.h"
#include <time.h>

namespace TV_REM {

class tv_clock;
class tv_panel;

/*!
	The tv_clock gets a callback every second.

	It displays (and updates) three labels,

		2002-10-06
		23:59:58
		Sunday 6 October

	Remember to cal set_locale

*/
class tv_clock : public w_vbox
{
	tm_parts T;
	guint call_back_id;
	obj_hold<w_label>	l1;
	obj_hold<w_label>	l2;
	obj_hold<w_label>	l3;
 public:
	obj_hold<w_label>	l4;
	obj_hold<tv_panel>	Panel;
	void get_time_now();
	tv_clock( tv_panel * panel ); // it can use the ticks too
	~tv_clock();
	void set_extra_line( str0 msg );
};

};

#endif
