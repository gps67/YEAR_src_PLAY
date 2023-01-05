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
#include "tm_parts.h"

struct sender_t {
	fd_hold_1 fd_src;	// read from file
	fd_hold_1 fd_dst;	// write to file

	u64 size_full = 0;

	// stats
	u64 size_sent = 0;

	bool re_init()
	{
		size_full = 0;
		size_sent = 0;
		fd_src.close();
		fd_dst.close();
		return true;
	}

	bool copy_src_name_dst_try(
		const char * src_over,
		const char * src_name_ext,
		const char * dst_over
	);

	bool skip_file_named( str0 name )
	{
		if( name.has_prefix(".tmp.")) {
			// INFO("SKIPPING OWN %s", (STR0) name );
			return true;
		}
		if( name.has_suffix(".cpy")) {
			// INFO("SKIPPING OWN %s", (STR0) name );
			return true;
		}
		return false; // OK dont skip
	}

	bool pre_flight_checks()
	{
	 	return true;
	}

	bool call_fsync() {
		return true;
		e_print("f_");
		if(! fd_dst.fsync() ) {
			return FAIL_FAILED();
		}
		e_print("\b");
		size_sent = 0;
		return true;
	}

	bool call_fdatasync() {
		return true;
		e_print("D_");
		if(! fd_dst.fdatasync() ) {
			return FAIL_FAILED();
		}
		e_print("\b");
		size_sent = 0;
		return true;
	}

	
};

