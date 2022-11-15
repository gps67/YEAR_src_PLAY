
#include "tty_ptmx.h"

#include <unistd.h>	// fork, close, dup2 ...
// //#include <sys/types.h>
// #include <sys/wait.h>	// wait()
#include <sys/ioctl.h>	// TIOCSCTTY
// #include <fcntl.h>	// O_RDWR
// #include <stdio.h>	// sprintf
// #include <stdlib.h>     // system
// #include <errno.h>      // errno

#include "dgb.h" // FAIL

using namespace BASE1;

#define letter1 "pqrstuvwxyz"
#define letter16 "0123456789abcdef"

/*!
	This is a specific UNIX dance to create /dev/tty99

	The use case, is the parent feeds IN->child->OUT 

	With some other options, the child process
	gets fd { 0 1 2 }
	is_a_tty(fd)
	IS_A_STREAM(fd_0,fd_1))
	ERROUT_IS_TEE_LOGFILE_TEE_FILTER_to_DEST
	ERROUT_is-parents_fd2 # buffer STDERR points to same
	ERROUT_is_FILTER_EVENTS_TELL_LINE
	ERROUT_is_ONE_WAY_API # no return signal # check_world_patch #
	ERROUT_is_CODE_POINT_CONNECTING_TO_BENCH_RUNNING_ACTION_QUERY
	ERROUT_is_dup_STDOUT # same log file filter action cache

	ONE_WAY_FILTER_api
		SEND OPCODE ARGS
	ONE_WAY_FILTER_MACRO_PATTERNS_api
		SET VAR VALUE
		SET VAR2 VALUE2
		SET VALUE3 [EVAL ARGS]

		ARGS == EXPR {

			SET VAR VALUE {
				SUBLEX_CODE_POINT {
				 SUBLEX_CODE_POINT {
					LEX "SET" {
					 ALIAS_lower
					 MATCH_adjacent // UpperLowerMix
					 SUBLEX // SYL-A-BLE S
					}
				 }
				 SUBLEX_CODE_POINT {
					LEX "%s" VAR_name {
					MACRO VAR_name VAR.name
					 LEX "%s" ITEM(NAME) 
					  %s == NAME == "VAR"
					}
				 }
				 SUBLEX_CODE_POINT {
					LEX "%s" VALUE_name {
						"EXPR"
					}
					LEX "%s" VALUE_name {
						"VALUE"
					}
				 }
				}
			}
		}

		EIGHT_t u64_WORD [ u48_idx ] // N == "N"
		// DIALECT { N == "N" } AUTO_VAR NAME = "cident99"
	
	SET("cident99") at CODE_POINT 
	 detect well known "cident99"
	 MATCH "cident99" { += well_known_dialect_noun }
	 MATCH "cident99" // consider { cident99 SCRIPT } {
	  MATCH
	 	{
			{ cident99 SCRIPT } {
			// HERE //
			}
		}
	  MATCH
	 	{
			HERE {
			// HERE //
			}
		}

	  MATCH
	 	{
			Permit_Paradox LOOPY_NAME
			// LOOPY += SELF += EXPR SELF OTHER as ARGS
			// The provider of NAME_t starts with AUTO_VAR
			// all of these OPTIONS to PICK are STR0
			// PARSED SCRIPT // { 
			// HERE // resolves to ITEM // tokenised_promise
			// DECL { CXX { HERE_t HERE } }
			// excessive { SUB_ITEM } or { PAIR }
			// POSSIBLE MATCH { TUPLO }
			// POSSIBLE MATCH { TUPLO CODE_POINT CSR STO }}
			// TOKENISED MATCH { ANYSTR }
			// IDX TOKEN_t TOKEN // CODE_POINT TOKEN PLUS
			// CODE_POINT is the PLUS as SESS and OBJV
			// CXX has same available STACK GLOBALS
			// CXX has API "TOKEN_from_ANYSTR"
			// but still cident99 // VIEW as "x%X"
			// { FMT "x%X" } # MATCH idx_ITEM %X u48_idx
			// u48_idx // or 4 bit nybble // UPTO u48
			// implies have used u16_lo_of_u64
			// USING CPU REGISTER_BYTE_A
			// USING CPU REGISTER_BYTE_B
			// USING CPU REGISTER_BYTES_BA
			// obviously it was "ABCD" in MEM
			// now seen in u16_hilo loaded from _lohi
			// u16 // u16_cpu_byte_order // u16_lohi
			// u16_AB // u16_loaded_from_AB
			// u16_BA // u16_loaded_from_AB // VIEW_BA
			// VIEW_BA is a name of a VIEW mode
			// SET %s VIEW_BA // KNOW // HINT //
			// }
		}
	 }

*/
bool tty_ptmx_t::open_pair ()
{
	const char * name_master = "/dev/ptmx";
	fd_master = open( name_master, O_RDWR );
	if(fd_master == -1) {
		FAIL("open %s", name_master);
		return false;
	}

	if( 0!=ptsname_r( fd_master, name_slave, sizeof(name_slave) )) {
		return FAIL("ptsname()");
		return false;
	}

	INFO( "# ptsname says '%s'", name_slave );

	if( 0!=grantpt( fd_master )) {
		return FAIL("grantpt()");
		return false;
	}

	if( 0!=unlockpt( fd_master )) {
		return FAIL("unlockpt()");
		return false;
	}

	fd_slave = open( name_slave, O_RDWR );
	if(fd_slave == -1) {
		return FAIL( "OPEN %s", name_slave );
		close( fd_master );
		return false;
	}
	int fd_test = open("/dev/null", O_RDWR);
	INFO("opened PIPE %d TTY %d NULL %d", fd_master, fd_slave, fd_test );
	close(fd_test);
	return true;
}

