#ifndef CSET_byte_MAP_H
#define CSET_byte_MAP_H 2
#pragma interface
#ifndef CSET_BIT_MAP_H
#include "cset_bit_map.h"
#endif

#define dgb_fd stdout
/*
	cset_byte_map is byte order independent
	it is an array [256] of byte's
	used as bool is_ipper[c1] or mapping (special handlers for 0)
*/

class cset_byte_map
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

//	u8	get_byte( u8 i )
//	void	set_bit( u8 i )
//	void	reset_bit( u8 i )
// copy range //
//	void	set_range( u8 lo, u8 hi )
//	{
//		for( int i=lo; i <= hi; i++ )
//			set_bit( i );
//	}
///* :EG: no headings - added by dgb */
///* :eg: LHS_headings_added */

// :XFS2:sibling crystal:
// :FS_LOCN:	//LIBR/[]/LIBR_(92)//CLASS//dgb//dgb_print_str

	void	dgb_print_str( char * str )
	{
		fprintf( dgb_fd, "%s", str );
	}

	void	dgb_print_space( void )
	{
		fprintf( dgb_fd, " " );
	}

// MOVE TO CLASS dgb_print //
// THEN TO NEW CLASS print_dgb OF print AND dgb //

	void	dgb_print_chr( unsigned char c1 )
	{
		if( c1 < ' ' ) c1='.';
		fprintf( dgb_fd, "%c", c1 );
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
	//		fprintf( dgb_fd, " [ %1X_ ]", i );
			fprintf( dgb_fd, ":", i );
			for( j=0; j < 16 ;j++,k++ )
				dgb_print_byte( k );
			fprintf( dgb_fd, " :\n" );
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

