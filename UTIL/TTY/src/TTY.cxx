#include "dgb.h"
#include "tty_ptmx.h"
#include "str0.h"
#include <unistd.h> // execve
#include <sys/wait.h> // wait

// TODO // ecit with childs exit code

using namespace BASE1;

class tty_wrap_t { public:
	tty_ptmx_t tty_ptmx;

};

bool bool_main( int argc , char ** argv, char ** envp )
{
	int arg_i = 1; // skip arg[0]
	str0 tee_file;
	while( arg_i < argc ) { // or break
		str0 arg = argv[arg_i];

		if( arg == "--tee_a" ) {
			arg_i++;
			tee_file = argv[arg_i];
			if( arg_i >= argc ) {
				return FAIL("--tee_a filename # missing filename");
			}
			arg_i++;
			PASS("--tee_a '%s'", (STR0) tee_file );
			continue;
		}

		if( arg == "--tee" ) {
			arg_i++;
			tee_file = argv[arg_i];
			if( arg_i >= argc ) {
				return FAIL("--tee filename # missing filename");
			}
			arg_i++;
			PASS("--tee '%s'", (STR0) tee_file );
			continue;
		}

		if( arg.starts_with("-") ) {
			return FAIL("opt '%s' not recognised", (STR0) arg );
		}

		break; // should leave argv
	}
	if( arg_i >= argc ) {
		return FAIL("absent CMD ARGS");
	}

	tty_wrap_t tty_wrap;
	int pid_child = tty_wrap.tty_ptmx.fork_vtty();
	if( pid_child == -1 ) return FAIL_FAILED();
	if( pid_child == 0 ) { // CHILD
		INFO("calling execve '%s' ...", argv[arg_i] );
		// tty_wrap already done dup2(0|1|2)
		int t = execvpe( argv[arg_i], argv+arg_i, envp );
		if( t == -1 ) {
			return FAIL("execve '%s' ...", argv[arg_i] );
		}
		return FAIL("THIS exec SHOULD NEVER RETURN");
	}
	// PARENT //
	// READ tty_wrap.tty_ptmx.fd_master // copy OUT to OUT
	// EOF tty_wrap.tty_ptmx.fd_master // look at vwait
	// READ STDIN - copy IN
	// WRITE tty_wrap.tty_ptmx.fd_master // copy IN to IN

	sleep(1);

	int exit_child = 0;
	int options = WNOHANG;
	u8 buff[1024];
	int n = read( tty_wrap.tty_ptmx.fd_master, buff, 1024 );
	write( 1, buff, n );
	waitpid( pid_child, &exit_child, options );
	INFO("waitpid exit_child %d", exit_child );




	return PASS("OK");
	return true;
}

int main( int argc , char ** argv, char ** envp )
{
	// without ANY need, ld optimised out base1.so
	INFO("HERE");
	if( bool_main( argc, argv, envp ) ) 
		return 0;
	else {
		if(errno) {
			FAIL_FAILED();
			return errno;
		}
		WARN("FAILED without errno");
		return 1;
	}
}
