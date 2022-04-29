#ifndef fd_dev_H
#define fd_dev_H

#include "fd_hold.h"
#include "b512_data.h"

#include "fd_restart_file.h"
#include "pair_steps.h"

class fd_dev_t 
{
 public:

	str1 dev_name;
	fd_hold_1 fd; // fd_device
	u64 file_size_in_bytes;
	u32 block_size_in_bytes; // set to 0 or set to u32 not u64

	// move to def_fd_curr_page // sector //
	b512_data_t sect_in;	
	b512_data_t sect_out;

	// progress 
	fd_restart_file_t fd_restart_file; // fd and data

	// filter to plot data
	pair_steps_t pair_steps;

	fd_dev_t();

	bool flush_buffer_cache();

	bool open_abb( const char * abb_dev_name ); // "sdb"
	bool open_device( const char * _dev_name ); // "/dev/sdb"
	bool open_restart_file( const char * _restart_file_name ); // ./.fill_2T_sdb.restart
	bool check_sector_zero_of_device();
	bool read_next_sector();
	bool write_next_sector();
	bool show();

	bool WRITE_SWEEP_RESUME();
	bool READ_SWEEP_RESUME();
	bool SHOW_RESTART();

};

#endif
