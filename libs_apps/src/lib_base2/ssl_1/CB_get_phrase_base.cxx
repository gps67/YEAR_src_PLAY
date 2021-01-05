#include "CB_get_phrase_base.h"
#include "dgb.h"
#include "evp_cipher.h" // EVP_des_ede3_cbc()

CB_get_phrase_base:: CB_get_phrase_base( const EVP_CIPHER * _enc )
: enc( _enc )
, flg_CB_called( false )
{
#if 0
	if(!enc) enc = EVP_des_ede3_cbc(); // suitable for cert_key.pem
	if(!enc) enc = EVP_bfs_cbc();
	if(!enc) enc = EVP_aes_128_cbc();
#endif
}

// STATIC
const EVP_CIPHER * CB_get_phrase_base::get_enc_for_key_pem()
{
	return EVP_des_ede3_cbc(); // suitable for cert_key.pem
}

bool CB_get_phrase_base:: set_enc_for_key_pem()
{
	enc = get_enc_for_key_pem();
	return true;
}

CB_get_phrase_base:: ~CB_get_phrase_base()
{
}

bool CB_get_phrase_base:: is_null_phrase()
{
	if(!enc) {
		STEP("OK enc is NULL - so no key can be of any use");
		return true;
	}
	STEP("base class presumes that if enc is set, a key is present");
	// derived callses can do own thing
	return false;
}

// its your callback, if you dont want to double check, thats OK

int CB_get_phrase_base:: get_phrase_with_double_check( u8 * buf, int maxlen ) {
	return get_phrase(buf,maxlen);
}


/*!
	OpenSSL uses this callback - 

		PEM_read_bio_X509(...)
*/
int CB_get_phrase_base::C_get_passwd_fn(char *buf, int size, int rwflag, void *u)
{
	// buf : where to place the passphase-key-data
	// size : max sizeof buf
	// rwflag : if r getting it wrong will soon be apparent, if w ask twice
	// u user define object
	//
	// returns int = len of key placed in buf, or 0 if error
	//
	// NOTE:
	//	you have no way of knowing WHAT the object is
	//	except (u) and the usage as a pem_ cb
	CB_get_phrase_base *u_obj = reinterpret_cast<CB_get_phrase_base *>( u );

	if(!u_obj) {
		FAIL("u_obj is NULL");
		return 0;
	}

	u_obj -> flg_CB_called = true;

	if(rwflag)
		return u_obj->get_phrase_with_double_check( (u8*) buf, size );
	else
		return u_obj->get_phrase( (u8*) buf, size );
}

CB_get_phrase_NONE:: 
CB_get_phrase_NONE()
: CB_get_phrase_base(NULL)
{
}

int CB_get_phrase_NONE:: get_phrase( u8 * buf, int maxlen ){
	FAIL("an empty phrase must never be asked for");
	if(buf) buf[0] = 0;
	return 0;
}

bool CB_get_phrase_base::CB_called()
{
	return flg_CB_called;
}

bool CB_get_phrase_base::CB_called_clear()
{
	bool was = flg_CB_called;
	flg_CB_called = false;
	return was;
}
