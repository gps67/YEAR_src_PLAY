/// util_file_2
#include "util_buf.h" // 
#include "file_stat.h" // 
#include "dgb_fail.h" // 
#include "fd_hold.h" // 

#ifndef WIN32
#include <unistd.h>
#endif

typedef fd_hold_1 fd_hold;


/*
	libs_apps version of stat is inside out
	want to follow and not follow links - at the same time!
	eg add an lstat function
	eg add lstat_data
	for now reimplement here, knowing old badness
	(cost of doing business this way)
*/
bool get_file_size( const char * filename, i64 & size )
{
	size = 0;
	file_stat f_stat;
	if(!f_stat.stat( filename )) {
		// option of being silent or not
		return WARN("'%s'", filename );
		return false;
	}
	if( f_stat.file_type == is_link ) {
		// LIBR gives us file_stat .st .linked_st // poss unreset
		if( f_stat.linked_file_type != is_file ) {
			return FAIL("not a file %s", filename );
		}
		size = f_stat.linked_st.st_size;
	} else {
		size = f_stat.st.st_size;
	}
	return true;
}


bool get_file_size_32( const char * filename, u32 & size )
{
	size = 0; // fail will leave this

	i64 file_size_64;
	if( !get_file_size( filename, file_size_64 )) return false;

	const u32 max_uns_32 = (u32) -1; // 0xFFFF ## FFFF
	const u32 max_uns_31 = max_uns_32 >> 1;

	// enforce 2G-1 limit
  if(1)	// limiting to 2G is probably sensible (if not 64 bit)
	if( file_size_64 > max_uns_31 ) {
		size = max_uns_31;	// 32 or maybe 31 ?
		return FAIL("filesize exceeds 31 bits 2GB ");
	}

	// enforce 4G-1 limit
	// stretching to 4GB is probably a bad idea (when not 64 bit)
	if( file_size_64 > max_uns_32 ) {
		size = max_uns_32;	// 32 or maybe 31 ?
		return FAIL("filesize exceeds 32 bits 4GB ");
	}

	size = file_size_64;
	if(0) INFO("'%s' = %d", filename, size );
	return true;
}

////////////////////// file_buf



bool blk_read_entire_file(
	blk1 & mem_in,			// append to this memory buffer
	const char * filename,
	u32 max_K_size
)
{
	if( max_K_size == 0 ) max_K_size = 200; // 200K or 1<<21; or infinite

	// get file size
	// what if file does not exist, or exceeds 4G
	u32 file_size_32 = 0;
	if( !get_file_size_32( filename, file_size_32 )) return false;

	u32 file_size_K = file_size_32 >> 10 ; // 1024 = 1K // rounding down

	// keyfile should not reach 200K
	if( file_size_K > max_K_size ) { 
		return FAIL("file %s too big %d", filename, file_size_32 );
	}

	if(!mem_in.get_space( file_size_32 )) return false;

	obj_hold<fd_hold> f = new fd_hold();
	bool wait_for_sync = true; // WAIT_FOR_SYNC not notted for async !
	bool wait_async = !wait_for_sync ; // WAIT_FOR_SYNC not async !
#if 0
	str0 filename_str0;
	filename_str0 = filename;
	// HERE auto cast from STR0 to str0 
	// problem is INFO/print using str0 not casting to STR0
#endif
	if(!f->open_RO( filename, wait_async )) return false;
	// default sync, so will wait

	// one huge slurp - but key file should not block much
	i32 file_size_31 = file_size_32;
	int t = f->read( mem_in.gap_addr(),file_size_31 );
	if(file_size_31 != t ) {
		INFO("expected %d got %d", file_size_31 , t );
		return FAIL(filename);
		return false;
	}
	f->close();
	mem_in.nbytes_used += file_size_31;
	mem_in.trailing_nul();
	return true;
}

bool blk_write_to_file( blk1 & mem_out, const char * filename )
{
	return blk_write_to_file_mask( mem_out, filename, 0664 );
	// group can edit
}

bool blk_write_to_file_mask( blk1 & mem_out, const char * filename, mode_t mask )
{
	obj_hold<fd_hold> f = new fd_hold();
	// _CREATE is OK if it already exists, its not _CREATE_EXCL
	bool async = false;
	if(!f->open_RW_CREATE_modmask( filename, async, mask )) return false;
	i32 file_size_32 = (int) mem_out.nbytes_used;

	if( file_size_32 != f->write(
		mem_out.buff,
		mem_out.nbytes_used
	)) {
		return FAIL(filename);
	}
	f->fsync();
	f->close();
	return true;
}

bool blk_write_to_file_mask_uid_gid(
	blk1 & mem_out,
	const char * filename,
	mode_t mask,
	int uid,
	int gid
)
{
	bool ok = blk_write_to_file_mask( mem_out, filename, mask );
#ifdef WIN32
	FAIL("WIN32 ownership of file .. ... ....");
#else
	if(chown(filename,uid,gid)) {
		ok = FAIL("chown(%s,%d,%d)", filename, uid, gid );
	}
#endif
	return ok;
}

///////////////////////////////


