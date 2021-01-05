#ifndef fd_sel_ftp_ctrl_H
#define fd_sel_ftp_ctrl_H

#include "fd_sel_line.h"
#include "buffer2.h"
#include "obj_hold.h"

class fd_sel_ftp_data_listener;

/*!
	A prototype class for use in FTP clients and servers
	(which have similar protocol layouts).

	Other protocols are SMTP CDDB which
*/
class fd_sel_ftp_ctrl : public fd_sel_line
{
 public:
	typedef enum {
		W_INIT,
		W_USER,
		W_PASS,
		W_PROMPT,
		W_MID_RESPONSE,
		W_XXX
	} W_state;

	W_state state;

	buffer2 text;
	obj_hold<fd_sel_ftp_data_listener> data_listener;

	virtual ~fd_sel_ftp_ctrl();

	fd_sel_ftp_ctrl( int _fd = -1 );

	/*
		NOTE: this code is allowed to poke NUL values
		into got_line()'s buffer, its not CONST char *.

		ALSO: line is guaranteed to end with NUL (not CRLF),
		even data!
	*/

	void select_says_connected();

// VIRTUAL - from fd_sel_line
	void got_line( str0 line, int len );
	void got_data( void * buff, int len );
	void got_eof();
	void got_error();

	void test1();

// exported virtual 

} ;

#endif

