
#include "audacity_mod_script_pipe.h"
#include "buffer1.h"
#include <unistd.h>
// #include <sys/types.h>
#include "dgb.h"


/*
	There is one FILE_NAME_PART "audacity_script_pipe"
	There is one FILE_NAME_PART "1016" // SAMPLE VALUE 
	There is pair of FILE_NAME_PART "to" "from"
	add above to get ...
	There is PAIR of FILE_NAME

		/tmp/audacity_script_pipe.to.1016 
		/tmp/audacity_script_pipe.from.1016 
	
	# NB PAIR is AVAR "one" is not OPTION switch with PICK CAPS "ONE"
	# dgb_DIAG_VIEW_of_AVAR # SCRIPT over AVAR 


./audacity_mod_script_pipe_test.elf t1_arg1
# INFO # int main(int, char**) # ARGV[0] './audacity_mod_script_pipe_test.elf'
# INFO # int main(int, char**) # ARGV[1] 't1_arg1'
# INFO # bool audacity_mod_script_pipe::set_filename_PAIR_to_default() # filename_READ = /tmp/audacity_script_pipe.from.1016
# INFO # bool audacity_mod_script_pipe::set_filename_PAIR_to_default() # filename_WRITE = /tmp/audacity_script_pipe.to.1016
# INFO # virtual bool audacity_mod_script_pipe::open_pipe() # opening /tmp/audacity_script_pipe.to.1016 ...
# ---- # CLEAR() # UNIX errno(err 2) == ENOENT - No such file or directory ## 
# FAIL # bool fd_hold_1::open_RW(str0, bool) # open /tmp/audacity_script_pipe.to.1016 2d
# FAIL # virtual bool audacity_mod_script_pipe::open_pipe() # FAILED
# FAIL # bool bool_main() # FAILED

	This default must be the very specific correct answer, or FAIL

		uses euid_t unix_user_id == 1016 // SAMPLE VALUE in TOKEN POOL

		any_other clever ATTR_in_NAME "/tmp/audacity_script_pipe" "." 
		...
	
	If you wanted to parametise the CTOR,
	then do that INSTEAD of doing this

*/


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
	


