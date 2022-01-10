#ifndef CSET_byte_MAP_H
#define CSET_byte_MAP_H
// #ifndef CSET_BIT_MAP_H
// #include "cset_bit_map.h"
// #endif

/*
	cset_byte_map is byte order independent
	it is an array [256] of byte's
	used as bool is_ipper[c1] or mapping (special handlers for 0)
*/

#include "misc.h"
#include "ints.h"
#include "e_print.h"

class cset_byte_map : public GRP_lib_base
{
public:
	u8	byte[ 256 ];

	u8 &	operator[]( u8 idx )
	{
		return byte[ idx ];
	}
	void	set_null( void )
	{
		for( int i=0; i<256; i++ )
			byte[ i ] = 0;
	}
	cset_byte_map & operator=( cset_byte_map & rhs )
	{
		for( int i=0; i<256; i++ )
			byte[i] = rhs.byte[i];
		return *this;
	}
	cset_byte_map & operator|=( cset_byte_map & rhs )
	{
		for( int i=0; i<256; i++ )
			byte[i] |= rhs.byte[i];
		return *this;
	}
	cset_byte_map & operator&=( cset_byte_map & rhs )
	{
		for( int i=0; i<256; i++ )
			byte[i] &= rhs.byte[i];
		return *this;
	}

	void	dgb_print_str( const char * str )
	{
		e_print( "%s", str );
	}

	void	dgb_print_space( void )
	{
		e_print( " " );
	}

	void	dgb_print_chr( unsigned char c1 )
	{
		if( c1 < ' ' ) c1='.';
		e_print( "%c", c1 );
	}

	void	dgb_print_byte( int k )
	{
		int t = byte[ k ];
		dgb_print_space();
		dgb_print_chr( k );
		dgb_print_str(" -> ");
		dgb_print_chr( t );
		dgb_print_space();
	}

/*
	this self writing bench is doing my head in
	sed s,:dgb_,:lex_dgb_,g
	smail route
	TI
*/
	void	dgb_print1( void )
	{
		int i,j,  k=0;
		for( i=0; i < 16; i++ )
		{
	//		e_print( " [ %1X_ ]", i );
			e_print( ":" );
			for( j=0; j < 16 ;j++,k++ )
				dgb_print_byte( k );
			e_print( " :\n" );
		}
	}
};

#endif
/*
	(1)	limited to BYTE orientated cset, ie 8859 or PCMAP
		NO support for JE or multibyte char
	(2)	can be used as bitset, but intended for XMAP conversion
		usage will probably derive and define fns
	(3)	dgb stuff needs moving about
*/

