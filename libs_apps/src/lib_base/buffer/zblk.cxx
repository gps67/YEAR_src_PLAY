// pdb_blobz also does the un_compress itself (merge somethine)

#include <stdio.h>

#include "zblk.h"
#include "e_print.h"


//! constructor
zblk::zblk() {
}

//! destructor
zblk::~zblk() {
}

//! clear buffer, and compress src/len into this buffer
bool zblk::compress( const char * src, uLong len ) {
	// here zlib requires sufficient space, or it stops
	uLong extra = (len + 1000) / 1000 + 12;
	buff.nbytes_used = 0;
	buff.get_space( len + extra );
	// dest_len is used on input (and result)
	uLongf dest_len = (uLongf) buff.nbytes_alloc;
	int t = ::compress(
		(Bytef *) buff.buff,
		&dest_len,
		(Bytef *) src,
		len
	);
	buff.nbytes_used = dest_len;
	// buff.trim_size(); // caller might be in a loop keeping largest
	if( Z_OK == t ) {
		return TRUE;
	}
	fprintf(stderr,"ERROR zblk::compress %d %s\n", t, "Z_MSG" );
	return FALSE;
}

//! expand this buffer, WITH EXACT EXPECTED length
bool zblk::expand( blk1 & outbuff, uLong expected_len) {
	// unused, the class is not really symmetrical!
	// ie the fields in the struct should be opposing halves/both
	outbuff.get_space( expected_len + 20 );
	Bytef *dest = (Bytef *) & outbuff.buff[ outbuff.nbytes_used ];
	uLongf destLen = outbuff.nbytes_alloc - outbuff.nbytes_used - 1;
	const Bytef *source = (Bytef *) buff.buff;
	uLong sourceLen = buff.nbytes_used;
	int t = uncompress(
		dest,  
		& destLen,
		source,
		sourceLen
	);
	outbuff.nbytes_used = destLen;
	outbuff.trim_size();
	if( destLen != expected_len ) {
		e_print("zblk::expand()  destLen(%ld) != expected_len (%ld))\n",
			destLen, expected_len );
	}
	if( t==Z_OK)
		return TRUE;
	fprintf(stderr,"zblk:expand() error %d\n", t );
	return FALSE;
}

//! some test code to call
bool zblk::test() {
	char * data = (char *) "Hello";
	int len = strlen( data ) + 1;
	// NB data must include its own NULL
	if( !compress( data, len) ) {
		fprintf(stderr,"failed\n");
		return FALSE;
	}
	blk1 out;
	if( !expand( out, len ) ) {
		fprintf(stderr,"Failed\n");
		return FALSE;
	}
	// nb null was included in data
	fprintf(stdout,"<out>%s</out>\n", out.buff);
	return TRUE;
}

