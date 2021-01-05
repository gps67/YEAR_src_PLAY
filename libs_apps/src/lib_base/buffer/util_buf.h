#ifndef util_buf_H
#define util_buf_H

// functions that are not in classes
// maybe should have been in the original classes

#include "ints.h" // i64
#include "blk1.h" // blk1 is base class of buffer3

	// extern means provided here // near // .h .cxx .o .lib .so .exec
	// extern means export // in C add function to LIST of callables
	// extern means export // in C add function as ITEM of callable
	// SUBLEX[s] // SUGGEST MACRO Name[x] [P0[P2 foreach idx { ... }

	extern
	bool get_file_size( const char * filename, i64 & size );
	extern
	bool get_file_size_32(
		const char * filename,
		u32 & size
	);
	extern
	bool blk_read_entire_file(
		blk1 & mem_in,		// append not set
		const char * filename,
		u32 max_K_size
	);
	extern
	bool blk_write_to_file(
		blk1 & mem_out,			// DATA buffer
		const char * filename		// FILE filename
	);
	extern
	bool blk_write_to_file_mask(
		blk1 & mem_out,			// DATA_buffer
		const char * filename,		// dir/file.ext
		mode_t mask			// 644 mode
	);
	extern
	bool blk_write_to_file_mask_uid_gid(
		blk1 & mem_out,
		const char * filename,
		mode_t mask,			// 644_RWX_User_group_Other
		int uid,			// user id of process
		int gid				// group id of process
						// those values on NEW file
	);

#endif
