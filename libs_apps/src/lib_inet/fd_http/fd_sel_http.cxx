
#include "fd_sel_http.h"


// VIRTUAL
/*!
	destructor - the underlying class closes the fd
*/
	fd_sel_http::~fd_sel_http()
	{
	}

/*!
	constructor - you provide the fd
*/
	fd_sel_http::fd_sel_http( int _fd )
	: fd_sel_line( _fd )
	{
		line1_seen = false;
		in_header = true;
	}

	/*
		NOTE: this code is allowed to poke NUL values
		into buffer, its not const char *.

		ALSO: line is guaranteed to end with NUL (not CRLF),
		even data!
	*/

//////////////////////////////////////////////////////////////////////////

// VIRTUAL - from fd_line_sel

/*!
	a complete line has been received (when in line mode),
	you can, but dont have to override this method.
*/
	void fd_sel_http::got_line( str0 line, int len )
	{
		/*
		if(1) {
			uchar c = line[len];
			line[len] = 0;
			e_print("fd_sel_http(%s,%d)\n", line, len );
			line[len] = c;
		}
			sub class probably leaves this as-is,
			also leave got_head* but do intercept
			http_attr() and got_head_emptyline()
		*/
		// HTTP specific
		if(!line1_seen) {
			line1_seen = true;
			got_head_line1( line, len );
		} else if( in_header ) {
			if( len ) {
				got_head_line( line, len );
			} else {
				// HTTP end of headers
				in_header = false;
				got_head_emptyline();
			}
		} else {
				got_data_line( line, len );
		}
	}


// VIRTUAL - from fd_line_sel

/*!
	You are supposed to override this function.
	Data has arrived, but the class is not in line mode.
*/
	void fd_sel_http::got_data( void * data, int len )
	{
		// data IS NUL terminated, and it may or may not be text
		e_print( "DATA: ---->(NL)\n%s<--- :DATA\n", (char*)data );
	}

// VIRTUAL - fd_line_sel

/*!
	Your derived function doest have to do much, but this is how
	you know that eof has arrived - possibly unexpectedly.
*/
	void fd_sel_http::got_eof()
	{
		e_print( "-EOF-\n" );
	}

// VIRTUAL - fd_line_sel

/*!
	Your derived function would shutdown gracefully.
*/
	void fd_sel_http::got_error()
	{
		e_print( "-ERROR- abandon any connection (if not eof_seen)\n" );
	}

//////////////////////////////////////////////////////////////////////////

// VIRTUAL
/*!
	This is overridable, but should do the right thing. The first
	line of the RESPONSE has arrived, giving the error-code (OK).

	buff gets overwritten
*/
	void fd_sel_http::got_head_line1( str0 buff, int len )
	{
		// RESPONSE not request!!
		// RESP:'HTTP/1.1 404 Not Found'
		// RESP:'HTTP/1.1 200 OK'

		e_print( "RESP:'%s'\n", (STR0) buff );

		str0 resp_ver;
		int    resp_code = 0;
		str0 resp_msg;

		uchar * p1 = buff;
		uchar * p2 = p1; // lose the const - how

		while( *p2 > ' ' ) p2++;
		if( *p2 != ' ' ) goto bad;
		*(uchar *)(const uchar *) p2 = 0;
		p2++;

		if( !strcmp( (const char*)p1, "HTTP/1.0" )) {
		} else if( !strcmp( (const char *)p1, "HTTP/1.1" )) {
		} else goto bad;

		resp_ver = p1;

		p1 = p2; // ++ done
		while( *p1 == ' ') p1++; // should be empty
		p2 = str0(p1).str_chr( ' ' );
		if(!p2) goto bad;
		*(uchar *)(const uchar *) p2 = 0;
		p2++;
		resp_code = atoi( (const char *)p1 );

		p1 = p2;
		while( *p1 == ' ') p1++;
		resp_msg = p1;

		http_resp(
			resp_ver,
			resp_code,
			resp_msg
		);

//	 ok:
		return;
	 bad:
		e_print( "got_head_line1(%s) - bad\n", (STR0) buff ); // NUL truncated
		gdb_break_point();
		http_resp(
			(str0) "",
			0,
			(str0) ""
		);
		return;
	}

