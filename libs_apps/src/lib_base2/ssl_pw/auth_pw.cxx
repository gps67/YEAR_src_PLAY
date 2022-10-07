#include "auth_pw.h"

#include "dgb.h"
#include "blk_base64.h"
#include "SSL_global_base.h" // RAND_ -- presume init'd
#include <openssl/des.h>
#include "cset_bit_map.h"
#include "ASCII_chars.h"
#include "bit_flip.h"
#include "evp_cipher.h"
#include "util_buf.h"

using namespace AUTH;

/*!
	compare the old fashioned unix 'crypted password, with some attempt
	(probably passed over the net in base64, which is a good thing)
*/
bool PW_UTIL_UNIX:: compare_passenc_pass64( const char * crypted_true_pass, const char * clearpass64 )
{
	buffer2 clearpass;
	blk_base64 conv;
	if(!conv.decode( clearpass64, clearpass )) return FAIL_FAILED();
	// use DES_fcrypt openSSL THREAD SAFE replacement for UNIX crypt
	return compare_passenc_pass( crypted_true_pass, (STR0) clearpass );
}

/*!
	compare the old fashioned unix 'crypted password, with some clear text
*/
// static
bool PW_UTIL_UNIX:: compare_passenc_pass( const char * crypted_true_pass, const char * clearpass )
{
	// old fashioned unix uses hashed = crypt( attempt, salt )
	// openssl provides compatible DES_crypt( attempt, salt )
	// openssl has thread safe extra paramter DES_fcrypt( attempt, salt, mid)
	char mid_buff[14];

	// the SALT is 2 ASCII chars stored in the crypted_true_pass
	// originally it was from RAND as ASCII

	const char * clear_attempt = clearpass;
	const char * salt = crypted_true_pass; // 2 char string [a-zA-Z0-9]
	const char * crypted_attempt = DES_fcrypt( clear_attempt, salt, mid_buff );
	if(1) {
		FAIL("TUT TUT debugging TUT"); 
		INFO("clearpass            '%s'", (STR0) clearpass );
		INFO("crypted_true_pass    '%s'", (STR0) crypted_true_pass );
		INFO("crypted_attempt      '%s'", (STR0) crypted_attempt );
	}

	if( strcmp( crypted_true_pass, crypted_attempt ) == 0 ) {
		return true;
	} else {
		//              curr_row.dgb_show(); // there is no curr_row
		return FAIL("no password match");
		return false;
	}
	return FAIL("pass = '%s'", (STR0) clearpass );
}


// static 
bool PW_UTIL_CSET:: pick_n_random_bytes(
		char * buf,
		int pickn,
		int pooln,
		const char * pool
)
{
//	gdb_invoke();
	if( !pool ) {
		pool =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789"
			"./" ; // 26 + 26 + 10 + 2 = 64
	}
	if( !pooln ) {
		pooln = strlen( pool );
	} else if(1) {
		int pooln2 = strlen( pool );
		if( pooln != pooln2 ) {
			return FAIL("Expect %d got %d", pooln, pooln2 );
		}
	}
	if( pooln < 1 ) return FAIL("absurd");

	RAND_add_int_plus( (long) buf );
	buffer2 randfeed;
	if(!RAND_get_nn_into_blk( pickn, randfeed )) return FAIL_FAILED();

	unsigned bigpos = *((unsigned*)randfeed.buff); // any +value is OK
	bigpos *= 0x100101; // avoid even first value
	
	for( int i = 0; i < pickn; i++ ) {
		u8 r = randfeed.buff[ i ];
		bigpos += r;
		int pos = bigpos % pooln;
		u8 s = pool[ pos ];
		buf[ i ] = s;
	}
	return true;
}


// static
bool PW_UTIL_CSET:: pick_n_random_bytes(
	buffer2 & buffer,
	int pickn,
	int pooln,
	const char * pool
)
{
	if(!buffer.get_space( pickn + 1 )) return FAIL_FAILED();
	if(! pick_n_random_bytes( (char *) buffer.buff, pickn, pooln, pool ))
		return FAIL_FAILED();
	buffer.nbytes_used += pickn;
	buffer.trailing_nul();
	return true;
}


// static
bool PW_UTIL_UNIX:: RAND_salt2( char * salt )
{
	const char * salt_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789"
		"./" ; // 26 + 26 + 10 + 2 = 64

	int len = strlen( salt_chars );
	if(len != 64 ) return FAIL("Expected 64 got %d", len );

	salt[2] = 0; // additional incase of usage as string
	return PW_UTIL_CSET:: pick_n_random_bytes( salt, 2, len, salt_chars );
}

