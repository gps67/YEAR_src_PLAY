
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <windows.h>
#else
#include <netinet/in.h> 
#include <netdb.h> 
#include <sys/ioctl.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#endif

#include <glib.h>
#include <gtk/gtk.h>

#ifdef WIN32
// somehow this is probably available via ...
extern "C" void g_io_channel_win32_set_debug (GIOChannel *channel, gboolean flag);
#endif

const char * global_web_host = "www.google.co.uk";
// const char * global_web_host = "127.0.0.1";


// these simple source "string" concatenation ("fmt")

#define ALOG_0( pfx, fmt ) fprintf(stderr, "# " pfx " # " fmt "\n" );
#define ALOG_1( pfx, fmt, A ) fprintf(stderr, "# " pfx " # " fmt "\n", A );
#define ALOG_2( pfx, fmt, A, B ) fprintf(stderr, "# " pfx " # " fmt "\n", A, B );
#define ALOG_3( pfx, fmt, A, B, C ) fprintf(stderr, "# " pfx " # " fmt "\n", A, B, C );

#define INFO( fmt )             	ALOG_0( "info",  fmt );
#define INFO( fmt, arg1 )       	ALOG_1( "info", fmt, arg1 );
#define INFO( fmt, arg1, arg2 ) 	ALOG_2( "info", fmt, arg1, arg2 );

#define PASS( fmt )			ALOG_0( "PASS",  fmt );
#define PASS( fmt, arg1 )		ALOG_1( "PASS", fmt, arg1 );
#define PASS( fmt, arg1, arg2 )	ALOG_2( "PASS", fmt, arg1, arg2 );

#define FAIL( fmt )			ALOG_0( "FAIL",  fmt );
#define FAIL( fmt, arg1 )		ALOG_1( "FAIL", fmt, arg1 );
#define FAIL( fmt, arg1, arg2 )	ALOG_2( "FAIL", fmt, arg1, arg2 );


bool perr_win32()
{
#ifdef WIN32
        // int errno_orig = errno;
        DWORD WIN32_err =  GetLastError();
	SetLastError( 0 );

        if(WIN32_err == 0 ) {
                // INFO( "# WIN32 OK #" );
		return true; // no error
	} else {

		if(WIN32_err == WSAEWOULDBLOCK ) {
			INFO( "# WIN32 _err # %s", "WSAEWOULDBLOCK" );
			// (1) This has no actual message from WIN32
			// (2) This is not an error (EINPROGRESS)
			return true;
		}

                static const unsigned int sz = 400;
                char buf[ sz ];

                DWORD dw = WIN32_err;

                DWORD dwFlags = 0
//              |       FORMAT_MESSAGE_ALLOCATE_BUFFER
                |       FORMAT_MESSAGE_FROM_SYSTEM
                |       FORMAT_MESSAGE_IGNORE_INSERTS
                ;
                LPCVOID lpSource = NULL;
                DWORD dwMessageId = dw;
                DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
                LPTSTR lpBuffer = (LPTSTR) &buf;
                DWORD nSize = sz;
                va_list *Arguments = NULL;


                int t = FormatMessage(
                        dwFlags,
                        lpSource,
                        dwMessageId,
                        dwLanguageId,
                        lpBuffer,
                        nSize,
                        Arguments
                );
		if(!t) {
                        sprintf( lpBuffer, "%s\n", "(WIN32.FormatMessage.nothing)" );
                }

                INFO( "# WIN32 ERROR # %d # %s", (int) dw, (const char *) buf );
                // NEWLINE ... varies?
                // FORMAT_MESSAGE_ALLOCATE_BUFFER // LocalFree(lpDisplayBuf);
        }


	// errno = errno_orig;
	return false; // false is the state of an error
#else
	return true; // no error
#endif
}

bool perr( const char * msg )
{
	bool ok = true;
	int e = errno;
	errno = 0;
	if( msg ) {
		ALOG_1( "STEP", "%s", msg );
	}
#ifdef WIN32
	if(!perr_win32()) ok = false;
#endif
	if(e) {
		ok = false;
		const char * msg_errno = strerror(e);
		ALOG_1( "UNIX", "%s", msg_errno );
	}

	fflush(0);
#ifdef WIN32
        DWORD WIN32_err =  GetLastError();
        if( WIN32_err ) {
		ALOG_1( "----", "FAIL within FAIL # WIN32_err = %d", WIN32_err );
		SetLastError( 0 );
	}
#endif
	if( errno ) {
		ALOG_1( "----", "FAIL within FAIL # errno = %d", errno );
	}
	errno = 0;
	return ok; // usually false, because of errno
}

