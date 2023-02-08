#include "key_munger_base.h"
#include "dgb.h"
#include "ASCII_chars.h"

#include "evp_md_calculator.h"
#include "dgb.h"
#include "dgb_fail.h"
// #include "evp_cipher.h"
// #include "util_buf.h"

key_munger_base::key_munger_base()
: cipher_type(NULL)
, md_type( NULL )
{
}

key_munger_base::~key_munger_base()
{
}

key_munger_base::key_munger_base( 
	const EVP_CIPHER * _cipher_type,
	const EVP_MD * _md_type,
	str0 _system,
	str0 _purpose
)
: cipher_type( _cipher_type )
, md_type( _md_type )
, blk_system( _system )
, blk_purpose( _purpose )
{
}

bool key_munger_base::set_cipher_type( const EVP_CIPHER * _cipher_type )
{
	cipher_type = _cipher_type;
	return true;
}

bool key_munger_base::set_system(str0 system)
{
	blk_system = system;
	return true;
}


bool key_munger_base::set_purpose( str0 purpose ) {
	blk_purpose.set( purpose );
	if( !purpose ) {
		WARN("NULL purpose leaves phrase without modification");
	}
	return true;
}

/*!
	The real phrase can be a binary block, str0 is for convenience
*/
bool key_munger_base:: make_key_from_phrase( key_holder & key, str0 phrase_str )
{
	buffer2 phrase;
	phrase.set( phrase_str );
	return make_key_from_phrase( key, phrase );
}

bool key_munger_base:: check_phrase_isnt_daft( blk1 & phrase )
{
	bool USED = (bool) phrase;
	if( !phrase ) { 
		// keep the above tests before user data is at fault
		return FAIL("NULL pass phrase");
	}
	if( phrase.nbytes_used < 4 ) { 
		// dont reveal secret, but be informative
		return FAIL("phrase.len = %d", phrase.nbytes_used );
	}
	return true; // not fantastic, but not daft
}

bool key_munger_base:: make_buffer_from_phrase( buffer2 & blk_phrase_plus, blk1 & phrase )
{

	//
	// MD can handle NUL bytes - no problem
	// Prevent string keys from possibly merging
	//
	// TODO: if(!put_byte()) return FAIL_FAILED();// malloc error
	//
	// but do we want other tools to get a single aggregate phrase ?
	// that they can handle, using say '&COMMA;' as a separator,
	// too bad, its binary around here, and this function against itself!
	// not the option of binary system purpose and phrase ;-)
	//
	// (but the result will probably have NUL bytes removed, and base64)
	//

	// NO - this prevents use of plain phrase, without multi-munging
	// blk_phrase_plus.put_byte( ASCII_NUL );
	//
	if( blk_system ) {
		blk_phrase_plus.put( blk_system );
		blk_phrase_plus.put_byte( ASCII_NUL );
	}
	if( blk_purpose ) {
		blk_phrase_plus.put( blk_purpose );
		blk_phrase_plus.put_byte( ASCII_NUL );
	}
	if( phrase ) {
		blk_phrase_plus.put( phrase );
		blk_phrase_plus.put_byte( ASCII_NUL );
	}
	return true;
}

/*!
	all system go, in 3-2-1
*/
bool key_munger_base:: zap_prep( key_holder & key )
{
	/*
		if relying on defaults, ensure they are in place
		or at least code wont trip up over NULL
	*/

	if(!cipher_type) {
		WARN("KEY_MUNGER: cipher_type was not specified");
		cipher_type = EVP_bf_cbc(); // a good default
	}

	if(!md_type) {
		WARN("KEY_MUNGER: md_type was not specified");
	 //	md_type = EVP_whirlpool(); // 512 bits is a lot
		md_type = EVP_sha1(); // 512 bits is a lot
	}


	/*
		// remove this as it is done by
		//	 key.set_cipher_type( cipher_type );
		//
		// clear() and protect any previous values from leaking out
		//
		key.blk_iv.clear();	// zero length
		key.blk_key.clear();	// zero length
		key.blk_iv.zero_all();	// secure erase
		key.blk_key.zero_all();	// secure erase
	*/
	//	if(!key.blk_key.get_space( EVP_MAX_KEY_LENGTH )) return false;;
	/*
		grow the ZERO key and iv to the correct NBYTES
	*/
	key.set_cipher_type( cipher_type ); // will zap old mem again!

	/*
		warn/reject algorithms that use less than 32/128 bits
	*/
	if( key.key_len < 16 ) {
		WARN("requested key_len %d is short", key.key_len);
	}
	if( key.key_len < 4 ) {
		return FAIL("requested key_len %d is bad", key.key_len);
	}

	return true;
}