// static 
bool PW_UTIL_UNIX:: dense_chars_1(const char * * ptr_str )
{
	// dense_chars_1( & myptr ) // returns computed constant, malloced into static var str1
	// MINOR RISK during first call // add a lock here
	static str1 str1_dense;
	if(!str1_dense) {
		// ... LOCK ... and repeat check str1_dense is NULL as well as LOCK
		cset_bit_map cset;
		cset.set_range( 0x21, 0x7E ); // NOT SPACE DEL
		cset.reset_bit(ASCII_QUOTE_SINGLE); 
		cset.reset_bit(ASCII_QUOTE_DOUBLE); 
		cset.reset_bit(ASCII_QUOTE_BACK); 
		cset.reset_bit(ASCII_BRACE_LEFT); 
		cset.reset_bit(ASCII_BRACE_RIGHT); 
		cset.reset_bit(ASCII_BACKSLASH); 
		cset.reset_bit(':'); 	// protect DB backup from itself
		cset.reset_bit('/'); 	// protect DB backup from itself //
		cset.reset_bit('%'); 	// protect C from %s
		cset.reset_bit('#'); 	// protect shells from #

		buffer2 builder;
		builder.get_space( 128 ); // a good guess
		for(u8 c = 0x20; c < 0x80; c++ ) {
			if( cset.get_bit( c ) ) builder.put_byte( c );
		}
		// ATOMIC // write PTR into static var
		str1_dense = (STR0) builder; // malloc own copy
	}
	if(!str1_dense) {
		return FAIL("That somehow failed");
	}
	// simply copy the pointer over
	* ptr_str = (STR0) str1_dense;
	return true;
}

// static 
bool PW_UTIL_UNIX:: easy_chars_2(const char * * ptr_str )
{
	static str1 str1_easy;
	if(!str1_easy) {
		// ... LOCK ... and repeat check return
		cset_bit_map cset;
		cset.set_range( 'A', 'Z' ); // glyphs
		cset.set_range( 'a', 'z' ); // glyphs
		cset.set_range( '0', '9' ); // glyphs
		//
		cset.reset_bit('O'); 	// if looks like digit it is digit
		cset.reset_bit('o'); 	// if looks like digit it is digit
		cset.reset_bit('I'); 	// if looks like digit it is digit
		cset.reset_bit('B'); 	// B looks lik 8
		cset.reset_bit('S'); 	// S looks lik 5
		cset.reset_bit('Z'); 	// Z looks lik 2

		buffer2 builder;
		builder.get_space( 128 ); // a good guess
		for(u8 c = 0x20; c < 0x80; c++ ) {
			if( cset.get_bit( c ) ) builder.put_byte( c );
		}
		str1_easy = (STR0) builder; // malloc own copy
	}
	if(!str1_easy) {
		return FAIL("That somehow failed");
	}
	// simply copy the pointer over
	* ptr_str = (STR0) str1_easy;
	return true;
}

// static
bool PW_UTIL_UNIX:: RAND_pass8_dense( buffer2 & buff )
{
	const int LEN8 = 8;
	if(!buff.get_space(LEN8+1)) return FAIL_FAILED();

	const char * user_chars = NULL;
	if(!dense_chars_1( & user_chars ) ) return FAIL_FAILED();

	if(! PW_UTIL_CSET:: pick_n_random_bytes( buff, LEN8, 0, user_chars )) 
		return FAIL_FAILED();

	INFO("DENSE(%s)", (STR0) buff );
	return PASS("OK");
}

// static
bool PW_UTIL_UNIX:: RAND_pass8_easy2( buffer2 & buff )
{
	const int LEN8 = 8;
	if(!buff.get_space(LEN8+1)) return FAIL_FAILED();

	const char * user_chars = NULL;
	if(!easy_chars_2( & user_chars ) ) return FAIL_FAILED();

	if(! PW_UTIL_CSET:: pick_n_random_bytes( buff, LEN8, 0, user_chars )) 
		return FAIL_FAILED();

	INFO("EASY2(%s)", (STR0) buff );
	return PASS("OK");
}

// this is not what I'm getting from my code :-(
// sure it was once upon a time tho
// echo abcd5678 | vncpasswd -f | hd -c
// 00000000  c2 c9 a2 24 51 d1 5b d3    |...$Q.[.|
// 0000000    �  �  �  $  Q  �  [  �                 
// ok fix was to set odd parity in _TRAD now looking at _EVP

