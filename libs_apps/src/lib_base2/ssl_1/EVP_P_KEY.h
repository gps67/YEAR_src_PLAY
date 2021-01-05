#ifndef EVP_P_KEY_H
#define EVP_P_KEY_H

#include "obj_ref.h"
#include "SSL_stubs.h"
#include "CB_get_phrase.h"

/*!
	hold a EVP_PKEY private key, and its public side.

	hold/apply the Phrase+method that protexts the private key (CB=callback)

	The key is probably RSA, but could be DSA.

	load (save) it from files and memory

	making this a POINTED thing needs obj_hold ...

	This class also tracks if the CB was used during load (from buffer/file)
	which tells you if the file was safely encrypted on disk, or not.
*/
class EVP_P_KEY : public obj_ref0
{
 public:

	//! The openSSL envelope object that is the Public/Private Key
	EVP_PKEY * pkey;

	//! The callback password (after key_munger) to en-de-crypt on load-save
	obj_hold<CB_get_phrase_base> cb_pass;

	//! Did the very recent load use the encryption callback?
	bool KEY_was_crypted; // useful to know

	~EVP_P_KEY();

	EVP_P_KEY();
	EVP_P_KEY( CB_get_phrase_base & _cb_pass );
	EVP_P_KEY( CB_get_phrase_base * _cb_pass );
	void set_cb_pass( CB_get_phrase_base * _cb_pass );

	void set( EVP_PKEY * _pkey );

	bool is_DSA();
	bool is_RSA();
	const EVP_MD * get_correct_digest_method_for_X509_sign();

	bool read_from_file( const char * filename );
	bool read_from_buffer( buffer2 & buf );
	bool read_from_pem_text( str0 pemtext );

	/* TODO - it really does not need to be a buffer3, 

		But add binary-asn1 DER (instead of text PEM)
		and then provide blk1 and others.

		WITH buffer3 THE POINTER IS ADVANCED, text is 'eaten'
	 */
	
	//! create a new RSA key of nbits
	bool gen_RSA_nbits( int nbits );
	bool gen_RSA_1024() { return gen_RSA_nbits( 1024 ); }
	bool gen_RSA_2048() { return gen_RSA_nbits( 2048 ); }

	//
	// these are all PEM format
	// it would also be good to get 2048/8 raw bytes into a blk1
	// after decode
	// but is it RSA or DSA ...
	
	bool write_PrivateKey( blk1 & buf );
	bool write_PublicKey( blk1 & buf );

	bool write_PrivateKey( const char * filename );
	bool write_PublicKey( const char * filename );
	bool read_PrivateKey( const char * filename ) {
		return  read_from_file( filename );
	}

	bool check_is_private_key();
	bool test_write_read_PrivateKey( const char * filename );

};

#endif
