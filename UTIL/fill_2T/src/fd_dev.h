#ifndef fd_dev_H
#define fd_dev_H

#include "fd_hold.h"
#include "b512_data.h"

#include "fd_restart_file.h"

class fd_dev_t 
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
	fd_restart_file_t fd_restart_file; // fd and data

	fd_dev_t()
	: fd()
	, file_size_in_bytes(0)
	, block_size_in_bytes(0)
	{
		fd.ref_static(); // else stack smashing crash
	}

	bool flush_buffer_cache();

	bool open_abb( const char * abb_dev_name ); // "sdb"
	bool open_device( const char * _dev_name ); // "/dev/sdb"
	bool open_restart_file( const char * _restart_file_name ); // ./.fill_2T_sdb.restart
	bool read_next_sector();
	bool write_next_sector();
	bool show();

	bool WRITE_SWEEP_RESUME();
	bool READ_SWEEP_RESUME();



};

#endif
