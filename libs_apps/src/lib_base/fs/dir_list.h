#ifndef dir_reader_h
#define dir_reader_h

#include "file_stat.h"
#include <dirent.h>

/*!
	dir_reader(_base) is for iterating over a dir
	and calling file_stat::stat() on each item file.

	skip_dot_all [ default FALSE ] means skip '.hidden_file'

	skip_dotdot [defult TRUE] means skip '.' and '..'
*/
class dir_reader_base : public GRP_lib_base
{
	//! DIR is a libc thing
	DIR *dir;
	struct dirent *dp;

	//! dir_name has to be retained to stat each dir_name/itemname
	char * dir_name;
	int dir_name_len;

	void set_dir_name( const char * name );
	void clear();

 public:

	file_stat item;
	dev_t stat_dot_dev;	// major(3) minor(3) of dev
	bool item_is_dir_mount_point; // move to within file_stat
	/*
		if( !stat_dot_dev ) {
			// presume . comes first and provides dev
			// hope not .. as that might be different 
			// then check is_dir and different dev
			//
			// CODE // stay within FS for find, tree_walk, etc
			// CODE // dont follow symb links, except items ?
			// TODO //
			//
			stat_dot_dev = item.st.st_rdev;
		}
	*/

	bool skip_dot_all;	// dont even stat to see if its a dir
	bool skip_dotdot;	// save on stat and callbacks

	dir_reader_base();
	~dir_reader_base();
	void close();
	bool open( const char * path );
	bool next_name();
	bool next();
	const char * name();
};

#endif
