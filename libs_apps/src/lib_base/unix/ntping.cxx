
#include "ntping.h"
#include "fork_exec_setup.h"

#include "fd_sel_line.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "buffer2.h"

////////////// ntping fd handlers ////////////////////

/*!
	The KBD side is WRITE only

	This shows the need for fd_sel_WRITE
*/
class ntping_fd_kbd : public fd_selectable_base
{
 public:
	ntping_fd_kbd( int fd )
	: fd_selectable_base( fd )
	{
	}
	bool select_says_read()
	{
		/*
			This is called ONCE - to indocate OK
			As in something to to with O_NDELAY
			and fifo is NOW writable for first time
		*/

		char buff[1025];
		int len = read( buff, 1024 );

		// got len == -1
		e_print("NTPING.KBD select_says_read !!!!!!!! len = %d\n", len );

		e_print_hex_dump( buff, len );

		return true;
	}
	void got_eof()
	{
		e_print("NTPING.KBD EOF \n" );
	}
	void got_error()
	{
		e_print("NTPING.KBD ERROR \n" );
	}
	void got_data( void * data, int len )
	{
		e_print("NTPING.KBD DATA *********** \n" );
	}
	void got_line( str0 line, int len )
	{
		e_print("NTPING.KBD line %s\n", (STR0) line );
	}
	virtual void select_says_connected()
	{
		e_print("NTPING.KBD select_says_connected \n" );
	}
};


class ntping_fd_out : public fd_sel_line
{
 public:
	fork_exec_parent * fork_parent_1;

	ntping_fd_out( int fd, fork_exec_parent * _tool )
	: fd_sel_line( fd )
	, fork_parent_1( _tool )
	{
	}
	void got_eof()
	{
		e_print("NTPING.OUT EOF \n" );
		if( fork_parent_1 )
		{
			fork_exec_parent * p = fork_parent_1;
			fork_parent_1 = NULL;
			p->got_eof_1();
		}
	}
	void got_error()
	{
		e_print("NTPING.OUT ERROR \n" );
	}
	void got_data( void * data, int len )
	{
		e_print("NTPING.OUT DATA *********** \n" );
	}
	void got_line( str0 line, int len )
	{

// 127.0.0.1     4 ms  
		char hostname[80];
		int ms;

		if( 2 == sscanf( (STR0) line, "%80s %d ms", (char *)hostname, &ms ) )
		{
			e_print("NTPING %4d ms from %s\n", ms, (STR0) hostname );
		}
		else if( 1 == sscanf( (STR0) line, "%80s *", (char *)hostname ) )
		{
			e_print("NTPING: TIMEOUT from %s\n", (STR0) hostname );
		}
		else {
			e_print("NTPING.OUT FAILS: %s\n", (STR0) line );
		}
	}
};

class ntping_fd_err : public fd_sel_line
{
 public:
	ntping_fd_err( int fd )
	: fd_sel_line( fd )
	{
	}
	void got_eof()
	{
		e_print("NTPING.ERR EOF \n" );
	}
	void got_error()
	{
		e_print("NTPING.ERR ERROR \n" );
	}
	void got_data( void * data, int len )
	{
		e_print("NTPING.ERR DATA *********** \n" );
	}
	void got_line( str0 line, int len )
	{
		e_print("NTPING.ERR line %s\n", (STR0) line );
	}
};

////////////// ntping ////////////////////

ntping::ntping()
: fork_exec_parent()
{
}

fd_selectable_base * ntping::mk_fd_kbd( int fd )
{
	return new ntping_fd_kbd( fd );
}

fd_selectable_base * ntping::mk_fd_out( int fd )
{
	return new ntping_fd_out( fd, this );
}

fd_selectable_base * ntping::mk_fd_err( int fd )
{
	return new ntping_fd_err( fd );
}

bool path_lookup( buffer2 & buf )
{
	str0 s = str0(buf);
	if( s == "ntping" )
	{
		buf.set("/media/hda3/usr/bin/ntping" );
		return true;
	}
	fprintf(stderr, "npting::path_lookup failed to find %s\n", (STR0)s );
	return false;
}

// VIRTUAL
bool ntping::exec_child()
{
	char * filename = (char *) "/usr/local/bin/ntping";
#warning "LURK - exec fails file-not-found, but stderr is eaten by ... ?"
	filename = (char *) "/media/hda3/usr/bin/ntping";
	filename = (char *) "ntping";
	char * const argv[] = { filename, NULL, NULL };
	char * const envp[] = { NULL, NULL };
	buffer2 buf;
	buf.set( filename );
	if(!path_lookup( buf ))
	{
		fprintf(stderr, "EXEC exec_child %s FAIL -- FILE NOT FOUND \n", filename );
		return false;
	}
	char * filename2 = (char *)str0(buf);
	if(1)
	{
		// show that parent is catching stderr
		fprintf(stderr, "EXEC %s\n", filename );
	}
	fflush(0);
	execve( filename2, argv, envp );
	fprintf(stderr, "EXEC %s FAIL %m \n", filename );
	fflush(0);
	e_print( "ALERT exec() %s %m\n", filename );
	return false;
}

void ntping::got_eof_1()
{
	int status = 0;
	int options = WNOHANG;
	// int p2 = 
	waitpid( pid_child, &status, options );
	e_print("got_eof_1() status = %d\n", status );
}

// VIRTUAL
bool ntping::test1()
{
	obj_hold<fork_exec_setup> builder = new fork_exec_setup( this );
	if(! builder ) return false;

	builder->link.stderr_action = fd_pipe_triple::stderr_piped;
//	builder->link.stderr_action = fd_pipe_triple::stderr_as_was;
	if(! builder->setup() ) return false;

	const char * list[] = {
		"bird",
		"ntlgw bird 127.0.0.1",
		"192.168.10.1",
		"83.151.192.66",
		"83.151.192.72",
		"bird",
		"127.0.0.1",
	};
	int N = sizeof( list ) / sizeof( list[0] );
	for( int i=0; i<N; i++ )
	{
		fd_kbd -> print( "%s\n", list[i] );
	}
	fd_kbd -> flush();

	/*
		ntping takes a list of words, so it needs EOF
		but not yet ?
	 */

 if(1) // this shows a weakness with HUP appearing
	fd_kbd -> eof_synthetic();
	return true;
}




// EXTERN
void run_ntping()
{
        static obj_hold<ntping> pinger;
        pinger = new ntping();
        pinger->test1();
}








