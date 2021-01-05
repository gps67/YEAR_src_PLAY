#ifndef CSET_BIT_MAP_H
#define CSET_BIT_MAP_H 1

/*
	presumes an 8 bit charset
	stores a 32 byte bitmap of 256 bits
	functions to test set reset set-range copy ...
*/

#include "str0.h"

class cset_bit_map : public GRP_lib_base
{
private:
	u8	byte[ 32 ];	// 256/8=32
public:

	void	set_null( void )
	{
		for( int i=0; i<32; i++ )
			byte[ i ] = 0;
	}
	cset_bit_map & operator=( const cset_bit_map & rhs )
	{
		for( int i=0; i<32; i++ )
			byte[i] = rhs.byte[i];
		return *this;
	}
	cset_bit_map & operator|=( const cset_bit_map & rhs )
	{
		for( int i=0; i<32; i++ )
			byte[i] |= rhs.byte[i];
		return *this;
	}
	cset_bit_map & operator&=( const cset_bit_map & rhs )
	{
		for( int i=0; i<32; i++ )
			byte[i] &= rhs.byte[i];
		return *this;
	}
/*
bool 	group_include( const cset_bit_map & rhs )
	{
		this |= rhs;
		return TRUE;
	}
bool 	group_exclude( const cset_bit_map & rhs )
	{
	}
-LATER-
*/
	u8	get_bit( u8 i ) const
	{
		int j = i>>3;
		const u8 * p = &byte[ j ];
		i = 1 << (i & 7);
		return *p & i ;
	}
	void	set_bit( u8 i )
	{
		int j = i>>3;
		u8 * p = &byte[ j ];
		i = 1 << (i & 7);
		*p = *p | i ;
	}
	void	reset_bit( u8 i )
	{
		int j = i>>3;
		u8 * p = &byte[ j ];
		i = 1 << (i & 7);
		i = ~i;
		*p = *p & i ;
	}
	void	set_range( u8 lo, u8 hi )
	{
		for( int i=lo; i <= hi; i++ )
			set_bit( i );
	}

	void	dgb_print_bit( int k ) const
	{
		int t = get_bit( k );
		if ( t )
		{
			u8 c1 = k;
			if( c1 < ' ' ) c1 = '.';
			e_print( " %c", c1 );
		}
		else
			e_print( " %2.2x", k );
//		dgb_print_space();
//		dgb_print_space();
	}
	void	dgb_print1( void ) const
	{
		int i,j,  k=0;
		for( i=0; i < 16; i++ )
		{
	//		e_print( " [ %1X_ ]", i );
			e_print( ":", i );
			for( j=0; j < 16 ;j++,k++ )
				dgb_print_bit( k );
			e_print(" :\n");
		}
	}
};

/*	set_bits( "[A-Z]" )
	set_bits( "[a-z]" )
	set_bits( "ACCENTS" ) = set_bits( "ACCENTS_UC" ) + ACCENTS_LC
	EXPR 

	ie set by name !!

-------------------------------------------------------------------
Each letter is scanned for AREA( name ) == CSET_AREA1[ c1 ]; 
	A-Z a-z 0-9 ACCENTS

:AREA:AREA_1:
:AREA( A-Z ): AREA_LO_HI( "A", "Z" )
:AREA( a-z ): AREA_LO_HI( "a", "z" )
:AREA( 0-9 ): AREA_LO_HI( "0", "9" )
:AREA( ACCENTS ): AREA_LO_HI( 0xC0, 0xFF )
:AREA( ACCENTS_UC ): AREA_LO_HI( 0xC0, 0xDF )
:AREA( ACCENTS_LC ): AREA_LO_HI( 0xE0, 0xFF )
:AREA( CTRL_a ): AREA_LO_HI( 0, 31 )
:AREA( CTRL_b ): AREA_LO_HI( 0x7F, 0x9F )
:AREA( PUNCT_a ): AREA_LO_HI( 0x20, 0x2F )
:AREA( PUNCT_b ): AREA_LO_HI( 0x3A, 0x40 )
:AREA( PUNCT_c ): AREA_LO_HI( 0xB5, 0x60 )
:AREA( PUNCT_d ): AREA_LO_HI( 0x7B, 0x7E )
:AREA( PUNCT_e ): AREA_LO_HI( 0xA0, 0xBF )
:AREA:AREA_2:

*/
#endif

