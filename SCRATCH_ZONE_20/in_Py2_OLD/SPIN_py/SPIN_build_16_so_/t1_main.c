extern void test_link();
#include <Python.h>
                                                                                
int
main(int argc, char *argv[])
{
	const char * cmd1 =
		"from time import time,ctime\n"
		"print 'Today is',ctime(time())\n";
	const char * cmd2 =
		"import sys\n"
		"print 'path is',sys.path\n";

	const char * cmd = argv[1];
	if(!cmd)
		cmd = cmd2;
	Py_Initialize();
	PyRun_SimpleString( cmd1 );
	PyRun_SimpleString( cmd );
	Py_Finalize();
	exit(0);
	test_link();
	return 0;
}
