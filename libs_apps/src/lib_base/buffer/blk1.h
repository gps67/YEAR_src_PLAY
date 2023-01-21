#ifndef BLK1_H
#define BLK1_H

//	u8 BLK[ nbytes ] // but already a stretch version // often STR0
//	BLK & blk // base class of buffer1 which has printf //
//	blk.put_byte( u8 )
//	blk.put_nbytes( STR, N ) 
//	blk.put_nwords( MEM, N ) // load SEGMENT
//	blk.put_actually( 3 word sub-segment of buff ) // INLINE MACRO USED
//	blk.put_dialect - strange group of fprint functions
//
//	blk == STRETCH BUFFER print bytes to STR0 // MINI_MACHINE compiled
//	blk == STRETCH_BUFFER stdout // then break into pages of 4K
//	blk == DATA readable by MINI_MACHINE and with SPEC

#include <string.h>
#include "misc.h"
#include "str0.h"
#include "p0p2.h"
#include "is_diff.h"
#include "dgb.h"

typedef unsigned char uchar;
#define BLK1_N_STATIC 48 // often allocated on stack space
// 3 words in 16 bytes // 48 bytes // 64 total // ish //

// to change that // recompile everything // from CONSTEXPR to OPTIMIESED OUT 

#include "ASCII_chars.h"
// now defined as static const char DEFINE
// #ifndef ASCII_NUL
// #define ASCII_NUL 0
// #endif

// #ifndef ASCII_LF
// #define ASCII_LF '\n'
// #endif

//
// ASCII_NAME // _NAME // byte_name // %s expr // itemname("%s") byte_data _byte
// byte_name %s // Module == "token == idx // CMNT " IDX // u8 idx // u8_idx //
// u8 Name = MINI_MACHINE_step_name // CMNT

// ASCII_BYTE ASCII_%s BYTE %s
// CODE_POINT HERE match %s 
// STR = VAR_NAME VAR_DATA // DATA = STR in UTF8_from_BYTE encoding_filter
// STR = VAR_NAME("%s","VAR_NAME") // BENCH OPTION LIKE_ _VAR_NAME // _t //
// _t // idx_TOK = N++ // lookup %s // alloc %d idx_TOK // CODE_POINT //
// CODE_POINT // EA_SCRIPT_XPOS // XPOS == "{ P0P2_of_TEXT  P1_as_PTR }"
// XPOS == "{ P0P2_of_TEXT  P1_as_PTR } // CMNT "
// XPOS == OFFS // # triggers BIND VAR("OFFS") // connects this code 
// CPOS == CSR // CSR == idx // CSR == OFFS // CSR == EXPR CSR CTXT
//
// OK so very foundational XPOS == OFFS via CODE_POINT in SCRIPT
// DIALECT += HERE_LOCAL_DIALECT { SCRIPT } { EG TEST_SAMPLE_NN }
//
// HIDDEN above is use of MINI_MACHINE uses VAR_CALLED_BY_A_SINGLE_LETTER
// a few variables can be decoded { u8 RECODE( { u8_idx ITEM  = "Z" Z = UDEF
//


/*!
	A dynamic byte-based buffer with at least 60 (BLK1_N_STATIC) bytes.

		3 WORDS BASE=buff nbytes=u32_nbytes u32_OFFS

			3 WORDS HEAD {
				u8 * buff // { u8 * P0 } // STR0 // UTF8_TEXT
				u32 nbytes_alloc // u32_nbytes // nbytes //
				u32 nbytes_used // u32_OFFS CSR // PRINT_HEAD 
			}

			// P2 = P0 + N_alloc // N
			// P1 = P0 + N_used // N 
			// P0 = "{ u8 * P0 }"  ALIAS buff //
			// malloc PTR to STR0 // or binary

			// buff --> BOCK_OF_BYTES[ nbytes_alloc ]
			// QUOTA --> BOCK_OF_BYTES[ nbytes_used ]
			// OUTPUT is about CSR moving upto BUFF_FULL // GROW

			BYTES LAYOUT

			u64 PTR
			u32 N_used
			u32_N_alloc

			// that is 16 BYTES 
			// 64 - 16 == 48 

	Used for text buffers, for arrays, ... but the user must convert
	the type, this is bytes only. EG text buffers can be any encosing

	NB There can be ANY number of bytes used, but there is an
	initial overhead of 60 (BLK1_N_STATIC) bytes, but that doesnt use
	malloc (if a local buffer on the stack). Thats wasted when >60.

	NB LIMIT u32 nbytes; // NO LIMIT // u20 MB // 16_MB // u24_16_mb

	NB nbytes_used is set by the user, after calling get_space().

	There is no "gap_to_the_left"

	NB It is a lot easier to try to preserve the trailing NUL,
	so clear() presumes it is there or needs to be added.
	BUT a lot of code knows that it will add it later ...
	so think about a write-phase, read-phase, move-about-moments

	uchar == u8
*/
struct blk1 : public GRP_lib_base
{

