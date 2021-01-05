#ifndef fd_sel_ftp_data_H
#define fd_sel_ftp_data_H

#include "sock_server.h"
#include "fd_sel_line.h"
#include "buffer2.h"
#include "obj_hold.h"

class fd_sel_ftp_ctrl;

/*!
*/
class fd_sel_ftp_data : public fd_sel_line
{
 public:
	obj_hold<fd_sel_ftp_ctrl> ctrl;	// tell when done


	virtual ~fd_sel_ftp_data();

	fd_sel_ftp_data();

// VIRTUAL - from fd_sel_line
	void got_line( str0 line, int len );
	void got_data( void * buff, int len );
	void got_eof();
	void got_error();

	void test1();

// exported virtual 

} ;

#endif


/*!
	The LISTENING ftp data port has to go through the
	standard server listening sequence, and shutdown on the
	first connect (?). That then spawns this fd ..

	AGAIN:

	FTP does have an option where the server can provide its port,
	but its easier to presume 22 (firewalls).

	So the DATA connection has to be a different PORT each time,
	and a ONE-OFF sock_server_tcp used each time (with a queue of 1).
	The DATA_SERVER shuts down on the first connect, leaving the
	ESTABLISHED connection

	FURTHER

	Since we want the C++ class of the connection to be specified by
	the calling code, it will have to create a connection-half with
	an INVALID fd (-1) then we can instanciate it with the fd - 
	OR we run a callback when we have an fd, which the caller
	converts into an fd_sel_XXX pointer 
	(This is for progress reports, where to put the received data, etc)
*/

class fd_sel_ftp_data_listener : public sock_server_tcp
{
 public:
	// one or a list of ...
	obj_hold<fd_sel_ftp_ctrl> controller;
	obj_hold<fd_sel_ftp_data> fd_connected;

	~fd_sel_ftp_data_listener();
	fd_sel_ftp_data_listener( fd_sel_ftp_ctrl * _controller );
	void fork_session_ip( sock_addr_ip & spec, int fd2 );

	str0 ftp_port_str();
};
