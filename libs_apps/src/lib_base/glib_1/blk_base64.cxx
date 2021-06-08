#include "blk_base64.h"

#warning TODO WRITE THIS and its tests currently forked glib call
#include <glib.h>

#include "dgb.h"

blk_base64::
blk_base64()
: multi_line( true )
, line_length( line_length_76 )
, base64url( false )
, pad4( true )
, gap4( true )
// , gap4( false )
{
	// getting uninitialised bytes from valgrind
	// but surely it knows it is C++ zero, optimisation?
	state = 1;
	save = 1;
	// call something here
	state = 0;
	save = 0;
	space_on_line = line_length;
}

/*!
	encode( buffer2 & text, buffer2 & base64 ) || FAIL FAILED
	decode( buffer2 & base64, buffer2 & text ) || FAIL FAILED
	dialect opcode from into
	dialect opcode lhs rhs // comment // ... // EA_LOCN CODE_POINT

	multi_line 1 means a BASE64_DATA_SCROLL with a polite WIDTH76 margin
	multi_line 0 means a BASE64_STREAM_of_N_bytes // limit u24 // block u16

	if you want to encode in multiple steps, part by part
	you must provide a multiple of 3 bytes
	so that it pauses and resumes seamlessly
	line breaks and gap4 will vary

	TODO
		leave last 0 1 or 2 bytes in a carry-over-state
		with line_space_remaining

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

	space_on_line = line_length;

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

		if( multi_line ) {
		  if( space_on_line < 4 ) {
			blk_out.put_byte( '\r' );
			blk_out.put_byte( '\n' );
			space_on_line = line_length;
		  }
		}

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
		space_on_line -= 4;
		if( gap4 ) {
		  // OK leave trailing SP on line //
		  blk_out.put_byte( ' ' );
		  space_on_line -= 1;
		}
	}

	blk_out.trailing_nul();
	return true;
}

inline bool decode_B64( u8 ch, int & val )
{
	if(( 'A' <= ch ) && ( ch <= 'Z' )) {
		val = ch - 'A';
		return true;
	}
	if(( 'a' <= ch ) && ( ch <= 'z' )) {
		val = ch - 'a' + 26;
		return true;
	}
	if(( '0' <= ch ) && ( ch <= '9' )) {
		val = ch - '0' + 26 + 26;
		return true;
	}
	switch( ch ) {
	  case '+': val = 62; return true;
	  case '/': val = 63; return true;
	  case '-': val = 62; return true;
	  case '_': val = 63; return true;
	}
	INFO("NOT B64 %c %2.2X", ch, ch );
	return false;
}

void write_word_as_bytes( blk1 & blk_out, int val, int n_bits ) {
// put 
// put does not do what I think it should
// now it should not exist!!
// put_byte is OK
	blk_out.put('#');
	blk_out.put_byte('!');
	INFO("blk_out == '%s'", (STR0) blk_out );
	blk_out.dgb_dump("blk_out");
	int nbytes = n_bits / 8;
 	INFO("nbits %d val %6.6X nbytes %d", n_bits, val, nbytes );
	if(( nbytes == 0 ) || ( nbytes > 3 )) {
		FAIL("nbytes must be 1 2 or 3 # got %d", nbytes );
		return;
	}

	u8 Z = val & 0xFF;
	val >>= 8;
	u8 Y = val & 0xFF;
	val >>= 8;
	u8 X = val & 0xFF;
	val >>= 8;
	if( val ) {
		WARN("val should now be zero, have %d", val );
	}

	INFO("DATA as text '%c' '%c' '%c' ", X, Y, Z );

	switch( nbytes ) {
	 case 3: blk_out.put( (u8) X ); // and stay for next case
	 case 2: blk_out.put( (u8) Y ); // and stay for next case
	 case 1: blk_out.put( (u8) Z ); // and done
	}
}


bool blk_base64::decode( blk1 & blk_in, blk1 & blk_out )
{
	blk_out.put((STR0)"{at-->}");
	blk_out.put('@');
	blk_out.put((STR0)"{<--put(STR)}");
	// got to stop somewhere // NUL required // no other thread may add
	if(!blk_in.trailing_nul()) return FAIL_FAILED();

	int len1 = blk_in.nbytes_used;
	int len2 = calc_size_decoded( len1 );

	if(!blk_out.get_space( len2 )) return FAIL_FAILED();
	// NB do check for malloc fail because
	// we are NOT doing incremental block-by-block with carry over
	// we are doing whole file convertion (eg a few certs)
	// if they are ever huge files, need to check early

	// DEMAND // no SP within group of 4 // 

	// parse direct within blk_in
	// optionally export P to whatever STOP eg {SCRIPT {B64=}}
	const u8 * P = blk_in.buff;

	bool more = true;
	int n_bits = 0; // 0 6 12 18 24 //
	int val_24_bits = 0; // 6 12 18 24 bits
	int val_6_bits = 0; // each B64 char carries 6 bits
	// one loop for each bunch of 4 chars
	while(1) {
	  // bad design but cute
	  // if a word was built, write it
	  // if this is the first loop there is no word
	  // if this is the last loop, check more AFTER writing it
	  if( n_bits ) {
	  	if( n_bits == 6 ) {
			FAIL("looped on 6 - should be caught later");
		}
		write_word_as_bytes( blk_out, val_24_bits, n_bits );
		val_24_bits = 0; // 6 12 18 24 bits
//		val_24_bits = -1; // check shift
		n_bits = 0; // 0 6 12 18 24 //
	  }
	  if(!more) break; // while

	  // skip gaps but stop at nul
	  while( (*P) && (*P <= ' ') ) { P++; }
	  if(!P) {
			more = false;
			continue; // break or continue
	  }
		
	  for( int i=0; i<4; i++ ) {
	    u8 ch = *P;
	    if( decode_B64( ch, val_6_bits ) ) {
	    		INFO("ch %c val %2.2X", ch, val_6_bits );
			P++;
			val_24_bits <<= 6;
			val_24_bits += val_6_bits;
			n_bits += 6;
	    } else {
		// anything that is not B64 stops file
		// P points to it
		more = false;
		switch( n_bits ) {
		 case 0: // non-starter // OK
		 break; // switch
		 case 6: // 1 in is not enough
			FAIL("base_64 requires at least 2 chars");
			INFO("ch is 0x%2.2X '%c'", (int) ch, ch );
		 break; // switch
		 case 12: // 12 bits might be OK
			if( '=' == ch ) {
				P++;
				ch = *P;
				if( '=' == ch ) {
					P++;
					ch = *P;
					PASS("AB==");
				} else {
					WARN("base_64 expects AB== 2 ");
					INFO("ch is 0x%2.2X '%c'", (int) ch, ch );
				}
			} else {
				WARN("base_64 expects AB== 1 ");
				INFO("ch is 0x%2.2X '%c'", (int) ch, ch );
			}
		 break; // switch
		 case 18: // 18 bits might be OK
			if( '=' == ch ) {
				P++;
				ch = *P;
				// ABC= // perfect 16 bits PAD4
				PASS("ABC=");
			} else {
				WARN("base_64 expects = ABC= ");
				INFO("ch is 0x%2.2X '%c'", (int) ch, ch );
			}
		 break; // switch
		 default: // cant happen
		 	FAIL("CODE ERROR cant happen");
		}

		// now pad the bits to 24 high
		val_24_bits <<= (24 - n_bits );
		// X 0 0 // 12
		// X Y 0 // 18
		// X Y Z 
		// keep n_bits

		break; // break for loop
	    }
	  }

	} // while
	blk_out.trailing_nul(); // no need // debugging luck



	/*
		A B C D     => X Y Z
		  B C D =   =>   Y Z
		    C D = = =>     Z
		      D =   ERROR
			=   EMPTY
			= || EOT || ANYSTOPCHAR // return EA(*P)

		They all get similar when aligned to RHS BIT 0
	*/

	return true;
}

#if 0
// glib based
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

	blk_out.trailing_nul(); // might be binary, might be ASCII
	return true;
}
#endif

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
