#ifndef CB_get_phrase_base_H
#define CB_get_phrase_base_H

#ifndef OBJ_REF_H
#include "obj_ref.h"
#endif
#include "SSL_stubs.h"

/*!
	OpenSSL opening an encrypted PEP file wants it decrypted

	eg X509 certificate needs KEY

	eg KEY needs KEY

	slight misnomer, maybe rename "phrase"
	where phrase can be upto 1024 bytes of stuff,
	maybe

	You need to know which key/OBJ to apply to which (mem/file),
	and the default one might not be the one you want,
	but all such things use this callback system.

	When decrypting, no double check is needed, it will fail.

	When encrypting, double check is needed, else data lost!
	It is up to the UI to ask twice and be sure about the new secret.
*/
class CB_get_phrase_base : public obj_ref0
{
 public:
	const EVP_CIPHER * enc; // the encoding algorith to use (if any)

	CB_get_phrase_base( const EVP_CIPHER * _enc );
	~CB_get_phrase_base();

 static
	const EVP_CIPHER * get_enc_for_key_pem();
	bool set_enc_for_key_pem();

	// used to detect when the KEY was not crypted
	bool flg_CB_called;
	bool CB_called();
	bool CB_called_clear();

	virtual int get_phrase( u8 * buf, int maxlen ) = 0 ;
	virtual int get_phrase_with_double_check( u8 * buf, int maxlen );

	// you have to call the functions with a NULL callback fn
	// you cannot return a NULL phrase - that is too late!
	
	virtual bool is_null_phrase();
	bool phrase_exists() { return !is_null_phrase(); }

	static int C_get_passwd_fn(char *buf, int size, int rwflag, void *u);
};


class CB_get_phrase_NONE : public CB_get_phrase_base
{
 public:
	CB_get_phrase_NONE();
	~CB_get_phrase_NONE(){}

	virtual int get_phrase( u8 * buf, int maxlen );
	virtual bool is_null_phrase() { return true; }

};

#endif

