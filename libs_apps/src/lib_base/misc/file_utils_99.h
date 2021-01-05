#ifndef file_utils_99_H
#define file_utils_99_H

#include <glib.h>
#include <glib/gstdio.h> // g_chdir

struct file_utils_99
{
	struct stat st;

	bool chdir( const char * dir );
	bool file_exists( const char * name );
	bool dir_exists( const char * dir );
	bool chdir_if_exists( const char * dir );

	bool can_open_file_rw( const char * filename );
};

#endif
