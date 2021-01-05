//#include "tbl_collect.h"
#include "glib.h"

// open ...
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// read ... sleep
#include <unistd.h>
// sscanf
#include <stdio.h>

struct GRP_lib_play {};

typedef const char * cstr;

//SEE: /usr/src/linux/fs/proc/proc_misc.c

class call_sysinfo : public GRP_lib_play
{
 public:
	struct sysinfo info;

	// when these took (int val) they lost unsigned war
	// compiler converts input field to double, sorted
	// RETVAL double too

	double M( double val )
	{
		double d = val;
		d *= info.mem_unit;
		d /= (1024*1024);
		return (double) d; 
	}

	double G( double val )
	{
		double d = val;
		d *= info.mem_unit;
		d /= (1024*1024*1024);
		return (double) d; 
	}

	double F( double val )
	{
		double d = val;
		d /= (64 *1024);
		return (double) d;
	}

	void header()
	{

	 g_print( "%s\n",
		"loads[0]"
		", "
		"loads[1]"
		", "
		"loads[2]"
		", "
		"totalram"
		", "
		"freeram"
		", "
		"sharedram"
		", "
		"bufferram"
		", "
		"totalswap"
		", "
		"freeswap"
		", "
		"procs"
		", "
		"totalhigh"
		", "
		"freehigh"
		", "
		"mem_unit"
	);
	}

	bool read_file()
	{
		if(-1==sysinfo( &info )) return FALSE;

		// g_print( "%d %d %d %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %d %d %d %d \n",
		// g_print( "%d %d %d %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f %d %d %d %d \n",
		g_print( "%5.3f %5.3f %5.3f %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f %d %d %d %d \n",

			F( info.loads[0] ),
			F( info.loads[1] ),
			F( info.loads[2] ),
			M( info.totalram ),
			M( info.freeram ),
			M( info.sharedram ),
			M( info.bufferram ),
			M( info.totalswap ),
			M( info.freeswap ),
			(int)info.procs,
			(int)info.totalhigh,
			(int)info.freehigh,
			(int)info.mem_unit

		);
		return TRUE;
	}
};


main()
{
	call_sysinfo test1;
	test1.header();
	
	for( int i=0; i<1000; i++ )
	{
		test1.read_file();
		sleep(5);
	}
}
