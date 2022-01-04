#ifndef map_file_H
#define map_file_H

#include "ints.h"	// u8 GRP_
#include "p0p2.h"	// ger P0P2

/*!
	MMAP memory-mapped file holder

	map_file is more general mmap_file
	which might include SEGMENT elf dlopen ld_load

	When in ROM we have to be able to detect EOF with no NUL in sight
	When in MMAP we MIGHT have a trailing NUL beyonf EOF upto PAGE_EDGE
	When in SEGMENT use P0P2 

	MMAP can also borrow  pages from elf_LOADED_SEGMENTS incl RW_sections!
	MMAP_DATA asks HEAP_CTRL for access to PAGES within SHARED FILES
	MMAP_DATA asks FILE_CTRL for PAGES of MMAP as if from filename.ext
*/
struct mmap_file : public GRP_lib_base // NO BASE CLASS // TODO ref0 
{
	mmap_file( void );
	~mmap_file();

	int fd;		/* fd of file */

	int nbytes;	/* file length from inode */
	char * page0;	/* mmapped */ /* NOT alloc'ed */
	bool writable;	/* was opened RDWR */

/*
	TODO GET_P0P2( mmap_file, P0P2 ) {
		SEGMENT is LOCK RDWR RO 
		PARSER wants RO
		STO wants RW
		SCRIPT wants RO SNAPSHOT

		STO wants to relocate the MMAP as it grows
		STO wants to flip between ODD EVEN CYCLES
		STO wants RDWR
		ROM wants RO
	}
*/

	int map_in_file( const u8 * filename, int wwritable );
	int map_in_file( const char * filename, int wwritable )
	{
		return map_in_file( (const u8 *) filename, wwritable );
	}

	int mmap_remap( );	/* eg after grow file */
	int fsync( );		/* calls ::fsync() */
	int sync( )		{ return fsync(); }
	void mmap_unmap( );		/* unload file */
	bool close();

	int grow_file( int new_size );	/* appends to file */

	int test1(void);

	operator p0p2 () { return p0p2( page0, nbytes ); }

	bool check_nl_at_eof();

/* ifdef P0P2_H
	operator p0p2 ( return p0p2( f.page0, f.nbytes );}

*/
};
#endif