bool sender_t::copy_src_name_dst_try(
	const char * src_over,
	const char * src_name_ext,
	const char * dst_over
 ) {
	re_init();

//	gdb_invoke(false);

	if(0) {
	INFO("src_over     %s", src_over );
	INFO("src_name_ext %s", src_name_ext );
	INFO("dst_over     %s", dst_over );
	}

	// dst_over must exist as a dir 
	{
	 file_stat dst_over_stat; // temp to this { block }
	 if( !dst_over_stat.stat_expect_is_dir( dst_over )) {
		return FAIL_FAILED();
	 }
	}

	// do not copy residual files from own naming system
	// maybe other files from other systems too
	if( skip_file_named( src_name_ext )) {
		INFO("SKIPPING OWN %s", src_name_ext );
		return true;
	}

	// recompute src_name
	buffer1 src_name;
	if( src_over )
		src_name.print("%s/%s", src_over, src_name_ext );
	else 
		src_name.print("%s", src_name_ext );

	file_stat src_stat;
	file_stat dst_stat;

	// src file must exist, as a file, or as ...
	if( !src_stat.stat( (STR0) src_name )) {
		// it really should exist
		return FAIL_FAILED();
	}

	// total file size
	size_full = src_stat.st.st_size;

	// warn if zero size
	if( size_full == 0 ) {
		WARN("CODE error forgot to stat or ZERO size file");
		// check mtime
		// return FAIL("CODE error forgot ot stat");
	}

	// 31 BIT warning
	// we cope with huge files, but mention it
	// even 2G is half the usual limit
	if( size_full >> 31 ) {
		float szg = size_full;
		szg /= 1024; // K
		szg /= 1024; // M
		szg /= 1024; // G
		WARN("SIZE more than 2G, %4.2fG", szg);
	}

	// get BASENAME and gen some temp names // from src_name
	str1 dst_name_tmp;	// copy into this
	str1 dst_name;		// timestamped final copy // name of

	// decode src_name
	dir_name_ext src_dir_name_ext;
	if(!src_dir_name_ext.decode_filename(src_name)) return FAIL_FAILED();

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
	if( !dst_stat.stat_quiet( (STR0) dst_name )) {
		return FAIL_FAILED();
	}
	if( dst_stat.file_type != is_absent ) {
		// we should check size date perms uid gid
		// and be happy if they match
		// but we simply fail so the calling script does that
		 return true;
		 return PASS(" already exists %s", (STR0) dst_name );
	}

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
	 	return FAIL("TODO %s %s", src_stat.file_type_str(), src_name_ext );
	 break;
         case is_dev_b:
	 	return FAIL("TODO %s %s", src_stat.file_type_str(), src_name_ext );
	 break;
         case is_link: {
	 	// clone the link to the same // abs or rel // the same
		file_stat tmp_stat;
		buffer1 link_text;
	 	if(!tmp_stat.readlink_to_buf( src_name, link_text ))
			return FAIL_FAILED();
		if(!symlink( (STR0) link_text, dst_name )) 
			return FAIL_FAILED();
		return true;
		// note reuse of words "src" and "dst"
		// BECAUSE src -> LINK
		// NORMALLY when talking about symlink src => dst
		// WANT dst_name -> link
	 }
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


	// open to read from src
	if(!fd_src.open_RO( (STR0) src_name )) {
		return FAIL_FAILED();
	}

	// write to mid file
	// LOOK for restart
	// open RW +- CREATE // seek zero or stat.midfile.size_part
	u64 size_part = 0; // previous run left _part already copied
	if( !dst_stat.stat_quiet( (STR0) dst_name_tmp )) {
		// stat_quiet returns TRUE when filename does not exist
		// but it returns FALSE if some other error happens
		return FAIL_FAILED(); // stat failed // not absent 
	}
	if( dst_stat.linked_file_type == is_absent ) {
		// absent file means start copy from beginning
		if(!fd_dst.open_RW_CREATE( (STR0) dst_name_tmp )) {
			return FAIL_FAILED();
		}
	} else {
		// exists means restart
		// we really should be only files by now
		// others were found above
		// .tmp is our unique filename, and it is dir or something odd
		// must be a file
		if( dst_stat.linked_file_type != is_file ) {
			return FAIL("%s not is_file", (STR0) dst_name_tmp );
		}
		// size_part == amount already copied
		size_part = dst_stat.st.st_size;
		if(!fd_dst.open_RW( (STR0) dst_name_tmp )) {
			return FAIL_FAILED();
		}
		// TODO TEST on 32 bit CPU
		// seek both src and dst to size_part
		if(!fd_dst.seek_SET_64( size_part )) { // must match fd_dst
			return FAIL_FAILED();
		}
		if(!fd_src.seek_SET_64( size_part )) { // must match fd_dst
			return FAIL_FAILED();
		}
	}
	// amount left to do is size_left
	u64 size_left = size_full - size_part;

	// end // size_part
	int size_block = 1024 * 32; // 32K is DVD block size
	static const int loops_per_sync = 1000; // 500; // 100; // 50;
	if(0) size_block = 1024; // TEST slow down
	if(1) size_block = 1024*256; // TEST slow down
	char buff[ size_block ];
	int fake_stop = 5; // TEST partial copy using fixed limit progress
	int loops_count = loops_per_sync;
	static const int every_n_seconds = 2; // 10; // 5; // do full fsync not fdatasync
	time_t time_last_sync; // rounded seconds
	time_t time_last_fsync; // rounded seconds
	time_t time_right_now; // rounded seconds
	time_last_fsync = time_last_sync = time_right_now = time(NULL);
//	u64 size_sent_tide = 1024*1024; // 1 MB ~ 20 seconds 
	u64 size_sent_tide = 512*1024; // 256 K ~ 5 seconds // ?
	while (size_left > 0) {

		time_right_now = time(NULL);
		if( time_right_now != time_last_sync ) {
			// every changed second
			time_last_sync = time_right_now;
			if( time_last_fsync + every_n_seconds <= time_right_now ) {
				time_last_fsync = time_right_now ;
				// every 5 seconds // if check within that sec
				if(0) {
					if(!call_fsync() )
						return FAIL_FAILED();
				}
				if(1) {
					if(!call_fdatasync()) 
						return FAIL_FAILED();
				}
			} else {
				// every second other than multiple
				if(1) {
					if(!call_fdatasync()) 
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
				if(0) {
					if(!call_fdatasync()) 
						return FAIL_FAILED();
				}
			}
		}
		if(0)
		 if(!fake_stop--) {
			return FAIL("fake_stop");
		 }

		 tm_parts tm_part;
		 tm_part.local_from_time(time(NULL));

		e_print("\r %s size_sent K x%lX size_left %7.3f M ", tm_part.str_http(), (long) (size_sent/1024), (float) size_left / (1024*1024) ) ;
	//	if(!loops_count) e_print("\n");
	//	e_print("\n");
	//	fflush(0); // e_print does not need fflush(0)
		int sz_write = size_block; // also size_block == sizeof( buff )
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
		e_print(".");
		wrote = fd_dst.write( buff, sz_write );
		if( wrote != sz_write ) {
			FAIL("ERROR WRITE expected %d got %d", sz_write, wrote);
			sleep(2);
			return FAIL_FAILED(); // try complete restart
			continue;
		}
		size_left -= wrote;
		size_sent += wrote;

		if(size_sent >= size_sent_tide) {

			if(1) {
				if(!call_fdatasync()) 
					return FAIL_FAILED();
			}

			if(0) {
				if(!call_fsync() )
					return FAIL_FAILED();
			}

		}
	} // while
	e_print("\r");
	if(!call_fsync() )
		return FAIL_FAILED();
	if(!call_fdatasync()) 
		return FAIL_FAILED();
	if(! fd_dst.close() ) 
		return FAIL_FAILED();
	// if there was an actual fail, this point is not reached

	// clone mtime // but not perms ?? // not UID // 
	if(!src_stat.utime_to_filename( (STR0) dst_name_tmp ) ) {
		return FAIL_FAILED();
	}

	if(! file_rename( (STR0) dst_name_tmp, (STR0) dst_name ) ) {
		return FAIL_FAILED();
	}
	
	return PASS("%s", (STR0) dst_name );
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
		sender_t sender;
		if( sender.copy_src_name_dst_try( src_over, src_name_ext, dst_over )) {
			return true;
		}
		INFO("retry %d of %d", i+1, loops );
	}
	return FAIL_FAILED();
}

