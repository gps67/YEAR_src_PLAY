#ifndef fork_exec_H
#define fork_exec_H
#include "obj_ref.h"
#include "obj_hold.h"

#include "fd_sel_line.h"
#include "buffer2.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "fork_exec_parent.h"

/*!
	fd_pipe_ends holds the 2 ends of a pipe. read and write.

	It is possible to SEND an fd (over another local fd),
	but the usual usage is to fork a child process.

	You normally pick a derived type, either of

		pipe_to_child
		pipe_from_child
	
	the default CTOR will init -1, call that.open_pipe(), and fork()
	
	on the child, call that.as_child()
	on the parent call that.as_parent()

	you could then use that.fd_here
	but to make it STDIN or STDOUT or STDERR call that.dup2(2)

	that.dup2_close(fd2) is that.dup2(fd2) then close of the pre-dup2-fd
	It puts the new correct fd in fd_here, in-case your code uses that

	The main job fd_pipe_ends does, is to know which end of the pipe
	is which, and wrap the call to pipe()

	SEE: dgb_fork_stderr_to_tcl_text() in dgb.cxx

TODO

	I like fd_pipe_ends, but everything else near here is convoluted
	and needs to be reworked. EG ntping::exec_child()

	rename _t






	fd_pipe_triple uses fd_pipe_ends as a base class for pipe_to_child,
	pipe_from_child, etc. They use fd_here as the resulting fd
*/
struct fd_pipe_ends : public GRP_lib_base
{
	int fd_here;	//!< for when its known that this is parent/child, rd/wr
	int fd_write;	//!< write side of pipe
	int fd_read;	//!< read side of pipe

	//! constructor - init to -1
	fd_pipe_ends()
	{
		fd_here = -1;
		fd_read = -1;
		fd_write = -1;
	}

	//! open a new pipe (both ends)
	bool open_pipe()
	{
		int fds[2];
		if( pipe( &fds[0] ) == -1 )
		{
			perror("pipe(fds[2]) syscall failed\n");
			e_print("pipe(fds[2]) syscall failed %m\n");
			FAIL("pipe(fds[2]) syscall failed");
			return false;
		}
		fd_read = fds[0];
		fd_write = fds[1];
		return true;
	}

	//! Can be used to make fd_here into stdin (fd012)
	bool dup2( int fd012 )
	{
		if( fd_here == -1 )
		{
			return FAIL("fd_here == -1 // call open_pipe and as_child");
			return false;
		}
		if( -1 == ::dup2( fd_here, fd012 ) )
		{
			perror( "dup2_close" );
			return false;
		}
		return true;
	}

	//! make fd_here into stdin (fd012) and close old fd number (beware of flush)
	bool dup2_close( int fd012 )
	{
		if( fd_here == -1 )
		{
			return false;
		}
		if( !dup2( fd012 ) ) return false;
		::close( fd_here );
		fd_here = fd012;
		return true;
	}

};

/*!
	This probably becomes the childs stdin
*/
struct pipe_to_child : public fd_pipe_ends
{
	void as_parent()
	{
		fd_here = fd_write;
		close( fd_read );
		fd_read = -1;
	}
	void as_child()
	{
		fd_here = fd_read;
		close( fd_write );
		fd_write = -1;
	}
};

/*!
	This probably becomes the childs stdout
*/
struct pipe_from_child : public fd_pipe_ends
{
	void as_parent()
	{
		fd_here = fd_read;
		close( fd_write );
		fd_write = -1;
	}
	void as_child()
	{
		fd_here = fd_write;
		close( fd_read );
		fd_read = -1;
	}
};

struct fd_pipe_triple_settings : public GRP_lib_base
{
	//! what happens to stderr
	// typedef 
	enum e_stderr_action {
		stderr_piped,	
		stderr_stdout,
		stderr_as_was,
		stderr_closed
	};

	/*
		these settings can be set after constructor
	*/

