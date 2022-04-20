#ifndef dev_fd_H
#define dev_fd_H

#include "fd_hold.h"
#include "b512_data.h"

struct restart_data_t	// written to restart_file
{
	u64 seek_eof;	// different device could have same capacity

	u64 seek_wr;
	u64 seek_rd;
	/*
	// tree writes half, quarter, 1/8, 1/16, ...
	u64 tree_wr;
	u64 tree_rd;
	*/

	void zero() {
		seek_eof = 0;
		seek_wr = 0;
		seek_rd = 0;
	}
};

class dev_fd_t 
{
 public:

	str1 dev_name;
	fd_hold_1 fd;
	u64 file_size_in_bytes;
	u32 block_size_in_bytes; // set to 0 or set to u32 not u64

	// move to def_fd_curr_page // sector //
	b512_data_t sect_in;	
	b512_data_t sect_out;

	// progress 
	fd_hold_1 fd_restart_file;
	restart_data_t restart_data;


	dev_fd_t()
	: fd()
	, file_size_in_bytes(0)
	, block_size_in_bytes(0)
	{
		fd.ref_static(); // else stack smashing crash
		fd_restart_file.ref_static(); // else stack smashing crash
	}

	bool flush_buffer_cache();

	bool open_abb( const char * abb_dev_name ); // "sdb"
	bool open_device( const char * _dev_name ); // "/dev/sdb"
	bool open_restart_file( const char * _restart_file_name ); // ./.fill_2T_sdb.restart
	bool show();

};

#endif
