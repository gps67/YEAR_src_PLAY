#include "evp_md_calculator.h"
#include <openssl/evp.h>

// #include "file_stat.h"
#include "dgb_fail.h"
// #include "SSL_global.h"
// #include "fd_hold.h"
// typedef fd_hold_1 fd_hold;

typedef long long i64;



// EVP_ripemd160()	"ripemd" 	160 bits
// EVP_sha1()		"sha1"		160 bits
// EVP_md5()		"md5"		128 bits

md_calculator_base::md_calculator_base()
:ctx(NULL)
{
	ctx_new();
	WARN("dont forget - type not yet set");
	init( EVP_md_null() );
}
md_calculator_base::md_calculator_base(const EVP_MD * type )
:ctx(NULL)
{
	ctx_new();
	if(!type) {
		WARN("poor choice of default EVP_md_null()");
		type = EVP_md_null();
	}
	init( type );
}

md_calculator_base::~md_calculator_base()
{
	ctx_del();
}

bool md_calculator_base::ctx_new()
{
	if(ctx) {
		FAIL("ctx already non null");
		ctx_del();
	}
	WARN("ctx has become opaque");
	ctx = EVP_MD_CTX_create();
	if(!ctx) return FAIL("EVP_MD_CTX_create()");
	return true;
}
bool md_calculator_base::ctx_del()
{
	if(!ctx) {
		return FAIL("ctx is NULL");
	}
	EVP_MD_CTX_destroy(ctx);
	ctx = NULL;
	return true;
}

#if 0
/*
	This is because ubuntu keeps using 0.9.8 not 1.0.0.
	and whirlpool is not yet available
*/
#warning EVP_whirlpool returning NULL
static /* C */ const EVP_MD * EVP_whirlpool()
{
	FAIL("STUBBED OUT NULL");
	return NULL;
}
#endif

// static
static /* C */ const EVP_MD * lookup_md_name_2( str0 s )
{

	if(( s == "whirlpool"))	return EVP_whirlpool();
	if(( s == "sha1")) 	return EVP_sha1();
	if(( s == "sha256"))	return EVP_sha256();
	if(( s == "sha512"))	return EVP_sha512();
	if(( s == "ripemd160"))	return EVP_ripemd160();
	if(( s == "md5"))	return EVP_md5();

	if(( s == "EVP_whirlpool"))	return EVP_whirlpool();
	if(( s == "EVP_sha1")) 		return EVP_sha1();
	if(( s == "EVP_sha256"))	return EVP_sha256();
	if(( s == "EVP_sha512"))	return EVP_sha512();
	if(( s == "EVP_ripemd160"))	return EVP_ripemd160();
	if(( s == "EVP_md5"))		return EVP_md5();

	if(( s == "md5"))	return EVP_md5();
	if(( s == "md5"))	return EVP_md5();
	if(( s == "md5"))	return EVP_md5();
	FAIL("BAD MD NAME '%s' (eg try 'md5')", (STR0) s );
	return NULL;
}

// static // C++ method
bool md_calculator_base::lookup_md_name( const EVP_MD *& val, str0 s )
{
	val = lookup_md_name_2( s );
	if(val) return true;
	val = EVP_whirlpool(); // not NULL
	return FAIL_FAILED();
}

bool md_calculator_base::init_md_name( str0 md_name )
{
	const EVP_MD * md_type = NULL;
	if(!lookup_md_name( md_type, md_name )) {
	 //	init_whilpool();
		return FAIL_FAILED();
	}
	return init( md_type );
}

bool md_calculator_base::init( const EVP_MD * type )
{
	if(ctx) {
	}
	if(!type) {
		return FAIL("null type");
	}
	EVP_DigestInit( ctx, type );
	return true;
}
bool md_calculator_base::init_md5()
{
	return init( EVP_md5() );
}
bool md_calculator_base::init_ripemd160()
{
	return init( EVP_ripemd160() );
}
bool md_calculator_base::init_sha1()
{
	return init( EVP_sha1() );
}
bool md_calculator_base::init_sha256()
{
	return init( EVP_sha256() );
}
bool md_calculator_base::init_sha512()
{
	return init( EVP_sha512() );
}
bool md_calculator_base::init_whirlpool()
{
	return init( EVP_whirlpool() );
}

bool md_calculator_base::update( u8 * mem, int len )
{
	if(EVP_DigestUpdate( ctx, mem, len )) {
		return true;
	} else {
		// ERR_ //
		return FAIL("EVP_DigestUpdate()");
		return false;
	}
}

bool md_calculator_base::update_from_str( str0 s )
{
	return update( s, s.str_len() );
}

bool md_calculator_base::update_from_buf( blk1 & buf )
{
	if(!update( buf.buff, buf.nbytes_used )) return FAIL("HERE");
	buf.clear();
	return true;
}

bool md_calculator_base::final()
{
	return final_into( md );
}

bool md_calculator_base::final_into( blk1 & buf )
{
	buf.clear();
	if( !buf.get_space( EVP_MAX_MD_SIZE )) {
		return FAIL("md_calculation get_space()");
	}
	unsigned int len = 0;
	if(!EVP_DigestFinal_ex( ctx, buf.buff, & len )) {
		return FAIL("!EVP_DigestFinal_ex()");
	}
	buf.nbytes_used = (int) len;
	return true;
}

int md_calculator_base::md_size_bytes()
{
	return (int) EVP_MD_CTX_size(ctx);
	// ie  EVP_MD_size((e)->digest)
	// ie	((e)->digest->md_size)
}


