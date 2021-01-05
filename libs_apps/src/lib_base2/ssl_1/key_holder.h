#ifndef key_holder_H
#define key_holder_H

#include "blk1.h"
#include "buffer2.h"

// #include <openssl/evp.h>
#include "SSL_stubs.h" // CB_get_phrase_base

/*!
	hold a key and describe a cipher, 

	This is the future, because of the way AES
	needs to know ENC/DEC when it initialises keys.

	Other ciphers can switch encr on and off (decrypt)
	without needing to be told the key again,
	and that is why this call is here, to hold the key,
	so that you can tell it evp_cipher.
	(hence this has no special funcs/args, evp_cipher has this as an arg)

	Refactor evp_cipher to only be initialised from this
	(or nothing and late_init), and now that the KEYS are
	held here, scrub here later.

	Repeated use for tight looped (UDP packets) might need
	init_from_key_holder ONCE then reinit_from_unchanged_key_holder(k)
	or changing IV without changin key, ... will vary with non AES

	Holding iv_len and key_len is a repettition, ok.
*/
class key_holder {
 protected:
	const EVP_CIPHER * cipher_type;
 public:
	buffer2 blk_key;	// OUTPUT BLOB 
	buffer2 blk_iv;		// OUTPUT BLOB of NULL
	int iv_len;
	int key_len;
	bool encr;

	~key_holder(); // scrub all fields
	key_holder( const EVP_CIPHER * _cipher_type = NULL );
	bool set_cipher_type( const EVP_CIPHER * cipher_type );
	const EVP_CIPHER * get_cipher_type() { return cipher_type; }
	bool set_len_iv_key( int _iv_len, int _key_len );

	bool copy_from( key_holder & rhs );

//	bool as_cb_phrase( obj_hold<CB_get_phrase_base> & cb_phrase );
};

#endif

