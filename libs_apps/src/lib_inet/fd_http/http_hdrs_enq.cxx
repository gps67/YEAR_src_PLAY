
#include "http_hdrs.h"

#ifdef WIN32
#warning "MISSING rindex"
char * rindex( const char * p1, char c1 )
{
	return NULL;
}
#endif

/*!
	destructor - NOOP
*/
	http_hdrs_enq::~http_hdrs_enq()
	{
	}

/*!
	constructor - headers
*/
	http_hdrs_enq::http_hdrs_enq()
	{
		encoding_gzip = false;
		encoding_compress = false;
	}


/*!
	web browser is ...
*/
	void http_hdrs_enq::got_user_agent( char * buff )
	{
// eg  Lynx/2.8.4dev.8 libwww-FM/2.14 SSL-MM/1.4.1 OpenSSL/0.9.5a
// eg  Mozilla/4.75 [en] (X11; U; Linux 2.4.19 i586)
		user_agent = buff ;
		e_print("OK user_agent = %s\n", (STR0) user_agent );
	}

/*!
	web browser asked for ...
*/
	void http_hdrs_enq::got_req_line( char * buff )
	{
		char * p1 = buff;
		while( *p1 > ' ') p1++;
		char * p2 = p1;
		if( *p2 ) p2 ++;
		*p1 = 0;
		action.set( (str0) buff );

		while( *p2 == ' ' ) p2 ++;

		char * p3 = rindex( p2, ' ');
		if(!p3) {
			throw "bad req";
			return;
		}
		char * p4 = p3 + 1;
		if(0==strcasecmp( p4, "HTTP/1.0" )) http_ver_1_0 = true;
		else
		if(0==strcasecmp( p4, "HTTP/1.1" )) http_ver_1_1 = true;

		*p3 = 0;
		uri.set( (str0) p2 );

		e_print("REQ: %s\n", buff );
		e_print("REQ: '%s' '%s' \n", (STR0) action, (STR0) uri );
	}

/*!
	web browser accepts - this mime type
*/
	void http_hdrs_enq::add_accept_item_mime( str0 val )
	{
		e_print("ACCEPT_ITEM MIME '%s'\n", (STR0) val );
	}

/*!
	web browser accepts - gzip compressed file ?
*/
	void http_hdrs_enq::add_accept_item_encoding( str0 val )
	{
		if(0==strcasecmp( val, "gzip" )) 
		{
			encoding_gzip = true;
			return;
		}
		if(0==strcasecmp( val, "compress" )) 
		{
			encoding_compress = true;
			return;
		}
		e_print("ACCEPT_ITEM ENCODING '%s'\n", (STR0) val );
	}

/*!
	web browser is asking for english ?
*/
	void http_hdrs_enq::add_accept_item_language( str0 val )
	{
		e_print("ACCEPT_ITEM LANGUAGE '%s'\n", (STR0) val );
	}

/*!
	web browser accepts utf8 ?
*/
	void http_hdrs_enq::add_accept_item_charset( str0 val )
	{
		e_print("ACCEPT_ITEM CHARSET '%s'\n", (STR0) val );
	}


/*!
	web browser accepts - mime type (line)
*/
	void http_hdrs_enq::add_accept_line_mime( char * val )
	{
		char * p1 = val;
		while( *p1 == ' ' ) p1++ ;
		char * p2 = p1;
		while( 1 )
		{
			char c = *p2;
			switch( c ) {
			 case ',':
				*p2 = 0;
				add_accept_item_mime( p1 );
				p1 = p2 + 1 ;
				while( *p1 == ' ' ) p1++ ;
				break;
			 case 0:
				add_accept_item_mime( p1 );
				return;
				p1 = p2;
			}
			p2++;
		}
	}
/*!
	see add_accept_item_encoding()
*/
	void http_hdrs_enq::add_accept_line_encoding( char * val )
	{
		char * p1 = val;
		while( *p1 == ' ' ) p1++ ;
		char * p2 = p1;
		while( 1 )
		{
			char c = *p2;
			switch( c ) {
			 case ',':
				*p2 = 0;
				add_accept_item_encoding( p1 );
				p1 = p2 + 1 ;
				while( *p1 == ' ' ) p1++ ;
				break;
			 case 0:
				add_accept_item_encoding( p1 );
				return;
				p1 = p2;
			}
			p2++;
		}
	}

