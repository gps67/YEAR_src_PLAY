#include "btn_col.h"

btn_col::btn_col()
: w_win_scrolled( GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC )
, keep_list()
{
	col = new w_vbox();
	pack_unscrollable( col );
	// act_like_radio_group = TRUE;
}

btn_col::~btn_col()
{
	col = NULL;
}

void btn_col::append( w_widget0 * btn )
{
	// REMOVE VAR TOO // keep_list.append( btn );
	col->pack_asis( btn );
}

