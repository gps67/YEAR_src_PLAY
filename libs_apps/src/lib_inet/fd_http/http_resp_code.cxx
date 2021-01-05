#include "http_resp_code.h"

#define RESP_CODE_DEX( N, ary, Array ) \
	N = (sizeof(Array)/sizeof(const char *)); \
	ary = Array;

#include <string.h>

	const char * http_resp_code::get_str_from_code( int _code )
	{
		// These string borrowed from www.apache.org Thanks
		static const char * const strings_100[] = {
		    "100 Continue",
		    "101 Switching Protocols",
		    "102 Processing"
		};
		static const char * const strings_200[] = {
		    "200 OK",
		    "201 Created",
		    "202 Accepted",
		    "203 Non-Authoritative Information",
		    "204 No Content",
		    "205 Reset Content",
		    "206 Partial Content",
		    "207 Multi-Status"
		};
		static const char * const strings_300[] = {
		    "300 Multiple Choices",
		    "301 Moved Permanently",
		    "302 Found",
		    "303 See Other",
		    "304 Not Modified",
		    "305 Use Proxy",
		    "306 unused",
		    "307 Temporary Redirect"
		};
		static const char * const strings_400[] = {
		    "400 Bad Request",
		    "401 Authorization Required",
		    "402 Payment Required",
		    "403 Forbidden",
		    "404 Not Found",
		    "405 Method Not Allowed",
		    "406 Not Acceptable",
		    "407 Proxy Authentication Required",
		    "408 Request Time-out",
		    "409 Conflict",
		    "410 Gone",
		    "411 Length Required",
		    "412 Precondition Failed",
		    "413 Request Entity Too Large",
		    "414 Request-URI Too Large",
		    "415 Unsupported Media Type",
		    "416 Requested Range Not Satisfiable",
		    "417 Expectation Failed",
		    "418 unused",
		    "419 unused",
		    "420 unused",
		    "421 unused",
		    "422 Unprocessable Entity",
		    "423 Locked",
		    "424 Failed Dependency"
		};
		static const char * const strings_500[] = {
		    "500 Internal Server Error",
		    "501 Method Not Implemented",
		    "502 Bad Gateway",
		    "503 Service Temporarily Unavailable",
		    "504 Gateway Time-out",
		    "505 HTTP Version Not Supported",
		    "506 Variant Also Negotiates",
		    "507 Insufficient Storage",
		    "508 unused",
		    "509 unused",
		    "510 Not Extended"
		};

		int h = _code / 100;
		int u = _code % 100;
		int N;
		const char * const * ary;
		switch(h ) {
		 case 1: RESP_CODE_DEX( N, ary, strings_100 ); break;
		 case 2: RESP_CODE_DEX( N, ary, strings_200 ); break;
		 case 3: RESP_CODE_DEX( N, ary, strings_300 ); break;
		 case 4: RESP_CODE_DEX( N, ary, strings_400 ); break;
		 case 5: RESP_CODE_DEX( N, ary, strings_500 ); break;
		 default:
			return NULL;
		}
		if( u >= N )
		{
			return NULL;
		}
		const char * str = ary[ u ];
		if( 0==strcmp( str+4, "unused" ) )
		{
			return NULL;
		}
		return str+4;
}

/*
 inline
	void set( int _code, const char * _msg )
	{
		code = _code;
		msg = _msg;
		if( ! msg ) set_500_internal_error();
	}

	void set( int _code )
	{
		set( _code, get_str_from_code( _code ));
	}

 public:
	int code;
	const char * msg;

	http_resp_code()
	{
		set_500_internal_error();
	}
	void set_manually( int _code, const char * _msg ) { set( _code, _msg ); }
	void set_manually( int _code ) { set( _code ); }

	// these are more readable, using the linker to validate the strings
	void set_500_internal_error()	{ set( 500 ); }
	void set_200_ok()		{ set( 200 ); }
	void set_400_bad_request()	{ set( 400 ); }
	void set_404_not_found()	{ set( 404 ); }
*/