	uchar * buff;		// dynamic buffer
	unsigned nbytes_alloc;	// dynamic buffer or static buffer size
	unsigned nbytes_used;	// current fit

	uchar   buff_static[BLK1_N_STATIC]; // stack allocated

	// No fn_malloc // stack alloc 64 bytes 8 words of u64
	// BLK0 = HEAD = u64 WORDS[4] // 48 byte preallocated starter buffer
	// 
	// u8 buff_static[64]; // alloca 64 // 
	// u8 BLK[ nbytes ] // output STR0 buffer // trailing_nul() //

//	NBYTES HEAD // see_above means  [P0[P2 <--N--> // other ascii art
	
	// END HEAD // 
	// END DATA // 

	/**/	blk1( int N = 0 );
	/**/	~blk1( void );

	blk1( const blk1 & b );
	blk1( const str0 & b );

	/*!
		set the buffer to an initial value
	*/
	blk1 & operator = ( const char * s )
	{
		set(s);
		return * this;
	}

	/*!
		set the buffer to an initial value
	*/
	blk1 & operator = ( str0 s )
	{
		set(s);
		return * this;
	}

	bool put_ASCII( const char * s ); // ASCII STR0 // ie _bytes

	bool set( const char * s ) { return set( (str0) s ); }
	bool set( str0 s );
	bool set( str0 s, int len );
	bool set( u8 * P, int len );
	bool set( p0p2 zone ) { return set( zone.p0, zone.str_len() ); }
	bool set_8859( p0p2 zone ) { return set( zone.p0, zone.str_len() ); }
	operator p0p2 () {
		return p0p2( buff, (buff+nbytes_used ) );
	}

	u8 * get_P0() { return buff ; }
	u8 * get_P2() { return buff+nbytes_used ; }

	bool set( blk1 & rhs ) {
		// no cset matching ... simply copy bytes over
		return set( rhs.buff, rhs.nbytes_used );
	}

	/*!
		NB trailing_nul not added if not already there and empty
	*/
	void	clear()
	{
		if(nbytes_used) clear_fn();
	}

	/*!
		restart the buffer, but retain the malloc's buffer
	*/
	void	clear_fn();

	/*
		crypto usage suggests we ought to erase memory sometimes
		The easiest way is to do it in free(mem), but the app
		might want to jump in and zero_used() before clear()
	*/
	void	zero_used();
	void	zero_above();
	void	zero_all();
	void	scrub(); // zero_all() and clear()
	void	zap() { scrub(); }
	void	dgb_dump(str0 msg) const;
	void	dgb_max_str(int max, u8 * msg);

	/*!
		an empty buffer is FALSE: an empty list or empty string
	*/
	operator bool() const
	{
		// seems this is not inherited
		// e_print("operator bool says nbytes_used %d", nbytes_used );
		return nbytes_used;
	}

	/*!
		realloc (down) to the exact used space
	*/
	bool	trim_size()
	{
		return resize_bytes( nbytes_used + 1 ); // trailing_nul
	}

	/*!
		get space for (at least) n bytes more
	*/
	bool	get_space( unsigned space )	// add padding (first time exact)=0)
	{
		unsigned required = nbytes_used + space; // what we need
		if( nbytes_alloc >= required )
			return TRUE;			// already got it
		return get_space_fn( space );
	}

	/*!
		return the amount of space actually available
	*/
	int	space_avail()
	{
		return nbytes_alloc - nbytes_used;
	}

	/*!
		a pointless, non-checking function,
		but it does give you confidence that it is the one to call,
		after all, there couldnt be two such functions ... search ...
	*/
	void	nbytes_used_add( unsigned added ) {
		nbytes_used += added;
	}

	/*!
		return the TEMP pointer to the buffer gap address
	*/
	char *	gap_addr_signed_char()
	{
		return (char*) gap_addr();
	}

	/*!
		return the TEMP pointer to the buffer gap address
	*/
	uchar *	gap_addr()
	{
		return & buff[ nbytes_used ];
	}

	/*!
		return the offset of the gap
	*/
	unsigned gap_offs( void )
	{
		return nbytes_used;
	} 

	bool	get_space_fn( unsigned space );
	bool	trailing_nul();
	bool	resize_bytes( unsigned N );
	void	del_copy_down( uchar * p2 );
	void	del_copy_down( int n );

	uchar get_last_byte(); // NUL if empty
	bool ends_with_lf() { return ASCII_LF == get_last_byte(); }

	/*!
		return the address of CHAR at index [i]
	*/
	uchar * raw_addr_of_offset( int offs )
	{
		// should validate
		return buff + offs;
	}

