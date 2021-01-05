
#include "EVP_P_KEY.h"
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include "util_buf.h"
#include "BIO_mem.h"

EVP_P_KEY::~EVP_P_KEY()
{
	set(NULL);
}

EVP_P_KEY::EVP_P_KEY()
{
	pkey = NULL;
	KEY_was_crypted = false;
}

EVP_P_KEY::EVP_P_KEY( CB_get_phrase_base & _cb_pass )
{
	pkey = NULL;
	KEY_was_crypted = false;
	set_cb_pass( &_cb_pass );
}

EVP_P_KEY::EVP_P_KEY( CB_get_phrase_base * _cb_pass )
{
	pkey = NULL;
	KEY_was_crypted = false;
	set_cb_pass( _cb_pass );
}

void EVP_P_KEY:: set_cb_pass( CB_get_phrase_base * _cb_pass )
{
	KEY_was_crypted = false;
	cb_pass = _cb_pass;
	if(cb_pass) cb_pass -> CB_called_clear();
}

void EVP_P_KEY:: set( EVP_PKEY * _pkey )
{
	KEY_was_crypted = false;
	if(_pkey == pkey) 
		return;
	if(pkey) {
		EVP_PKEY_free(pkey);
		pkey = NULL;
	}
	pkey = _pkey;
}

bool EVP_P_KEY:: is_DSA()
{
	if(!pkey) {
		FAIL("null pkey");
		return false;
	}
	return EVP_PKEY_id (pkey) == EVP_PKEY_DSA;
	// was // return EVP_PKEY_type (pkey->type) == EVP_PKEY_DSA;
	// https://stackoverflow.com/questions/41097650/how-to-get-the-type-of-an-evp-pkey
}

bool EVP_P_KEY:: is_RSA()
{
	if(!pkey) {
		FAIL("null pkey");
		return false;
	}
	return EVP_PKEY_id (pkey) == EVP_PKEY_RSA;
}

const EVP_MD * EVP_P_KEY:: get_correct_digest_method_for_X509_sign()
{
	if(is_DSA()) {
		return (EVP_MD*) EVP_sha1();
		// was // return (EVP_MD*) EVP_dss1();
	} else
	if(is_RSA()) {
		return (EVP_MD*) EVP_sha1();
	} else {
		FAIL("KEY is neither RSA nort DSA - dont know what digest to use");
		return EVP_sha1();
		return NULL;
	}
}

bool EVP_P_KEY::read_from_file( const char * filename_key )
{
	/*
		I know that openSSL has a function that does this,
		but I want to be certain that the BIO_mem_read class works.
		It if didnt, neither would.
	*/
	debug_set_obj_name( filename_key );

	u32 max_K_size = 60;
	buffer2 mem_in;
	if(!blk_read_entire_file( mem_in, filename_key, max_K_size )) {
		return FAIL("%s", filename_key );
	}

	bool ok = read_from_buffer( mem_in );

	return ok;
}

bool EVP_P_KEY:: read_from_buffer( buffer2 & mem_in )
{
	/*
		TODO - advance the mem_in pointers ... or drop to buffer2
	*/
	if( cb_pass ) cb_pass -> CB_called_clear();
	KEY_was_crypted = false;
	BIO_mem_read bio_mem( mem_in );

	if(cb_pass && cb_pass -> phrase_exists() ) {
		// it is OK for enc to be NULL - PEM defines enc
		PEM_read_bio_PrivateKey(
			bio_mem,
			&pkey,            // EVP_PKEY **x,
			CB_get_phrase_base::C_get_passwd_fn,
			cb_pass
		);
	} else {
		PEM_read_bio_PrivateKey(
			bio_mem,
			&pkey,            // EVP_PKEY **x,
			NULL,
			NULL
		);
	}

	if(pkey) {
		if( cb_pass && cb_pass -> CB_called() )
			KEY_was_crypted = true;
		INFO(
			"PEM_read_bio_PrivateKey( %s )",
			debug_get_obj_name()
		);
		return true;
	} else {
		return FAIL(
			"PEM_read_bio_PrivateKey( %s )",
			debug_get_obj_name()
		);
	}
	// unreached
}

bool EVP_P_KEY:: read_from_pem_text( str0 pemtext )
{
	buffer2 pembuff;
	pembuff.set( pemtext );
	if( !read_from_buffer( pembuff ) ) return FAIL_FAILED();
	return true;
}


int a_kept = -1;
void C_progress_genkey( int a, int b, void * cb_arg )
{
	return;
	/*
		The above return makes this a silent callback

		On UNIX at least, (binutils, ldd.so), you can override this,
		by defining your own function, with the same name,
		and linking, with options like those used in ../samples/2010/..

		The linker uses your.o before the shared lib, neat!

		The following example code, needs a newline,
		but only once after being called many times
	*/

        if( a_kept != a ) {
		if( a_kept != -1 ) printf("\n");
                a_kept = a;
                printf("C_progress_genkey( %d, ", a );
        }
        printf("%d",b); // basically just counts to unknown varying ceiling
        printf(".");
        fflush(0);
}

