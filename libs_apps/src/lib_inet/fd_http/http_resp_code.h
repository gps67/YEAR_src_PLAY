#ifndef http_resp_code_H
#define http_resp_code_H

#include "str0.h"

/*!
	holds the HTTP response code (200 OK) for output
*/
class http_resp_code : public GRP_lib_inet
{
 /*!
	convert 200 into "OK"
 */
 static
	const char * get_str_from_code( int _code );

 inline
	void set( int _code, str0 _msg )
	{
		code = _code;
		msg = _msg;
		if( ! msg ) set_500_internal_error();
	}
	void set( int _code )
	{
		set( _code, (str0) get_str_from_code( _code ));
	}

 public:
	int code;
	str0 msg;

	http_resp_code()
	: msg( "-UNSET-" )
	{
		set_500_internal_error();
	}
	void set_manually( int _code, str0 _msg )
	{
		set( _code, _msg );
	}
	void set_manually( int _code )
	{
		set( _code );
	}

	// these are more readable, using the linker to validate the strings
	void set_500_internal_error()	{ set( 500 ); }
	void set_200_ok()		{ set( 200 ); }
	void set_400_bad_request()	{ set( 400 ); }
	void set_404_not_found()	{ set( 404 ); }
};

#endif

