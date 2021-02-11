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

inline int	least( int a, int b )
{
	return (a<b)?a:b;
}

#define IS_LESS	-1
#define IS_MORE 1
#define IS_SAME 0

struct p0p2
{
	u8 * p0;	// points to first char  INSIDE
	u8 * p2;	// points to first char OUTSIDE

	p0p2( void )
	{
		p0 = p2 = (u8 *)(-1);
	}
	p0p2( u8 * P0, u8 * P2 )
	{
		p0 = (u8 *)P0;
		p2 = (u8 *)P2;
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
	p0p2( p0p2 & rhs )
	{
		p0 = rhs.p0;
		p2 = rhs.p2;
	}

int	str_len(void) { return p2 - p0 ; }
char *	str_cpy( void )
{
	int len = str_len();
	char * s = (char *)malloc( len+1 );
	if(!s) return s;
	memcpy( s, (char * )p0, len ); /* see +1 above */
	s[len]='\0';
	return s;
}

friend	int cmp( p0p2 & lhs, p0p2 & rhs )
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
}
};

#endif
