#ifndef X509_REQ_util_H
#define X509_REQ_util_H

#include "obj_ref.h"
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include "X509_util_NAME.h"
#include "X509_util_EXT.h"

class EVP_P_KEY;
class CB_get_phrase_base;
class X509_VALS_SITE_funcs;
class X509_VALS_key_usage;
class X509_VALS_subjectAltName;
class X509_VALS_CA_depth;
class X509_VALS_base;
class str1;

// some utils to build X509 for REQ and for ISS

class X509_util_base : public obj_ref0
{
 public:
	X509_util_base();
	~X509_util_base();

	X509_util_NAME subject;
	//	"CN",	NID_commonName,
	//	"C",	NID_countryName,
	//	"L",	NID_localityName,
	//	"ST",	NID_stateOrProvinceName,
	//	"O",	NID_organizationName,
	//	"OU",	NID_organizationalUnitName,

	X509_util_EXT extensions;
	//	EMAIL, DNS, IP, URI, ...
	//	key_usage
	//	CA_pathlen
	//	

	bool SET_values_base( X509_VALS_base & vals );
};

/*!
	The X509_util_REQ class, is used to set parameters manage a X509 REQ

	The client is the future owner of the X509, and knows the RSA.

	The server issues the X509, and knows its own RSA, to sign this X509,
	but the server does not know the client RSA.

	... except here ... where the server prepares it all, sends it
	to the client, and forgets that it knew the unknowable secret.
	This is because the client might generate a request that I 
	dont like, and I would have to search every corner to check.
	Instead I prepare it.

	An alternative, would be to prepare the REQ, send it to the client
	to sign, check it comes back unchanged except for the SIGN,
	(which is just a question of RTFM, and a bit of coding,
	not a comprehensive search for what I never knew to look for),
	then sign it.

	The down side of this approach, is that the client might
	trust a MITM a bit too much.

	The MITM is also why the cb_pass is probably not used,
	and if it is, why it should be a blk1 after the key has been munged.
	After all, if my passphrase is "bananas" this week, it might be
	"..." next week.
*/
class X509_util_REQ : public X509_util_base
{
 public:

	//! the openSSL X509 REQ
	X509_REQ *req;

	//! the RSA key that client knows
	obj_hold<EVP_P_KEY> client_rsa;

	//
	// temp vars for building with 
	//

	~X509_util_REQ();

	/*!
		Prepare a REQ without a KEY phrase (yet)
	*/
	X509_util_REQ();

	/*!
		Prepare a REQ with a KEY phrase
	*/
	X509_util_REQ( CB_get_phrase_base * _cb_pass );

	/*!
		UNUSED
	*/
	bool init();

	/*!
		AFTER you have setup client_rsa, call this, to get started
	*/
	bool init_req_with_pub_key();

//	bool REQ_subjectAltName( X509_VALS_subjectAltName &vals );
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
