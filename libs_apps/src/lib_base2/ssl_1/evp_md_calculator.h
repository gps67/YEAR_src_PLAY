#ifndef evp_md_calculator_H
#define evp_md_calculator_H

#include "buffer2.h"
#include "SSL_stubs.h"

/*!
	compute the MD message digest using OpenSSL EVP
*/
class md_calculator_base
{
 public:
//	input buffer is callers problem
//	buffer2 buf;
	EVP_MD_CTX * ctx;
//	output md is retained here
	blk1 md;

	// EVP_ripemd160()	"ripemd" 	160 bits
	// EVP_sha1()		"sha1"		160 bits
	// EVP_md5()		"md5"		128 bits

	~md_calculator_base();
	md_calculator_base();
	md_calculator_base(const EVP_MD * type );

	static bool lookup_md_name( const EVP_MD *& val, str0 md_name );

	bool ctx_new();
	bool ctx_del();
	bool init( const EVP_MD * type );
	bool init_md_name( str0 md_name );
	bool init_md5();
	bool init_ripemd160();
	bool init_sha1();
	bool init_sha256();
	bool init_sha512();
	bool init_whirlpool();	// 512

	bool update( u8 * mem, int len );
	bool update_from_str( str0 s );
	bool update_from_buf( blk1 & buf );
	bool final_into( blk1 & buf );
	bool final();
	int md_size_bytes();
};


#endif


