#ifndef str_base64_H
#define str_base64_H

#include "blk1.h"

class str_base64
{
 public:

//	int line_length_72;	// actually you have hobsons choice
	static const int line_length_72 = 72; // used in computations
	bool multi_line;

	int state;
	int save;

	str_base64();

	int calc_size_encoded( int n ) { 
		// 3 bytes become 4, end marker is EOT or =within
		// 72 linelength, CRLF
		int n3 = (n / 3) + 1;
		int nb = n3 * 4;
		int nl = 1;
		if( multi_line ) {
#if 0
			if( line_length_72 < 4 ) {
				line_length_72 = 72;
			}
#endif
			nl += nb / line_length_72;
		}
		/*
			This function is ALWAYS sufficient
		*/
		int xs = 5;
		return (nl * 2) + nb + xs;
	}
	int calc_size_decoded( int n ) {
		/*
			This function is ALWAYS sufficient
		*/
		int xs = 2;
		return (n / 4) * 3 + 3 + xs ;
	}

	// nb blk_out is NOT clear()ed down
	// nb 72 BPL is the only style, no spaces, sums or clever stuff
	// these can maybe be static

	bool encode( blk1 & blk_in, blk1 & blk_out );
	bool decode( blk1 & blk_in, blk1 & blk_out );

	bool encode( const char * str_in, blk1 & blk_out );
	bool decode( const char * str_in, blk1 & blk_out );

	bool encode( const char * str_in, str1 & blk_out );
	bool decode( const char * str_in, str1 & blk_out );

	static bool test1();
};

#endif