/////////////////////////////////////////////////////
// class vnc_key_holder


vnc_key_holder:: vnc_key_holder()
: key_holder( EVP_des_cbc() )
{
	INFO("LOOK AT ERRNO %d", errno );
	FAIL("LOOK AT errno %d", errno );
	FAIL("NOT A FAIL + OK");
	INFO("CTOR BODY has called CTOR key_holder( EVP_des_cbc())");
}

vnc_key_holder:: ~vnc_key_holder()
{
}

bool vnc_key_holder:: set_to_well_known_vnc_key()
{
	const unsigned char well_known_key[] = { 23,82,107,6,35,78,88,7 };
	const int LEN8=8;

	blk_key.clear();
	if(!blk_key.get_space(LEN8)) return FAIL_FAILED();
	for(int i=0; i<LEN8; i++ ) {
		blk_key.put_byte( well_known_key[i] );
	}
	if(!flip_all_bytes_in_key()) return FAIL_FAILED();
	return true;
}

bool vnc_key_holder:: flip_all_bytes_in_key()
{
	const int LEN8=8;
	for(int i=0; i<LEN8; i++ ) {
		blk_key.buff[i] = reverse_bits_in_byte( blk_key.buff[i] );
	}
	return true;
}

bool  vnc_key_holder:: set_to_users_pass8( buffer2 & plain )
{
	blk_key.clear();
	const int LEN8=8;
	const int LENi=plain.nbytes_used;
	for(int i=0; i<LEN8; i++ ) {
		u8 c = ASCII_NUL; // pad to 8 bytes with NUL byte
		if( i < LENi ) c = plain.buff[i];
		blk_key.buff[i] = c;
	}
	if(!flip_all_bytes_in_key()) return FAIL_FAILED();
	return true;
}


/////////////////////////////////////////////////////
//  PW_UTIL_VNC:: 

bool PW_UTIL_VNC:: RAND_pass8_dense_vnc( buffer2 & pw_vnc_plain )
{
	return PW_UTIL_UNIX:: RAND_pass8_dense( pw_vnc_plain ); // vnc is dense 8 - I guess
}

bool PW_UTIL_VNC:: encrypt_vncpass64( buffer2 & crypt, const char * plain64 )
{
	buffer2 plain; // always has 8 space
	blk_base64 conv;
	if(!conv.decode( plain64, plain )) return FAIL_FAILED(); // lhs -> rhs
	// use DES_fcrypt openSSL THREAD SAFE replacement for UNIX crypt
	return encrypt_vncpass( crypt, (STR0) plain );
}

//	static
bool PW_UTIL_VNC:: decrypt_vncpass64( buffer2 & plain64, const char * crypt )
{
	buffer2 plain;
	if(! decrypt_vncpass( plain, crypt ))
		return FAIL_FAILED();

	blk_base64 conv;
	if(!conv.encode( plain, plain64 )) return FAIL_FAILED(); // lhs -> rhs
	return true;
}

// switch _EVP _TRAD


bool PW_UTIL_VNC:: encrypt_vncpass( buffer2 & crypt, const char * plain )
{
	if(0)	return encrypt_vncpass_EVP( crypt, plain );
	else	return encrypt_vncpass_TRAD( crypt, plain );
}

bool PW_UTIL_VNC:: decrypt_vncpass(  buffer2 & plain, const char * crypt )
{
	if(0)	return decrypt_vncpass_EVP( plain, crypt );
	else	return decrypt_vncpass_TRAD( plain, crypt );
}


bool PW_UTIL_VNC:: encrypt_vncpass_EVP( buffer2 & crypt, const char * plain )
{
	// this is failing compare to _TRAD
	INFO("plain == '%s'", plain );
	const int LEN8 = 8;
	
	vnc_key_holder vnc_well_known_key;
	if(!vnc_well_known_key.set_to_well_known_vnc_key()) return FAIL_FAILED();
	gdb_invoke();
	evp_cipher_base crypter( vnc_well_known_key, en_crypt );
	//	test CTOR for it not failing
	//	return FAIL_FAILED();;

	int i = 0;
	while( i++ < LEN8 ) {
		u8 c = *plain;
		if(c) plain++;	// effectively PAD plain to 8 bytes with NUL
		crypter.buf_in.put_byte( c );
	}
	if(!crypter.set_padding( false )) return FAIL_FAILED(); // else 8 to 16
	if(!crypter.process_entire_buf()) return FAIL_FAILED();
	if(!crypt.set( crypter.buf_out )) return FAIL_FAILED();
	return true;
}

