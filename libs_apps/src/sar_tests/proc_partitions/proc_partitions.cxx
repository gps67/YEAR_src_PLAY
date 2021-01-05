#include "e_print.h"
#include "str1.h"
#include "buffer2.h"

// #include "glib.h"

// open ...
#include <fcntl.h>
// read ... sleep
#include <unistd.h>
#include "sleep_ms.h"
// sscanf
#include <stdio.h>

// struct GRP_lib_play {};

typedef const char * cstr;

class test_group_base : public GRP_lib_play
{
 public:
	virtual bool fetch() = 0;
	virtual bool send() = 0;
	virtual ~test_group_base() {}
};

//SEE: /usr/src/linux/fs/proc/proc_misc.c

#define K16 (1024*16)

/*!
	read + store (but not parse) /proc/partitions (but format might vary)

	speed is needed for the repeated IO stats
	(not the occasional config reading)

	The figres are sent to ... the screen

# LINE1:
major minor  #blocks  name     rio rmerge rsect ruse wio wmerge wsect wuse running use aveq
# LINE*:
  22     1    4194760 hdc1 17071 0 17071 19090 0 0 0 0 0 19090 19090

# LINE1:
major minor  #blocks  name

# LINE2: BLANK

# LINE*:
   3     2    8393962 hda2


*/
class read_partitions : public test_group_base
{
	str1 filename;
	buffer2 buff;
	static const int min_size = 2;	// any staring value ?
	static const int max_size = K16;	// /proc/data (not /proc/mem)
 public:
	read_partitions()
	: filename()
	{
		buff.get_space( min_size );
		filename = "/proc/partitions";
	}

	virtual ~read_partitions()
	{
	}

	void header()
	{
	 e_print( "%s\n",
		"read_partitions::header()"
	);
	}

	int open_readonly()
	{
		int fd = open( (STR0)filename, O_RDONLY );
		if( fd < 0 ) {
			e_print( "# ERROR # open_READONLY() # %s - %m\n",
				(STR0) filename
			);
		}
		return fd;
	}

	bool fetch_text()
	{
		int fd;
		buff.clear();
		if(!buff.trailing_nul()) return false;
		fd = open_readonly();
		if( fd < 0 ) {
			return false;
		}
		while(1) { // grow buffer to fill
			if( !buff.get_space( 10 )) {
				// reported
				return false;
			}
			int N = buff.space_avail() - 1;
			int t = read( fd, buff.gap_addr(), N );
			if( t < 0 ) {
				perror( (STR0)filename );
				return false;
			}
			buff.nbytes_used += t;
			buff.trailing_nul();
			// if( t == 0 ) { /* OK but (almost) never happens on blocking */ }
			if( t < N )  { break; } // EOF 

			// loop again with a bigger buffer
			// this is normal but set vals to avoid
			e_print( "# INFO # multi-read growing buffer %d\n", N );
		}
		close( fd );
		return true;
	}

	bool fetch()
	{
		if( !fetch_text() ) return false;
		return true;
	}

	bool send()
	{
		write( 1, buff.buff, buff.nbytes_used );
		return true;
	}

	bool read_file()
	{
		if( ! fetch() ) return false;
		if( ! send() ) return false;
		return true;
	}
};


int main_proc_partitions()
{
	read_partitions test1;
	test1.header();
	
	for( int i=0; i<1000; i++ )
	{
		test1.read_file();
		sleep_secs(5);
	}
	return 0;
}
