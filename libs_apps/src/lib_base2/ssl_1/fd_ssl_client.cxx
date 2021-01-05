#include "fd_ssl_client.h"
#include <openssl/ssl.h>
#include "dgb.h"

// ------------------------


/*!
*/
int fd_ssl_client::test1_Main( SSL_global_base & SSL )
{
	errno_zero();
	debug_set_obj_name("fd_ssl_client::test1");
	int ret=1;

	// connect 
	const char * host="localhost";
	host="192.168.96.147";
	host="cloud147.kyzo.co.uk";
	const char * port = "4433";
	sock_addr_ip addr;
	addr.set_addr_name( host );
	addr.set_port_number( port );
	// open_TCP( addr, true ); // async = true
	open_TCP_async( addr ); // async = true = non_blocking 

	if( !build_bio_client( SSL ) ) return -1;

	if(0) out_buff.put( "GET /test1_Main HTTP/1.0\r\n\r\n" );

	errno_zero();
	return(ret);
}

void fd_ssl_client::ssl_says_connected()
{
	ssl_write_some();
}

/*!
*/
// static
void fd_ssl_client::test1( SSL_global_base & SSL )
{
	errno_zero();
	STEP("Setting up a new client");
	// this doesnt make sense as the client is never deleted, BUT
	static obj_hold<fd_ssl_client> ssl_client;
	ssl_client = new fd_ssl_client();
	ssl_client -> test1_Main(SSL);
	ssl_client -> subscribe_each();
#warning this should be in fd_says_connected in base class
	errno_zero();
}




// virtual 
void fd_ssl_client::avail_space()
{
	STEP("You may write more and maybe unblock upstream");
}

// virtual 
void fd_ssl_client::avail_data()
{
	STEP("You should read as much as possible or else maybe unblock upstream");
	uchar * pos = in_buff.circ_addr();
	int len = in_buff.circ_len();
	fwrite( (const char *) pos, 1, len ,stdout);
	in_buff.circ_used( len );
}

fd_ssl_client::fd_ssl_client()
: fd_sel_ssl( true ) // I am client
{
	STEP("CTOR");
}

fd_ssl_client::~fd_ssl_client()
{
	STEP("DTOR");
}
