//#include "tbl_collect.h"
#include "glib.h"

// read ... sleep
#include "sleep_ms.h"

#ifdef WIN32
int main_stats_sysinfo()
{
	return 0;
}
#else

// open ...
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
// sscanf
#include <stdio.h>

struct GRP_lib_play {};

typedef const char * cstr;

class test_group_base : public GRP_lib_play
{
 public:
	virtual bool fetch() = 0;
	virtual bool send() = 0;
	virtual ~test_group_base() {}
};

//SEE: /usr/src/linux/fs/proc/proc_misc.c

class call_sysinfo : public test_group_base
{
 public:
	struct sysinfo info;

	virtual ~call_sysinfo() {}

	float M( int val )
	{
		long double d = val;
		d *= info.mem_unit;
		d /= (1024*1024);
		return (float) d; 
	}

	float G( int val )
	{
		long double d = val;
		d *= info.mem_unit;
		d /= (1024*1024*1024);
		return (float) d; 
	}

	float F( int val )
	{
		long double d = val;
		d /= (64 *1024);
		return (float) d;
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

	bool fetch()
	{
		if(-1==sysinfo( &info )) return FALSE;
		return true;
	}

	bool send()
	{
		// g_print( "%d %d %d %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %d %d %d %d \n",
		// g_print( "%d %d %d %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f %d %d %d %d \n",
		g_print( "%5.3f %5.3f %5.3f %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f %d %ld %ld %d \n",

			F( info.loads[0] ),
			F( info.loads[1] ),
			F( info.loads[2] ),
			M( info.totalram ),
			M( info.freeram ),
			M( info.sharedram ),
			M( info.bufferram ),
			M( info.totalswap ),
			M( info.freeswap ),
			info.procs,
			info.totalhigh,
			info.freehigh,
			info.mem_unit

		);
		return TRUE;
	}

	bool read_file()
	{
		if( ! fetch() ) return false;
		if( ! send() ) return false;
		return TRUE;
	}
};


int main_stats_sysinfo()
{
	call_sysinfo test1;
	test1.header();
	
	for( int i=0; i<1000; i++ )
	{
		test1.read_file();
		sleep_secs(5);
	}
	return 0;
}
#endif
