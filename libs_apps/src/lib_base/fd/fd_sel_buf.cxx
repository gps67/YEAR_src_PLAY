#include "fd_sel_buf.h"


bool fd_sel_buf::debug_show_stats()
{
	INFO( "TODO: ask in and out");
	dgb_show_in();
	dgb_show_out();
	return true;
}

void fd_sel_buf::dgb_show_in()
{
	in_buff.dgb_show( debug_get_obj_name(), "in_buff" );
}

void fd_sel_buf::dgb_show_out()
{
	out_buff.dgb_show( debug_get_obj_name(), "out_buff" );
}

