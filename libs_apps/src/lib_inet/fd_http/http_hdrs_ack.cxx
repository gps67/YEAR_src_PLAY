#include "http_hdrs.h"

// #include "fd_selectable.h"
#include "buffer1.h"
#include "http_resp_code.h"

#include <time.h>
#include "tm_parts.h"


	http_hdrs_ack::~http_hdrs_ack()
	{
	}

	/*
		you can add these as your code grows
	*/
	void http_hdrs_ack::set_content_type_text_html()
	{
		content_type = "text/html";
	}
	void http_hdrs_ack::set_content_type_text_plain()
	{
		content_type = "text/plain";
	}

	/*
		gen_headers() printers
	*/
	void http_hdrs_ack::pair_poss0( buffer1 & fd, str0 attr, int val )
	{
		// content length is possible zero, initialised -1
		fd.print("%s: %d\r\n", (STR0) attr, (int) val );
	}
	void http_hdrs_ack::pair( buffer1 & fd, str0 attr, str0 val )
	{
		if( val ) if(val) fd.print("%s: %s\r\n", (STR0) attr, (STR0) val );
	}
	void http_hdrs_ack::pair( buffer1 & fd, str0 attr, int val )
	{
		if( val ) pair_poss0( fd, attr, val );
	}
	void http_hdrs_ack::pair_date( buffer1 & fd, str0 attr, time_t val )
	{
		// type confusion with int
		if( val ) pair( fd, attr, tm_parts::str_http( val ) );
	}

/*!
	Generate the set of headers into a text buffer, where they
	can be written to fd (or debug).

	Other than the first line, HTTP doesnt really care, but this
	class provides consistency. 
*/
	void http_hdrs_ack::gen_headers( buffer1 & fd )
	{
		// gen_etag( fd );
		str0 v_str = "";
		if( http_ver_1_0 ) v_str = "HTTP/1.0";
		else
		if( http_ver_1_1 ) v_str = "HTTP/1.1";
		else
			throw "HTTP version not set";
		fd.print("%s %d %s\r\n", (STR0) v_str, resp_code.code, (STR0) resp_code.msg );

		pair_date( fd, "Date",	time(0) );
		pair( fd, "Server", server );
		pair_date( fd, "Last-Modified",	last_modified );
		pair( fd, "ETag", etag );
		if( http_ver_1_1 ) 
			pair( fd, "Accept-Ranges", "bytes" );
		// gzip applied by http, not the file itself
		pair( fd, "Content-Encoding", content_encoding );
		pair( fd, "Content-Language", content_language ); 
	 if( content_length >=0 )
		pair_poss0( fd, "Content-Length", content_length );
	 if( !connection_keep_alive )
		pair( fd, "Connection",	"close" );
	/* else nothing 
	 else
		pair( fd, "Connection",	"Keep-Alive" );
	*/
		pair( fd, "Content-Type", content_type );
		fd.print("\r\n");
		
	}

/*
RESP: VER: 'HTTP/1.1' CODE: 200 MSG 'OK'
ATTR:            Date VAL: 'Mon, 16 Sep 2002 16:37:14 GMT'
ATTR:          Server VAL: 'Apache/1.3.26 (Unix) PHP/4.2.3'
ATTR:   Last-Modified VAL: 'Mon, 16 Sep 2002 16:37:01 GMT'
ATTR:            ETag VAL: '"18f13e-0-3d8608ad"'
ATTR:   Accept-Ranges VAL: 'bytes'
ATTR:  Content-Length VAL: '0'
ATTR:      Connection VAL: 'close'
ATTR:    Content-Type VAL: 'text/plain'
*/
