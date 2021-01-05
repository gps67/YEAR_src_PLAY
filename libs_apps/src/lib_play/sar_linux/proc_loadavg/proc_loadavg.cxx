//#include "tbl_collect.h"
#include "glib.h"

// open ...
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

class proc_loadavg : public GRP_lib_play
{
 public:
	static const cstr filename;
	// 0.05 0.09 0.13 1/131 2265
	static const cstr ptn1;
	float f1;
	float f2;
	float f3;
	int i1;
	int i2;
	int i3;

	bool read_file()
	{
		int fd = open( filename, O_RDONLY );
		int ll = 180;
		char buff[ll];
		int t = read( fd, buff, ll );
		close( fd ); // would be later in multi line - use a class
		if( t<1 ) return FALSE; // LURK fd
		if( t>=ll ) return FALSE; // LURK fd
		int nfield = sscanf( buff, ptn1, &f1, &f2, &f3, &i1, &i2, &i3 );
		if( nfield != 6 ) {
			return FALSE;
		}
		g_print( ptn1, f1, f2, f3, i1, i2, i3 );
		g_print( " # From %s\n", filename );
		return TRUE;
	}
};

	const cstr proc_loadavg::filename = "/proc/loadavg";
	const cstr proc_loadavg::ptn1 = "%f %f %f %d/%d %d";

main()
{
	proc_loadavg test1;
	for( int i=0; i<1000; i++ )
	{
		test1.read_file();
		sleep(1);
		sleep(4); // total 5
	}
}