void perr_clear()
{
	perr(NULL);
}

bool STEP_OK( const char * stepname, bool wasok )
{
	if(wasok) {
		PASS( "%s", stepname ); // remember "string" concat 
		perr(NULL);
	} else {
		FAIL( "%s", stepname ); // remember "string" concat 
		perr( NULL );
	}
	return wasok;
}

bool STEP_ERR( const char * stepname, bool waserror )
{
	return !STEP_OK( stepname, !waserror );
}

bool win_sock_init()
{
#ifdef WIN32
	WSADATA wsadata;
	int err = WSAStartup( MAKEWORD(2,0), &wsadata ); 
	bool ok = (err==0);
	STEP_OK( "WSAStartup( MAKEWORD(2,0), &wsadata );", ok );
	return ok;
#else
	return true;
#endif
}

bool win_sock_tidy()
{
#ifdef WIN32
	WSADATA wsadata;
	int err = WSACleanup();
	bool ok = (err==0);
	STEP_OK( "WSACleanup();", ok );
	return ok;
#else
	return true;
#endif
}


bool set_non_block( int fd )
{
	INFO( "set_non_block(fd = %d)", fd );
#ifdef WIN32

	// have to return true - otherwise called exits! 
	// return STEP_OK( "###### SKIPPING NON_BLOCK ######", true );
 
	u_long nonb = 1; // set nbio true
	int t = ioctlsocket(fd, FIONBIO, & nonb);
	if(!STEP_OK( "ioctlsocket()", t==0 )) return false;
	return true;
#else
	int flags = 0;
	int t = 0;
	t = fcntl( fd, F_GETFL, 0 );
	if(STEP_ERR( "ioctl F_GETFL", t==-1 )) return false;
	flags = t;
	INFO( "flags = 0x%x", flags );
	flags |= O_NONBLOCK;
	t = fcntl( fd, F_SETFL, flags );
	if(STEP_ERR( "ioctl F_SETFL - with O_NONBLOCK", t==-1 )) return false;
	return true;
#endif
}

#ifdef WIN32
bool addr4_set_a_b_c_d( struct sockaddr_in & sa4, const char * addr_str )
{
	unsigned long u_addr = inet_addr( addr_str );
	// report one possibly confused case
	if( u_addr == INADDR_ANY ) {
		STEP_OK( "INADDR_ANY", false );
		return false;
	}
	if(!STEP_ERR( "inet_addr() - WIN32", ( u_addr == INADDR_NONE ) )) return false;
	sa4.sin_addr.s_addr = htonl( u_addr );
	return true;
}
#else
bool addr4_set_a_b_c_d( struct sockaddr_in & sa4, const char * addr_str )
{ // 0=fail 1=pass cast to bool
	bool ok = inet_aton( addr_str, &sa4.sin_addr );
	if(!ok) {
		INFO( "'%s'", addr_str ); 
	}
	return STEP_OK( "inet_aton() - UNIX",  ok );
}
#endif

bool addr4_set_name( struct sockaddr_in & sa4, const char * addr_str )
{
	struct hostent * h = gethostbyname( addr_str );
	if(!STEP_OK( "gethostbyname()", h )) return false;
	memcpy( & sa4.sin_addr, h->h_addr_list[0], h->h_length );
	INFO( "'%s'", addr_str ); 
	return true;
}

bool addr4_set_addr_str( struct sockaddr_in & sa4, const char * addr_str )
{
	if(addr4_set_a_b_c_d( sa4, addr_str )) goto ok;
	if(addr4_set_name( sa4, addr_str )) goto ok;
 fail:
	STEP_OK( addr_str, false );
	return false;
 ok:
	STEP_OK( addr_str, true );
	return true;
}

void C_select_removal( gpointer data )
{
	const char * str = (const char *) data;
	INFO( "C_select_removal(%s)", str );
}

