#include "dgb.h"
#include <sys/utsname.h>

struct utsname uts_name;
int main()
{
	int t = uname( & uts_name );
	if( t ) {
		FAIL("uname()");
		return errno;
	}
	INFO("sysname    -s %s", uts_name.sysname );
	INFO("nodename   -n %s", uts_name.nodename );
	INFO("release    -r %s", uts_name.release );
	INFO("version    -v %s", uts_name.version );
	INFO("machine    -m %s", uts_name.machine );
	INFO("domainname XX %s", uts_name.domainname );
	return 0;
}

/*

(gdb) p uts_name
$2 = {sysname = "Linux", '\000' <repeats 59 times>,
  nodename = "E6320", '\000' <repeats 59 times>,
  release = "5.8.0-59-generic", '\000' <repeats 48 times>,
  version = "#66~20.04.1-Ubuntu SMP Thu Jun 17 11:14:10 UTC 2021", '\000' <repeats 13 times>, machine = "x86_64", '\000' <repeats 58 times>,
  domainname = "(none)", '\000' <repeats 58 times>}

	sysname = "Linux",
	nodename = "E6320",
	release = "5.8.0-59-generic",
	version = "#66~20.04.1-Ubuntu SMP Thu Jun 17 11:14:10 UTC 2021",
	machine = "x86_64",
	domainname = "(none)",

 uname -i -p -m # all return x86_64
 -p == processor, which may vary
 -i == hardware platform !!
 -m == machine hardware name

 so it must be -m 

*/

