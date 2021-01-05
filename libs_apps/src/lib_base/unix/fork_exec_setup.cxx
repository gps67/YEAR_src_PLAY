
#include "fork_exec_setup.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h> // fprintf(stderr, "" ); // used for brief time - e_print in child ??

#include "obj_hold.h"

/*!
	constructor 
*/
fork_exec_setup::fork_exec_setup( fork_exec_parent * fork_parent_ )
: fork_parent( fork_parent_ )
, link()
{
}

/*!
	open pipes, call fork(), call fork_exec_parent->mk_fd_kbd(...)
*/
bool fork_exec_setup::setup()
{
	bool ok = true;
	if(! link.open_pipes() ) return false;
	fflush(0); // dont fork messages in buffers
	fork_parent->pid_child = fork();
	if( fork_parent->pid_child == -1 )
	{
		e_print("ALERT fork() failed %m\n");
		return false;
	}
	if( fork_parent->pid_child == 0 )
	{
		fprintf(stderr,"CHILD from fork\n");
		fflush(0);
		link.as_child();
		if( link.dup_to_012 ) link.child_install_std();
 		if( link.close_3_plus )
		{
			for( int f = 3; f<22; f++ )
			{
				close( f );
			}
		}
		fork_parent->exec_child();
		_exit( errno );
		return false;
	}
	// PARENT
	link.as_parent();

	/*
		WARNING: ref_count of XXX may be zero
		TODO: create a list of that is cleared every idle
	*/

	fork_parent->fd_kbd = fork_parent->mk_fd_kbd( link.pipe_parent_to_child.fd_here );
	if( fork_parent->fd_kbd )
		fork_parent->fd_kbd -> subscribe_out( );
	else
		ok = false;

	fork_parent->fd_out = fork_parent->mk_fd_out( link.pipe_child_to_parent.fd_here );
	if( fork_parent->fd_out )
		fork_parent->fd_out -> subscribe_in();
	else
		ok = false;

	if( link.stderr_action == fd_pipe_triple::stderr_piped )
	{
		fork_parent->fd_err = fork_parent->mk_fd_err( link.pipe_child_to_stderr.fd_here );
		if( fork_parent->fd_err )
			fork_parent->fd_err -> subscribe_in();
		else
			ok = false;
	}

	return ok;
}


