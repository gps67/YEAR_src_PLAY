#ifndef fd_restart_H
#define fd_restart_H

#include "fd_hold.h"
#include "b512_data.h"
#include "mmap_file.h"

struct restart_data_t	// written to restart_file
{
	u8 abb[8]; // sdb

	u64 seek_eof;	// different device could have same capacity

	u64 seek_wr;
	u64 seek_rd;

	/*
	// tree writes half, quarter, 1/8, 1/16, ...
	u64 tree_wr;
	u64 tree_rd;
	*/

	void zero();
	void init(const char * _abb, u64 dev_size);

	restart_data_t() {
		zero();
	}

	bool show_info();
};

struct fd_restart_file_t : public STO:: mmap_file {
	restart_data_t  * restart_mmap; // -> page0

	// restart_data_t restart_data;

	fd_restart_file_t()
	: STO:: mmap_file()
	{
		// fd.ref_static(); // owner of this must call
	}

	bool open_abb( const char * _abb, u64 dev_size );
	bool open_restart_file(
		const char * _abb,
		const char * _restart_file_name,
		u64 dev_size // bytes
	);
	bool grow_file_write_data(const char * _abb, u64 dev_size);
	bool mmap_file_data();
	bool false_report_mismatch();
	const static int sz_data = sizeof( restart_data_t );

};

#endif
