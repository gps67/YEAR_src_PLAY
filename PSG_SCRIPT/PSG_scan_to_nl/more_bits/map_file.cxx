
#include <sys/stat.h>
#include "e_print.h"
#include "map_file.h"

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
map_file::map_file( void )
{
	fd = -1;
	writable = FALSE;
	fd_size = 0;
	page0 = (char *) 0;
}

/*!
	destructor - release file and mem
*/
map_file::~map_file()
{
	close();
}

/*!
	unmap() releases the mmap file
*/
void	map_file::unmap( void )
{
	if( ! page0 ) return;
	sync();
	munmap( page0, fd_size );
	page0 = (char *) 0;
}

/*!
	fsync(fd) the mapped file
*/
int map_file::fsync( void )
{
	if( fd < 0 ) return 0;	/* file wasn't open */
	if( 0 == ::fsync( fd ) )
		return 0;	/* done */
	return errno;
}

/*!
	mmap in ALL of filename, +- writable
*/
int map_file::map_in_file( const u8 * ufilename, int wwritable )
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
		unmap();
		::close( fd );
	}
	fd = open( filename, mod );
	if( fd < 0)
	{
		e_print( "## ERROR ## open(%s) %m (PCT M)\n", filename );
		return errno;
	}
	return remap();
}

/*!
	unmap and re-mmap the entire file (eg after grow file)

	fails for zero size files
*/
int map_file::remap( void )
{

#define SYS_PICK_LOCN 0
#define OFFSET0 0

	unmap();

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
	fd_size=st.st_size;

	caddr_t P = (caddr_t) mmap(
		SYS_PICK_LOCN,
		fd_size,
		PROT,
		FLAGS,
		fd,
		OFFSET0
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
	Extend the file to new_size with hole pages and NULL bytes. remap()

	Works by seek-end-of-file, and write a '+' byte, then remap.

	This cannot shrink the file.
*/
int map_file::grow_file( int new_size )
{
	if( new_size < fd_size )
	{
		e_print( "# ERROR # grow_file( %d ) cannot shrink from %d \n",
			new_size,
			fd_size
		);
		return 1;
	}
	if( new_size == fd_size )
	{
		e_print( "# INFO # grow_file( %d ) already %d \n",
			new_size,
			fd_size
		);
		return 0;
	}
	unmap();
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
	remap();
	return 0;
}

bool map_file::close( void )
{
	unmap();
	::close(fd);
	return true;
}

#include <string.h>

/*!
	test -
*/
int map_file::test1( void )
{
	int t;
	t=map_in_file( "/tmp/fh1", TRUE ); // -BUG- EINVAL
//	t=map_in_file( "/tmp/fh1", FALSE );
	remap();
	if(t!=0) return t;
	grow_file( 600 );
	t = write( 1, page0, fd_size );
	if(t!=fd_size) {
	}
	strcpy( page0, "-HELLO-" );
	return 0;
}

