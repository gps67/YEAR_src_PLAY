#include "key_holder.h"

#include "dgb_fail.h"
#include "util_buf.h"
#include <openssl/evp.h>


key_holder::key_holder( const EVP_CIPHER * _cipher_type )
{
	if(!_cipher_type) {
		INFO("DEFAULT CIPHER TYPE is bf");
		_cipher_type = EVP_bf_cbc();
	}
	if(!set_cipher_type( _cipher_type ))
		THROW_dgb_fail("CTOR set_cipher_type()");
}

key_holder::~key_holder()
{
	cipher_type = NULL;
	blk_key.scrub();
	blk_iv.scrub();
	iv_len = 1;
	key_len = 1;
	encr = false;;
}

bool
key_holder::
copy_from( key_holder & rhs )
{
	/*
		Ironically, this is probably identical to the auto generated
		(or would be if I assigned them to these sub-methods)
		field-by-field copy, from the compiler, but its reassuring
		to know that any 'special magic' could be added here.
	*/
	cipher_type = rhs.cipher_type;
	// its the blks that have a COPY issue ?
	// I copy them very rarely
	if(!blk_key.set( rhs.blk_key )) return FAIL_FAILED();
	if(!blk_iv.set( rhs.blk_iv )) return FAIL_FAILED();
	iv_len = rhs.iv_len;
	key_len = rhs.key_len;
	encr = rhs.encr;
	if(!cipher_type) return FAIL("CIPHER TYPE IS NULL - what use is this key?");
	return true;
}

/*!
	the cipher_type also tells the iv_len and key_len,
	but for those ciphers that have a VARIABLE key kength,
	you will have to figure out virtuals that change the key_len
	here, and later in the evp_cipher_base( key_munger )

	As a kludge, you could eg enforce only NN bits for bf()
	as though that was the only option (for your app),
	and set that here, and there.
*/
bool key_holder::set_cipher_type( const EVP_CIPHER * _cipher_type )
{
	cipher_type = _cipher_type;
	if(!cipher_type) return FAIL("NULL cipher_type");
	if(!set_len_iv_key(
		EVP_CIPHER_iv_length(cipher_type),
		EVP_CIPHER_key_length(cipher_type)
	)) {
		return FAIL("iv/key len");
	}
	return true;
}

bool key_holder::set_len_iv_key( int _iv_len, int _key_len ) {
	// scrub() out any old value - then free() releases them, clear()
	// you must do that first because free() might happen
	// you then do it after get_space() for luck
	blk_key.zap();
	blk_iv.zap();
	if(1) {
		// just incase I get the IV_LEN wrong
		// extend to MAX with NUL
		// so that it is the same wrong every time
		blk_iv.get_space( EVP_MAX_IV_LENGTH );
		blk_key.get_space( EVP_MAX_KEY_LENGTH );
	}
	blk_iv.get_space( _iv_len );
	blk_key.get_space( _key_len );
	// FILL space with NUL bytes incase they actually get used
	blk_iv.zero_all();
	blk_key.zero_all();
	key_len = _key_len;
	iv_len = _iv_len;
	// complain after using the values though ...
	if( key_len <= 0 ) {
		return FAIL("key_len %d < 0", key_len );
	}
	if( iv_len < 0 ) {
		return FAIL("iv_len %d  < 0", iv_len );
	}
	if( key_len > EVP_MAX_KEY_LENGTH ) {
		return FAIL("key_len %d  > EVP_MAX_KEY_LENGTH", key_len );
	}
	if( iv_len > EVP_MAX_IV_LENGTH ) {
		return FAIL("iv_len %d  > EVP_MAX_IV_LENGTH", iv_len );
	}
	return true;
}

/*!
	convert this 256bit AES secret (held in this secret key_holder),
	into a cb_phrase, to be used to decrypt (en-) an RSA key.

	So that is 3 keys:

	(3) a plain phrase "my secret pass phrase to my rsa key"
	(2) a munged secret blob hexbyte[16] in this key_holder
	(1) an RSA key encrypted by this key_holder

	The key_holder (this) must already have a suitable RSA enc
	if you want to use standard PKS PEM files
bool key_holder:: as_cb_phrase( obj_hold<CB_get_phrase_base> & cb_phrase )
{
	cb_phrase = new CB_get_phrase_key_holder( *this );
	return true;
}
*/
