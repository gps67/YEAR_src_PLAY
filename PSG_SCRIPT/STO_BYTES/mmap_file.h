#ifndef mmap_file_H
#define mmap_file_H /* mmap file readonly */

#include "fd_hold.h"

/* file-heap */
#include <unistd.h>
#include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "ints.h"
#include "dgb.h"
// #include "dgb_print.h"
// #include "dgb_fluff.h"
/*
	mmap file READONLY (pending LDR-95-Jul)
	grow_file is untested (RO)
	limit 63K for u16 offset (minor edits for u32 seeks)
	later add sub_class with u16 iface + checks to u32 core
	add file_name

	file must not be empty (my choice - why?)
	options on limiting to 64K for mini-heap technology
*/

namespace STO {
class mmap_file : public fd_hold_1 // obj_ref_
{ public:
	mmap_file();
	~mmap_file();

// fd from fd_hold_1
//	int fd;		/* fd of file store provider = fd of file */
	int writable;	/* was opened RDWR */
	u32 fd_size;	/* file length */
	char * page0;	/* mmapped */ /* NOT alloc'ed */

	u32 fd_grumble_size; /* file as big as this, have different handler */

	bool mmap_in_file_RW( const char * filename)
	{ return map_in_file( (const u8*) filename, true ); }
	bool mmap_in_file_RO( const char * filename)
	{ return map_in_file( (const u8*) filename, false ); }
	bool map_in_file( const u8 * filename, bool wwritable );
	bool map_in_file( const char * filename, bool wwritable ) {
		return map_in_file( (const u8 *) filename, wwritable ); }
	bool remap( void );		/* eg after grow file */
//	bool sync( void );		/* calls fsync() */
	bool unmap( void );		/* unload file */

	bool test1(void);
	bool grow_file_16( u16 new_size );	/* appends to file */
	bool grow_file_32( u32 new_size );	/* appends to file */
	bool grow_file_k_u16( u16 new_size_k ); // u16 // u26 //

/* ifdef P0P2_H
	operator p0p2 ( return p0p2( f.page0, f.fd_size );}
*/
}; // struct
}; // namespace
#endif
