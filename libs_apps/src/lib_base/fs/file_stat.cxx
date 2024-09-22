
#include "file_stat.h"

// #include <fcntl.h>
// #include <sys/stat.h>


// #include <sys/types.h>
// #include <dirent.h>

// #include <sys/stat.h>
// #include <unistd.h>

// #include <stdlib.h>
#include <stdio.h> // perror
// #include <string.h>

// #include <errno.h>
#include "dgb.h"
#include "buffer1.h" // printf
#include <unistd.h>
#include "err_int.h"

/*!
	Convert File_Type to a string
*/
const char * str_file_type( File_Type file_type )
{
	switch( file_type ) {
	       case is_absent:	return "ABSENT";	// WRAP += is_absent_DIR
	break; case is_file:	return "file";
	break; case is_dir:	return "dir";
	break; case is_dev_c:	return "dev_c";
	break; case is_dev_b:	return "dev_b";
	break; case is_link:	return "link";
	break; case is_fifo:	return "fifo";
	break; case is_socket:	return "socket";
	break; case is_error:	return "ERROR";
	break; case is_other:	return "XX_OTHER__XX";
	}
	return "XX_ERROR_XX";
}


//////// file_stat

	/*!
	*/
	file_stat::file_stat()
	{
		clear();
	}

	/*!
		NOT in CLASS
	*/
	inline void clear_st( struct stat & st )
	{
		memset( &st, 0x00, sizeof( st ) );
	}

	/*!
	*/
	void file_stat::clear()
	{
		clear_st( st );
		clear_st( linked_st );
		readlink_val.clear();
		file_type = is_absent;
		linked_file_type = is_absent;
		saved_filename.clear();
	}

	/*!
	*/
	bool file_stat:: readlink_to_val( const char * filename )
	{
		return readlink_to_buf( filename, readlink_val );
	}

	/*!
		clear buf
		call OS readlink
		no help with CSET of disk though

		// file_stat struct has own readlink_val
	*/
	bool file_stat::readlink_to_buf( const char * filename, blk1 & buf )
	{
		int gap_sz = 0;
		int gap_max = 1024 * 3;
		gap_sz = buf.space_avail();

// BUG _alloc == 0 // 
//		INFO("gap=%d", gap_sz );
//		INFO("buf.nbytes_alloc =  %d", buf.nbytes_alloc );
//		INFO("buf.nbytes_used  =  %d", buf.nbytes_used );
		buf.clear();
//		INFO("buf.nbytes_alloc =  %d", buf.nbytes_alloc );
//		INFO("buf.nbytes_used  =  %d", buf.nbytes_used );

		// so that is the usual default half line available
		// limit get_space( gap_max )
		if(!filename) return FAIL("NULL filename" );
		int len=0; // should be size_t but -1 ??

		while( gap_sz < gap_max ) {
		  gap_sz = buf.space_avail();

		  len = readlink(
			filename,
			buf.gap_addr_signed_char(),
			gap_sz
		  );
	//	  INFO("%d from file %s gap=%d", len, filename, gap_sz );
		  if( -1 == len ) {
			WARN("(-1) %s so get_space(more)", filename );
		  } else
		  if( len < gap_sz ) {
			// == might be overflow truncated
			buf.nbytes_used_add( len );
			buf.trailing_nul(); // no poss fail
	//	  	INFO("LINK %s", (STR0) buf );
			return true;
		  }

		  gap_sz += 20;
		  gap_sz *= 2;
		  if(!buf.get_space( gap_sz )) return FAIL_FAILED();
		}

		return FAIL("link name size %d or system error", gap_sz);
	}

	/*!
		TODO set saved_filename to something
	*/
	bool file_stat:: stat_fd( int fd )
	{
		file_type = is_error;
		linked_file_type = is_absent; // is_unset
		readlink_val.clear();
		saved_filename.clear();
		saved_filename.printf("fd_%x02X", fd ); // EDIT int to fd_t //

		if(fd<0) return FAIL("fd<0 %d", (int) fd);

#ifdef WIN32
#error WIN32
#else
		// call lstat - which is stat without following links
		if(-1==fstat( fd, &st ))
		{
			return FAIL("fd == %d", fd);
		}
#endif

		file_type = get_file_type( st );

	#if 0
		// fd is open, cant really be a symb link ?
		if( file_type == is_link )
		{
			if(!readlink_to_buf( filename, readlink_val ))
				return FAIL_FAILED();

			/*
				get the stat info of the linked target,
				following all intermediate links (poss excess)
			*/
			if(-1==::stat( filename, &linked_st ))
			{
				linked_file_type = is_absent;
			} else {
				linked_file_type = get_file_type( linked_st );;
			}
		}
	#endif
		return true;
	}

	/*!
		get file stat - absent file is an error

		as indeed it is, UNIX sets errno and so on
	*/
	bool file_stat::stat( const char * filename ) {
		save_filename(filename);
		bool FAIL_if_absent = true;
		return stat_FAIL_if_absent( filename, FAIL_if_absent );
	}

	/*!
		return true - stat done without error

		return with file_type == is_absent
	*/
	bool file_stat::stat_OK_if_absent( const char * filename ) {
		bool FAIL_if_absent = false;
		return stat_FAIL_if_absent( filename, FAIL_if_absent );
	}

	/*!
	*/
	bool file_stat::stat_quiet( const char * filename ) {
		return stat_OK_if_absent( filename );
	}

	/*!
	*/
	bool file_stat::stat_FAIL_if_absent( const char * filename, bool FAIL_if_absent )
	{
		file_type = is_error;
		linked_file_type = is_absent; // is_unset
		readlink_val.clear();
		if(!filename) return FAIL("NULL filename");

#ifdef WIN32
		if(-1==::stat( filename, &st ))
		{
			// WIN32 - may need editing
			if( errno == ENOENT ) {
				file_type = is_absent;
				if( FAIL_if_absent ) {
					return FAIL("ENOENT %s", filename);
				} else {
					err_int_t errr;
					errr zap_OS_error();
					return true;
					return false;
				}
			}
			return FAIL("%s", filename);
		}
#else
		// call lstat - which is stat without following links
		if(-1==lstat( filename, &st ))
		{
			if( errno == ENOENT ) {
				file_type = is_absent;
				if( FAIL_if_absent ) {
					return FAIL("ENOENT %s", filename);
					// false matches -1 from syscall
				} else {
					err_int_t err_int;
					err_int.zap_OS_error();
					return true;
					return false;
				}
			}
			return FAIL("%s", filename);
		}
#endif
		file_type = get_file_type( st );

		if( file_type == is_link )
		{
			if(!readlink_to_buf( filename, readlink_val ))
				return FAIL_FAILED();

			if(1)	
			 INFO("called readlink_to_buf() %s -> %s", filename, (STR0)readlink_val );

			/*
				get the stat info of the linked target,
				following all intermediate links (poss excess)
			*/
			if(-1==::stat( filename, &linked_st ))
			{
				linked_file_type = is_absent;
				err_int_t errr;
				errr.zap_OS_error();
				WARN("symb_link to absebt file");
			} else {
				linked_file_type = get_file_type( linked_st );;
			}
		} else {
			linked_file_type = file_type;
		}
		return true;
	}

	/*!
	*/
	// STATIC 
	File_Type file_stat::get_file_type( const struct stat & s )
	{
		mode_t m = s.st_mode;
		if(S_ISREG(m)) return is_file;
		if(S_ISDIR(m)) return is_dir;
		if(S_ISCHR(m)) return is_dev_c;
		if(S_ISBLK(m)) return is_dev_b;
		if(S_ISFIFO(m)) return is_fifo;
#ifndef WIN32
		if(S_ISLNK(m)) return is_link;
		if(S_ISSOCK(m)) return is_socket;
#endif
		return is_other;

		// is_absent only from stat + ENOENT
		// is_error only from stat and errno
	}

	// src => dst
	// ln -s something_that_probably_already_exists new_thing
	// ln -s dst src
	bool file_stat:: symlink( const char * dst, const char * src )
	{
		if( ::symlink( dst, src ) == 0 ) {
			return true;
		}
		return FAIL("symlink(%s,%s)", dst, src );
	}

	/*!
	*/
	bool file_stat:: check_ln_s( const char * dst, const char * src )
	{
		if(!stat( src )) return FAIL_FAILED();
		if( is_link != file_type ) {
			INFO("%s not a link // to %s", src, dst );
			return false;
		}
		if( readlink_val == dst ) {
			PASS("%s is a link to %s", src, dst );
			return true;
		}
		WARN("%s is a link to %s but not %s",
			src, (STR0) readlink_val, dst );
		return false;
	}

	/*!
	*/
	bool file_stat::renew_ln_s( const char * dst, const char * src )
	{
		if( check_ln_s( dst, src ) )
			return true;
	
		switch( file_type ) {
		
		 case is_absent:	// link to missing object
		 case is_file:
		 case is_dir:
		 case is_dev_c:
		 case is_dev_b:
		 case is_link:
		 case is_fifo:
		 case is_socket:
		 case is_other:
		 case is_error:
		 	return FAIL("code tbs");
		};

		if( is_link != file_type ) {
			INFO("%s not a link (to %s)", src, dst );
			return false;
		}
		if( readlink_val == dst ) {
			PASS("%s -link-> to %s", src, dst );
			return true;
		}
		WARN("%s is a link to %s but not %s",
			src, (STR0) readlink_val, dst );
		return false;
	}

	/*!
		copy mtime from prev stat'd file to filename2
	*/
	bool file_stat:: utime_to_filename( const char * filename )
	{
		struct timespec times[2];
		times[0] = st.st_atim;
		times[1] = st.st_mtim;
		int dirfd = AT_FDCWD; // if relative pathname use current dir
		int flags = 0; // AT_SYMLINK_NOFOLLOW // man utimensat
#ifdef WIN32
#  warning does this work on WIN32 ??
// will probably be an absent function
#endif
		int t = utimensat( // nano second u-time // touch file mtime
			dirfd,		// either fd of dir or AT_FDCWD
			filename,
			times,
			flags
		);
		if(t) {
			return FAIL("%s", filename );
		}
		return PASS("%s", filename );
	}

	bool file_stat:: expect_is_dir() // accept is_link
	{
		// this is why you must reset linked_file_type to is_absent
		if( linked_file_type == is_dir ) return true;
		INFO("%s", get_filename() );
		return FAIL("expected is_dir got %s", str_file_type( linked_file_type ));
	}
	bool file_stat:: stat_expect_is_dir(const char * filename)
	{
		if(!stat( filename )) return FAIL_FAILED();
		return expect_is_dir();
	}

	bool file_stat:: expect_is_file() // accept is_link // FAIL with message
	{
		if( linked_file_type == is_file ) return true;
		INFO("%s", get_filename() );
		return FAIL("expected is_file got %s", str_file_type( linked_file_type ));
	}

	bool file_stat:: stat_expect_is_file(const char * filename) // accept is_link // FAIL with message
	{
		if(!stat( filename )) return FAIL_FAILED();
		return expect_is_file();
	}

	const char * file_stat:: file_type_str()
	{
		return str_file_type( file_type );
	}

	const char * file_stat:: linked_file_type_str()
	{
		return str_file_type( linked_file_type );
	}

