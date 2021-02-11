#ifndef FH1_H
#define FH1_H /* mmap file readonly */
/* file-heap */
#include <unistd.h>
#include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#include "dgb_print.h"
#include "dgb_fluff.h"
/*
	mmap file READONLY (pending LDR-95-Jul)
	grow_file is untested (RO)
	limit 63K for u16 offset (minor edits for u32 seeks)
	later add sub_class with u16 iface + checks to u32 core
	add file_name

	file must not be empty (my choice - why?)
	options on limiting to 64K for mini-heap technology
*/

struct map_file : public dgb_print
{
	map_file( void );
	~map_file();

	int fd;		/* fd of file store provider = fd of file */
	int writable;	/* was opened RDWR */
	u16 fd_size;	/* file length */
	char * page0;	/* mmapped */ /* NOT alloc'ed */

	int map_in_file( u8 * filename, int wwritable );
	int map_in_file( char * filename, int wwritable ) {
		return map_in_file( (u8 *) filename, wwritable ); }
	int remap( void );		/* eg after grow file */
	int sync( void );		/* calls fsync() */
	void unmap( void );		/* unload file */

	int test1(void);
	int grow_file( u16 new_size );	/* appends to file */

/* ifdef P0P2_H
	operator p0p2 ( return p0p2( f.page0, f.fd_size );}
*/
};
#endif