// VIRTUAL
/*!
	An HTTP header line has arrived. You can override this,
	but should hook into http_attr( name, l , value, l );
*/
	void fd_sel_http::got_head_line( str0 buff, int len )
	{
		uchar * p2 = (uchar *) buff.str_chr( ':' );
		if(!p2) {
			throw "HTTP attribute without :";
			return;
		}
		*p2 = 0;
		int l1 = p2 - (uchar *)buff;
		p2++;
		if( *p2 == ' ' ) p2++;
		int l2 = len - (p2-(uchar *)buff);
		http_attr( buff, l1, p2, l2 );
	}

//////////////////////////////////////////////////////////////////////////

// VIRTUAL
/*!
	You definitely want to override this, and act on the header response
	just received. (Headers are terminated by a blank line).

	This switches into data_mode (not line mode) to receive a binary
	file.
*/
	void fd_sel_http::got_head_emptyline()
	{
		/*
			got_line has already set in_header = false,
			the point of this virtual function is to
			process the headers actually received,
			and maybe decide on Content-Length

RESP: VER: 'HTTP/1.1' CODE: 200 MSG 'OK'
ATTR:            Date VAL: 'Mon, 16 Sep 2002 16:37:14 GMT'
ATTR:          Server VAL: 'Apache/1.3.26 (Unix) PHP/4.2.3'
ATTR:   Last-Modified VAL: 'Mon, 16 Sep 2002 16:37:01 GMT'
ATTR:            ETag VAL: '"18f13e-0-3d8608ad"'
ATTR:   Accept-Ranges VAL: 'bytes'
ATTR:  Content-Length VAL: '0'
ATTR:      Connection VAL: 'close'
ATTR:    Content-Type VAL: 'text/plain'

			If you want the data delivered as lines (without CRLF)
			do nothing, otherwise call: set_data_mode();
		*/

		e_print( "----\n" );

		//set_line_mode(); // do nothing to retain this
		set_data_mode();
	}

// VIRTUAL
/*!
	The data file can be received in LINE_MODE or binary_MODE.
	(Default == binary).
	
	In LINE_MODE you get this call every line, with the CRLF removed.
*/
	void fd_sel_http::got_data_line( str0 buff, int len )
	{
		e_print( "TEXT:'%s'\n", (STR0) buff );
	}

// VIRTUAL
/*!
	The data file is received in packets of arbitrary size.
	You override this method to accept data. At present,
	no notice is taken of Content-Length, but in future,
	there would be an exact break at that point.
*/
	void fd_sel_http::got_data_blk( void * data, int len )
	{
		e_print( "DATA:'%s'\n", (char *) data );
	}

//////////////////////////////////////////////////////////////////////////

// VIRTUAL
/*!
	The response code has been received and parsed. The headers
	are yet to come, but you know the immediate outlook. You override
	this function, and do ... (something) ...
*/
	void fd_sel_http::http_resp( str0 resp_ver, int resp_code, str0 resp_msg )
	{
		e_print( "RESP: VER: '%s' CODE: %d MSG '%s'\n",
			(STR0) resp_ver,
			(int)  resp_code,
			(STR0) resp_msg
		);
	}

// VIRTUAL
/*!
	The response included this header (CURR: including Content-Length),
	you use these to decide how to process the file.
*/
	void fd_sel_http::http_attr( str0 attr, int l1, str0 val, int l2 )
	{
		e_print( "ATTR: %15s VAL: '%s'\n", (STR0) attr, (STR0) val );
	}

// VIRTUAL
void fd_sel_http::http_attr_Date( time_t t )
{
}

// VIRTUAL
void fd_sel_http::http_attr_Date( str0 val, int l2 )
{
}

// VIRTUAL
void fd_sel_http::http_attr_Server( str0 val, int l2 )
{
}

// VIRTUAL
void fd_sel_http::http_attr_X_Powered_By( str0 val, int l2 )
{
}

// VIRTUAL
void fd_sel_http::http_attr_Set_Cookie( str0 val, int l2 )
{
}

// VIRTUAL
void fd_sel_http::http_attr_Connection( str0 val, int l2 )
{
}

// VIRTUAL
void fd_sel_http::http_attr_Content_Type( str0 val, int l2 )
{
}


