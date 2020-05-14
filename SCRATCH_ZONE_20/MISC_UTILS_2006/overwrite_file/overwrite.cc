#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

typedef long long u64;
typedef long long u48;
typedef u48 OFFS_t; // 
typedef char * STR0;

/*!
	a buffer aligned at 4K page

	glibc provides an alignment facility,
	but this is here

	overwrite may or may not be better with this, but
	(1) the copy from user to kernel is better at page
	(2) the kernel MIGHT use MMU to fork on change
	(3) the data is in one page (not straddled badly)

	NB preallocate throws away data, fills with NUL bytes

*/
struct aligned_buffer
{
	void * page0;
	void * allocated;

	~aligned_buffer()
	{
		deallocate();
	}

	aligned_buffer()
	: allocated( 0 )
	{
	}

	aligned_buffer( int align, int sz )
	: allocated( 0 )
	{
		preallocate( align, sz );
	}

	void deallocate()
	{
		if( allocated ) {
			free(allocated);
			allocated = NULL;
		}
	}

	bool preallocate( int align, int sz )
	{
		deallocate();
		// over allocate by an entire block
		allocated = malloc( sz + align );
		if(!allocated) {
			perror( "malloc" );
			return false;
		}
		// buffer starts life a NUL - can omit if supplier says ZERO
		memset( allocated, 0, sz + align );

		// treat PTR as INT by converting it to OFFS 
		OFFS_t p0 = (OFFS_t) allocated;
		OFFS_t p0_furthest = p0 + align - 1 ;
		int excess = p0_furthest % align; // remainder after div // overshoot
		p0 = p0 - excess;
		page0 = (void *) p0;
		if(1) printf("aligned_buffer::preallocate( align=0x%4.4X, sz=0x%x )\n\t alloc=%p \n\t    p0=%p\n",
			align, sz, allocated, page0 
		);
		return true;
	}

};

struct progress_tracker
{
	off_t max;
	off_t pos;
	progress_tracker( off_t _max )
	{
		max = _max;
		pos = 0;
	}
	void progress( off_t done )
	{
		pos += done;
		float percent = 100.0 * pos / max;
		printf("\rWriting at file pos 0x%lx %4.1f%% ... ", pos, percent );
		fflush(stdout); // fsync(0);
	}
};

int main( int argc, char ** argv )
{
	int t;

	if( argc != 2 ) {
		printf( "usage: %s filename\n", argv[0] );
		return 22;
	}
	const char * filename = "splatter.ded" ;
	filename = argv[1];

	aligned_buffer buf;

	struct stat st;
	t = stat( filename, & st );
	if( t == -1 ) {
		perror( filename );
		exit( errno );
	}

	int fd = open( filename, O_RDWR );
	int iosz = st.st_blksize * 32;		// 16K - 64K - 
	if( ! buf.preallocate( 4096, iosz ) ) return 1;

	progress_tracker prog( st.st_size );
	prog.progress( 0 );

	off_t remaining = st.st_size; 
	while( remaining > 0 )
	{
		if( remaining < iosz )
		{
			iosz = st.st_blksize; // overshoot upto block boundry
		//	iosz = remaining;     // retain filesize, but read-tail?
		}
		t = write( fd, buf.page0, iosz );
		if( t == -1 ) {
			perror( "write" );
			exit( errno );
		}
		fdatasync(fd); // overkill, it will get flushed
		prog.progress( t );
		remaining -= t;
	}
	fsync(fd); 
	printf("DONE\n");
}
