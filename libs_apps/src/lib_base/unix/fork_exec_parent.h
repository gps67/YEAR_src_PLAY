#ifndef fork_exec_parent_H
#define fork_exec_parent_H

#include "obj_hold.h"
#include "fd_selectable.h"

/*!
	After doing the fork and exec and XXX, this is left.

	A class derived from this, provides the virtual functions
	that customise the behavior of the fd handlers.

	see ntping::test1 for usage
*/
class fork_exec_parent : public obj_ref
{
 public:
	int pid_child;

	obj_hold<fd_selectable_base> fd_kbd;
	obj_hold<fd_selectable_base> fd_out;
	obj_hold<fd_selectable_base> fd_err;

	fork_exec_parent()
	: pid_child( -1 )
	{
	}

 virtual
	bool exec_child() = 0;
 virtual
	bool test1() = 0;
 virtual
	void got_eof_1() = 0;
 virtual
	fd_selectable_base * mk_fd_kbd( int fd ) = 0;
 virtual
	fd_selectable_base * mk_fd_out( int fd ) = 0;
 virtual
	fd_selectable_base * mk_fd_err( int fd ) = 0;

};

#endif

