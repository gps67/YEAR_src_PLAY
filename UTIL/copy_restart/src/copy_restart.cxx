#include "dgb.h"
#include "ints.h"
#include <stdio.h>
#include "file_stat.h"
#include "dir_name_ext.h"
#include "fd_hold.h"
#include "file_rename.h"
// typedef unsigned int uns;

bool bool_main( int argc, char ** argv ) {

	set_prog_name( argv[0] ); // check this
//	gdb_sigaction( argv[0] ); // check this

//	gdb_invoke(false);

	const char * src_name = "/home/gps/G/RIPS/eg_rip_DVD_here/iso/"
	"THE_DVD_DISC_1.iso";
	const char * dst = "/nfs/NAS_ln_s/NAS3_a2_DVDS/iso";

	if( argc != 3 ) {
		errno = 22;
		FAIL("USAGE %s src dst # argc %d", get_prog_alias(), argc );
		WARN("test defaulting src and dst");
//		return false;
	} else {
		src_name = argv[1];
		dst = argv[2];
	}

	file_stat stat_src;
	file_stat stat_dst;
	u64 size_full = 0;

	if( !stat_src.stat( src_name )) {
		// src file must exist, end of
		return FAIL_FAILED();
	}
	if( stat_src.file_type != is_file ) {
		WARN("src must be a file %s", src_name);
		return false;
	}
	size_full = stat_src.st.st_size;
	if( size_full >> 31 ) {
		WARN("SIZE more than 2G");
	}

	if( !stat_dst.stat( dst )) {
		// dst may be a dir, which might not exist ?
		return FAIL_FAILED();
	}
	if( stat_dst.file_type != is_dir ) {
		WARN("dst must be a dir %s", src_name);
		return false;
	}

	dir_name_ext dir_name_ext_src;
//	dir_name_ext_src.decode_filename(src_name) || return FAIL_FAILED();
//	problem is that || return FAIL_FAILED(); needs return to be bool
	if(!dir_name_ext_src.decode_filename(src_name)) return FAIL_FAILED();
//	dir_name_ext_src.test_print();
	str1 dst_name_tmp;
	str1 dst_name;
	buffer1 buff_dst_name_tmp;
	buffer1 buff_dst_name;
	buff_dst_name_tmp.print("%s/.tmp.%s_%s.cpy",
	 (STR0)	dst, 
	 (STR0)	dir_name_ext_src.name,
	 (STR0)	dir_name_ext_src.ext
	);
	buff_dst_name.print("%s/%s.%s",
	 (STR0)	dst, 
	 (STR0)	dir_name_ext_src.name,
	 (STR0)	dir_name_ext_src.ext
	);
	dst_name_tmp = (STR0) buff_dst_name_tmp;
	dst_name = (STR0) buff_dst_name;

	INFO("dst_name_tmp %s", (STR0) dst_name_tmp );
	INFO("dst_name %s", (STR0) dst_name );

	// EXIT if repeat dst already exists
	if( stat_dst.stat_quiet( dst_name )) {
		// we should check size date perms uid gid
		// and be happy if they match
		// but we simply fail so the calling script does that
		WARN(" already exists %s", (STR0) dst_name );
		return FAIL_FAILED();
	}

	// we will fail on 32 bit systems, u31 limit
	// RTFM man stat64 -D_FILE_OFFSET_BITS=64
	u64 size_part = 0;
	fd_hold_1 fd_src;
	if(!fd_src.open_RO( (STR0) src_name )) {
		return FAIL_FAILED();
	}
	fd_hold_1 fd_dst;
	// LOOK for restart
	if( stat_dst.stat_quiet( (STR0) dst_name_tmp )) {
		// exists means restart
		if( stat_dst.file_type != is_file ) {
			return FAIL("not is_file");
		}
		size_part = stat_dst.st.st_size;
		if(!fd_dst.open_RW( (STR0) dst_name_tmp )) {
			return FAIL_FAILED();
		}
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
	int size_block = 1024 * 32;
if(0) size_block = 1024;
	char buff[ size_block ];
int fake_stop = 5;
	while (size_left > 0) {
	 if(0)
		if(!fake_stop--) {
			return FAIL("fake_stop");
		}
		e_print("\r size_left %7.2f M", (float) size_left / (1024*1024) ) ;
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
		if(! fd_dst.fdatasync() ) {
			return FAIL_FAILED();
		}
	}
	if(! fd_dst.fdatasync() ) {
		return FAIL_FAILED();
	}
	if(! fd_dst.close() ) {
		return FAIL_FAILED();
	}

	if(!stat_src.utime_to_filename( (STR0) dst_name_tmp ) ) {
		return FAIL_FAILED();
	}

	if(! file_rename( (STR0) dst_name_tmp, (STR0) dst_name ) ) {
		return FAIL_FAILED();
	}
	


	return true;
}

int main( int argc, char ** argv ) {
	if( bool_main( argc, argv )) {
		PASS("DONE");
		return 0;
	}
	if(errno) 
		return errno;
	WARN("NO ERRNO");
	return 1;
}
