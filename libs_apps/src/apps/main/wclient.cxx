
// #include <stdio.h>
// #include <memory.h>
#include <errno.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <netdb.h>
#include "sleep_ms.h" // sleep
// #include <stdlib.h> // atoi
// #include <stdarg.h> // va_list
// #include <fcntl.h> // O_NONBLOCK
// #include <sys/ioctl.h>

#include <glib.h> 

#include "obj_ref.h"
#include "obj_hold.h"

// static int active_count = 0;
static GMainLoop * main_loop;

#include "sock_addr_ip.h"
#include "sock_addr_unix.h"
#include "fd_sel_http.h"
#include "http_hdrs.h"
#include "buffer2.h"
#include "str_builder.h"

#include "w_widget0.h"
#include "gtk_text_top_app.h"

// #include "ir_client.h"
#include "sock_server.h"

#include "fd_sel_ftp_ctrl.h"
// TEST FAST // #include "yahoo_history_parse.h"
extern int run_YH_parse( str0 tag, str0 file );

#define AF_UNIX_ONE "/tmp/AF_UNIX_ONE.sock"

// EXT // #include "ntping.h"

///// util ///////

/*
	bool bind_to_device( str0 eth0 )
	{
		// no this didnt work
		if( 0 == setsockopt(
			sd.fd_get(),
			SOL_SOCKET,
			SO_BINDTODEVICE,
			eth0,
			strlen(eth0)
		)) return true;
		e_print( "bind_to_device(%s)", eth0 );
		return false;
	}
*/


/*!
	a connected session, for HTTP
*/
class http_serv_sess : public fd_sel_http
{
 public:
	http_hdrs_enq hdrs_in;
	http_hdrs_ack hdrs_out;
	buffer2 resp;
	buffer2 text;

	http_serv_sess( int _fd )
	: fd_sel_http( _fd )
	, resp( 2 )
	, text( 4 )
/*
	, resp( 2000 )
	, text( 4000 )
*/
	{
		set_non_block();
		subscribe_each();
	}
	void got_head_line1( str0 buff, int len )
	{
		hdrs_in.got_req_line( buff );
	}
	void http_attr( str0 attr, int l1, str0 val, int l2 )
	{
		hdrs_in.add( attr, val );
	}
	void send_headers_and_body_and_close(){
		set_block();
		hdrs_out.content_length = text.nbytes_used;
		hdrs_out.gen_headers( resp );
		text.trailing_nul();
		write( resp );
		write( text );
		flush();
		unsubscribe(); // does ref_decr
	}
	void got_head_emptyline()
	{
		hdrs_out.etag = "\"1234\"";
		hdrs_out.http_ver_1_0 = true;
		hdrs_out.set_content_type_text_html();
		hdrs_out.resp_code.set_200_ok();

		text.put("<HTML><HEAD><TITLE>wclient</TITLE></HEAD><BODY>\n");
		text.put("<PRE>\n");
		text.put("The response headers: (Content-Length: not-yet-set)\n\n");
		hdrs_out.content_length = 999;
		hdrs_out.content_length = -1;
		hdrs_out.gen_headers( text );
		text.put("</PRE>\n");
		text.put("</BODY></HTML>\n");

		// hdrs_out.resp_code.set_500_internal_error();

		send_headers_and_body_and_close();
		
	}
};

// ############################################################################

class sock_server_tcp_for_8282 : public sock_server_tcp
{
 public:
	virtual void fork_session_ip( /*const*/ sock_addr_ip & spec, int fd2 )
	{
		if(1) e_print("Connection from %s:%d\n",
			(STR0) spec.get_addr_number(),
			spec.get_port_number()
		);
		// http_serv_sess * sess =
		new http_serv_sess( fd2 );
		
	}
};

void run_server ()
{
	static obj_hold<sock_server_tcp_for_8282> srv;
	srv = new sock_server_tcp_for_8282();
	// srv->ref_incr();	// static
	srv->restart( 8282 );
}

