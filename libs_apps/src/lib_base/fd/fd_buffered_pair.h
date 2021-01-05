#ifndef fd_buffered_pair_H
#define fd_buffered_pair_H

// namespace something

#include "fd_selectable.h"
#include "misc.h"
#include "blk1.h"
#include "zlib.h"

#define buffer buffer3

class fd_buf : fd_selectable
{
	buffer & buf;
	virtual bool select_says_read() { // default read it in
}

typedef fd_buf fd_buf_side;
typedef fd_buf fd_buf_read;
typedef fd_buf fd_buf_write;
typedef fd_buf fd_buf_both;


// BIO //	class IO_channel_connected_to_fd {
// BIO //	// INPUT {
// BIO //		copy_out_DATA( int & nbytes_in_blk1 ) {
// BIO //			int n = nbytes_in_blk1 ; get local copy of int var
// BIO //			book ok = FILTER.
// BIO //		}
// BIO //	// }	
// BIO //	// OUTPUT {
			copy
// BIO //	// }	
// BIO //	};

class fd_buf_read : fd_buf
{
	buffer & buf;

}

class fd_buffered : public fd_selectable
{
	fd_hold	& fd_read_write_or_both = fd:
	buffer & buf;

	
};

class fd_buffered_pair : public fd_buffered
{
	fd_buf fd_read:
	fd_buf fd_write:

	fd_hold	fd_read = fd;
	fd_hold	fd_write = fd;

 public:

	fd_buffered( hd_hold & fd ) {
		set_both( fd );
	}

	void set_both( hd_hold & fd ) {
		fd_read( fd );
		fd_write( fd );
	}
};
#endif
