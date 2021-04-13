#include "blk_base64.h"

#warning TODO WRITE THIS and its tests currently forked glib call
#include <glib.h>

#include "dgb.h"

blk_base64::
blk_base64()
: multi_line( true )
, base64url( false )
, pad4( true )
, gap4( false )
{
	// getting uninitialised bytes from valgrind
	// but surely it knows it is C++ zero, optimisation?
	state = 1;
	save = 1;
	// call something here
	state = 0;
	save = 0;
}

/*!
	encode( buffer2 & text, buffer2 & base64 ) || FAIL FAILED
	decode( buffer2 & base64, buffer2 & text ) || FAIL FAILED
	dialect opcode from into
	dialect opcode lhs rhs // comment // ... // EA_LOCN CODE_POINT

	multi_line 1 means a BASE64_DATA_SCROLL with a polite WIDTH72 margin
	multi_line 0 means a BASE64_STREAM_of_N_bytes // limit u24 // block u16

	// block u12 // block 4K // sector_size 4096 //
	// SEEKABLE SUB SEGMENT DVD_32K_sector_size // u15 // u16 //

	// next word size is u32 then u48 then u64 
	// view P0 P2 over stream_segment_cache

	// YAWN write a BASE64 function 

*/
bool blk_base64::encode( blk1 & blk_in, blk1 & blk_out )
{
	int len1 = blk_in.nbytes_used;
	int len2 = calc_size_encoded( len1 );
	blk_out.get_space( len2 );

	static const int mask6 = 0x3F; // 6 bits 

	u8 char_of_6[ 64 + 1 ] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789"
	"+/" // -_
	; // NUL is +1

	if( base64url) {
		char_of_6[ 62 ] = '-';
		char_of_6[ 63 ] = '_';
	}

	const u8 * P = blk_in.buff;
	while( len1 > 0 ) {
		int word3 = 0; // a 3 byte word

		switch( len1 ) {
		 case 1:
			word3 += *P++; word3 <<= 8;
			word3 += 0;    word3 <<= 8;
			word3 += 0;
			// gcc will merge those <<16
		 break;
		 case 2:
			word3 += *P++; word3 <<= 8;
			word3 += *P++; word3 <<= 8;
			word3 += 0;
		 break;
		 default:
			// pack word3 left to right
			word3 += *P++; word3 <<= 8;
			word3 += *P++; word3 <<= 8;
			word3 += *P++;
		}

		// unpack ABCD word3 right to left 
		D = word3 & mask6; word3 >>= 6;
		C = word3 & mask6; word3 >>= 6;
		B = word3 & mask6; word3 >>= 6;
		A = word3 & mask6;

//		INFO("%d %d %d %d ", A, B, C, D );

		A = char_of_6[ A ];
		B = char_of_6[ B ];
		C = char_of_6[ C ];
		D = char_of_6[ D ];

		switch( len1 ) {
		 case 1:
		  blk_out.put_byte( A );
		  blk_out.put_byte( B );
		  if( pad4 ) {
		  blk_out.put_byte( '=' );
		  blk_out.put_byte( '=' );
		  }
		 break;
		 case 2:
		  blk_out.put_byte( A );
		  blk_out.put_byte( B );
		  blk_out.put_byte( C );
		  if( pad4 ) {
		  blk_out.put_byte( '=' );
		  }
		 break;
		 default:
		  blk_out.put_byte( A );
		  blk_out.put_byte( B );
		  blk_out.put_byte( C );
		  blk_out.put_byte( D );
		 break;
		}
		len1 -= 3;
		if( gap4 )
		  blk_out.put_byte( ' ' );
		WARN("TODO lines of 72");
	}

	blk_out.trailing_nul();
	return true;
}

bool blk_base64::decode( blk1 & blk_in, blk1 & blk_out )
{
	int len1 = blk_in.nbytes_used;
	int len2 = calc_size_decoded( len1 );

	// this is confused // look at later

	int word3 = 0;
	while( len1 > 0 ) { // and no junk found
	 len1--;
	}

	guint saveu;
	blk_out.get_space( len2 );
	len2 = g_base64_decode_step(
 (gchar*) 	blk_in.buff,
		blk_in.nbytes_used,
// NO	//	multi_line,
		blk_out.gap_addr(),
		&state,
		&saveu
	);
	blk_out.nbytes_used += len2;

#if 0
#endif
	blk_out.trailing_nul(); // might be binary, might be ASCII
	return true;
}

bool blk_base64:: encode( const char * str_in, blk1 & blk_out )
{
	blk1 blk_in;
	blk_in.set( str_in );
	return encode( blk_in, blk_out );
}

bool blk_base64:: decode( const char * str_in, blk1 & blk_out )
{
	blk1 blk_in;
	blk_in.set( str_in );
	return decode( blk_in, blk_out );
}

bool blk_base64:: encode( const char * str_in, str1 & str_out )
{
	blk1 blk_in;
	blk1 blk_out;
	blk_in.set( str_in );
	if(! encode( blk_in, blk_out )) {
		str_out = "";
		return FAIL_FAILED();
	}
	str_out.set( blk_out );
	return true;
}

bool blk_base64:: decode( const char * str_in, str1 & str_out )
{
	blk1 blk_in;
	blk1 blk_out;
	blk_in.set( str_in );
	if(! decode( blk_in, blk_out )) {
		str_out = "";
		return FAIL_FAILED();
	}
	str_out.set( blk_out );
	return true;
}



#include "buffer2.h"
#include "dgb_fail.h"

bool blk_base64::test1()
{
	blk_base64 b64_coder;

        buffer2 blk_txt;
        buffer2 blk_b64;
        blk_b64.put("aGVsbG8K"); // includes a newline
        b64_coder.decode( blk_b64, blk_txt );
        INFO("'%s' -> '%s'", STR0(blk_b64), STR0(blk_txt) );
        blk_b64.clear();
        blk_txt.clear();
        blk_txt.put("hello");
        b64_coder.encode( blk_txt, blk_b64 );
        INFO("'%s' <- '%s'", STR0(blk_b64), STR0(blk_txt) );
	return false;
}


#if 0
#include <string.h>

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

char *base64(const unsigned char *input, int length);

int main(int argc, char **argv)
{
  char *output = base64("YOYO!", sizeof("YOYO!"));
  printf("Base64: *%s*\n", output);
  free(output);
}

char *base64(const unsigned char *input, int length)
{
  BIO *bmem, *b64;
  BUF_MEM *bptr;

  b64 = BIO_new(BIO_f_base64());
  bmem = BIO_new(BIO_s_mem());
  b64 = BIO_push(b64, bmem);
  BIO_write(b64, input, length);
  BIO_flush(b64);
  BIO_get_mem_ptr(b64, &bptr);

  char *buff = (char *)malloc(bptr->length);
  memcpy(buff, bptr->data, bptr->length-1);
  buff[bptr->length-1] = 0;

  BIO_free_all(b64);

  return buff;
}

#endif