class sock_server_http_for_AF_UNIX : public sock_server_unix
{
 public:
	virtual void fork_session_unix( /*const*/ sock_addr_UNIX & spec, int fd2 )
	{
		if(1) e_print("Connection from %s\n",
			(STR0) spec.get_pathname()
		);
		// http_serv_sess * sess =
		new http_serv_sess( fd2 );
		
	}
};

void run_server_unix ()
{
	static obj_hold<sock_server_http_for_AF_UNIX> srv;
	srv = new sock_server_http_for_AF_UNIX();
	// srv->ref_incr();	// static
	srv->restart( AF_UNIX_ONE );
}

class soak_random : public fd_sel_http
{
 public:
	str0 hostname;
	str0 filename;
	int id;
	int port;
	sock_addr_ip spec;

	~soak_random()
	{
		printf("~soak_random()\n");
	}
	soak_random()
	{

		hostname = "postcards.rin.ru";
		port = 80;
		hostname = "127.0.0.1";
		port = 8080;

		spec.set_port_number(port);
		spec.set_addr_name( hostname );
		e_print( "Call to address: %s\n", (STR0) spec.get_addr_number() );

#ifdef WIN32
		srand(time(0));
#else
		srandom(time(0));
#endif
		restart();

	}
//	virtual void got_line( str0 line, int len ){}
//	virtual void got_data( void * data, int len ){}

	void eof_input()
	{
		fd_selectable_base::call_eof_input();
		// restart();
		if( eof_seen ) return;
		eof_seen = true;
		flush();
		got_eof();
		close();
		unsubscribe();

		// restart(); // must be after close
	}

	void got_eof()
	{
	}
	void restart()
	{
	//	printf("restart ");
		printf(".");
		sleep_secs(1);
		// done // close();
		open_TCP( spec );
		subscribe_each(); // to select

		filename = "/cgi-bin/get.pl?n=34userid=";
		filename = "/?id=";
		filename = "/web_play/session_login_test.php";
		// id=random();
		// print("GET %s%d HTTP/1.0\r\n", (STR0) filename, id );
		print("GET %s HTTP/1.0\r\n", (STR0) filename );
		print("Host: %s\r\n", (STR0) hostname );
		print( "\r\n" );
		flush();
		// sd->set_block(); 
		fflush(0);
	}
};

void run_soaker()
{
	static obj_hold<soak_random> r1 = new soak_random(); sleep_secs(1);
//	static obj_hold<soak_random> r2 = new soak_random(); sleep_secs(1);
//	static obj_hold<soak_random> r3 = new soak_random(); sleep_secs(1);
//	static obj_hold<soak_random> r4 = new soak_random(); sleep_secs(1);
}

void run_client ()
{
	str0 hostname = "localhost";
	str0 filename = "/";
	int port = 80;

	hostname = "trix";
	hostname = "sky";
	hostname = "127.0.0.1";
	filename = "/absent";
	filename = "/found";
	filename = "/table.csv";
	filename = "/web_play/session_login_test.php";
	port = 8181;
	port = 8282;
	port = 8080;

	sock_addr_ip spec;
	spec.set_port_number(port);
	spec.set_addr_name( hostname );
	e_print( "Call to address: %s\n", (STR0) spec.get_addr_number() );

	// s.bind_to_device("eth0");	// illegal seek
	// s.bind_to_device("lo");		// Invalid argument

	/*
		A minimal request,
		LURK: in non_block mode - no handling of vfprintf blocked
	*/
	static obj_hold<fd_sel_http> sd;
	sd = new fd_sel_http();
//	sd->ref_incr();
	sd->set_non_block();
	// s.connect();
	// LATER - write buffereing required // sd->open_TCP_async( spec );
	sd->open_TCP( spec );
	sd->print("GET %s HTTP/1.0\r\n", (STR0) filename );
	sd->print("Host: %s\r\n", (STR0) hostname );
	sd->print( "\r\n" );
	sd->flush();
	// sd->set_block(); 
	sd->subscribe_each(); // to select
}

