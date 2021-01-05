#ifndef btn_col_H
#define btn_col_H

#include "w_vbox.h"
#include "w_win_scrolled.h"
#include "obj_list.h"


/*!
	A vertical list of buttons with a vertical scroll bar
	Needs a bit more work.

	WANT: thinner scroll-bar
	WANT: request own height ... into row of cols
*/
class btn_col : public w_win_scrolled
{
 public:
	obj_hold<w_vbox> col;
	// bool act_like_radio_group;

	obj_list<w_widget0> keep_list;

	btn_col();
	~btn_col();

	void append( w_widget0 * btn );
	void new_sub_width( int wid ); // private
};

#endif