bool EVP_P_KEY:: gen_RSA_nbits( int nbits )
{
	INFO( "nbits = %d", nbits );
	unsigned long e = RSA_F4; // 3 17 or 65537

	BIGNUM * bne = BN_new();
	if(!bne) {
		return FAIL("BN_new()");
	}
		
	if(!BN_set_word(bne,e)) {
		BN_clear_free(bne);
		return FAIL("BN_set_word(bne,e)");

	}
	RSA * rsa = RSA_new();
	if(!rsa) {
		BN_clear_free(bne);
		RSA_free(rsa);
		return FAIL("RSA_new()");
	}
	if(!RSA_generate_key_ex(rsa, nbits, bne, NULL)) {
		BN_clear_free(bne);
		RSA_free(rsa);
		return FAIL("!RSA_generate_key_ex()");
	}
	BN_clear_free(bne);
	bne = NULL;

//	void (*callback)(int,int,void *) = C_progress_genkey;
//	void * cb_arg = NULL;
//	RSA *rsa = RSA_generate_key ( nbits, e, callback, cb_arg);
//	// this newline is irritating, if C_progress_genkey was silent
//	// probably doesnt work if a_kept is not written elsewhere
//	if( a_kept != -1 ) {
//		printf("\n");
//		fflush(0);
//		a_kept = -1;
//	}
//	if(!rsa) return FAIL("RSA_generate_key()");

	EVP_PKEY *pkey1 = EVP_PKEY_new();
	// check NULL
	EVP_PKEY_set1_RSA( pkey1, rsa );
	// ref count ? delete rsa ?

	set( pkey1 );
	return true;
	//
	// http://www.codepool.biz/how-to-use-openssl-generate-rsa-keys-cc.html
	// bloody openSSL keeps changing
	// source of bugs
	//
}

bool EVP_P_KEY:: write_PrivateKey( blk1 & buf )
{
	STEP("to buffer");
	//
	// The encryption method to use
	// also option PEM PKCS8
	//

	if(!pkey) {
		return FAIL("NULL pkey - nothing to write!");
	}

	KEY_was_crypted = false;
	BIO_mem bio_mem;
	//
// gdb_invoke(true);
	bool use_enc = false;
	if(cb_pass) {
		cb_pass -> CB_called_clear(); // POINTLESS
		if( cb_pass -> phrase_exists() ) {
			if( cb_pass -> enc ) {
				use_enc = true;
			} else {
				FAIL("no encryption method set");
			}
		}
	}
	if(use_enc) {
#warning this crashed - when called with EVP_bf_cbc() not aes_256 cb_pass->enc
		if(!PEM_write_bio_PKCS8PrivateKey(
			bio_mem, //operaotor BIO *
			pkey,
			cb_pass->enc,
			NULL,	// kstr phrase is NULL kstr,
			0,	// kstr len is 0 klen,
			CB_get_phrase_base::C_get_passwd_fn, // callback
			cb_pass	// callback parameter (obj)
		))
			return FAIL("PEM_write_bio_PrivateKey()");
		if(!cb_pass -> CB_called()) {
			FAIL("YEAH but NO but *****************************");
			FAIL("YEAH but NO but ** CB_called not set properly");
			FAIL("YEAH but NO but *****************************");
		}
		KEY_was_crypted = true;
	} else {
		if(!PEM_write_bio_PKCS8PrivateKey(
			bio_mem, //operaotor BIO *
			pkey,
			NULL,	// cb_pass->enc,
			NULL,	// kstr phrase is NULL kstr,
			0,	// kstr len is 0 klen,
			NULL,	// C_function
			NULL	// obj
		))
			return FAIL("PEM_write_bio_PrivateKey() without phrase returned FALSE");
	}

	if(!bio_mem.write_to_buffer( buf )) return FAIL_FAILED();
	return true;
}

bool EVP_P_KEY:: write_PublicKey( blk1 & buf )
{
	STEP("to buffer");
	BIO_mem bio_mem;
	if(!PEM_write_bio_PUBKEY( bio_mem, pkey ))
		return FAIL("!PEM_write_bio_PUBKEY()");

	if(!bio_mem.write_to_buffer( buf )) return FAIL_FAILED();
	return true;
}

/*!
	REVIEW: you could argue, that the filename (or other)
	has a loose association with the enc method.
	ie when written to <file1> use <tripledes.PEM>
	ie when written to <file2> use <plain.DER>
	ie when written to <fifo3> use <plain.u8[16]> // or 32
	but its always in PEM using the selected CB phrase+method
*/
bool EVP_P_KEY:: write_PrivateKey( const char * filename )
{
	INFO("%s", filename );
	// gdb_invoke(true);
	blk1 buf;
	if(! write_PrivateKey( buf ) ) return FAIL_FAILED();
	return blk_write_to_file_mask( buf, filename, 0600 );
}

bool EVP_P_KEY:: write_PublicKey( const char * filename )
{
	INFO("%s", filename );
	blk1 buf;
	if(!write_PublicKey( buf )) return FAIL_FAILED();
	return blk_write_to_file( buf, filename );
}

bool EVP_P_KEY:: check_is_private_key()
{
	buffer2 PEMBUFF;
	if(!write_PrivateKey( PEMBUFF )) return FAIL_FAILED();
	return true;
}

bool EVP_P_KEY:: test_write_read_PrivateKey( const char * filename )
{
	if(!write_PrivateKey( filename )) return FAIL_FAILED();

	if(!read_PrivateKey( filename )) return FAIL_FAILED();

	// yes its encrypted
	bool crypted = KEY_was_crypted;
	if(crypted) {
		INFO("KEY was encrypted");
	} else {
		WARN("KEY was NOT encrypted");
	 if(0)	return FAIL("KEY was NOT encrypted");
	}
	return PASS("OK");
}


