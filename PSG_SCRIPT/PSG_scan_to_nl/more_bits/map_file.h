#ifndef map_file_H
#define map_file_H

#include "ints.h"	// u8 GRP_

/*!
	memory-mapped file holder
*/
struct map_file : public GRP_lib_base
{
	map_file( void );
	~map_file();

	int fd;		/* fd of file */
	bool writable;	/* was opened RDWR */
	int fd_size;	/* file length from inode */
	char * page0;	/* mmapped */ /* NOT alloc'ed */

	int map_in_file( const u8 * filename, int wwritable );
	int map_in_file( const char * filename, int wwritable )
	{
		return map_in_file( (const u8 *) filename, wwritable );
	}

	int remap( );		/* eg after grow file */
	int fsync( );		/* calls ::fsync() */
	int sync( )		{ return fsync(); }
	void unmap( );		/* unload file */
	bool close();

	int grow_file( int new_size );	/* appends to file */

	int test1(void);

/* ifdef P0P2_H
	operator p0p2 ( return p0p2( f.page0, f.fd_size );}

*/
};
#endif
