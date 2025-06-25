
#include <sys/stat.h>
#include "map_file.h"

map_file::map_file( void )
{
	fd = -1;
	writable = FALSE;
	fd_size = 0;
	page0 = (char *) 0;
}
map_file::~map_file()
{
	unmap();
}

void	map_file::unmap( void )
{
	if( ! page0 ) return;
	sync();
	munmap( page0, (u32)fd_size );
	page0 = (char *) 0;
}

int map_file::sync( void )
{
	if( fd < 0 ) return 0;	/* file wasn't open */
	if( 0 == fsync( fd ) )
		return 0;	/* done */
	perror( "SYNC" );	/* maybe unsyncable fs ? */
	return errno;
}

int map_file::map_in_file( const u8 * ufilename, int wwritable )
{
	char * filename = (char *) ufilename;
	writable = wwritable;
	int mod ;
	if( writable )
	{
		mod = O_RDWR;
//		mod = O_RDONLY; -good- EACCESS from mmap
	} else {
		mod = O_RDONLY;
	}
	if( fd >= 0 )
	{
		unmap();
		close( fd );
	}
	fd = open( filename, mod );
	if( fd < 0)
	{
		perror( filename );
		return errno;
	}
	return remap();
}

int map_file::remap( void )	/* eg after grow file */
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
//		PROT = PROT_READ;

	} else {
		PROT = PROT_READ;
		FLAGS = MAP_FILE | MAP_SHARED;
	}

	struct stat st;
	if( fstat( fd, &st ) != 0 )
	{
		perror( "map_file::remap:stat_failed:");
		return errno;
	}
	if( st.st_size == 0 )
	{
		dgb_fmt( "zero length file (fd=%d)\n",fd);
		return 1;
	}
	if( st.st_size > (63*1024) ) // limited by u16 fd_size // WHY ??
	{
		dgb_fmt( "max filelength 63 K - %d too much\n",st.st_size );
		return 1;
	}
	fd_size=(u16)st.st_size;

	caddr_t P ;
		P = (caddr_t)mmap( SYS_PICK_LOCN, fd_size, PROT, FLAGS, fd, OFFSET0 );
	if( P == (caddr_t) -1 )
	{
		perror("MMAP");
		return errno;
	}
	page0 = P;
	return 0; /* PASS_ZER0 */
}
/* ---------------- */

int map_file::grow_file( u16 new_size )
{
	if( new_size < fd_size )
	{
		dgb_fmt( "grow_file( %d ) already %d \n",new_size, fd_size );
		return 1;
	}
	if( new_size == fd_size )
	{
		dgb_fmt( "grow_file( %d ) already %d -OK- \n",new_size, fd_size );
		return 0;
	}
	unmap();
// allow unix ext2 to fill sparse file with missing pages or NULL in last page
// seek and write last byte
	if(-1 == lseek( fd, new_size-1, SEEK_SET ))
	{
		perror("LSEEK");
		return errno;
	}
	if(1!= write( fd, "+", 1 ))
	{
		dgb_fmt("if not writable gives BAD HANDLE ");
		perror("WRITE");
		return errno;
	}
	return remap();
}

int map_file::test1( void )
{
	int t;
	t=map_in_file( "/tmp/fh1", TRUE ); // -BUG- EINVAL
//	t=map_in_file( "/tmp/fh1", FALSE );
	remap();
	if(t!=0) return t;
	grow_file( 600 );
	write( 1, page0, fd_size );
	strcpy( page0, "-HELLO-" );
	return 0;
}
