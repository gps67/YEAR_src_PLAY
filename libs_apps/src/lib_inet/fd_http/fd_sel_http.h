#ifndef fd_sel_http_H
#define fd_sel_http_H

#include "fd_sel_line.h"

/*!
	A prototype class for use in HTTP clients and servers
	(which have similar protocol layouts).

	It distinguishes between headers-line-mode, and data-file-either-mode,
	it detects the 'blank-line' and in future it might do chunking.
*/
class fd_sel_http : public fd_sel_line
{
	bool line1_seen;
	bool in_header; // DATA might still be line oriented
 public:
	virtual ~fd_sel_http();

	fd_sel_http( int _fd = -1 );

	/*
		NOTE: this code is allowed to poke NUL values
		into buffer, its not const char *.

		ALSO: line is guaranteed to end with NUL (not CRLF),
		even data!
	*/

// VIRTUAL - from fd_sel_line
	void got_line( str0 line, int len );
	void got_data( void * buff, int len );
	void got_eof();
	void got_error();

// exported virtual 
	virtual void got_head_line1( str0 buff, int len );
	virtual void got_head_line( str0 buff, int len );
	virtual void got_head_emptyline();
//
	virtual void got_data_line( str0 buff, int len );
	virtual void got_data_blk( void * buff, int len );
//
	virtual void http_resp( str0 resp_ver, int resp_code, str0 resp_msg );
	virtual void http_attr( str0 attr, int l1, str0 val, int l2 );
	// following happen INSTEAD of above
	virtual void http_attr_Date( time_t t );
	virtual void http_attr_Date( str0 val, int l2 );
	virtual void http_attr_Server( str0 val, int l2 );
	virtual void http_attr_X_Powered_By( str0 val, int l2 );
	virtual void http_attr_Set_Cookie( str0 val, int l2 );
	virtual void http_attr_Connection( str0 val, int l2 );
	virtual void http_attr_Content_Type( str0 val, int l2 );

} ;

#endif

