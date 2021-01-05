#include "fd_sel_ftp_ctrl.h"

#include "ctype.h"
#include "sock_addr_ip.h"
#include "sleep_ms.h"

#include "fd_sel_ftp_data.h"

/*
	telnet localhost 21
	220 sky FTP server (Version wu-2.6.1(1) Tue Oct 3 14:29:19 CEST 2000) ready.

	USER gps
	331 Password required for gps.

	PASS PaSsWoRd
	230 User gps logged in.

-else-
	530 Please login with USER and PASS.

	PWD
	257 "/home/gps" is current directory.

	CWD sdf
	550 sdf: No such file or directory.

	CWD tmp
	250 CWD command successful.


	421 Timeout (900 seconds): closing control connection.

*/


/*!
*/
fd_sel_ftp_ctrl::fd_sel_ftp_ctrl( int _fd )
: fd_sel_line( _fd )
, text( 200 )
{
	state = W_INIT;
	data_listener = new fd_sel_ftp_data_listener( this );
	sleep_secs(2);
}

fd_sel_ftp_ctrl::~fd_sel_ftp_ctrl()
{
}

/*!
*/
void fd_sel_ftp_ctrl::got_line( str0 line, int len )
{
	g_print("# FTP # got_line(%s)\n", (STR0) line );
	switch(state)
	{
		default: ;
	};
	if(
	   ( len >= 4 ) 
	&& isdigit(line[0])
	&& isdigit(line[1])
	&& isdigit(line[2])
	&& !isdigit(line[3])
	)
	{
		bool is_dash = false;
		if(line[3]=='-')
		{
			is_dash = true;
			text.put( & line[4] );
			state = W_MID_RESPONSE;
			return;
		}
		 else if(line[3]==' ')
		{
			is_dash = false;
		}
		 else 
		{
			g_print("# FTP # ERROR expected 123-\n" );
			text.put( line );
			return;
		}
		if(is_dash)
		{
		}
		line[3] = 0;
	}
	 else
	{
		text.put( line );
		return;
	}
}

/*!
*/
void fd_sel_ftp_ctrl::got_data( void * buff, int len )
{
}

/*!
*/
void fd_sel_ftp_ctrl::got_eof()
{
}

/*!
*/
void fd_sel_ftp_ctrl::got_error()
{
}

/*!
*/
void fd_sel_ftp_ctrl::select_says_connected()
{
	str0 user = "test1";
	str0 pass = "pass2";
	user="anonymous";
	pass="Info@";
	print("USER %s\r\n", (STR0) user );
	print("PASS %s\r\n", (STR0) pass );
	print("CWD tmp\r\n");
	print("NOOP\r\n");

	sock_addr_ip addr_here;
	// own name or remote name - own name 
	if(!addr_here.get_socket_name_IP( fd_get() )) return;
	str0 addr_str = addr_here.ftp_addr_str();
	e_print("# FTP # addr_str %s\n", (STR0) addr_str );

	if(!data_listener) return;
	if(!data_listener->expect_open()) return;
	if(!addr_here.get_socket_name_IP( data_listener->fd_get() )) return;
	str0 port_str = addr_here.ftp_port_str();
	e_print("# FTP # port_str %s\n", (STR0) port_str );

	if( !port_str[0] )
	{
		e_print("# TEST1 # no port\n");
	}
	 else
	{
		print("PORT %s,%s\r\n", (STR0) addr_str, (STR0) port_str );
	}
	print("LIST\r\n");
	print("QUIT\r\n");
	flush();
}

void fd_sel_ftp_ctrl::test1()
{
	str0 remote = "localhost";
	// remote = "ftp.python.org";
	g_print("ftp::test1()\n");
	sock_addr_ip addr;
	addr.set_port_number( 21 );
	addr.set_addr_name( remote );
	if(!open_TCP_async( addr ) ) return;
	subscribe_each();
	// done by subscribe_each() // set_non_block();
}