/*!
	see add_accept_item_language(str)
*/
	void http_hdrs_enq::add_accept_line_language( char * val )
	{
		char * p1 = val;
		while( *p1 == ' ' ) p1++ ;
		char * p2 = p1;
		while( 1 )
		{
			char c = *p2;
			switch( c ) {
			 case ',':
				*p2 = 0;
				add_accept_item_language( p1 );
				p1 = p2 + 1 ;
				while( *p1 == ' ' ) p1++ ;
				break;
			 case 0:
				add_accept_item_language( p1 );
				return;
				p1 = p2;
			}
			p2++;
		}
	}

/*!
	calls add_accept_item_charset(str)
*/
	void http_hdrs_enq::add_accept_line_charset( char * val )
	{
		char * p1 = val;
		while( *p1 == ' ' ) p1++ ;
		char * p2 = p1;
		while( 1 )
		{
			char c = *p2;
			switch( c ) {
			 case ',':
				*p2 = 0;
				add_accept_item_charset( p1 );
				p1 = p2 + 1 ;
				while( *p1 == ' ' ) p1++ ;
				break;
			 case 0:
				add_accept_item_charset( p1 );
				return;
				p1 = p2;
			}
			p2++;
		}
	}

/*!
	Add a LINE pair of (attr,val), by parsing the attr name
*/
	void http_hdrs_enq::add( str0 attr, char * val )
	{
		if( attr.IS_same_AS( "Host" )) {
			// eg localhost:8282
			host_port.set( val );
			return;
		}
		if( attr.IS_same_AS( "Accept")) {
			add_accept_line_mime( val );
			return;
		}
		if( attr.IS_same_AS( "Accept-Encoding")) {
			// eg gzip, compress
			add_accept_line_encoding( val );
			return;
		}
		if( attr.IS_same_AS( "Accept-Language")) {
			// eg en
			add_accept_line_language( val );
			return;
		}
		if( attr.IS_same_AS( "Accept-Charset")) {
			// eg en
			add_accept_line_charset( val );
			return;
		}
		if( attr.IS_same_AS( "User-Agent")) {
			got_user_agent( val );
			return;
		}
		if( attr.IS_same_AS( "Connection")) {
			if(0==strcasecmp(val,"Close")) {
				e_print("OK Connection: Close %-15s -- %s\n", (STR0) attr, (STR0) val );
				return;
			}
			if(0==strcasecmp(val,"Keep-Alive")) {
				connection_keep_alive = true;
				e_print("OK Connection: %-15s -- %s\n", (STR0) attr, (STR0) val );
				return;
			}
		}
		if( attr.IS_same_AS( "Pragma")) {
			if(0==strcasecmp(val,"no-cache")) {
				e_print("OK Pragme_no_cache: %-15s -- %s\n", (STR0) attr, (STR0) val );
				return;
			}
		}
		if( attr.IS_same_AS( "Referer")) {
			referer = val;
		}
		if( attr.IS_same_AS( "If-Modified-Since")) {
			if(0==strcasecmp(val,"no-cache")) {
				e_print("OK Pragme_no_cache: %-15s -- %s\n", (STR0) attr, (STR0) val );
				return;
			}
		}

 // Accept-Charset: so-8859-1,*,utf-8
 // Connection: Keep-Alive
 // Pragma: no-cache
 // 
 // Authorisation: Basic base64((user:pass))
 // From: user@domain.dom
 // If-Modified-Since: _HTTP_DATE_ // send 304 or 200
 // If-Unmodified-Since: _HTTP_DATE_

 // Referer: http://site/file.html

// 1.1
 // If-Match: _ENTITY_TAG_ // delete
 // If-None-Match: ...
 // If-Range: __ENTITY_TAG_or_DATE_ // send range if match, send all if no match
 // Max-Forwards: 9 // used with TRACE, servers ignore
 // Proxy-Authorization: Basic _BASE64(XXX)_

 // Range: bytes=200-300,400-

		e_print("HDR: %-15s -- %s\n", (STR0) attr, (STR0) val );
	}


