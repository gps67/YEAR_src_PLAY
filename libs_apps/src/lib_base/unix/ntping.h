#ifndef ntping_H
#define ntping_H

#include "fork_exec_parent.h"

class ntping : public fork_exec_parent
{
 public:
	ntping();

 virtual
	bool exec_child();
 virtual
	bool test1();
 virtual
	void got_eof_1();
 virtual
	fd_selectable_base * mk_fd_kbd( int fd );
 virtual
	fd_selectable_base * mk_fd_out( int fd );
 virtual
	fd_selectable_base * mk_fd_err( int fd );

};

extern void run_ntping();

#endif
