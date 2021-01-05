#ifndef X509_util_EXT_H
#define X509_util_EXT_H

#include <openssl/x509.h>
#include <openssl/x509v3.h>

class X509_VALS_subjectAltName;
class X509_VALS_key_usage;
class X509_VALS_CA_depth;

/*!
	X509 extensions, altsubject name etc
*/
class X509_util_EXT
{
 public:
	STACK_OF (X509_EXTENSION) * extlist;
	bool ADD_EXT_subject_key_identifier_hash; // later not now!

	X509_util_EXT();
	~X509_util_EXT();

	bool add_ext_nid_val( int nid, const char * value );
	bool OK_add_ext_nid_val( int nid, const char * value );
	bool OK_add_ext_nid_pfx_val( int nid, const char * pfx, const char * value );
	bool OK_add_ext_DNS( const char * val );
	bool OK_add_ext_IP( const char * val );
	bool OK_add_ext_URI( const char * val );
	bool OK_add_ext_EMAIL( const char * val );

	bool OK_add_ext_subjectAltName( X509_VALS_subjectAltName & vals );

	bool add_ext_key_usage( X509_VALS_key_usage & vals );
	bool add_ext_CA_pathlen( bool ca, int pathlen );
	bool add_ext_CA_depth( X509_VALS_CA_depth & vals );
	bool add_ext_subject_key_identifier_hash(); // 4+60 bit sha-1 or 160 bit sha-1
};

#endif

