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
	fd_grumble_size = 1<<31; // 2G // WARN SIGN 
	//  warning: integer overflow in expression [-Woverflow]
	// going into unsigned, via CONST in C
	fd_grumble_size = 1024 * 1024 * 1024 * 2; // 1<<31 // 2G // WARN SIGN 
	fd_grumble_size = 1024 * 200; // a lot for a .ini tree
	fd_grumble_size = 1024 * 20; // test mini files
}
mmap_file::~mmap_file()
{
	unmap();
}

bool	mmap_file::unmap( void ) // calls munmap( page0, fd_size ); // byte_size
{
	// USAGE caller should check first // or KNOW
	if( !is_open() ) {
		WARN("not open"); // log if you think that is strange
		return true;
	}

	// calls fsync(fd) // flush all pages to file to disk
	// or at least start an OS sweep // flush_modified non_blocking
	sync();

	// already unmapped
	if( ! page0 ) return true; // already unmapped
// altern code fragment // editable // both
	// sanity check // page0 == NULL
	if( !page0 ) {
		return true;
		WARN("no page0");
	}

	// unmap the FILE_page [s] from this process' MMAP
	if(-1 == munmap( page0, (u32)fd_size )) {
		return PASS("munmap");
	}

	// DO NOT ACCESS WITHIN WHERE THE PAGES USED TO BE
	page0 = (char *) 0;
	return true;
}

#if 0 // BASE 
bool mmap_file::sync( void )
{
	if( fd < 0 ) return true;	/* file wasn't open */
	if( 0 == fsync( fd ) )
		return true;	/* done */
	
	return FAIL( "fsync(fd)" );	/* maybe unsyncable fs ? */
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

 // TODO rewrite open_RW_non_BLOCKING _non // SUBLEX can subtract _non case

	bool async = false; // maybe
	if( writable ) {
	 if(! open_RW( filename, async ))
	  return FAIL_FAILED();
	} else {
	 if(! open_RO( filename, async ))
	  return FAIL_FAILED();
	}

	// useful debug, trace fd by association
	// review INFO var value tracers
	INFO("FD %d FILE %s", fd, filename );

	if(! remap()) return FAIL_FAILED();;

	// PASS was INFO
	return true;
}

#include "file_stat.h"

bool mmap_file:: remap( void )	/* eg after grow file */
{

#define SYS_PICK_LOCN 0
#define OFFSET0 0

	// if this fails
	unmap();

	int PROT;
	int FLAGS;
	str0 cmnt = "bool writable == TRUE; // CMNT";
	if( writable )
	{
		PROT = PROT_READ | PROT_WRITE; 
		FLAGS = MAP_FILE | MAP_SHARED;
//		PROT = PROT_READ;

	} else {
		cmnt = "load pages as not writable";
		cmnt = "bool writable == FALSE; // ARGS[idx] requesting '%s'; //";
		//
		PROT = PROT_READ;
		FLAGS = MAP_FILE | MAP_SHARED;
	}
	INFO("%s", (STR0) cmnt ); // the (STR0) is redundent, but clarifies

	file_stat st;
	// struct stat st;
	if( !st.stat_fd( fd ) != 0 )
		return FAIL_FAILED();

	if( st.st.st_size == 0 )
	{
		return FAIL( "zero length file (fd=%d)\n",fd);
	}
	if( st.st.st_size > fd_grumble_size ) // eg 2G 
	{
		int szk = (int)( st.st.st_size >> 10);
		WARN( "max filelength 63 K - %ld is too much\n", szk );
		// its onlw a warning, from loading
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

bool mmap_file::grow_file_k_u16( u16 new_size_k ) // u16 * 1k  // eg 4K MMAP
{
	return grow_file_32( (u32) new_size_k * 1024 ); // guaranteed to fit 
}

bool mmap_file::grow_file_16( u16 new_size ) // u16 // possible var in STO
{
	return grow_file_32( (u32) new_size ); // guaranteed to fit 
}
bool mmap_file::grow_file_32( u32 new_size )
{
	if( new_size < fd_size )
	{
		// api for truncate is elsewhere (const)
		return FAIL( "grow_file( %d ) already %d \n",new_size, fd_size );
	}
	if( new_size == fd_size )
	{
		return PASS( "grow_file( %d ) already %d -OK-",new_size, fd_size );
	}
	u32 old_size = fd_size; // that auto updates during remap //
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
	return PASS( "grow_file( %d ) from %d -OK- \n",new_size, old_size );
	return true;
}

bool system_od_file ( const char * filename ) // octal dump // replace
{
	buffer1 out;
	const char * trace_on = "set -x;"; // system shell must parse this
	const char * trace_off = ""; // not even ;
	out.print( "%s od -c -t x1 '%s'", trace_on, filename );
	// system is an awful security check
	// mostly due to ENVP, SQL injection, quoting, etc
	// when it's only me 
	INFO( "system(%s)", (STR0) out ); 
	system( (STR0) out );
	return true;
}

extern "C" char * strcpy( char *, const char * );
bool mmap_file::test1( void )
{
	const char * filename = "/tmp/fh1";
	// filename = "/home/gps/YEAR/tmp/fh1"; // NFS is OK

	bool t = true; // OK // almost unused

	buffer1 cmd1;
	cmd1.clear();

	cmd1.print( "date > '%s'", filename );
	INFO( (STR0) cmd1 );
	system( (STR0) cmd1 );
	cmd1.clear();

// nearest phrase
//	t=map_in_file( "/tmp/fh1", FALSE );
//

#if 0
	// PARAMETER casts STR0 <--> str0 WHEN cast happens
	// NOT for INFO ... varags no cast
	INFO("filename %s", (STR0) filename );
	str0 filename2 = filename;
	INFO("filename2a %s", filename2 );
	INFO("filename2 %s", (STR0) filename2 );
#endif

	if(!mmap_in_file_RW( filename ))
		return FAIL_FAILED(); // 
	// KNOW is_open() fd=...

// DATA DUMP
	system_od_file ( filename );

	if(!remap()) return FAIL_FAILED();;
	system_od_file ( filename );

	if(!t) return FAIL_FAILED();
	grow_file_16( 256 ); // YEP STOPS exactly on
	grow_file_16( 1024 * 63 ); // 
	grow_file_k_u16( 64 ); // 
	grow_file_k_u16( 64 ); // 
	if(1) strcpy( page0+64, "-HELLO_VIA_MMAP-" );
	sync();
	system_od_file ( filename );
	return true;
}
