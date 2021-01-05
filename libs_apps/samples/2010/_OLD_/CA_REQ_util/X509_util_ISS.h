#ifndef X509_util_ISS_H
#define X509_util_ISS_H

#include "X509_util_REQ.h"

class X509_util_ISS : public X509_util_base
{
 public:

	//! the openSSL X509 ISS
	X509_cert * iss;

	//! the RSA key that client knows
	obj_hold<EVP_P_KEY> client_rsa;

	//
	// temp vars for building with 
	//

	~X509_util_ISS();

	/*!
		Prepare a ISS without a KEY phrase (yet)
	*/
	X509_util_ISS();

	/*!
		Prepare a ISS with a KEY phrase
	*/
	X509_util_ISS( CB_get_phrase_base * _cb_pass );

	/*!
		UNUSED
	*/
	bool init();

	/*!
		AFTER you have setup client_rsa, call this, to get started
	*/
	bool init_req_with_pub_key();

//	bool ISS_subjectAltName( X509_VALS_subjectAltName &vals );
	bool VALS_SITE_funcs( X509_VALS_SITE_funcs & vals );

 static
	bool longName_to_NID( const char * longName, int & nid );

	/*!
		convert "CN" to NID_commonName (or not)
	*/
 static
	bool shortName_to_NID( const char * shortName, int & nid );

 static const char * shortName_of_NID( int nid ) ;
 static const char * longName_of_NID( int nid ) ;

	/*
		add_subject_NID_value( NID_commonName, "My Common Name" );

		The subject of the X509 is the ITEM/person/machine/etc
		NOT the X509 itself, nor its other attibutes

		The subj var will have several of these items added.
	*/
	bool add_subject_NID_value( int nid, const char * value );

  	/*!
		add_subject_longName_value( "countryName", "GB" )
	*/
	bool add_subject_longName_value( const char * longName, const char * value );

	/*!
		The CN is the commonName, is the full name of the person/machine

		but see also Alternate ... extensions
	*/
	bool add_CN( const char * cn ) ;

	/*!
		After adding all the bits to the subject name variable,
		add the subject name to the certificate.
	*/
	bool set_subject_name() ;
	/*! DITTO but PRIVATE ?
	*/
	bool set_subject_name( X509_NAME * _subj );

	/*!
		add_ext( "subjectAltName", "CA:true" ); // CA only;
		add_ext( "subjectAltName", CLIENT_subjectAltName );

			EMAIL:
			DNS:
			IP:	(IP4 or IP6 dotted)
			URI:
			...

		http://www.openssl.org/docs/apps/x509v3_config.html
	*/
	bool add_ext( const char * name, const char * value );
	bool add_ext_nid_val( int nid, const char * value );
	bool add_ext_key_usage( const char * value );
//	bool add_ext_subjectAltName( const char * pfx, const char * val );

	bool add_ext_DNS( const char * val )
	{ return extensions.OK_add_ext_DNS(val); }

	bool add_ext_IP( const char * val )
	{ return extensions.OK_add_ext_DNS(val); }

	bool add_ext_URI( const char * val )
	{ return extensions.OK_add_ext_DNS(val); }

	bool add_ext_EMAIL( const char * val )
	{ return extensions.OK_add_ext_DNS(val); }

	//////////////////////////////////

	bool add_extensions();

	bool client_sign();

	bool write_to_buffer( blk1 & buf );

	bool write_req_to_file( const char * filename );

	/*!
		I use the above, that goes via buffer,
		but there is this comparison available
	*/
	bool write_to_file1( const char * filename );
};

#endif

