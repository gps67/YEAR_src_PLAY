#ifndef X509_util_EXT_H
#define X509_util_EXT_H

#include <openssl/x509.h>
#include <openssl/x509v3.h>

class EVP_P_KEY;

#if 1
// new
typedef lhash_st_CONF_VALUE LHASH;
#else
// old
typedef struct lhash_st LHASH;

#endif

#include "SSL_CA1_stubs.h"

namespace CA1 {

//	class X509_VALS_subjectAltName;
//	class X509_VALS_key_usage;
//	class X509_VALS_CA_depth;

/*!
	This was nice, until the authority identifier messed it up

	BUG: leaking extensions, particularly extlist
	MAYBE add_extensions should move here, and delete list (used)
*/
class X509_util_EXT_base
{
 public:
	X509_util_EXT_base();
 virtual // its not obj_ref0 !!
	~X509_util_EXT_base();

	X509_EXTENSION * extn_held;	// owner must delete, set NULL

	/*
		derived class places the extension in its list, or cert...
	*/
	virtual bool store_extn_held() = 0;

	bool store_clear_extn_held();
	bool clear_extn_held();
	bool drop_extn_held();

	bool mk_ext_cfg_ctx_nid_val(
		LHASH * cfg,
		X509V3_CTX * ctx,
		int nid,
		const char * value
	);

	// semi private - for ext subject key id hash
	bool add_ext_cfg_ctx_nid_val(
		LHASH * cfg,
		X509V3_CTX * ctx,
		int nid,
		const char * value
	);

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

	// I think that the KEY must be attached to the CERT
	// in so much as the ctx goes and fetches it
	bool add_ext_subject_key_identifier_hash( X509_REQ * req_cert_has_key );

};

/*!
	X509 extensions, altsubject name etc
*/
class X509_util_EXT_list : public X509_util_EXT_base 
{
 public:
	STACK_OF (X509_EXTENSION) * extlist;
 //	bool ADD_EXT_subject_key_identifier_hash; // later not now!

	X509_util_EXT_list();
	~X509_util_EXT_list();

	virtual bool store_extn_held();
};

class X509_util_EXT_iss : public X509_util_EXT_base 
{
	X509_cert * client_cert;
 public:
	X509_util_EXT_iss( X509_cert * _client_cert )
	:	client_cert( _client_cert )
	{
	}
	~X509_util_EXT_iss() {}

	virtual bool store_extn_held();

	bool add_ext_authority_key_identifier(
		X509 * issuers_certificate_has_key,
		const char * opts = NULL
	);

};

}; // namespace
#endif

