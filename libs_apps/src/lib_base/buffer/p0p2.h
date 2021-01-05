#ifndef P0P2_H
#define P0P2_H

/*
	p0p2  is a string found within other strings (or buffer)
	it could be arbitrary bytes not chars
	there is no NULL byte at the end - unless there is
	strings are only used for initialisation and produce result
	there is no malloc, you have to provide the original buffer
	used mainly to hold a mmap'd file p0= start p2=end p1=your-p1=inside
*/
/* p0p2.c - NONE */

#include <string.h>
#include <malloc.h>
#include "misc.h"
#include "CR_LF_NUL.h"
// hmmm - cant avoid this - inline
#include "str2.h"

inline int	least( int a, int b )
{
	return (a<b)?a:b;
}

#include "ints.h"
#include "is_diff.h"

/*!
	a block of memory bounded by two pointers

	p2 points outside (the next zones p0)

	[p0 ... [p2

	p1 doesnt exist - its your pointer within p0p2
*/
struct p0p2 : public GRP_lib_base
{
	u8 * p0;	// points to first char  INSIDE
	u8 * p2;	// points to first char OUTSIDE

	void clear( void )
	{
		p0 = p2 = (u8 *)(-1);
	}
	p0p2( void )
	{
		p0 = p2 = (u8 *)(-1);
	}
	p0p2( u8 * P0, u8 * P2 )
	{
		p0 = (u8 *)P0;
		p2 = (u8 *)P2;
	}
	p0p2( u8 * P0, unsigned int len )
	{
		p0 = P0;
		p2 = p0 + len;
	}
	p0p2( char * P0, char * P2 )
	{
		p0 = (u8 *)P0;
		p2 = (u8 *)P2;
	}
	p0p2( char * buff, int len )
	{
		p0 = (u8 *)buff;
		p2 = (u8 *)buff + len; /* NOT strlen */
	}
	p0p2( char * buff )
	{
		p0 = (u8 *)buff;
		p2 = p0 + strlen( buff );
	}
	p0p2( const p0p2 & rhs )
	{
		p0 = rhs.p0;
		p2 = rhs.p2;
	}

	void trim_leading_blanks()
	{
		while( p0 < p2 )
		{
			u8 c1 = *p0;
			if((c1 ==' ')||(c1 == '\t')||(c1 == '\r')||(c1=='\n'))
			{
				p0 ++;
			} else {
				break;
			}
		}
	}

	void trim_trailing_blanks()
	{
		while( p0 < p2 )
		{
			u8 c1 = *(p2 - 1);
			if((c1 ==' ')||(c1 == '\t')||(c1 == '\r')||(c1=='\n'))
			{
				p2 --;
			} else {
				break;
			}
		}
	}

	void trim_blanks()
	{
		trim_leading_blanks();
		trim_trailing_blanks();
	}

	int	N_bytes() const { return p2 - p0 ; }
	int	blk_len() const { return N_bytes(); }
	int	str_len(void) const { return p2 - p0 ; }

	char *	str_cpy( void ) const
	{
		int len = str_len();
		char * s = (char *)malloc( len+1 );
		if(!s) return s;
		memcpy( s, (char * )p0, len ); /* see +1 above */
		s[len]='\0';
		return s;
	}

	/*!
		copy to the dest buffer, with an added NUL
	*/
	void	mem_cpy_0( u8 * dest ) const
	{
		// if(!dest) ...
		int len = str_len();
		memcpy( dest, (char * )p0, len ); 
		dest[len]='\0';
		// return dest;
	}

// shouldnt this say STATIC ... even though inline its a C++ non-member

	static int cmp( const p0p2 & lhs, const p0p2 & rhs ) // const
	{
		int ll = lhs.str_len();
		int lr = rhs.str_len();
		int lo = least( ll, lr ); 
		int t = memcmp( lhs.p0, rhs.p0, lo );
		if( t<0 ) return IS_LESS;
		if( t>0 ) return IS_MORE;
		if( ll == lr ) return IS_SAME;
		if( ll <  lr ) return IS_LESS;
		if( ll >  lr ) return IS_MORE;
		// unreached!
		return IS_SAME;
	}


	/*!
		compare this with rhs, return IS_LESS IS_SAME IS_MORE
	*/
	int cmp( const char * rhs_ ) const
	{
		u8 * rhs = (u8 *) rhs_;
		u8 * lhs = p0;
		while(1) {
			u8 c1 = * lhs++;
			u8 c2 = * rhs++;
			if( lhs > p2 ) {
				// slight overrun - but OK
				c1 = NUL;
			}
			if( c1 < c2 ) return IS_LESS;
			if( c1 > c2 ) return IS_MORE;
			// lhs doesnt have to contain NUL - but it does
			if( ! c2 ) return IS_SAME;
		}
	}

	int cmp( str0 rhs ) const {
		return cmp( (const char *) rhs );
	}

	int cmp( const p0p2 & rhs ) const {
		return cmp( *this, rhs );
	}

///	int cmp( str0 s ) 
///	{
///		return cmp( (const u8 *) s);
///	}

// MOVE
	int cmp( str2 * s ) 
	{
		return cmp( (const u8 *) s);
	}

	bool operator==( str2 * s ) {
		return cmp( s ) == IS_SAME;
	}

};

#endif