	/*!
		return the address of CHAR at index [i]
	*/
	uchar * addr_of_offset( int offs )
	{
		// should validate
		trailing_nul();
		if( offs < 0 ) {
			FAIL("negative offs");
			offs = 0;
		} else
		if( (unsigned) offs > nbytes_used ) {
			FAIL("attempt to seek past end of buffer");
			offs = nbytes_used;
		}
		return buff + offs;
	}

	/*
		blk1 used for append - chars
	*/


	/*!
		put a block of bytes
	*/
	bool	put_blk( const blk1 & b )
	{
		return put_nn_bytes( b.nbytes_used, b.buff );
	}

#if 0
	// adding these was a surprise to me
	// the problem was not that they were missing
	// but that using them whilst missing
	// cause a silent NOOP
	// probably cast from (char) to (blk1) and empty blk1 ??
	// not good tho
	//
	// am happy with put_byte
	// but want complaint

	bool	put( const char * s )
	{
		while(*s) if(!put(*s++)) return FAIL_FAILED();
		return true;
	}

	bool	put( char ch )
	{
		return put_byte(ch);
	}
#endif

	/*!
		fast put_byte for when you KNOW you have enough space
	*/
	void	raw_put_byte( uchar byte )
	{
		buff[nbytes_used++] = byte;
	}
	/*!
		append a byte to the buffer // no re-interp of char
	*/
	bool	put_byte( uchar byte )
	{
		if(!get_space( 1 )) return FALSE;
		raw_put_byte( byte );
		return TRUE;
	}
	/*!
		append a few bytes to the buffer
	*/
	bool	put_n_bytes( unsigned n, str0 _bytes )
	{
		const uchar * bytes = _bytes;
		if(!get_space( n )) return FALSE;
		while( 0 < n-- ) {
			raw_put_byte( *bytes++ );
		}
		return TRUE;
	}
	/*!
		append (many) bytes to the buffer using memcpy
	*/
	bool	put_nn_bytes( unsigned n, str0 bytes )
	{
		if(!get_space( n )) return FALSE;
		memcpy( & buff[ nbytes_used ], bytes, n );
		nbytes_used += n;
		return TRUE;
	}
	/*!
		append 2 bytes - useful for UTF8
	*/
	bool	put_2_bytes( uchar c1, uchar c2 )
	{
		if(!get_space(2)) return false; 
		raw_put_byte( c1 );
		raw_put_byte( c2 );
		return true;
	}
	/*!
		append 3 bytes - useful for UTF8
	*/
	bool	put_3_bytes( uchar c1, uchar c2, uchar c3 )
	{
		if(!get_space(3)) return false; 
		raw_put_byte( c1 );
		raw_put_byte( c2 );
		raw_put_byte( c3 );
		return true;
	}
	/*!
		append 4 bytes - useful for UTF8
	*/
	bool	put_4_bytes( uchar c1, uchar c2, uchar c3, uchar c4 )
	{
		if(!get_space(4)) return false; 
		raw_put_byte( c1 );
		raw_put_byte( c2 );
		raw_put_byte( c3 );
		raw_put_byte( c4 );
		return true;
	}

	/*!
		Cast the blk1 into a nul terminated string pointer.

		The returned str0 pointer should not be held for long.

		The NUL is added for you (but not permanantly added,
		its not counted in nbytes_used, and further put_byte(c)
		will overwrite it).

		str0 my_str = (str0) my_blk1;
		printf("%s\n", (STR0)(str0) blk1 );
	*/
	operator str0();
	operator STR0(); // it is a linker of mangled name
	operator STR0() const; // its not really const
	operator char *();

	// be careful with these
	// I only added after comparing with str1 failed (pointer different)
	// to match str0 == str0

	/*
		dont you just hate a language that can add implicit things,
		but not make this leap of interpretation:  !(a==b)

		I dont know what it was doing, maybe pointer inequality.
		Now I dont know _WHEN_ it is doing it, derived classes?
	*/

	bool operator != ( const char * rhs ) const {return !operator==(rhs); }
	bool operator != ( const str0 & rhs ) const {return !operator==(rhs); }
	bool operator != ( const blk1 & rhs ) const {return !operator==(rhs); }

	bool operator == ( const char * rhs ) const;
	bool operator == ( const str0 & rhs ) const;
	bool operator == ( const blk1 & rhs ) const;

	IS_DIFF_t cmp( const blk1 & rhs ) const;
	IS_DIFF_t cmp( const p0p2 & rhs ) const;
	IS_DIFF_t cmp( const char * rhs ) const;

	bool replace_nul_bytes( u8 c1 );

private:

	blk1 & operator = ( const blk1 & rhs ); // set( _ )

};

#endif
