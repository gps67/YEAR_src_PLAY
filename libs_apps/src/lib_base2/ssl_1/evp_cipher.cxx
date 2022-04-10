
#include "evp_cipher.h"
#include "dgb_fail.h"
#include "key_holder.h"

// #include "buffer2.h"
#include "openssl/evp.h"

// LURK // because DES_fcrypt is deprecated when called through evp_cipher_base
#include <openssl/des.h>

	// http://www.vidarholen.net/contents/junk/vnc.html
	// says each byte of the password is bit-mirrored hilo->olih
	// because that keeps the LSB (B!=C) when losing one bit
	// done as part of the 16 byte challenge
	// so the DES may or may not work for that purpose!!
	// maybe

evp_cipher_base::~evp_cipher_base()
{
	cleanup();
}

evp_cipher_base::evp_cipher_base()
{
	en_crypt_flag = no_change;
	init_ctx();
}

bool evp_cipher_base::init_ctx()
{
	if(!ctx) {
		FAIL("ctx was not NULL");
		ctx = NULL; // in case 
	}

	if(!ctx) {
		ctx = EVP_CIPHER_CTX_new(); // added 2021_04 // how before ?
		if(!ctx) return FAIL("NULL from EVP_CIPHER_CTX_new()");
	}
	EVP_CIPHER_CTX_init( ctx );
	// THERE IS NO RETURN VALUE FROM SSL
	return true;
}

evp_cipher_base::evp_cipher_base(
	key_holder & key_munged,
	en_crypt_tag enc_flag
)
{
	en_crypt_flag = enc_flag;
	init_ctx();
	if(!
		init_from_spec( key_munged, enc_flag )
	) {
		FAIL("CTOR failed - but not throwing");
	}
}


bool evp_cipher_base::init_from_spec( key_holder & key_munged, en_crypt_tag enc_flag )
{
//	NOT SURE if re-init should call init_ctx or not
//	I think not because that simply overwrites with NULs
// ??	init_ctx();
//
	if(!init( key_munged.get_cipher_type() )) return FAIL_FAILED();
	if(!init_enc( enc_flag )) return FAIL_FAILED();
	if(!init_iv( key_munged.blk_iv )) return FAIL_FAILED();
	if(!init_key( key_munged.blk_key )) return FAIL_FAILED();
	return true;
}

evp_cipher_base::evp_cipher_base( str0 namedtype )
{
	init_ctx();
	const EVP_CIPHER * type = EVP_get_cipherbyname((STR0) namedtype );
	if(!type) {

		INFO("NULL value from EVP_get_cipherbyname('%s');", (STR0)namedtype);
		THROW_dgb_fail(namedtype);
	}
	if(!init( type )) FAIL_FAILED(); // should throw
}

evp_cipher_base::evp_cipher_base(const EVP_CIPHER * type )
{
	init_ctx();
	if(!init( type )) FAIL_FAILED(); // should throw
}


/*!
	Setting the CIPHER type, gives us default iv/key/blk sizes
*/
bool evp_cipher_base::init( const EVP_CIPHER * type )
{
	// OpenSSL allows us to specify parameters later
	// by calling the same Init function
	// giving them names makes it understandable
	//

		ENGINE *impl = NULL;
		unsigned char *key = NULL;
		unsigned char *iv = NULL;
		int enc = 0;
	
	bool ok = EVP_CipherInit_ex( ctx, type, impl, key, iv, enc );
	if(!ok) {
// gdb_invoke(false);
		// for me 'des-cbc' is the failing EVP_CIPHER type
		// that is used in VNC password decode
		// there were deprecated warning over there
		// maybe another route to same DES functionality
		FAIL("SSL_ERROR MESSAGE ABOVE");
		INFO("EVP_CIPHER type == '%s'",  EVP_CIPHER_name(type));
		INFO("EVP_CIPHER desc == '%s'",  EVP_CIPHER_description(type));
		INFO("0x%p", this ); 
// DUMPS //
//		INFO("%s", (STR0) dgb_info_str() ); 
		return FAIL("EVP_CipherInit_ex() returned false probably because type");
	}
	if(1) {
		WARN("Forcing padding to 1 ");
		EVP_CIPHER_CTX_set_padding(ctx,1);
	}

	if(1) { INFO("%s", (STR0) dgb_info_str() ); }

	return ok;
}

bool evp_cipher_base::get_enc()
{
	return (en_crypt_flag == en_crypt);
	FAIL("this is not so, was ctx->encrypt");
//	bool get_enc() { return ctx->encrypt; } // DIRECT
//	return EVP_CIPHER_CTX_get_encrypt(ctx); 
	return EVP_CIPHER_CTX_mode(ctx); 
}