/*!
	this is a dmo to compare results, and show direct calls
*/
bool PW_UTIL_VNC:: encrypt_vncpass_TRAD( buffer2 & crypt, const char * plain )
{
	DES_cblock key = {23,82,107,6,35,78,88,7};
	for( int i = 0; i<8; i++ ) {
		key[i] = reverse_bits_in_byte( key[i] );
	}

 if(0) {
	buffer2 see;
	see.put_nn_bytes( 8, key ); // key is (char *) // [8]+ //
	see.dgb_dump( "SEE well known key reversed");
	// E8 4A D6 60  C4 72 1A E0 //
 }

	// key MUST have odd parity and preset data isn't!!
	DES_set_odd_parity(& key); // pre-requirement

	// key in an internal format, for speed
	// set schedule from cblock
	DES_key_schedule schedule;
	int t = DES_set_key_checked(& key, & schedule);

 if(0) {
	buffer2 see;
	see.put_nn_bytes( 8, key );
	see.dgb_dump( "AFTER DES_set_key_checked");
	// E9 4A D6 61  C4 73 1A E0 //
 }

	if(t) {
		// -1 means parity error # -2 means weak key # 
		// return FAIL("DES_set_key_...() %d", t);
		FAIL("DES_set_key_...() %d", t);
		DES_set_key_unchecked(& key, & schedule);
	}

	// 8 byte + buffer
	DES_cblock input;
	DES_cblock output;

	int PASSLEN = strlen( plain );
	if(PASSLEN!=8) {
	 WARN("PASSLEN == %d plain == '%s'", PASSLEN, plain );
	 WARN("this is overwriting a const char *" );
	 WARN("caller should pre do this to buffer" );
	 char * plain2 = (char *) plain;
	 while(PASSLEN<8) {
	 	// actually DES will do this for us
		// indeed decrypt cant remove trailing NUL bytes
		// as it always expects multiple of 8, with no N
		plain2[PASSLEN++] = 0x00; // PAD password with NUL // not SP ?
		// NB plain is being extended // it MUST have space !!
	 }
	 plain = plain2;
	}
	if(PASSLEN!=8) return FAIL("This is for auto generated dense passwords only PASSLEN=%d", PASSLEN);
	memcpy( input, plain, PASSLEN ); // hope its not short!
	int enc = DES_ENCRYPT; // VNC encrypts
	DES_ecb_encrypt(& input, & output, & schedule, enc );

	crypt.set( output, PASSLEN );
if(1)	PASS("OK %s", plain);
	return true;
}

// static
bool PW_UTIL_VNC:: decrypt_vncpass_TRAD( buffer2 & plain, const char * crypt )
{
	// TODO // HERE // copy paste from encrypt // rewrite
	// this decrypts the VNC password that _TRAD encrypted
	// TODO non 8 char pass ...
	// a reference for _EVP ..

	DES_cblock key = {23,82,107,6,35,78,88,7};
	for( int i = 0; i<8; i++ ) {
		key[i] = reverse_bits_in_byte( key[i] );
	}
	// key MUST have odd parity and preset data isn't!!
	DES_set_odd_parity(& key); // pre-requirement

	// set schedule from cblock
	DES_key_schedule schedule;
	int t = DES_set_key_checked(& key, & schedule);

	if(t) {
		// -1 means parity error # -2 means weak key # 
		// return FAIL("DES_set_key_...() %d", t);
		FAIL("DES_set_key_...() %d", t);
		DES_set_key_unchecked(& key, & schedule);
		// RETCODE // ??
	}

	DES_cblock input;
	DES_cblock output;

	int PASSLEN = strlen( crypt );
	if(PASSLEN!=8) {
	 WARN("PASSLEN == %d crypt == '%s'", PASSLEN, "XXXXXXXX" ); // crypt );
	}
	if(PASSLEN!=8) return FAIL("This is for auto generated dense passwords only PASSLEN=%d", PASSLEN);

	// copy crypt to input
	memcpy( input, crypt, PASSLEN ); // hope its not short!

//	int enc = DES_ENCRYPT; // VNC encrypts
	int enc = DES_DECRYPT; // VNC decrypts
	DES_ecb_encrypt(& input, & output, & schedule, enc ); // en is de

	// copy output to plain 
	plain.set( output, PASSLEN );
	return true;
}


