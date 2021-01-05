#include "auth_pw.h"

#include "dgb.h"
#include "str_base64.h"
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
	str_base64 conv;
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


// testing above code - variations of DES_fcrypt
#if 0 // use standard UNIX crypt
// CRYPT
#include <unistd.h>
                const char * crypted_attempt_U = crypt( attempt, salt );
                INFO("crypted_attempt_U    '%s'", (STR0) crypted_attempt_U );
                const char * crypted_attempt_O = DES_crypt( attempt, salt );
                INFO("crypted_attempt_O    '%s'", (STR0) crypted_attempt_O );
                char mid_buff[14];
                const char * crypted_attempt = DES_fcrypt( attempt, salt, mid_buff );
#endif

///////////////


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
	static str1 str1_dense;
	if(!str1_dense) {
		// ... LOCK ... and repeat check return
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

/////////////////////////////////////////////////////
// class vnc_key_holder


vnc_key_holder:: vnc_key_holder()
: key_holder( EVP_des_cbc() )
{
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
	const int LEN8=8;
	const int LENi=plain.nbytes_used;
	for(int i=0; i<LEN8; i++ ) {
		u8 c = ASCII_NUL;
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
	buffer2 plain;
	str_base64 conv;
	if(!conv.decode( plain64, plain )) return FAIL_FAILED();
	// use DES_fcrypt openSSL THREAD SAFE replacement for UNIX crypt
	return encrypt_vncpass( crypt, (STR0) plain );
}

bool PW_UTIL_VNC:: encrypt_vncpass( buffer2 & crypt, const char * plain )
{
//	gdb_invoke();
	const int LEN8 = 8;
	
	vnc_key_holder vnc_well_known_key;
	if(!vnc_well_known_key.set_to_well_known_vnc_key()) return FAIL_FAILED();
	evp_cipher_base crypter( vnc_well_known_key, en_crypt );
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
bool PW_UTIL_VNC:: encrypt_vncpass_TIGHT( buffer2 & crypt, const char * plain )
{
	DES_cblock key = {23,82,107,6,35,78,88,7};
	for( int i = 0; i<8; i++ ) {
		key[i] = reverse_bits_in_byte( key[i] );
	}

 if(0) {
	buffer2 see;
	see.put_nn_bytes( 8, key );
	see.dgb_dump( "SEE well known key reversed");
 }

	// set schedule from cblock
	DES_key_schedule schedule;
//	DES_set_odd_parity(& key); // pre-requirement
	int t = DES_set_key_checked(& key, & schedule);
	if(t) {
		// return FAIL("DES_set_key_...() %d", t);
		FAIL("DES_set_key_...() %d", t);
		DES_set_key_unchecked(& key, & schedule);
	}

	DES_cblock input;
	DES_cblock output;

	int PASSLEN = strlen( plain );
	if(PASSLEN!=8) return FAIL("This is for auto generated dense passwords only");
	memcpy( input, plain, PASSLEN ); // hope its not short!
	int enc = DES_ENCRYPT; // VNC encrypts
	    enc = DES_DECRYPT; // encrypt, decrypt
	    enc = DES_ENCRYPT; // encrypt, decrypt
	DES_ecb_encrypt(& input, & output, & schedule, enc );

	crypt.set( output, PASSLEN );
	return true;
}

/*!
	slightly strange left to right sequence that matches encrypt ?
*/
bool PW_UTIL_VNC:: decrypt_vncpass( const char * crypt, buffer2 & plain )
{
//	gdb_invoke();
	const int LEN8 = 8;
	
	vnc_key_holder vnc_well_known_key;
	if(!vnc_well_known_key.set_to_well_known_vnc_key()) return FAIL_FAILED();
	evp_cipher_base crypter( vnc_well_known_key, de_crypt );
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
	if(blk_read_entire_file( pw_vnc_stored, filename, MAX1K ))
		return FAIL_FAILED();

	// decrypt it
	if(!decrypt_vncpass( pw_vnc_stored, pw_vnc_plain ))
		return FAIL_FAILED();

	if(1) pw_vnc_plain.dgb_dump("TUT TUT");
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


bool PW_UTIL_VNC:: hex10_encrypt_vncpass( buffer2 & hex10crypt, const char * plain )
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