	e_stderr_action stderr_action;	//!< what happens to stderr
 	bool dup_to_012;		//!< child uses stdin, stdout, stderr
 	bool tty_on_012;		//!< child wants a TTY
 	bool close_3_plus;		//!< child shouldnt see lots of fds
 //	bool child_is_not_subfunction;		//!< not an exec() ...

	//! constructor - defaults for when child is grep (not tty) with parent reading stderr
	fd_pipe_triple_settings()
	: stderr_action( stderr_piped )	// parent reads errout separately
	, dup_to_012( true )		// child process uses stdin, stdout, stderr
	, tty_on_012( false )		// child doesnt need TTY emulation
 	, close_3_plus( false ) 	// closing everything takes time
	{
	}
};

/*!
	When fork()ing, you need a fd_pipe_triple to hold the fd's
	and process them according to the settings in "bool tty_on_012" etc
*/
struct fd_pipe_triple : public fd_pipe_triple_settings
{
	// these are temp used across the fork bridge
	pipe_to_child   pipe_parent_to_child;
	pipe_from_child pipe_child_to_parent;
	pipe_from_child pipe_child_to_stderr;


	//! constructor - set defaults for grep
	fd_pipe_triple()
	: fd_pipe_triple_settings()
	, pipe_parent_to_child()	
	, pipe_child_to_parent()
	, pipe_child_to_stderr()
	{
	}

	/*!
		open three pipes (before fork)
	*/
	bool open_pipes()
	{
		if( !pipe_parent_to_child.open_pipe() ) return false;
		if( !pipe_child_to_parent.open_pipe() ) return false;

		if( stderr_action == stderr_piped )
			if( !pipe_child_to_stderr.open_pipe() ) return false;
		return true;
	}

	/*!
		after fork, setup as parent
	*/
	void as_parent()
	{
		pipe_parent_to_child.as_parent();
		pipe_child_to_parent.as_parent();

		if( stderr_action == stderr_piped )
		{
			pipe_child_to_stderr.as_parent();
		}
	}

	/*!
		after fork, setup as child
	*/
	void as_child()
	{
		pipe_parent_to_child.as_child();
		pipe_child_to_parent.as_child();

		if( stderr_action == stderr_piped )
		{
			pipe_child_to_stderr.as_child();
		}
	}

	/*!
		install as stdin, stdout and stderr

		as_child() has already been done

		stderr has options
	*/
	bool child_install_std()
	{
		bool ok = true;
		if(! pipe_parent_to_child.dup2_close( 0 ) ) ok = false;
		if(! pipe_child_to_parent.dup2_close( 1 ) ) ok = false;

		switch( stderr_action ) {
		 case stderr_piped:
			if(! pipe_child_to_stderr.dup2_close( 2 ) ) ok = false;
			break;
		 case stderr_stdout:
			if( -1 == ::dup2( 1, 2 ) )
			{
				perror( "dup2" );
				ok = false;
			}
			break;
		 case stderr_as_was:
			fprintf(stderr,"CHILD install_std \n");
			fflush(0);
			break;
		 case stderr_closed:
			if(! close(2) ) ok = false;
			break;
		}

		if(!ok)
		{
			fprintf(stderr,"CHILD install_std NOT OK \n");
			fflush(0);
			close(0);
			close(1);
			close(2);
		}

		return ok;
	}

};

/*!
	fork_exec_parent is the real owner, but the functionality
	has been factored off into this class which does SETUP 
*/
class fork_exec_setup : public obj_ref
{
 public:
	//! The parent object that owns the child, and will get events
	obj_hold<fork_exec_parent> fork_parent;

	//! Open pipes and hold the fds over the fork()
	fd_pipe_triple link;

	//! consructor
	fork_exec_setup( fork_exec_parent * fork_parent_ );

	//! fork child, setup fd's in parent
	bool setup();

};

#endif