/*!
	slightly strange left to right sequence that matches encrypt ?
*/
bool PW_UTIL_VNC:: decrypt_vncpass_EVP(  buffer2 & plain, const char * crypt )
{

//	gdb_invoke();
	const int LEN8 = 8;
	
	vnc_key_holder vnc_well_known_key;
	if(!vnc_well_known_key.set_to_well_known_vnc_key()) return FAIL_FAILED();
//	INFO("maybe null ctx ...");
	evp_cipher_base crypter( vnc_well_known_key, de_crypt );
//	INFO("or was OK");
	int i = 0;
	while( i++ < LEN8 ) {
		u8 c = *crypt;
		if(c) crypt++;
		crypter.buf_in.put_byte( c );
	}
	// bug if not 8
	if(!crypter.set_padding( false )) return FAIL_FAILED();
	if(!crypter.process_entire_buf()) return FAIL_FAILED();
	if(!plain.set( crypter.buf_out )) return FAIL_FAILED();
	return true;
}

/*!
	read the vncpass from a file, decrypt and return it
*/
// static
bool PW_UTIL_VNC:: vncpassfile_read(
	const char * filename,
	buffer2 & pw_vnc_plain
)
{
	// read it in
	buffer2 pw_vnc_stored;
	int MAX1K = 1;
	if(!blk_read_entire_file( pw_vnc_stored, filename, MAX1K ))
		return FAIL_FAILED();

	if(0) pw_vnc_stored.dgb_dump(filename);

	// decrypt it
	if(!decrypt_vncpass( pw_vnc_plain, pw_vnc_stored ))
		return FAIL_FAILED();

	if(0) pw_vnc_plain.dgb_dump("TUT TUT");
	return true;
	return PASS("DONE");
}

/*!
	write the vncpass from a file, encrypt it,
	and also set the file permissions
	THIS PRESUMES YOU ARE ROOT
*/
// static
bool PW_UTIL_VNC:: vncpassfile_write(
	const char * filename,
	const char * pw_vnc_plain
	// mod is 660
)
{
	// encrypt it
	buffer2 pw_vnc_stored;
	if(!encrypt_vncpass( pw_vnc_stored, pw_vnc_plain )) // may call _TRAD
		return FAIL_FAILED();

	// store it
	INFO("WRITING VNC to %s", (STR0) filename );
	int mask = 0600; // file permission
	if(!blk_write_to_file_mask(
		pw_vnc_stored,
		filename,
		mask
	)) {
		WARN("That would have failed if you ran it as non-root a second time");
		return FAIL_FAILED();
	}

	INFO("WRITING %s", (STR0) pw_vnc_plain );
	pw_vnc_stored.dgb_dump("STORED");
	INFO("abcd5678 => c2 c9 a2 24 51 d1 5b d3 # EXPECTED");
	return PASS("DONE");
}

/*!
	write the vncpass from a file, encrypt it,
	and also set the file permissions
	THIS PRESUMES YOU ARE ROOT
*/
// static
bool PW_UTIL_VNC:: vncpassfile_write_uid_gid(
	const char * filename,
	const char * pw_vnc_plain,
	// mod is 660
	int uid,
	int gid
)
{
	// encrypt it
	buffer2 pw_vnc_stored;
	if(!encrypt_vncpass( pw_vnc_stored, pw_vnc_plain ))
		return FAIL_FAILED();

	// store it
	INFO("WRITING to %s", (STR0) filename );
	int mask = 0660; // file permission
	mask = 0640; // group can read
	mask = 0400; // user cant write, without !
	mask = 0600; // group CANNOT read
	if(!blk_write_to_file_mask_uid_gid(
		pw_vnc_stored,
		filename,
		mask,
		uid,
		gid
	)) {
		WARN("That would have failed if you ran it as non-root a second time");
		return FAIL_FAILED();
	}

	INFO("WRITING %s", (STR0) pw_vnc_plain );
	return PASS("DONE");
}


bool PW_UTIL_VNC::
hex10_encrypt_vncpass(
	buffer2 & hex10crypt,
	const char * plain
)
{
	if( hex10crypt.nbytes_used ) {
		WARN("hex10crypt already has data in it - appending");
	}
	// WARNING THIS DOES NOT HAPPEN !! hex10crypt.clear();
	if(!hex10crypt.get_space(20)) return FAIL_FAILED();
	buffer2 crypt;
	if(!encrypt_vncpass( crypt, plain ))
		return FAIL_FAILED();
	int NB = crypt.nbytes_used;
	if(10 != NB ) {
		WARN("expected 10 got %d", NB );
	}
	for( int i = 0; i<NB; i++ ) {
		u8 b = crypt.buff[i];
		hex10crypt.print("%2.2X", b );
	}
	return true;
}