int tty_ptmx_t::fork_vtty_old()
{
	// deprecated for libs_app // kept for luck
	// redo // where fork() is OUTSIDE, many other things ALSO going on
	// pre_fork, post_fork_as_child, post_fork_as_parent, ...
	// distill these

	pid = -1;
	if( !open_pair() ) return -1;
	pid = fork();
	switch (pid) {
	case -1:
		FAIL("fork"); // probably no processes table full
		return -1;
	case 0:
		// child process
	//	setsid(); // so that /dev/tty works and signals etc
		// setsid = no need - should get parents /dev/tty
		// man 7 credentials

		// fd_slave possibly closed on exec
		fd_slave = open( name_slave, O_RDWR ); // for setsid 
		if( fd_slave == -1 ) {
			FAIL("open TTY '%s'", name_slave );
			exit( errno );
		}
		close( fd_master );	
	        ioctl (fd_slave, TIOCSCTTY, 0); // man tty_ioctl
		dup2 (fd_slave, 0);
		dup2 (fd_slave, 1);
		dup2 (fd_slave, 2);
		stty_saner( 0 );
		close( fd_slave );	
		
		return 0;
	default:
		// parent process
		close( fd_slave );	
		return pid;
	}
}

bool tty_ptmx_t::stty_saner( int fd )
{

	return true;
	return PASS("TODO");
	return WARN("TODO");
}

bool BASE1::is_a_tty( int fd )
{
	if( isatty( fd ) ) return true;
	switch( errno ) {
	 case ENOTTY:
	 	errno = 0;
	 	return false;
	 case EBADF:
	 	FAIL("fd %d - not open - %m", fd );
	 	errno = 0;
		return false;
	}
	return FAIL("fd %d - unexpected errno %d -  %m", fd, errno );
}
bool BASE1:: not_a_tty( int fd ) { return !is_a_tty(fd); }

bool BASE1:: need_fd_as_a_tty( int fd )
{
	if( is_a_tty( fd ) ) {
		return PASS("fd %d already a TTY", fd );
	}
	const char * dev_tty = "/dev/tty";
	int flags = O_RDWR; // supposed to be fd_
	int fd_dev_tty = open( dev_tty, flags );
	if(-1 == fd_dev_tty )
		return FAIL("open('/dev/tty')");
	if( dup2( fd_dev_tty, fd ) == -1 ) {
		return FAIL("dup2");
	}
	if( close( fd_dev_tty ) == -1 ) {
		FAIL("close");
	}
	return true;
}