bool key_munger_base:: make_key_from_phrase( key_holder & key, blk1 & phrase )
{

	//
	// zap out old memory
	// allocate space for key and iv 
	// ensure cipher_type is set
	// ensure that md_type is set
	// double check that final key size cipher isnt daft
	//
	if(!zap_prep(key)) return FAIL_FAILED();

	/*
		reject really bad passwords, as coding errors
		but it is up to the caller to check first
	*/
	if( !check_phrase_isnt_daft(phrase) ) {
		return FAIL_FAILED();
	}

	/*
		The virtual function can be overruled,
		so that you can combine extra bits differently

		blk_phrase_plus is the phrase plus the other strings
	*/
	buffer2 blk_phrase_plus;
	if(!make_buffer_from_phrase( blk_phrase_plus, phrase )) {
		return FAIL_FAILED();
	}

#if 0
#warning insecure - passwords will be visible in traces
	// show the secret key in ASCII
	blk_phrase_plus.dgb_dump( "SECRET INTO MD - key_munger" );
#endif

	/*
		make key from buffer, via md, then no_NUL_bytes
	*/

	md_calculator_base md( md_type );
	md.update_from_buf( blk_phrase_plus );
	md.final();

	// show the secret BINARY key (from ASCII)
	if(0) md.md.dgb_dump( "MD FROM SECRETs" );

	key.blk_key.set( md.md ); // blk1 does not know to stretch or fold
	md.md.zero_all(); // md doesnt normally erase data, here it must
	blk_phrase_plus.zap(); // blk1 doesnt normally erase data, here it must

	// stretch or reduce key and iv to exact required size
	// also key will NOT then contain NUL bytes (iv will)
	
	if(!key_fold_to_new_len( key.blk_key, key.key_len )) return false;
	if(!iv_fold_to_new_len( key.blk_iv, key.iv_len )) return false;
	return true;
}



//////////////////////////////////////////////////////////////////////

/*
	key_munger - converts ASCII to BLOB fit for a KEY

	key_folder - truncates 20 byte MD to 16 byte suitable for key

	key_folder - extends 4 byte CRC to 16 byte usable (but how suitable?)

	iv_folder - extends 4 byte CRC to 8 byte IV

	iv_folder - extends 0 byte nothing to 8 byte IV full of NUL's

*/

/*!
	fold_iv like fold_key, but happier if zero bytes initially,
	whereas key_folder refuses (quite rightly) to go that far beyond
	policy. Then goes to common code, for now.
*/
bool iv_fold_to_new_len( blk1 & b, int n2 )
{
	if(b.nbytes_used ==   0 ) {
		if(0) INFO("filling entire IV with NUL bytes");
		for( int i=0; i< n2; i++ )
			b.put_byte( 0 );
		// could return true; here, but tracers may help double check
	}
	return blk_fold_to_new_len( b, n2 );
}

/*!
	fold key - eg ripe-md-160 generates 160 bits, blowfish wants 128 bits

	In addition, overwrite NUL bytes with ('.') so that it is a string!

	On the one hand, that is a weakening, on the other hand,
	it allows passwords to be closer to binary, and almost strings.

	IV does not need this, only key
*/
bool key_fold_to_new_len( blk1 & md, int n2 )
{
	if(! blk_fold_to_new_len( md, n2 )) return FAIL_FAILED();
	md.replace_nul_bytes('.');
	md.trailing_nul();
	return true;
}

/*!
	fold a key (or IV) to shorter (or longer) length, or ASIS.

	This truncation algorithm (wrap and xor) cannot be changed
	without inconvenience, and is centralised here, to be consistent.

	Rewrite as blk_in blk_out ? (also clearer logic)
*/
bool blk_fold_to_new_len( blk1 & md, int n2 )
{
	if( n2 < 1 ) {
		return FAIL("n2 %d zero or negative", n2 );
	}
	if( n2 > EVP_MAX_MD_SIZE ) {
		return FAIL("n2 %d excess", n2 );
	}
	int n1 = md.nbytes_used;
	if( n1 < 4 ) {
		return FAIL("n1 %d too low", n1 );
	}
	int n_add = n2 - n1;
	if( !n_add ) {
		if(0) INFO("exact key size already %d", n2); // a good thing
		return true;
	}
	if( 0 <= n_add ) {
		if(!md.get_space( n_add )) return FAIL("get_space()");
		for( int i=0; i<n_add; i++ ) {
		//	md.put_byte(0); pad with NUL bytes
			// repeat starter bytes
			md.put_byte(md.buff[i]);
		}
		return true;
	}
	// [0[n2[n1 // shortening buffer n2 < n1 // not ==
	int n_sub = -n_add;
	int l = 0-1; // first dest left (-1)
	int r = n2-1; // first source right (-1)
	for( int i=0; i<n_sub; i++ ) {
		l++;
		r++;
		// destinatin left circles within its [0[n2 zone
		// this 'could' happen any number of times 
		if( l >= n2 ) l = 0;
		// [0[n2[n1 // n2<n1 // so this coding error never happens
		if( l >= n1 ) return FALSE_dgb_fail("coding error");
		// source right is being truncated, so it never passes end
		if( r >= n1 ) {
			return FALSE_dgb_fail("coding error");
		}
		// XOR high byte into low byte (left ^= right)
		md.buff[l] ^= md.buff[ r ];
		// pre-scrub scrub-byte
		md.buff[r] = 0;
		// md will be shrinking
		md.nbytes_used --;
	}
	if( md.nbytes_used < 4 ) {
		// same arbitrary min 4 as above, CRC32 option for future
		return FALSE_dgb_fail("coding error");
	}
	if( md.nbytes_used != u32( n2) ) {
		return FALSE_dgb_fail("coding error");
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