// LOCAL
bool test_compare_binary(
	const char * test_desc,
	const buffer2 & expected,
	const buffer2 & got
) {
	buffer2 message;
	buffer2 message2;
	message.print("TEST %s", test_desc);
	if( expected == got ) {
		message.print(" PASS GOT ");
		got.dgb_dump( (STR0) message );
		return true;
	} else {
		message2 = (STR0) message;
		message.print(" FAIL GOT ");
		message2.print(" FAIL EXPECTED ");
		got.dgb_dump( (STR0) message );
		expected.dgb_dump( (STR0) message2 );
		return false;
	}
}

// LOCAL
bool test_compare_ascii(
	const char * test_desc,
	buffer2 expected,
	buffer2 got
) {
	return test_compare_binary( test_desc, expected, got );
}


// static 
bool PW_UTIL_VNC:: test_one()
{
	buffer2 pw_plain_expected;
	buffer2 crypt_1;
	pw_plain_expected = "abcd5678";
	crypt_1.set_parse_hex_expect( "c2 c9 a2 24 51 d1 5b d3 # EXPECTED");
	if(!test_one_a( pw_plain_expected, crypt_1 ))
		return FAIL_FAILED();
	if(!test_one_b( pw_plain_expected, crypt_1 ))
		return FAIL_FAILED();
	return PASS("OK");
}

// static 
bool PW_UTIL_VNC:: test_one_a(
	const buffer2 & pw_plain_expected,
	const buffer2 & pw_crypted_expected
) {
	// encode and decode a password using the default functions
	// that might be fixed _EVP or _TRAD
	buffer2 pw_plain;
	buffer2 pw_crypted;

	// encrypt //
	if(!encrypt_vncpass( pw_crypted, (STR0) pw_plain_expected )) // LHS = ENC RHS
		return FAIL_FAILED();

	// compare //
	if( !test_compare_binary("_a encode", pw_crypted_expected, pw_crypted ))
		return FAIL_FAILED();

	// decrypt //
	if(!decrypt_vncpass( pw_plain, pw_crypted )) // LHS = DEC RHS
		return FAIL_FAILED();
	
	// compare //
	if( !test_compare_ascii("_a decode", pw_plain_expected, pw_plain )) {
		return FAIL_FAILED();
	}
	return PASS("OK");
}

// static 
bool PW_UTIL_VNC:: test_one_b(
	const buffer2 & pw_plain_expected, // "abcd5678";
	const buffer2 & pw_crypted_expected  // "c2 c9 a2 24 51 d1 5b d3 # EXPECTED";
) {
	// encode using _TRAD and also _EVP and compare
	buffer2 pw_plain_TRAD; // return journey comes here
	buffer2 pw_plain_EVP; // return journey comes here
	buffer2 pw_crypted_EVP;
	buffer2 pw_crypted_TRAD;

	INFO("HERE");

	// _TRAD is OK
	if(!encrypt_vncpass_TRAD( pw_crypted_TRAD, pw_plain_expected )) // LHS = ENC RHS
		return FAIL_FAILED();

	if(!encrypt_vncpass_EVP( pw_crypted_EVP, pw_plain_expected )) // LHS = ENC RHS
		return FAIL_FAILED();

	// compare //
	if( !test_compare_binary("_b encode _TRAD", pw_crypted_expected, pw_crypted_TRAD ))
		return FAIL_FAILED();

	// compare //
	if( !test_compare_binary("_b encode _EVP", pw_crypted_expected, pw_crypted_EVP ))
		return FAIL_FAILED();

	if(!decrypt_vncpass_TRAD( pw_plain_TRAD, pw_crypted_TRAD )) // LHS = DEC RHS
		return FAIL_FAILED();

	if(!decrypt_vncpass_EVP( pw_plain_EVP, pw_crypted_EVP )) // LHS = DEC RHS
		return FAIL_FAILED();

	// compare //
	if( !test_compare_ascii("_b decode _TRAD", pw_plain_expected, pw_crypted_TRAD ))
		return FAIL_FAILED();

	// compare //
	if( !test_compare_ascii("_b decode _EVP", pw_crypted_expected, pw_crypted_EVP ))
		return FAIL_FAILED();
	
	return PASS("OK");
}
