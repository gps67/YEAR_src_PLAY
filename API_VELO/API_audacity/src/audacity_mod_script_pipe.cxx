
#include "audacity_mod_script_pipe.h"
#include "buffer1.h"
#include <unistd.h>
// #include <sys/types.h>
#include "dgb.h"



bool 
audacity_mod_script_pipe::
set_filename_PAIR_to_default() {
	buffer1 buff_filename_WRITE;
	buffer1 buff_filename_READ;
#if WIN32
	STR0 lhs = "\\\\.\\pipe\\"; 
	STR0 srv = "Srv"; // must patch PipeServer
	buff_filename_WRITE.print("%s%s%s%s", lhs, "To", srv "Pipe");
	buff_filename_READ .print("%s%s%s%s", lhs, "From", srv "Pipe");
#else
	STR0 lhs = "/tmp/audacity_script_pipe";
	buff_filename_WRITE.print("%s.%s.%d", lhs, "to"  , getuid());
	buff_filename_READ .print("%s.%s.%d", lhs, "from", getuid());
#endif
	filename_READ = buff_filename_READ;
	filename_WRITE = buff_filename_WRITE;
	INFO("filename_READ = %s", (STR0) filename_READ );
	INFO("filename_WRITE = %s", (STR0) filename_WRITE );
	return true;
}

bool 
audacity_mod_script_pipe::
close_pipe()
{
	if( fd_READ ) { fd_READ->close(); }
	if( fd_WRITE ) { fd_WRITE->close(); }
	return true;
}
	

bool 
audacity_mod_script_pipe::
open_pipe()
{
	close_pipe();
	fd_WRITE = new fd_PIPE_base();
	fd_READ = new fd_PIPE_base();

	// made a difference to open WRITE before READ
	INFO("opening %s ...", (STR0) filename_WRITE );
	if(!fd_WRITE->open_RW_sync( (STR0) filename_WRITE )) return FAIL_FAILED();
	INFO("opening %s ...", (STR0) filename_READ );
	if(!fd_READ->open_RO_sync( (STR0) filename_READ )) return FAIL_FAILED();
	PASS("opened" );

	return true;
}
	


