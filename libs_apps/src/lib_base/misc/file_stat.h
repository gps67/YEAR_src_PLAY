#ifndef file_stat_H
#define file_stat_H

#include <sys/types.h>

// #include <dirent.h>

#include <sys/stat.h>
// #include <unistd.h>

// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>

// #include <errno.h>
#include "ints.h"
#include "blk1.h"

/*!
	File_Type normalises posix/UNIX file type flags

	There is no is_dir_which_is_a_mount_point
*/
// C // typedef 
enum File_Type
{
	is_absent,	// link to missing object
	is_file,
	is_dir,
	is_dev_c,
	is_dev_b,
	is_link,
	is_fifo,
	is_socket,
	is_other,
	is_error	// eg  SYS ERROR
};

/*
	Convert File_Type to a string
*/
const char * str_file_type( File_Type file_type );

/*!

	file_stat is a wrap over UNIX stat, to make it sensible (File_Type)

	If the file is a link, stat is called again (this time followed),
	so there are two sets of info:

		st
		file_type

		linked_st
		linked_file_type

	file_type is a re-wrap of the posix bits, because they're wierd,
	but the rest of the stat st structure is for the caller.

	An absent soft link returns TRUE/is_link/is_absent

	Hard linked name/inode s are not annotated, other than in stat,
	this is for a generically simple approach, not a cpio system.

	The filename is not retained, just used directly (no malloc)

	((TODO add readlink)). 

*/
class file_stat : public GRP_lib_base
{
 public:
	struct stat st;
	struct stat linked_st;
	blk1 readlink_val; // auto set by stat (in inode anyway)

	File_Type file_type;
	File_Type linked_file_type;

	file_stat();
	void clear();
	bool stat( const char * filename );
	bool stat_fd( int fd );
	bool readlink_to_buf( const char * filename, blk1 & buf );
	bool readlink_to_val( const char * filename );
	static File_Type get_file_type( const struct stat & s );
	bool check_ln_s( const char * dst, const char * src );
	bool renew_ln_s( const char * dst, const char * src );
};

#endif
