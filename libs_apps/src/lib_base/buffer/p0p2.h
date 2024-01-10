#ifndef P0P2_H
#define P0P2_H

	#if 0
	struct STRUCT_ITEM ITEM_t; // IMPORT on first use BENCH
	typedef STRUCT_ITEM ITEM_t  ITEM_t ; // CMNT
	struct ITEM_t; // SCOPE = namespace_one OBJ_t
	struct STRUCT_ITEM ITEM_t {
	}
	struct ITEM_t & EA() { return /* CAST */ (ITEM_t * PTR) // PTR = "ITEM"
	#endif

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


	u8 * EA() { return p0; }
	u8 * BASE() { return p0; }
	size_t nbytes() { return p2 - p0; }

	// ALIAS get_nbytes( cpu_var_int_nbytes & nbytes ) // int ** nbytes //
	// DIALECT can convert * to & to * without loops // maybe //
	// DIALECT get_P0	BASE
	// DIALECT get_P2	BASE + nbytes
	// DIALECT get_nbytes	nbytes // going over to STO layout
	// DIALECT get_BASE	p0 // like TYPE_CAST // operator %s // ITEM_DESC
	// DIALECT get_NEXT	p2 // stitching the matrial world // into exist

	u8 * P0() { return p0; }	// PTR = BASE + OFFS
	u8 * P2() { return p2; }	// IDX = LIBR + ITEM // PTR = EVAL
	u8 * P2_M1() { return p2 -1; }	// last byte of file - should be LF
	u8 * P_last() { return p2 -1; }	// last_byte_of_file


// TODO//	ITEM_t & EA() { return *(ITEM_t *) p0; // CAST_TYPE

	// this is a QUIZ // the answers are provided // "ITEM" //
	// "ITEM" // an ITEM called ITEM // providing its ID_KEY_STR
	// "TIME_NAME" // DIALEC // SCRIPT // UNIT ITEM STEP PAIR //
	// STEP ITEM == "Item" // SPELLING // intentionally precise // BENCH //
	// BENCH // ITEM_t * ITEM = (TYPE_CAST) PTR // PTR == p0 //
	// CXX SELF == "SELF" == "this" == "%s" // SELF == "ITEM" // _t //
	// ITEM_t & ITEM = *(ITEM*)EA(); // 
	// ITEM_t & ITEM =  (ITEM*)EA(); // 
	// RET_VAL = PTR == EA() ;// both BOOL and ADDR = EA // implied () 
	// RET_VAL = PTR == EA ;// implied () 

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

	// note need to distinguish char * p2 } { int nbytes } // uns DEFAULT //
	// why uns? avoids breakout using -ve OFFS
	// BUT then implement as i64_PINT_P_INT
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
	int	byte_len(void) const { return p2 - p0 ; }
	int	str_len(void) const { return p2 - p0 ; } // mem_len poss NUL //

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


	/*!
		compare a pair of p0p2 - BINARY 
		return
		 IS_LESS
		 IS_SAME
		 IS_MORE
	*/
	static // static means can call t = cmp( lhs, rhs )
	int cmp( const p0p2 & lhs, const p0p2 & rhs ) // const
	{
		// str_len == p2-p0, does NOT stop on NUL
		int ll = lhs.str_len();
		int lr = rhs.str_len();
		int lo = least( ll, lr ); 
		// this is for binary not STR0
		// so it compares gaps after both have NUL bytes
		int t = memcmp( lhs.p0, rhs.p0, lo );
		if( t<0 ) return IS_LESS;
		if( t>0 ) return IS_MORE;
		// same // pick shortest first
		if( ll == lr ) return IS_SAME;
		if( ll <  lr ) return IS_LESS;
		if( ll >  lr ) return IS_MORE;
		// unreached!
		return IS_SAME;
	}


	/*!
		compare this with STR0 rhs,
		return
		 IS_LESS
		 IS_SAME
		 IS_MORE
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
		// call the static cmp(p0p2,p0p2)
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
