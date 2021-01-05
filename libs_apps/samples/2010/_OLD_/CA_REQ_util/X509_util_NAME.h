#ifndef X509_util_NAME_H
#define X509_util_NAME_H

// #include <openssl/x509.h>
// #include <openssl/x509v3.h>

typedef struct X509_name_st X509_NAME;
class X509_VALS_subject;

/*!
	an X509_NAME is a 'subject' or 'issuer' or ...
*/
class X509_util_NAME
{
 public:

	X509_NAME * subj;

	~X509_util_NAME();
	X509_util_NAME();

	bool add_entry_nid_ascii( int nid, const char * value );
	bool add_entry_nid_utf8( int nid, const char * utf8 );
	bool OK_add_entry_nid( int nid, const char * utf8 );

	bool set( X509_VALS_subject & vals );
};

#endif

