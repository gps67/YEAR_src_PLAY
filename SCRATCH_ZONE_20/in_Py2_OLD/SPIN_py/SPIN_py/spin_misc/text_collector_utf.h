// Copyright (c) 1998 Graham Swallow
#ifndef TEXT_COLLECTOR_UTF_H
#define TEXT_COLLECTOR_UTF_H

#include "text_collector.h"

#include "buffer/misc.h"
#include "buffer/blk1.h"
#include "buffer/buffer1.h"

/*!
	The output is UTF8, the input is wide-char (int).

	LURK: this class assists in the wrap/space migration
	but it does it too much.
*/
class text_collector_utf : public text_collector {
 public:

	text_collector_utf(int N = 50); 
	~text_collector_utf();

	inline	// putc a single byte with CR -> LF depending on wrap
	void data_byte(char byte ) {
		// SP delivers CR, I prefer NL
		if( byte == '\r') 
			if(drop_cr)
				return;
			else
				byte = '\n';
		if(wrap) {
			// convert {TAB,LF} -> SP
			if( byte == '\t') 
				byte = ' '; // still have multi-sp
			if( byte == '\n') 
				byte = ' '; // still have multi-sp
			// use putc_prev to condense SP+ -> SP (return)
			if( byte == ' ' )
				if( putc_prev == ' ') // collapse multi-sp
					return;
			putc_prev = byte; // only kept if WRAP is on!
			// only used by wrap
		}
		buff.put_byte( byte );
	}


	void data_char( unsigned char chr ) { // unused proper route
		if( chr <= 0x7f ) {
			data_byte( chr );
#ifdef TEXTCOLL_double_strike
			// Interesting test text effect
			data_byte( 0x08 );
			data_byte( chr );
#endif
		} else {
			char c1 = 0xC0 + (chr >> 6);
			char c2 = 0x80 + (chr & 0x3F ); // 6 bits
			data_byte( c1 );
			data_byte( c2 );
		}
	}

	void data_Char( SGMLApplication::Char chr ) {
		// when (u8 Char) C usually complains about "chr < 800" 
#ifdef SP_MULTI_BYTE
		if( chr < 0x80 )
			data_byte( (char) chr );
		else if( chr < 0x800 ) {
			char c1 = 0xC0 + (chr >> 6);
			char c2 = 0x80 + (chr & 0x3F ); // 6 bits
		} else { // we only do 16 bit Char
			char c3 = 0x80 + (chr & 0x3F); chr = chr >> 6;
			char c2 = 0x80 + (chr & 0x3F); chr = chr >> 6;
			char c1 = 0xE0 + (chr);
			data_byte( c1 );
			data_byte( c2 );
			data_byte( c3 );
		}
#else
		data_char( (unsigned char) chr );
#endif
	}

	void data_string(const SGMLApplication::CharString & str );
	void data_str(const char * str );
	void data_string(const char * str ) {return data_str( str ); }
	AppString mk_string_py(void);
	AppString mk_string(void);
// parent class does this:
//	int trim_leading_space( void );	// utf space is ascii space
//	int trim_trailing_space( void ); // inherit handler
};
#endif
