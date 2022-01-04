
#include <sys/stat.h>
#include "e_print.h"
#include "map_file.h"
#include "ASCII_chars.h" // ASCII_NUL ASCII_LR
#include "dgb.h" // ASCII_NUL ASCII_LR

#include <sys/mman.h>	// mmap
#include <unistd.h>	// fsync

// not needed ...
// #include <sys/types.h>
// // #include <sys/stat.h>
// #include <fcntl.h>
// #include <errno.h>

/*!
	constructor - init null / -1
*/
mmap_file::mmap_file( void )
{
	fd = -1;
	writable = FALSE;
	nbytes = 0;
	page0 = (char *) 0;
}

/*!
	destructor - release file and mem
*/
mmap_file::~mmap_file()
{
	close();
}

/*!
	mmap_unmap() releases the mmap file
*/
void	mmap_file::mmap_unmap( void )
{
	if( ! page0 ) return;
	sync();
	munmap( page0, nbytes );
	page0 = (char *) 0;
}

/*!
	fsync(fd) the mapped file
*/
int mmap_file::fsync( void )
{
	if( fd < 0 ) return 0;	/* file wasn't open */
	if( 0 == ::fsync( fd ) )
		return 0;	/* done */
	return errno;
}

/*!
	mmap in ALL of filename, +- writable 
	// OPTION // return errno
	// OPTION // return u8 * P0 // NO //
*/
int mmap_file::map_in_file( const u8 * ufilename, int wwritable )
{
	char * filename = (char *) ufilename;
	writable = wwritable;
	int mod ;
	if( writable )
	{
//		mod = O_RDONLY; -good- EACCESS from mmap
		mod = O_RDWR;
	} else {
		mod = O_RDONLY;
	}
	if( fd >= 0 )
	{
		mmap_unmap();
		::close( fd );
	}
	fd = open( filename, mod );
	if( fd < 0)
	{
		e_print( "## ERROR ## open(%s) %m (PCT M)\n", filename );
		return errno;
	}
	// mmap_remap does it all
	return mmap_remap();
}

/*!
	mmap_unmap and re-mmap the entire file (eg after grow file)

	fails for zero size files
*/
int mmap_file::mmap_remap( void )
{

#define SYS_PICK_LOCN 0
#define OFFSET0 0

	mmap_unmap();

	int PROT;
	int FLAGS;
	if( writable )
	{
		PROT = PROT_READ | PROT_WRITE; 
		FLAGS = MAP_FILE | MAP_SHARED;

	} else {
		PROT = PROT_READ;
		FLAGS = MAP_FILE | MAP_SHARED;
	}

	// stat fd // nbytes in file.txt
	struct stat st;
	if( fstat( fd, &st ) != 0 )
	{
		// %m means message from errno (no parameter)
		e_print("## ERROR ## fstat() %m (PCT M)\n");
		return errno;
	}
	if( st.st_size == 0 )
	{
		e_print( "zero length file (fd=%d)\n",fd);
		return 1;
	}
	nbytes = st.st_size;

	// page0 = mmap fd OFFS nbytes of fd
	// OFFS = OFFSET0 = 0 // entire file from zero
	caddr_t P = (caddr_t) mmap(
		SYS_PICK_LOCN, // page0 = RET_VAL
		nbytes,	// file_size
		PROT,	// PROT_WRITE
		FLAGS,	// MAP_SHARED
		fd,	// fd
		OFFSET0	// SEEK =  entire file from zero
	);
	if( P == (caddr_t) -1 )
	{
		e_print("## ERROR ## mmap() %m (PCT M)\n");
		return errno;
	}
	page0 = P;
	return 0; /* PASS_ZER0 */
}

/*!
	Extend the file to new_size with hole pages and NULL bytes. mmap_remap()

	Works by seek-end-of-file, and write a '+' byte, then mmap_remap.

	This cannot shrink the file.
*/
int mmap_file::grow_file( int new_size )
{
	if( new_size < nbytes )
	{
		e_print( "# ERROR # grow_file( %d ) cannot shrink from %d \n",
			new_size,
			nbytes
		);
		return 1;
	}
	if( new_size == nbytes )
	{
		e_print( "# INFO # grow_file( %d ) already %d \n",
			new_size,
			nbytes
		);
		return 0;
	}
	mmap_unmap();
	/*
		ext2 will fill sparse files with sparse blocks,
		and pad the last block with NUL bytes.
	*/
	if(-1 == lseek( fd, new_size-1, SEEK_SET ))
	{
		e_print("## ERROR ## lseek() %m (PCT M)\n");
		return errno;
	}
	if(1!= write( fd, "+", 1 ))
	{
		e_print("## ERROR ## write() %m (PCT M)\n");
		return errno;
	}
	mmap_remap();
	return 0;
}

bool mmap_file::close( void )
{
	mmap_unmap();
	::close(fd);
	return true;
}

bool mmap_file:: check_nul_after_eof()
{
	if(!nbytes) return false;
	char * P = page0 + nbytes - 1; // last byte in file
	if( *P == ASCII_NUL ) {
		return true; // allow STR0 // PARSER must check too
	}
	// title says do this first
	// optimism says look inside first
	// first byte of next 
	P++;
	if( *P == ASCII_NUL ) {
		return true; // allow STR0 // PARSER must check too
	}
	return FAIL("wanted NUL at eot");
}

bool mmap_file:: check_nl_at_eof()
{
	if(!nbytes) return false;
	char * P = page0 + nbytes - 1;
	if( *P == ASCII_LF ) return true; // most commonly expected
	if( *P == ASCII_NUL ) {
		WARN("expected LF got NUL at eot");
		return true; // allow STR0 // PARSER must check too
	}
	return FAIL("expected LF at eot");
}

#include <string.h>

/*!
	test -
*/
int mmap_file::test1( void )
{
	int t;
	t=map_in_file( "/tmp/fh1", TRUE ); // -BUG- EINVAL
//	t=map_in_file( "/tmp/fh1", FALSE );
	mmap_remap();
	if(t!=0) return t;
	grow_file( 600 );
	t = write( 1, page0, nbytes );
	if(t!=nbytes) {
	}
	strcpy( page0, "-HELLO-" );
	return 0;
}

