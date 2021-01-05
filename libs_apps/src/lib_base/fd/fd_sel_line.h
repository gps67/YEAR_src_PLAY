#ifndef fd_sel_line_H
#define fd_sel_line_H

#include "fd_selectable.h"
#include "buffer1.h"
// #include <stdio.h> // perror

/*!
	fd_sel_line is an event driven io class which is usually in line mode.
	It can also be switched into data-mode.

	part_line buffers any incomplete lines, and calls got_line(..)
	for each complete line. It calls got_data(...) if you switched
	out of line mode, and you can switch back, ... if you can figure
	how to push data back.

	You can easily use this for other line-based protocols, such
	as FTP-ctrl. The CRLF is removed.

	You also get callbaks for EOF and ERROR.
*/
class fd_sel_line : public fd_selectable_base
{
	int sensible_read_size;
	buffer1 part_line;
	bool line_mode;
 public:

	fd_sel_line( int _fd = -1 );
	virtual ~fd_sel_line();

// IMPLEMENT hooks over base class
// VIRTUAL
	bool select_says_read();

// PROVIDE for subclass

	void set_line_mode();
	void set_data_mode();

// HOOKS to CALL sub-class with data

	virtual void got_line( str0 line, int len ) = 0;
	virtual void got_data( void * data, int len ) = 0;
	virtual void got_eof() = 0;
	virtual void got_error() = 0;

	/*
		TO SWITCH from data to line mode, you need to put
		back any unused data (which will probably already be in part_line)
		//
		void push_back_data( void * data, int len )
		{
		}
	*/

};

#endif

