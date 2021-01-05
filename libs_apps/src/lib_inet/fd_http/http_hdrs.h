#ifndef http_hdrs_H
#define http_hdrs_H

#include "str0.h"

// #include "fd_selectable.h"
#include "buffer1.h"
#include "http_resp_code.h"

#include <time.h>

#include <string.h>
#include "obj_hold.h"
#include "str1.h"


/*!
	HTTP RELATED
*/
class http_opts_list : public GRP_lib_inet
{
	float q_factor;
};

/*!
	HTTP RELATED
*/
class http_opts_list_parser : public GRP_lib_inet
{
 public:
	
};

/*!
	HTTP RELATED
*/
class http_hdrs : public GRP_lib_inet
{
 public:
	bool http_ver_1_0;
	bool http_ver_1_1;
	bool connection_keep_alive;

	http_hdrs()
	{
		http_ver_1_0 = false;
		http_ver_1_1 = false;
		connection_keep_alive = false;
	}
};

/*!
	HTTP RELATED

	The headers from client to server, and generators and parsers
	POST adds upload content length
	RENAME adds ...
	GET uri?a=1&b=2 -- all part of uri
*/
class http_hdrs_enq : public http_hdrs
{
	str1 action;
	str1 uri;
	str1 host_port;
	str1 user_agent; // split to main/ver plus plus plus
	str1 referer;
	bool encoding_gzip;
	bool encoding_compress;
 public:
	http_hdrs_enq();
	~http_hdrs_enq();

	// parser routines
	void got_req_line( char * buff );
	void got_user_agent( char * buff );
	void add_accept_item_mime( str0 val );
	void add_accept_item_encoding( str0 val );
	void add_accept_item_language( str0 val );
	void add_accept_item_charset( str0 val );
	void add_accept_line_mime( char * val );
	void add_accept_line_encoding( char * val );
	void add_accept_line_language( char * val );
	void add_accept_line_charset( char * val );
	void add( str0 attr, char * val );
};

/*!
	HTTP RELATED

	The headers from server to client, and generators and parsers
	Well known headers are specifically handled.

	This provides some utility functions to set well known fields
	in the headers. Future use would include using the Content-Length
	to control the return buffer

	Other well known fields should have well known values,
	and apps should have an easy method of consistently getting
	them consistently corekt.
*/
class http_hdrs_ack : public http_hdrs
{
 public:
	str1 server;
	http_resp_code resp_code; // (200, OK }
	str1 content_type;	// text/html
	str1 content_encoding;	// gzip | compress | (absent)
	int content_length;
	str1 content_language;	// en
	str1 etag;		// "18f13e-0-3d8608ad"
	time_t last_modified;

	~http_hdrs_ack();
	http_hdrs_ack()
	: resp_code()
	{
		server = "http_libr/0.0.1 (UNIX)";
		http_ver_1_0 = true;
		http_ver_1_1 = false;
		connection_keep_alive = false;
		last_modified = time(0)-(60*60*24);
		content_type = "text/plain";
		content_length = -1;
		// resp_code.set_500_internal_error(); // default
	}

	void set_content_type_text_html();
	void set_content_type_text_plain();

	void gen_headers( buffer1 & fd );
	void pair_poss0( buffer1 & fd, str0 attr, int    val );
	void pair_date(  buffer1 & fd, str0 attr, time_t val );
	void pair(       buffer1 & fd, str0 attr, int    val );
	void pair(       buffer1 & fd, str0 attr, str0   val );

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

};


#endif

