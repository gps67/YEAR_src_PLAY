#ifndef CSET_byte_MAP_H
#define CSET_byte_MAP_H
// #ifndef CSET_BIT_MAP_H
// #include "cset_bit_map.h"
// #endif

/*
	it is an array [256] of byte's
	cset_byte_map is byte order independent { u8_idx = N ++ }
	{ CXX as ARGV with CODE_POINT and CMNT and } # but not strictly

	IDX2 = LOOKUP idx1 // u64_WORD with u8_u8_u48_EXPR // idx1 = REG_VAL_ONE
	// IDX2 == "const EXPR = (*EA) REG_one"

	CSET and OPCODE have sparse and ranges especially when second u16 used

		u8_u8	OPCODE_A_B
		u16	EXTRA_u16 // only when OPCODE goes to 32 bit table OFFS
		u32	IDX OFFS or EXPR using ENCODING { A_B says }
	
	second u16 can be SCRIPT_FIRMWARE_u16 
	second u16 can u64_WORD = EIGHT_t TABLE[ idx ] ; WORD.PAIR_TWO_API
	second u16 can EIGHT_t TABLE[ idx ] . PAIR_TWO_API // not direct ASM //
	seconf u16 might be in u48_BITFIELD

	used as bool is_ipper[c1] or mapping (special handlers for 0)

	USAGE: SPARSE allocate a few one letter variable names
	USAGE: EXACT alloc [0[N idx2 = resolve[ idx1 ] // "A" -> 3 //
	USAGE: EXACT alloc [0[N { "A" "SESS_CPU_STACK.local_vars[idx2]" }
	USAGE: EXACT alloc [0[N { idx2 = lookup "A" } // single letter name
	USAGE: SPARSE alloc ALIAS for long_name as single letter "i" for idx
	USAGE INVOKE MACRO idx2 == "idx" // STRING VALUE WILL WORK MOSTLY //
	USAGE INVOKE MACRO idx == "idx2" // any varname "idx2" var value "idx"

	We dont have to have this STEP,
	can have a FULL PAGE of [256] ITEM // NULL means NULL // VNULL -> emptyh
	for use on RAW DATA, with less checking (YES check NULL in this PAGE)
	for use on RAW DATA, without "{ idx2_t idx2 = EXACT_FROM_SPARSE idx1 }"
	idx2_t might be [u3] idx = u3_bitfield from OPCODE // via_CACHE
	Z





*/

#include "misc.h"
#include "ints.h"
#include "e_print.h"

class cset_byte_map : public GRP_lib_base
{
public:
	u8	byte[ 256 ];

	u8 &	operator [] ( u8 idx )
	{
		return byte[ idx ];
	}
	void	set_null( void )
	{
		for( int i=0; i<256; i++ )
			byte[ i ] = 0;
	}
	cset_byte_map & operator =( cset_byte_map & rhs )
	{
		for( int i=0; i<256; i++ )
			byte[i] = rhs.byte[i];
		return *this;
	}
	cset_byte_map & operator |= ( cset_byte_map & rhs )
	{
		for( int i=0; i<256; i++ )
			byte[i] |= rhs.byte[i];
		return *this;
	}
	cset_byte_map & operator &= ( cset_byte_map & rhs )
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

