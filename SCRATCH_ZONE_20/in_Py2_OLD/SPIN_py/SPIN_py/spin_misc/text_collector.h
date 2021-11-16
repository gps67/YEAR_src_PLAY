// Copyright (c) 1998 Graham Swallow
#ifndef TEXT_COLLECTOR_H
#define TEXT_COLLECTOR_H

#include "buffer/misc.h"
#include "buffer/blk1.h"
#include "buffer/buffer1.h"

// cute feature - but problematic
// spin_py_mod.h: define TEXTCOLL_double_strike

#ifdef TEXTCOLL_double_strike
#define mark_eoln // another silly feature
#endif

class text_collector {
 public:

	text_collector(int N = 50); // becomes 100
	~text_collector();

	buffer1 buff; // holds 8859 data in dynamic buffer

	int flags;
	bool wrap;
	bool drop_cr;
	char putc_prev;

	inline int has_data(void)
	{
		return buff.nbytes_used;
	}

#ifdef TEXTCOLL_double_strike
#define mark_eoln // another silly feature
#endif

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
		} else {
			if( byte == '\n') {
# ifdef mark_eoln
				buff.put_byte( '$' );
				buff.put_byte( '\n' );
				buff.put_byte( '|' );
				buff.put_byte( '\t' );
				return; // put SP TAB on wrappable lines
# endif
			}
		}
		buff.put_byte( byte );
	}


	void data_char( char chr ) { // unused proper route
		data_byte( chr );
	}
	void data_Char( SGMLApplication::Char chr ) {
		char c1 = (char) chr;	// now lurking kere
		data_byte( c1 );	// in one place, not scattered
# ifdef TEXTCOLL_double_strike
		if( (unsigned) c1 <= ' ' ) return;
		data_byte( 8 );		// highlighting motion 
	if(0)	data_byte( c1 );	// highlighting bold
	else	data_byte( '_' );	// highlighting underline
# endif
	}

	void data_string(const SGMLApplication::CharString & str );
	void data_str(const char * str );
	void data_string(const char * str ) {return data_str( str ); }
	AppString mk_string_py(void);
	AppString mk_string(void);
	int trim_leading_space( void );
	int trim_trailing_space( void ); 
};
#endif
