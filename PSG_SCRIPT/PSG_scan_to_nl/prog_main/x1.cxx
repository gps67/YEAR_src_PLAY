
#include "scan_xml_1.h"
#include "dgb.h"

int main(int argc, char ** argv_)
{
	u8 ** argv = (u8**) argv_;

	// fork a wish script: tcl_text.tcl
	// and redirect this (parents) STDERR to it
	// switch off when done

	if(1)  dgb_fork_stderr_to_tcl_text();

	if(argc!=2)
	{
		printf("USAGE: %s filename\n",argv[0]);
//		exit(1);
		static const char * argv_default[] = {
			"ARGV0",
			"../test/test3.xml"
		};
		argv = (u8**) argv_default;
		argc = 2;
	}
	scan_xml_1 f1;
	if( f1.scan_file( argv[1]) ) {
		return 0;
	} else
		return 1;
}
/* -eof- */