str0 evp_cipher_base::dgb_info_str()
{
	static buffer2 buf;
	buf.clear();
	// The KEY has not been set yet, but its default size is known
	buf.print("%s key/iv/blk %d/%d/%d bytes (%d/%d/%d bit)",
	(STR0)	cipher_name(),
		key_len(),
		iv_len(),
		block_len(),
		8*key_len(),
		8*iv_len(),
		8*block_len()
	);
	// INFO("future  iv_len %d (%d bit)", iv_len(), 8*iv_len() );
	// INFO("future key_len %d (%d bit)", key_len(), 8*key_len() );
	return (str0) buf;
}
bool evp_cipher_base::dgb_dump_IN(str0 f) { return dgb_dump_IN_OUT( buf_in, f, "IN"); }
bool evp_cipher_base::dgb_dump_OUT(str0 f) { return dgb_dump_IN_OUT( buf_out, f, "OUT"); }
bool evp_cipher_base::dgb_dump_IN_OUT( blk1 & blk, str0 f, str0 IN_OUT)
{
	str0 str_objname = "(OBJNAME)";
	str0 str_decrypt = "decrypt";
	if( get_enc() ) str_decrypt = "encrypt";

	buffer2 hdr;
	hdr.print( "%s %s %s %s",
		(STR0) str_decrypt,
		(STR0) str_objname,
		(STR0) f,
		(STR0) IN_OUT
	);
	buffer2 db;
	db.clear();
	db.put( blk );
	db.dgb_dump((str0) hdr);

	return true;
}


/*!
	Blowfish cbc is generally recognised as a good choice. fast, safe, etc
*/
bool evp_cipher_base::init_bf_cbc()
{
	return init( EVP_bf_cbc() );
}

/*!
	cast
*/
bool evp_cipher_base::init_cast5()
{
	return init( EVP_cast5_cbc() );
}

/*!
	aes
*/
bool evp_cipher_base::init_aes_128()
{
	return init( EVP_aes_128_cbc() );
}

/*!
	aes
*/
bool evp_cipher_base::init_aes_192()
{
	return init( EVP_aes_192_cbc() );
}

/*!
	aes
*/
bool evp_cipher_base::init_aes_256()
{
	return init( EVP_aes_256_cbc() );
}

/*!
	rc5 
bool evp_cipher_base::init_rc5()
{
	return init( EVP_rc5_32_12_16_cbc() );
}
*/

/*!
	select encode or decode mode (only as part of init_ setup)
*/
bool evp_cipher_base::init_enc( en_crypt_tag enc_flag ) {

	en_crypt_flag = enc_flag;

	int enc = -1;
	switch( enc_flag ) {
	 case en_crypt: enc = 1; break;
	 case de_crypt: enc = 0; break;
	 case no_change: enc = -1; break;
	 default:
		return FAIL("bad value %d for enc_flag", (int) enc_flag );
	};

//	EVP_CIPHER_CTX *ctx,	// 

	const EVP_CIPHER *type = NULL;
	ENGINE *impl = NULL;
	unsigned char *key = NULL;
	unsigned char *iv = NULL;
	
	bool ok = EVP_CipherInit_ex( ctx, type, impl, key, iv, enc );
	if(!ok) return FAIL("EVP_CipherInit_ex()");
	return true;
}

bool evp_cipher_base::init_key(  blk1 & key ) {
	if( key_len() != (int) key.nbytes_used ) {
		return FAIL("Wrong key len expected %d got %d",
			key_len(),
			key.nbytes_used
		);
	}
	if(0) INFO("OK key len expected %d got %d",
		key_len(),
		key.nbytes_used
	);
	const EVP_CIPHER *type = NULL;
	ENGINE *impl = NULL;
//	unsigned char *key,
	unsigned char *iv = NULL;
	int enc = -1;
	
	key.dgb_dump("Putting KEY into CTX");
	if(!EVP_CipherInit_ex( ctx, type, impl, key.buff, iv, enc ))
		return FAIL("EVP_CipherInit_ex()");
	return true;
}

/*
	pass an IV full of NUL bytes to the CTX
*/
bool evp_cipher_base::init_iv() {
	blk1 iv;
	if(1) {
		INFO("PRE-FILLING iv with %d nulls then %d", EVP_MAX_IV_LENGTH, iv_len());
		iv.get_space( EVP_MAX_IV_LENGTH );
		iv.zero_all();
	}
	int N = iv_len();
	if(N==0) {
		return FALSE_dgb_fail("zero length iv");
		// actually, could have a cipher that does not use iv
		// but for me its a code error
	}
	for( int i=0; i<N; i++ ) {
		iv.put_byte( 0 );
	}
	INFO("iv len = %d", iv.nbytes_used );
	return init_iv( iv );
}

