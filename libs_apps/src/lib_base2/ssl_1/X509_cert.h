#ifndef X509_cert_H
#define X509_cert_H

// 2004
// 2010 - get to and from memory
// RTFM http://www.openssl.org/docs/ssl/SSL_CTX_set_verify.html
//
#include "buffer2.h"

extern "C" {
        struct x509_st;
        typedef struct x509_st X509;
};

#include "CB_get_phrase_base.h"


class X509_cert // : public obj_ref0
{
	// prevent thoughtless copies, of unsharable X509 structure 
	// this is an OpenSSL confusion - when is a copy not a copy
	X509_cert & operator=( const X509_cert & nocopies );
 public:
	X509 * cert;
	bool keep; // private reference counter
	// I dont trust the bool refcount_am_holder_of_a_ref; // holding
	// auto detect // cert.STO.kept = OBJ // a_ref_to_OBJ // _to_DATA

	/*!
		construct this holder from nothing
	*/
	X509_cert();

	/*!
		construct this holder from a pre-allocated reference
	*/
	X509_cert( X509 * _cert );

	/*!
		destructor - delete it if kept
	*/
	~X509_cert();

	operator bool() {
		return cert; // is/not null
	}

	/*!
		set this holder from a pre-allocated reference
	*/
	void set( X509 * _cert, bool _keep = true );
	void set_to_keep( X509 * _cert );
	void set_peek( X509 * _cert );

	X509 * get_and_remove(); // get, used, set NULL
	X509 * get_so_dont_delete(); // do not delete, but can reuse
	X509 * get_peek();

	operator X509 *();

	// DER is binary, with NUL bytes 
	// see also: _crl 
	// PEM is probably what you want, base64 encoded _ANYTHING_, see
	// http://www.openssl.org/docs/crypto/pem.html
	// BIO is necessary part of openssl see
	// http://www.openssl.org/docs/crypto/BIO_s_mem.html
	// BUF_MEM is a part of a BIO_s_mem
	// http://www.openssl.org/docs/crypto/buffer.html

	bool DER_out( buffer2 & buf );
	bool DER_in( buffer2 & buf );

	bool PEM_out( buffer2 & buf );
	bool PEM_in( CB_get_phrase_base * u_obj, buffer2 & buf );
	bool PEM_in( CB_get_phrase_base * u_obj, str0 val );

	/*
		when reading a CERT with an encrypted KEY use CN_get_phrase

		when writing, the CERT held in mem, is already crypted
		so the same KEY is already set?
	*/

	bool PEM_from_file( CB_get_phrase_base * u_obj, const char * filename );
	bool PEM_from_file( const char * filename );
	bool PEM_into_file( const char * filename );

	bool CA_set_pubkey( EVP_P_KEY * key );
	bool CA_sign_with_issuers_key( EVP_P_KEY * key );

	bool show_some_info();
	bool show_more_info();
	bool show_subject();
	bool show_issuer();
	bool show_msg_CN(const char * msg);

	const char * get_str_CN(buffer1 & buf);
	const char * get_str_CN(); // static buffer - single threaded OK

};

#endif