void run_client_unix ()
{
	str0 hostname = "localhost";
	str0 filename = "/";
	int port = 80;

	hostname = "trix";
	hostname = "sky";
	hostname = "127.0.0.1";
	filename = "/absent";
	filename = "/found";
	filename = "/table.csv";
	filename = "/web_play/session_login_test.php";
	port = 8181;
	port = 8282;
	port = 8080;

	sock_addr_UNIX spec;
	spec.set_pathname( AF_UNIX_ONE );
	e_print( "Call to address: %s\n", (STR0) spec.get_pathname() );

	// s.bind_to_device("eth0");	// illegal seek
	// s.bind_to_device("lo");		// Invalid argument

	/*
		A minimal request,
		LURK: in non_block mode - no handling of vfprintf blocked
	*/
	static obj_hold<fd_sel_http> sd;
	sd = new fd_sel_http();
//	sd->ref_incr();
	sd->set_non_block();
	// s.connect();
	// LATER - write buffereing required // sd->open_TCP_async( spec );
///	sd->open_TCP( spec );
	sd->open_AF_UNIX_async( AF_UNIX_ONE );
	sd->print("GET %s HTTP/1.0\r\n", (STR0) filename );
	sd->print("Host: %s\r\n", (STR0) hostname );
	sd->print( "\r\n" );
	sd->flush();
	// sd->set_block(); 
	sd->subscribe_each(); // to select
}

void run_udp_cat()
{
	static obj_hold<sock_server_udp> srv;
	srv = new sock_server_udp();
	srv->restart( 1514 );
}

extern void run_ntping();

int O_main ()
{
	GMainContext* CTXT = g_main_context_new();
	main_loop = g_main_loop_new( CTXT, FALSE );                                           

	if(1) run_server();
	if(1) run_client();
	if(1) run_udp_cat();
 
	g_main_run( main_loop );
	return 0;
}


int run_IR()
{
#if 1
	return 1;
	// IR comes as part of lib_media which doesnt run on WIN32
	// but wclient should run on WIN32
#else
	static obj_hold<IR_client> cli;
	cli = new IR_client( "/dev/lircd" );
	cli->test1();
	return 0;
#endif
}

int run_FTP()
{
	static obj_hold<fd_sel_ftp_ctrl> cli;
	cli = new fd_sel_ftp_ctrl();
	cli->test1();
	return 0;
}

/*
extern int run_YH_parse( str0 tag, str0 file );
{
	yahoo_history_parse parser;
	return parser.parse( tag, file );
}
*/


int main(int argc, char ** argv )
{
	{
		TOPAPP_Holder_gtk holder(&argc, &argv);

		char * filename = NULL;
		str0 command = "client";

		// PARSE command name
			// DONE
		argv++;
		argc--;

		while( argc > 0 )
		{
			if( argv[0][0] == '-' )
			{
				str0 option = argv[0];
				e_print( "ignoring option %s\n", (STR0) option );
				argv++;
				argc--;
				continue;
			}
			command = argv[0];
			argv++;
			argc--;
			break;
		}

// TODO: provide a top level window for each - or not!
//		new gtk_text_top_app( & holder, NULL /*filename*/ );
		e_print_show();

		if( false ) {}
		else if( command == "client" )	run_client();
		else if( command == "client_unix" )	run_client_unix();
		else if( command == "server" )	run_server();
		else if( command == "server_unix" )	run_server_unix();
		else if( command == "soaker" )	run_soaker();
		else if( command == "IR" )	run_IR();
		else if( command == "FRP" )	run_FTP();
		else if( command == "udp_cat" )	run_udp_cat();
#ifndef WIN32
		else if( command == "ntping" )	run_ntping();
#endif
		else if( command == "YH_parse" )
		{
			str0 tag="FTSC";
			str0 file="/home/gps/2002_src/PLAY/yahoo/FTSC_d_93.csv";
			if( argc == 2 )
			{
				tag = argv[0];
				file =  argv[1];
			}
			run_YH_parse( tag, file );
		}

		sleep_secs(2);
		gtk_main();
	}
	ref_tally.print_leaked_report();
	return 0;
}

/* EOF - cli.cpp */
// DOXYGEN Main Page

/*!

*/