const char * str_GIOStatus( GIOStatus s )
{
	switch( s ) {
	 case G_IO_STATUS_ERROR:  return "G_IO_STATUS_ERROR";
	 case G_IO_STATUS_NORMAL: return "G_IO_STATUS_NORMAL";
	 case G_IO_STATUS_EOF:	 return "G_IO_STATUS_EOF";
	 case G_IO_STATUS_AGAIN:  return "G_IO_STATUS_AGAIN";
	 default: return "G_IO_STATUS__XXX_";
	}
}

const char * str_GIOCondition( GIOCondition cond )
{
	switch( cond ) {
	 case G_IO_IN:  return "G_IO_IN";
	 case G_IO_OUT: return "G_IO_OUT";
	 case G_IO_PRI: return "G_IO_PRI";
	 case G_IO_ERR: return "G_IO_ERR";
	 case G_IO_HUP: return "G_IO_HUP";
	 case G_IO_NVAL: return "G_IO_NVAL";
	default:
	 // double loop only really catches unknown single bits
	 // alternative is two functions, first calls second (doesnt call first)
	 // alternative is to cater for mutiple/single bits (notice, count them)
	 static bool double_loop = false;
	 if(double_loop) return "G_IO_double_loop";
	 double_loop = true;
	 int c_rest = cond;
	 int c_bit = 1;
	 while( c_rest ) {
	  if( cond & c_bit ) {
		INFO( "MULTIBIT %s", str_GIOCondition( (GIOCondition) c_bit ));
	  }
	  c_bit <<= 1;
	  c_rest >>= 1;
	 }
	 double_loop = false;
	 return "G_IO_xxx";
	}
};

gboolean C_select_says(
	GIOChannel *channel,
	GIOCondition condition,
	gpointer data
)
{
	INFO( "C_select_says %s", str_GIOCondition( condition) );

	char buf[2048];
	gsize n = 0;
	GError *error = NULL;

	bool please_call_again = true;

	if(condition & G_IO_IN ) {
		
		INFO("# FD # C_select_says(IN)" );
		GIOStatus status = g_io_channel_read_chars( 
			channel,
			buf,
			sizeof(buf)-2,
			&n,
			&error
		);
		INFO( "# Status from read = %s", str_GIOStatus( status ));
		if((n==0)&&(status==G_IO_STATUS_EOF)) {
			perr_clear();
			return false; // dont stay subscribed
		}
		if((n==0)&&(status==G_IO_STATUS_ERROR)) {
			INFO( "# n==0 Status = %s - why does this happen?", str_GIOStatus( status ));
			perr_clear();
			return true; // stay subscribed
			
		}
		if(n>0) {
			buf[n] = 0;
			INFO( "# READ %d # %s", (int) n,  buf );
			please_call_again = true;
		} else {
			INFO( "# READ %d #", (int) n );
		}
	};

	if(condition & G_IO_OUT ) {

		INFO("# FD # C_select_says(OUT)" );
		
		static bool seen_connect = false;
		if(!seen_connect) {
			INFO("select_says_first_connect - writing something" );
			seen_connect = true;
		
			sprintf( buf, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n",
				global_web_host );
			GIOStatus status = g_io_channel_write_chars(
				channel,
				buf,
				strlen(buf),
				&n,
				&error
			);
			g_io_channel_flush( channel, &error );
			INFO( "# Status = %s", str_GIOStatus( status ));
			if(n>0) {
				buf[n] = 0;
				PASS( "# WROTE %d # ==>>>\n%s<<==", (int) n, buf );
			} else {
				PASS( "# WROTE %d NOT # ==>>>\n%s<<==", (int) n, buf );
			}
			// in this DEMO there is only one write
			please_call_again = false;
		}
		return please_call_again;
	};
	if(condition & G_IO_PRI ) {
		INFO("# FD _ # C_select_says(PRI)" );
		STEP_OK( "OK", true );
		return false;
	}
	if(condition & G_IO_ERR ) {
#ifdef WIN32
#else
		if( errno == EINPROGRESS )
		{
			INFO("# FD _ # C_select_says(ERR) EINPROGRESS" );
			return true;	// continue with further replies
		}
#endif
		INFO("# FD _ # C_select_says(ERR)" );
		STEP_OK( "SHOULD NOW EXIT", true );
		exit(1);
		return false;
	}
	if(condition & G_IO_HUP ) {
		INFO("# FD _ # C_select_says(HUP)" );
		STEP_OK( "SHOULD NOW EXIT", true );
		exit(1);
		return false;
	}
	if(condition & G_IO_NVAL ) {
		INFO("# FD _ # C_select_says(NVAL)" );
		STEP_OK( "SHOULD NOW EXIT", true );
		exit(1);
	}
	STEP_OK( "OK", true );
	return please_call_again;
return true; // stay subscribed
}

