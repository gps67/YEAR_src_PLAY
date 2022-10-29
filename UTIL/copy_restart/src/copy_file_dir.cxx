#include "dgb.h"
// #include "ints.h"
// #include <stdio.h>
#include "file_stat.h"
#include "dir_name_ext.h"
#include "fd_hold.h"
#include "file_rename.h"
#include "copy_restart.h"
// typedef unsigned int uns;

// put time into a module
#include <time.h>

bool copy_src_name_dst_try(
	const char * src_over,
	const char * src_name_ext,
	const char * dst_over
 ) {

//	gdb_invoke(false);

//	dir_name_ext test_dir_name_ext;
//	test_dir_name_ext.test1();

	if(0) {
	INFO("src_over     %s", src_over );
	INFO("src_name_ext %s", src_name_ext );
	INFO("dst_over     %s", dst_over );
	}

	str1 name = src_name_ext;
	if( name.has_prefix(".tmp.")) {
		INFO("SKIPPING OWN %s", (STR0) name );
		return true;
	}
	if( name.has_suffix(".cpy")) {
		INFO("SKIPPING OWN %s", (STR0) name );
		return true;
	}

	buffer1 src_name;
	if( src_over )
		src_name.print("%s/%s", src_over, src_name_ext );
	else 
		src_name.print("%s", src_name_ext );

	file_stat src_stat;
	file_stat dst_stat;
	u64 size_full = 0;

	// dst_name must exist as a dir 
	if( !dst_stat.stat_expect_is_dir( dst_over )) {
		return FAIL_FAILED();
	}


	// src file must exist, as a file, or as ...
	if( !src_stat.stat( (STR0) src_name )) {
		// it really should exist
		return FAIL_FAILED();
	}

	// 31 BIT warning
	size_full = src_stat.st.st_size;
	if( size_full >> 31 ) {
		float szg = size_full;
		szg /= 1024; // K
		szg /= 1024; // M
		szg /= 1024; // G
		WARN("SIZE more than 2G, %4.2fG", szg);
	}

	if( size_full == 0 ) {
		WARN("CODE error forgot to stat or ZERO size file");
		// check mtime
		// return FAIL("CODE error forgot ot stat");
	}

	// get BASENAME and gen some temp names // from src_name
	str1 dst_name_tmp;	// copy into this
	str1 dst_name;		// timestamped final copy // name of
	// decode src_name
	dir_name_ext src_dir_name_ext;
	if(!src_dir_name_ext.decode_filename(src_name)) return FAIL_FAILED();

//	src_dir_name_ext.test_print();

	// temp name // partial copy restart
	// we can take the EXT out because it is a mangled filename
	buffer1 buff_dst_name_tmp;
	buff_dst_name_tmp.print("%s/.tmp.%s_%s.cpy",
	 (STR0)	dst_over, 		// TOPDIR
	 (STR0)	src_dir_name_ext.name,	// FILE_NAME.EXT
	 (STR0)	src_dir_name_ext.ext	// .EXT 
	);

	// dest name // full copy
	// BEWARE of %s.%s which reintroduces absent . use name_ext
	buffer1 buff_dst_name;
	buff_dst_name.print("%s/%s", // ERROR 
	 (STR0)	dst_over, 
	 (STR0)	src_dir_name_ext.name_ext
	);

	dst_name_tmp = (STR0) buff_dst_name_tmp;
	dst_name = (STR0) buff_dst_name;

	if(0) {
	INFO("dst_name_tmp %s", (STR0) dst_name_tmp );
	INFO("dst_name %s", (STR0) dst_name );
	}

	// if dst_name already exists, it is A COMPLETE COPY
	// MAYBE check mtime nbytes and report mismatch
	// or not
	if( dst_stat.stat_quiet( dst_name )) {
		// we should check size date perms uid gid
		// and be happy if they match
		// but we simply fail so the calling script does that
		return true;
		return PASS(" already exists %s", (STR0) dst_name );
	}


	// maybe put these vars in a block in case
	file_stat tmp_stat;
	buffer1 link_text;

	switch( src_stat.file_type ) {
	 case is_absent:
	 	return FAIL("code error - really must exist");
	 break;
         case is_file:
	 	// stay for the rest of the function
	 break;
         case is_dir:
	 	return FAIL("code error - really cant be dir");
	 break;
         case is_dev_c:
	 	return FAIL("%s %s", src_stat.file_type_str(), src_name_ext );
	 break;
         case is_dev_b:
	 	return FAIL("%s %s", src_stat.file_type_str(), src_name_ext );
	 break;
         case is_link:
	 	if(!tmp_stat.readlink_to_buf( src_name, link_text ))
			return FAIL_FAILED();
		if(!symlink( (STR0) link_text, dst_name )) 
			return FAIL_FAILED();
		return true;
		// note reuse of words "src" and "dst"
		// BECAUSE src -> LINK
		// NORMALLY when talking about symlink src => dst
		// WANT dst_name -> link


	 	return FAIL("%s %s", src_stat.file_type_str(), src_name_ext );
	 break;
         case is_fifo:
	 	return FAIL("%s %s", src_stat.file_type_str(), src_name_ext );
	 break;
         case is_socket:
	 	return FAIL("%s %s", src_stat.file_type_str(), src_name_ext );
	 break;
         case is_other:
	 	return FAIL("%s %s", src_stat.file_type_str(), src_name_ext );
	 break;
         case is_error:
	 default: 
	 	return FAIL("%s %s", src_stat.file_type_str(), src_name_ext );
	}



	// read from src
	fd_hold_1 fd_src;
	if(!fd_src.open_RO( (STR0) src_name )) {
		return FAIL_FAILED();
	}

	// write to mid file
	// LOOK for restart
	fd_hold_1 fd_dst;
	u64 size_part = 0;
	if( dst_stat.stat_quiet( (STR0) dst_name_tmp )) {
		// exists means restart
		if( dst_stat.linked_file_type != is_file ) {
			return FAIL("not is_file");
		}
		size_part = dst_stat.st.st_size;
		if(!fd_dst.open_RW( (STR0) dst_name_tmp )) {
			return FAIL_FAILED();
		}
		// TODO TEST on 32 bit CPU
		if(!fd_dst.seek_SET_64( size_part )) { // must match fd_dst
			return FAIL_FAILED();
		}
		if(!fd_src.seek_SET_64( size_part )) { // must match fd_dst
			return FAIL_FAILED();
		}
	} else {
		if(!fd_dst.open_RW_CREATE( (STR0) dst_name_tmp )) {
			return FAIL_FAILED();
		}
	}

	u64 size_left = size_full - size_part;
	int size_block = 1024 * 32; // 32K is DVD block size
	static const int loops_per_sync = 1000; // 500; // 100; // 50;
	if(0) size_block = 1024; // TEST slow down
	char buff[ size_block ];
	int fake_stop = 5; // TEST partial copy using fixed limit progress
	int loops_count = loops_per_sync;
	static const int every_n_seconds = 10; // 5; // do full fsync not fdatasync
	time_t time_last_sync; // rounded seconds
	time_t time_last_fsync; // rounded seconds
	time_t time_right_now; // rounded seconds
	time_last_fsync = time_last_sync = time_right_now = time(NULL);
	while (size_left > 0) {

		time_right_now = time(NULL);
		if( time_right_now != time_last_sync ) {
			// every changed second
			time_last_sync = time_right_now;
			if( time_last_fsync + every_n_seconds <= time_right_now ) {
				time_last_fsync = time_right_now ;
				// every 5 seconds // if check within that sec
				if(1)
				if(! fd_dst.fsync() ) {
					return FAIL_FAILED();
				}
			} else {
				// every second other than multiple
				if(0)
				if(! fd_dst.fdatasync() ) {
					return FAIL_FAILED();
				}
			}
			// restart loop counter // every second
			loops_count = loops_per_sync;
		} else {
			// every loop // where tick has not happened
			if( !loops_count-- ) {
				// every n loops
				loops_count = loops_per_sync;
				// so why is it showing 0 when pausing not 100
				if(1)
				if(! fd_dst.fdatasync() ) {
					// syncs not working as expected
					return FAIL_FAILED();
				}
			}
		}
		if(0)
		 if(!fake_stop--) {
			return FAIL("fake_stop");
		 }
	//	e_print("\r size_left %7.2f M loops_count %3d ", (float) size_left / (1024*1024), loops_count ) ;
		e_print("\r size_left %7.2f M ", (float) size_left / (1024*1024) ) ;
	//	if(!loops_count) e_print("\n");
	//	e_print("\n");
	//	fflush(0); // e_print does not need fflush(0)
		int sz_write = size_block;
		if( sz_write > size_left ) {
			sz_write = (int) size_left;
		}
		int got = 0;
		got = fd_src.read( buff, sz_write );
		if( got != sz_write ) {
			FAIL("ERROR READ expected %d got %d", sz_write, got);
			sleep(2);
			return FAIL_FAILED(); // try complete restart
			continue;
		}
		int wrote = 0;
		wrote = fd_dst.write( buff, sz_write );
		if( wrote != sz_write ) {
			FAIL("ERROR WRITE expected %d got %d", sz_write, wrote);
			sleep(2);
			continue;
		}
		size_left -= wrote;
	} // while
	if(! fd_dst.fsync() ) { // also write inode
		return FAIL_FAILED();
	}
	if(! fd_dst.close() ) {
		return FAIL_FAILED();
	}
	// if there was an actual fail, this point is not reached

	if(!src_stat.utime_to_filename( (STR0) dst_name_tmp ) ) {
		return FAIL_FAILED();
	}

	if(! file_rename( (STR0) dst_name_tmp, (STR0) dst_name ) ) {
		return FAIL_FAILED();
	}
	
	return true;
}

bool copy_file_dir (
	const char * src_name, // src_name == src_file // ALIAS VAR
	const char * dst_over
	) {
	// decode src_name
	dir_name_ext
	 src_dir_name_ext_src;
	if(!src_dir_name_ext_src.decode_filename(src_name)) return FAIL_FAILED();

	STR0 src_over = src_dir_name_ext_src.dir;

	return copy_src_name_dst(
		src_over,
		src_dir_name_ext_src.name_ext,
		dst_over );
}


bool copy_src_name_dst(
	const char * src_over,
	const char * src_name_ext,
	const char * dst_over
 ) {
	int loops = 4;
	for( int i=0; i<loops; i++ ) {
		if( copy_src_name_dst_try( src_over, src_name_ext, dst_over )) {
			return true;
		}
	}
	return FAIL_FAILED();
 }
