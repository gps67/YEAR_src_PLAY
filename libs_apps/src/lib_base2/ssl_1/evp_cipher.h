#ifndef evp_cipher_H
#define evp_cipher_H

#include "buffer2.h"
#include "openssl/evp.h"

class key_holder; // #include lite

enum en_crypt_tag
{
	en_crypt = 1,
	de_crypt = 0,
	no_change = -1
};

/*!
	EVP is OpenSSL's wrapper over symmetric cipher encryption. (de-)

	evp_cipher [_base] is a wrapper over EVP_CIPHER_CTX * ctx

	The cipher (type) is set first, and rarely changes.
	(you can init without it, and set it asap).
	It is not checked for being unset.

	The key and IV can be set again, as can enc (direction).

	Large streams of encryption might like an event infrastructure,
	but for now, I dont have a need for hi-tide buffer processing,
	nor large files, so this class holds buf_in, buf_out.
	(Maybe they could be parameters, or buf_enc, buf_dec).

	This class is committed to adding padding, so leave a byte

	Initialising it from a key_munger (or better a key_holder),
	is the way to go, as that could then set key_len, if it extra.
	Note that OpenSSL does provide macros to get key_len(),
	but otherwise leaves you on your own, to get it right!

	NB Some ciphers can vary key size. In that case, write a setup_for_app()
	function that sticks to well chosen values, or good defaults.

	Default bf_cbc key/iv/blk is 128 bit/64/64. MAX-448-bit
*/
class evp_cipher_base
{
	bool init_ctx();
 public:
	buffer2 buf_in;
	blk1 buf_out;
	EVP_CIPHER_CTX * ctx;
	en_crypt_tag en_crypt_flag;

	// EVP_			"name"		iv, key, block
	// EVP_CIPHER_bf_cbc()	"bf_cbc" 	64, 128, 64
	// EVP_CIPHER_sha1()	"sha1-cbc-XXX"	160 bits

	~evp_cipher_base();
	evp_cipher_base();
	evp_cipher_base( key_holder & key_munged, en_crypt_tag enc_flag );

	evp_cipher_base( str0 namedtype );
	evp_cipher_base(const EVP_CIPHER * type );

	int cipher_block_size() {
		 return EVP_CIPHER_CTX_block_size(ctx);
	}

	// these are bytes (8,16) hence shorter name (_length _len)
	// they are all get_ from the CTX which gets defaults from CIPHER
	//
	int key_len()   { return EVP_CIPHER_CTX_key_length(ctx); }
	int iv_len()    { return EVP_CIPHER_CTX_iv_length(ctx); }
	int block_len() { return EVP_CIPHER_CTX_block_size(ctx); }
	str0 cipher_name() {
		return EVP_CIPHER_name( EVP_CIPHER_CTX_cipher(ctx)); }

//	bool get_enc() { return ctx->encrypt; } // DIRECT
#warning	bool get_enc();
	bool get_enc();


	str0 dgb_info_str();
	bool dgb_dump_IN(str0 func);
	bool dgb_dump_OUT(str0 func);
	bool dgb_dump_IN_OUT(  blk1 & blk, str0 func, str0 IN_OUT );

	// rework - enc or dec ? _in or out ? calc worse case, both
	// ie to encrypt 27 bytes, the output buffer must be 64 (space)
	// This is not what you want for 16K frames, but fine for streams
	bool get_space_for( int in );

	//
	bool init_from_spec( key_holder & key_munged, en_crypt_tag enc_flag );

// PRIVATE - OR DELETE SOON - design change, now init from key_munged

	bool init( const EVP_CIPHER * type );
	bool init_bf_cbc();
	bool init_cast5();
//	bool init_rc5();
	bool init_aes_128();
	bool init_aes_192();
	bool init_aes_256();
	// idea comes free in 2012
	// des_cbc (etc) not wanted anymore, even VNC/RFB adjusts it
	// CAST
	// RC5
	// EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_SET_RC5_ROUNDS, nrounds, NULL);
	// add what you actually want when you want it
	// BUT beware of OpenSSL excluding algorithms (ifndef)
	// opensslconf.h
	// # define OPENSSL_NO_GMP
	// # define OPENSSL_NO_JPAKE
	// # define OPENSSL_NO_KRB5
	// # define OPENSSL_NO_MD2
	// # define OPENSSL_NO_MDC2
	// # define OPENSSL_NO_RC5
	// # define OPENSSL_NO_RFC3779
	// # define OPENSSL_NO_STORE

	bool set_padding( bool on ); // always ok // default on
	bool set_padding_on() { return set_padding( true ); }
	bool set_padding_off() { return set_padding( false ); }


	bool init_enc( en_crypt_tag enc );
	bool init_encrypt() { return init_enc( en_crypt ); }
	bool init_decrypt() { return init_enc( de_crypt ); }
	bool init_padding( bool on ); // always ok // default on
	bool init_padding_on() { return init_padding( true ); }
	bool init_padding_off() { return init_padding( false ); }
//	bool init_key_iv( ... );
	bool init_key( blk1 & key );
	bool init_iv( blk1 & iv );
	bool init_iv();
	bool init_set_key_length( int len );

	bool update( u8 * mem, int len );
	bool update_from_blk( blk1 & blk ); // const blk
	bool update_from_str( str0 s );
	bool update_from_buf(); // clear own buf_in ready for more

	bool process_entire_buf();

	bool final();

	bool cleanup();

};


#endif


