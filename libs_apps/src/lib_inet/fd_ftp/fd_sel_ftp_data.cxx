#include "fd_sel_ftp_data.h"
#include "fd_sel_ftp_ctrl.h"

#include "fd_sel_line.h"
#include "buffer2.h"

class fd_sel_ftp_ctrl;

/*!
	destructor - default
*/
fd_sel_ftp_data::~fd_sel_ftp_data()
{
}

/*!
	constructor - null
*/
fd_sel_ftp_data::fd_sel_ftp_data()
: fd_sel_line(-1)
{
}

// VIRTUAL - from fd_sel_line
void fd_sel_ftp_data::got_line( str0 line, int len )
{
	g_print(" # FTP DATA # LINE # %s\n", (STR0) line );
}
/*!
*/
void fd_sel_ftp_data::got_data( void * buff, int len )
{
	g_print(" # FTP DATA # DATA # %s\n", (STR0) buff );
}
/*!
*/
void fd_sel_ftp_data::got_eof()
{
	g_print(" # FTP DATA # EOF #\n" );
}
/*!
*/
void fd_sel_ftp_data::got_error()
{
	g_print(" # FTP DATA # ERROR #\n" );
}


///////////




/*!
*/
fd_sel_ftp_data_listener::~fd_sel_ftp_data_listener()
{
}


/*!
*/
fd_sel_ftp_data_listener::fd_sel_ftp_data_listener( fd_sel_ftp_ctrl * _controller )
: sock_server_tcp()
, fd_connected()
{
	int port = 0;
	controller = _controller;

	create_socket_tcp();
	// if you expect all connections to arrive here
	if(!setsockopt_bool( SO_REUSEADDR, true )) {
		// continue
	}

	if(0)
		bind_any( port );
	else
		bind_lo( port );
	int backlog = 1;
	listen( backlog );
	set_non_block();
	subscribe_each();
}

// select_says_read()
// do_accept_form()
/*!
*/
void fd_sel_ftp_data_listener::fork_session_ip( sock_addr_ip & spec, int fd2 )
{
	close(); // the listener
	fd_connected->fd_set_socket( fd2 );
	fd_connected->set_non_block();
	fd_connected->subscribe_each();
}

/*!
*/
str0 fd_sel_ftp_data_listener::ftp_port_str()
{
	// static char buff[ 8*4+50 ];
	// buff[0] = 0;
	if(!fd_connected) return "";
	sock_addr_ip addr;
	if(!addr.get_socket_name_IP( fd_get() )) return "";
	return addr.ftp_port_str();
}
