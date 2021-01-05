#ifndef key_munger_base_H
#define key_munger_base_H

#include "key_holder.h"

#include <openssl/evp.h>

/*
	Move these into a namespace, class, or add a library prefix
*/

extern bool key_fold_to_new_len( blk1 & md, int n2 );
extern bool blk_fold_to_new_len( blk1 & md, int n2 );
extern bool iv_fold_to_new_len( blk1 & md, int n2 );

/*!
	key_munger_base - munge a PASSWORD into a KEY

	Combine the PASSWORD (or phrase) with other (semi-fixed) data.
	The same PASSWORD can produce a handful of KEYS, for different
	purposes, valid only within specific subsystems.

	STEP: concatenate several (binary) strings with your PASSPHRASE

		SUBSTEP: "SYSTEM" + "PURPOSE" + "PASSWORD"
		SUBSTEP: NUL bytes after each (IFF present)

	STEP: take a message digest

	STEP: fold/extend that to the exact key length

	STEP: splat over NUL bytes, so you can still use it as a 'string'
	(but obviously other quotes backslashes apply, but C really
	notices NUL bytes, and it is a small cost for occasional benefit).

	KEY: a 256 bit output key, or 128 or 512 or 32 or ...

	That is useful as it gives a more 'random' key (scattered 1's and 0's)
	so when applied to known data, it will be slower to crack.

	It also allows different uses of the same original password/phrase,
	into different keys, which you might store in different databases
	(dont pretent you never do, you just RSA them again).

	NOTE: that this is not particularly cryptographically secure,
	but if someone obtains the 'clear' KEY, they are still some
	distance from the original PASSWORD.

	NOTE: this is a lot like SRP, or WEB-MD5-PASS or ...

	PROBLEM: 128 (or 160) is only just enough for AES-128 (etc).
	WHIRLPOOL creates 512 bits. Is it any good?

	BENEFIT: rainbow lookup tables will be less common,
	when prefixed with "PURPOSE_MYSITE_LOGIN", and
	encouraging groups of users to have a group login
	(SERVER=group_hub) would again reduce the pool,
	and also allow regular shifts from old to new
	((MAYBE PUT THIS IN THE INI FILE))

	DISADVANTAGE: this needs to be replicated on the HUB,
	or a different scheme gets used, and that means passing
	the original password over the WAN clear (over an ecrypted
	line, but clear when it emerges at the other end of the 
	SSL 'wire'. The server gets to actually see the ASCII).

*/

/////////////// or move them to be static members of this clas ...

/*!
	This holds a KEY and everything with it, it also
	prepares that key (generate via MD, folds its key_length)
	so you can use THIS to encrypt with (symmetric EVP anyway),
	as long as you provide the EVP that uses this ...
*/
class key_munger_base
{
 public:
	const EVP_CIPHER * cipher_type; // width of IV and KEY
	const EVP_MD * md_type;
	buffer2 blk_system;	// "UNIX_BOX box1<NUL>" //
	buffer2 blk_purpose;	// "access_RO<NUL>" // 

	key_munger_base();
	virtual ~key_munger_base();

	key_munger_base( 
		const EVP_CIPHER * _cipher_type,
		const EVP_MD * _md_type,
		str0 system,
		str0 purpose
	);

	//
	// it is the cipher type of how the key will be used
	// that wont vary per usage, but will over several usages
	// The default (NULL) is bf_cbc is 16 bytes
	//
	bool set_cipher_type( const EVP_CIPHER * _cipher_type = NULL );
	bool set_system( str0 purpose );
	bool set_purpose( str0 purpose );

	bool zap_prep( key_holder & key );

	virtual bool check_phrase_isnt_daft( blk1 & phrase );
	virtual bool make_buffer_from_phrase( buffer2 & blk_phrase_plus, blk1 & phrase );
	virtual bool make_key_from_phrase( key_holder & key, str0   phrase );
	virtual bool make_key_from_phrase( key_holder & key, blk1 & phrase );
};

#endif