bool add_watches( 
	GIOChannel * channel,
	GIOCondition conditions
)
{
	gpointer user_data = (gpointer) "ObjectPointer";
	int id;
	const char * str = str_GIOCondition( conditions );
	INFO( "add_watch_full ... %s", str );
	id = g_io_add_watch_full(
		channel,
		G_PRIORITY_DEFAULT,
		(GIOCondition) conditions,
		C_select_says,
		user_data,
		C_select_removal
	);
	bool ok1 = (id!=0);
	STEP_OK( "add_watch ... ALL ONE", ok1 );
}

bool mk_g_io()
{
	int fd;

	int ret1 = 0;
	bool ok = true;
	bool ok1 = true;

	fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(STEP_ERR( "socket", fd == -1 )) return false;
	INFO("socket fd = %d", fd );

	if(!STEP_OK( "set_non_block()", set_non_block(fd) )) return false;

	const char * port_str = "23";
	const char * addr_str= "127.0.0.1";

	port_str="7";
	port_str="echo";
	port_str="http";
	addr_str= "81.5.187.147";
	addr_str= "www.google.com";
	addr_str= global_web_host;

	int port = atoi( port_str );
	if(!port) {
		const char * proto = "tcp"; // not udp
		struct servent * entry = getservbyname( port_str, proto );
		if(!STEP_OK( "getservbyname()", entry )) return false;
		if(!STEP_OK( port_str, entry )) return false;
		port = ntohs( entry->s_port );
	}

	INFO( "port %d", port );

	struct sockaddr_in sa4;
	sa4.sin_family = AF_INET;
	sa4.sin_port = htons(port);
//	sa4.sin_addr.s_addr = htonl(u_addr);

	if(!STEP_OK( "addr4_set_addr_str()", addr4_set_addr_str( sa4, addr_str ))) return false;

	ret1 = ::connect( fd, (struct sockaddr*) & sa4, sizeof(sa4));
	if(!STEP_OK( "connect()", ret1==0 )) ; // stay - return false;

#ifdef WIN32
	GIOChannel * channel = g_io_channel_win32_new_socket( fd );
	if(!STEP_OK( "g_io_channel_win32_new_socket()", channel )) return false;
	// for WIN32 switch debug on
	if(1) g_io_channel_win32_set_debug (channel, true);
#else
	GIOChannel * channel = g_io_channel_unix_new( fd );
	if(!STEP_OK( "g_io_channel_unix_new()", channel )) return false;
#endif



	// I want to de-re-subscribe to IN OUT and OTHER independently
	// hence three watches required!

	GIOCondition G_IO_others =(GIOCondition) 
		( G_IO_PRI
                | G_IO_ERR
                | G_IO_HUP
                | G_IO_NVAL );

if(1) {
	// you can experiement with variations of combinations
	// On UNIX it doesnt matter
	// On WIN32 the last one wins
	// The other watches are not quite right

	add_watches( channel, G_IO_others );
	add_watches( channel, G_IO_IN );
	add_watches( channel, G_IO_OUT ); // put this last ...

} else {
	// other experiments
	GIOCondition G_IO_all6 = (GIOCondition) 
		( G_IO_PRI
                | G_IO_ERR
                | G_IO_HUP
                | G_IO_IN
                | G_IO_OUT
                | G_IO_NVAL );

	GIOCondition G_IO_try_2 = (GIOCondition) ( 0
                | G_IO_IN
                | G_IO_OUT
	);

	add_watches( channel, G_IO_all6 );


}
if(1) {
	INFO("Setting NULL encoding and no buffering");
	g_io_channel_set_encoding(channel, NULL, NULL);
	g_io_channel_set_buffered(channel, FALSE);
}
	return true;
}

int main( int argc, char ** argv )
{
	if(!win_sock_init()) return false;
	gtk_init(&argc,&argv);
	STEP_OK( "gtk_init() returned ... ", true );
	if(!mk_g_io()) return 2;
	perr_clear();
	STEP_OK( "Starting gtk_main() ... ", true );
	gtk_main();
	if(!win_sock_tidy()) return false;
	return 0;
}
