#include <errno.h>

#include <sys/stat.h>
#include "STO/mmap_file.h"

#include "buffer1.h" // INFO builders

using namespace STO;

mmap_file::mmap_file( void )
{
	// BASE // fd = -1;
	writable = FALSE;
	fd_size = 0;
	page0 = (char *) 0;
}
mmap_file::~mmap_file()
{
	unmap();
}

bool	mmap_file::unmap( void )
{
	sync();
	if( !is_open() ) {
		return true;
		WARN("not open");
	}
	if( !page0 ) {
		return true;
		WARN("no page0");
	}
	if( ! page0 ) return true; // already unmapped
	if(-1 == munmap( page0, (u32)fd_size )) {
		return PASS("munmap");
	}
	page0 = (char *) 0;
	return true;
}

#if 0 // BASE 
bool mmap_file::sync( void )
{
	if( fd < 0 ) return true;	/* file wasn't open */
	if( 0 == fsync( fd ) )
		return true;	/* done */
	
	return FAIL( "SYNC" );	/* maybe unsyncable fs ? */
}
#endif

bool mmap_file:: map_in_file( const u8 * ufilename, bool wwritable )
{
	if( is_open() )
	{
		unmap();
		close(); // close( fd );
	}

	const char * filename = (const char *) ufilename;
	writable = wwritable;
	int mod ;
	if( writable )
	{
		mod = O_RDWR;
//		mod = O_RDONLY; // test // -good- EACCESS from mmap
	} else {
		mod = O_RDONLY;
	}

 // DO NOT CREATE FILE though
 // flag + mod

	bool async = false; // maybe
	if( writable ) {
	 if(! open_RW( filename, async ))
	  return FAIL_FAILED();
	} else {
	 if(! open_RO( filename, async ))
	  return FAIL_FAILED();
	}

	INFO("FD %d FILE %s", fd, filename );

	if(! remap()) return FAIL_FAILED();;
	return true;
}

#include "file_stat.h"

bool mmap_file:: remap( void )	/* eg after grow file */
{
	INFO("CALL(...)");

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

	file_stat st;
	// struct stat st;
	if( !st.stat_fd( fd ) != 0 )
		return FAIL_FAILED();

	if( st.st.st_size == 0 )
	{
		return FAIL( "zero length file (fd=%d)\n",fd);
	}
	if( st.st.st_size > (63*1024) ) // limited by u16 fd_size // WHY ??
	{
		int szk = (int)( st.st.st_size >> 10);
		WARN( "max filelength 63 K - %ld is too much\n", szk );
	}
	fd_size=(u32)st.st.st_size;

	caddr_t P ;
		P = (caddr_t)mmap( SYS_PICK_LOCN, fd_size, PROT, FLAGS, fd, OFFSET0 );
	if( P == (caddr_t) -1 )
	{
		return FAIL("M1==-1");
	}
	page0 = P;
	return true; /* PASS_ZER0 */
}
/* ---------------- */

bool mmap_file::grow_file_16( u16 new_size )
{
	return grow_file_32( (u32) new_size );
}
bool mmap_file::grow_file_32( u32 new_size )
{
	if( new_size < fd_size )
	{
		return FAIL( "grow_file( %d ) already %d \n",new_size, fd_size );
	}
	if( new_size == fd_size )
	{
		return PASS( "grow_file( %d ) already %d -OK- \n",new_size, fd_size );
	}
	unmap();
// allow unix ext2 to fill sparse file with missing pages or NULL in last page
// seek and write last byte
	if(!seek_SET_32( new_size - 1 ))
	 return FAIL_FAILED();

#if 1
	const char * LAST_BYTE_STR = "+"; // visible
#else
	const char * LAST_BYTE_STR = ""; // a NUL byte
#endif
	if(!write( LAST_BYTE_STR, 1 )) // SHOW 
	 return FAIL_FAILED();

	if(!remap()) return FAIL_FAILED();;
	return true;
}

bool system_od_file ( const char * filename )
{
	buffer1 out;
	out.print( "set -x; od -c -t x1 '%s'", filename );
	INFO( "system(%s)", (STR0) out ); 
	system( (STR0) out );
	return true;
}

extern "C" char * strcpy( char *, const char * );
bool mmap_file::test1( void )
{
	bool t = true; // OK
	const char * filename = "/tmp/fh1";

	buffer1 cmd1;
	cmd1.clear();

	cmd1.print( "date > '%s'", filename );
	INFO( (STR0) cmd1 );
	system( (STR0) cmd1 );
	cmd1.clear();

// nearest phrase
//	t=map_in_file( "/tmp/fh1", FALSE );
//
	if(!mmap_in_file_RW( filename ))
		return FAIL_FAILED(); // 

// DATA DUMP
	system_od_file ( filename );

	if(!remap()) return FAIL_FAILED();;
	system_od_file ( filename );

	if(!t) return FAIL_FAILED();
	grow_file_16( 600 );
	if(0) strcpy( page0+30, "-HELLO_VIA_MMAP-" );
	sync();
	system_od_file ( filename );
	return true;
}
