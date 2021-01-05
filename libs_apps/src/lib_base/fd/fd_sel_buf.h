#ifndef fd_sel_buf_H
#define fd_sel_buf_H

#include "fd_selectable.h"
#include "buffer3.h"

// TODO: uses P0P4_circ instead of buffer3 circ_*

/*
	libs_apps/
		src/lib_win32/fd_win32/fd_ssl_client.h
		src/lib_win32/
		src/lib_base/
		src/lib_base/fd/
		src/lib_base/fd_sel/
class fd_ssl_client : public fd_selectable_base

*/

/*!
	This is a base class for fd_sel_ssl

	This is a base class for fd_ssl_{client,server}

	When they receive a PKTIN, they keep the data in the same buffer
	(still trying to reduce that, but double buffering is tricky).
	So to pass the PKTIN to the application (eg another to write out),
	they pass "this", as the base class fd_sel_buf.

	EG a 'telnet´ over SSL, would recieve screen data from the remote,
	and write it to the TTY fd (itself another buffer). Any activity
	on the keyboard fd would get written to the fd_sel_buf.out_buf.

	The amount of space available is that which is in fd_sel_buf,
	possibly it might have a spillage area, and then it is XOFF.

	'in' and 'out' are from here out via the SSL box out on the wire
	(or in).
*/
class fd_sel_buf : public fd_selectable_base
{
// 	P0P4_circ
public:
	buffer3 out_buff;	// output buffer - circular
	buffer3  in_buff;	//  input buffer - circular

	void dgb_show_in();
	void dgb_show_out();

	fd_sel_buf()
	{
	}

	~fd_sel_buf()
	{
	}

	bool debug_show_stats();

	virtual bool nudge_write_some() {return true;}
	virtual bool nudge_read_some() {return true;}

};



#endif
