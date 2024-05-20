#include <turbojpeg.h>
#include "TJ_blk.h"
#include "blk1.h"
#include "util_buf.h" // filesize
#include "fd_hold.h" 

using namespace TJ;

TJ_blk_t:: TJ_blk_t() {
	buff = NULL;
	size = 0;
	borrowed = false;
}

TJ_blk_t:: ~TJ_blk_t() {
	release();
}

void TJ_blk_t:: release() {
	if(borrowed) {
		INFO("borrowed");
	} else if( !buff ) {
			INFO("OK already NULL");
	} else {
		tjFree(buff); // NOTE DLL's Free
	}
	buff = NULL;
	size = 0;
	borrowed = false;
}

bool TJ_blk_t:: tj_alloc( long _size ) {
	release();
	buff = tjAlloc( _size );
	if( !buff ) {
		return FAIL("size %ld", _size );
	}
	size = _size;
	return true;
}

void TJ_blk_t:: borrow_from_blk1( blk1 & blk ) {
	release();
	borrowed = true;
	buff = blk.buff;
	size = blk.nbytes_used;
}

bool TJ_blk_t:: move_to_blk( TJ_blk_t & blk ) {
	blk.release(); // pref not be same as this
	blk.buff = buff; buff = NULL;
	blk.size = size; size = 0;
	blk.borrowed = buff; borrowed = false;
	release(); // any other init0
	return true;
}

bool TJ_blk_t:: move_write_to_blk( blk1 & blk ) {
	// buffer1 type blk
	// blk allocates own memory
	// blk copies in the data
	// slight waste but minor
	if(! blk.set( buff, size )) {
		return FAIL_FAILED();
	}
	release();
	return true;
}

bool TJ_blk_t:: read_entire_file( const char * filename, long K_max ) {
	i64 filesize = 0;
	if(!get_file_size( filename, filesize)) 
		return FAIL_FAILED();
	if( K_max < 0 ) K_max = 1<<(32-10-1); // 2G
	i64 filesize_max = K_max << 10; // K to units 
	if( filesize >= filesize_max ) {
		INFO("file %s size %Ld", filename, filesize );
		INFO("K_max %ld", K_max );
		return FAIL("file too big to be real, it will stress memory");
	}
	if(!tj_alloc( (int) filesize )) {
		INFO("file %s size %Ld", filename, filesize );
		return FAIL_FAILED();
	}
	obj_hold<fd_hold_1> fd_hold = new fd_hold_1();
	if(!fd_hold->open_RO( filename )) // sync
		return FAIL_FAILED();

	char * P = (char *)buff;
	int chunk_size = 1024 * 32;
	long left_to_read = filesize;
	while( left_to_read ) {
		if( chunk_size > left_to_read )
			chunk_size = left_to_read;
		int t = fd_hold->read( P, chunk_size );
		if( t != chunk_size ) {
			return FAIL("read %s expected %d got %d", filename, chunk_size, t );
		}
		if( t <= 0 ) {
			WARN("fd_hold.read( P, %d ) returned %d", chunk_size, t );
			return FAIL_FAILED();
		}
		left_to_read -= t;
		P += t;
	}
	PASS("loaded %s %ld bytes", filename, size );
	return true;
}

bool TJ_blk_t:: write_as_file( const char * filename ) {

	obj_hold<fd_hold_1> fd_hold = new fd_hold_1();
	if(!fd_hold->open_RW_CREATE( filename )) // sync plus
		return FAIL_FAILED();


	char * P = (char *)buff;
	int chunk_size = 1024 * 32;
	long left_to_write = size;
	while( left_to_write ) {
		if( chunk_size > left_to_write )
			chunk_size = left_to_write;
		int t = fd_hold->write( P, chunk_size );
		if( t != chunk_size ) {
			return FAIL("write %s expected %d got %d", filename, chunk_size, t );
		}
		if( t <= 0 ) {
			return FAIL_FAILED();
		}
		left_to_write -= t;
		P += t;
	}
	PASS("saved %s %ld bytes", filename, size );
	return true;
}