/*!
	CTX holds the current IV - set it to our prepared value

	Maybe it would be a good idea to initialise the buffer max
	beyond the previously predicted iv_len(), there or here
	IE Filled with NUL bytes, it is harmless to overflow (under supply)
	upto MAX.
*/
bool evp_cipher_base::init_iv(  blk1 & iv ) {
	if( iv.nbytes_used == 0 ) return init_iv();
	if( iv_len() != (int) iv.nbytes_used ) {
		gdb_invoke(true);
		return FAIL("Wrong iv len expected %d got %d",
			iv_len(),
			iv.nbytes_used
		);
	}
	if(1) {
		INFO("PRE-filling iv with %d nulls then %d", EVP_MAX_IV_LENGTH, iv_len());
		iv.get_space( EVP_MAX_IV_LENGTH );
		iv.zero_all();
	}
//	EVP_CIPHER_CTX *ctx,
	const EVP_CIPHER *type = NULL;
	ENGINE *impl = NULL;
	unsigned char *key = NULL;
//	unsigned char *iv = NULL;
	int enc = -1;
	
	// iv is always NUL currently
	if(0) iv.dgb_dump("Putting IV into CTX");
	if(!EVP_CipherInit_ex( ctx, type, impl, key, iv.buff, enc ))
		return FAIL("EVP_CipherInit_ex()");
	return true;
}

bool evp_cipher_base::init_set_key_length( int len )
{
	bool ok = EVP_CIPHER_CTX_set_key_length(ctx, len);
	if(!ok) {
		return FAIL("need loookup of SSL ERR_ codes"); 
	}
	return true;
}

bool evp_cipher_base:: set_padding( bool on )
{
	int val = 0;
	if( on ) {
		 val = 1;
	} else {
		WARN("Switching padding off - better be fixed length input");
	}
	if(!EVP_CIPHER_CTX_set_padding(ctx,val))
		return FAIL("EVP_CIPHER_CTX_set_padding()");
	return true;
}


bool evp_cipher_base::get_space_for( int in ) {
	//
	// one for a guard NUL and also for when in==0 
	// the guard NUL will be pointless
	// but someone will add it and cause a realloc (maybe)
	// the block size() is expected by SSL
	// inlen probably part of that blocksize so overshoot
	//
	return buf_out.get_space( in + cipher_block_size() - 1 + 1 );
}

bool evp_cipher_base::update( u8 * mem, int inl )
{
	if(!get_space_for( inl )) return FAIL("alloc");

//	str0 str_decrypt = "decrypt";

//	dgb_dump_IN(__FUNCTION__);

	unsigned char *out = buf_out.gap_addr();
	int outl = 0;
	unsigned char *in = mem;

	bool ok = EVP_CipherUpdate(
		ctx,
		out,
		&outl,
		in,
		inl
	);
	buf_out.nbytes_used += outl;
	buf_out.trailing_nul(); // paranoia for text decrypters
//	dgb_dump_OUT(__FUNCTION__);
	if(!ok) {
		return FAIL("EVP_CipherUpdate()");
	}
	return ok;
}

bool evp_cipher_base::update_from_blk( blk1 & blk )
{
	unsigned char *in = blk.buff;
	int inl = (int) blk.nbytes_used; // < 2G
	return update( in, inl );
}

bool evp_cipher_base::update_from_str( str0 s )
{
	return update( s, strlen(s) );
}

bool evp_cipher_base::update_from_buf() 
{
	bool ok = update_from_blk( buf_in );
	buf_in.zero_used(); // overwrite data in with NUL bytes
	buf_in.clear(); // done
	if(!ok) FAIL_FAILED();
	return ok;
}

bool evp_cipher_base::final()
{
	if(!get_space_for( 2 )) return FAIL("alloc");
	unsigned char *out = buf_out.gap_addr();
	int outl = 0;

	if(!EVP_CipherFinal_ex(ctx, out, &outl)) {
		buf_out.trailing_nul(); // paranoia for text decrypters
		FAIL("EVP_CipherFinal_ex()" );
		if(outl) WARN("outl is non zero !!!!!!!!!");
 if(0)		gdb_invoke(true);
		return FAIL("EVP_CipherFinal_ex()" );
	}
	buf_out.nbytes_used += outl;
	buf_out.trailing_nul(); // paranoia for text decrypters


	if(0) dgb_dump_OUT(__FUNCTION__);
	return true;
}

bool evp_cipher_base::process_entire_buf()
{
	if(!update_from_buf()) return FAIL("HERE");
	if(!final()) return FAIL("HERE");
	return true;
}

/*!
	deletes sensitive information ?
*/
bool evp_cipher_base::cleanup()
{
	EVP_CIPHER_CTX_cleanup(ctx);
	buf_in.scrub();
	buf_out.scrub();
	return true;
}




