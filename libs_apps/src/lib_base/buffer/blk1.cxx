#include "blk1.h"
#include "misc.h"
#include "e_print.h"
#include "dgb_fail.h"
#include "dgb.h"
#include <string.h> // memset

#include "buffer2.h"

extern "C"        void *memset(void *s, int c, size_t n);

// resize_bytes will throw if its request is more than this
// simply dont define it, to remove the debugging code
// #define MAX_resize_limit 1024*500
// CET_data_file is 600 K
#define MAX_resize_limit 1024*1024*2

#ifndef ASCII_NUL
#define ASCII_NUL 0
#endif


#warning TODO - add const for use with readonly parameters
#warning TODO - add operator= and copy ctors, so can be used without thought

void my_bzero( u8 * PTR, size_t nbytes )
{
	explicit_bzero( PTR, nbytes );
}

/*!
	constructor - eg to 50 bytes
*/
/**/	blk1::blk1( int N )
{
//	if(N==0) N = BLK1_N_STATIC;
	if(N==0) N = 1; // any non zero // reset knows logic of BLK1_N_STATIC

	nbytes_alloc = 0;
	nbytes_used = 0;
	buff = 0;

#if 1
	// it is OK to leave the buff_static as RAW OLD_VAL
	// it is also OK to clean up for tests
	my_bzero( buff_static, BLK1_N_STATIC);
	memset( buff_static, '_', BLK1_N_STATIC); // CATCH abset trailing_nul)
#endif

	// resize_bytes from 0 to 1 to BLK1_N_STATIC upto NN
	resize_bytes( N );
}

/*!
	constructor - copy - exact size with space for trailing_nul()
*/
/**/	blk1::blk1( const blk1 & b )
{
	nbytes_alloc = 0;
	nbytes_used = 0;
	buff = 0;
	// this will avoid allocating 3/2 times but allows a trailing NUL
	resize_bytes( b.nbytes_used + 1 );
	put_blk( b );
	trailing_nul();
}

/*!
	constructor - init from string, as exact size
*/
/**/	blk1::blk1( const str0 & s )
{
	nbytes_alloc = 0;
	nbytes_used = 0;
	buff = 0;
	int len = s.str_len();
	resize_bytes( len + 1 );
	set( s, len );
	trailing_nul();
}

/*!
	destructor - frees memory
*/
/**/	blk1::~blk1( void )
{
	zero_all();		// memset buff and STATIC area
	clear_fn();		// not using  (scrub() calls above and this)
	resize_bytes( 0 );		// releases memory
}

/*!
	get_space(n) ensures that there are n BYTES available to write into
	You call the inline get_space(n) macro, it calls this.
	Repeat the logic to allow direct calls.
*/
bool	blk1::get_space_fn( unsigned space )	// add padding (first time exact)=0)
{
	if(!buff) {
		buff_static[0] = ASCII_NUL;
		buff = buff_static;
		nbytes_alloc = BLK1_N_STATIC;
	}
	unsigned required = nbytes_used + space; // what we need
	if( nbytes_alloc >= required )
		return TRUE;			// already got it
	/*
		0 51 127 241 412 669 1054 1632 2499 ...
	*/
	int N = ((nbytes_alloc * 3)/2) + space + 50;	// ask for more (semi exp) 
	if( resize_bytes( N ) ) return TRUE;		// got more space
	N = nbytes_alloc + space;		// try asking for less (exact)
	return resize_bytes( N );
}

/*!
	resize_bytes to EXACTLY N bytes, free buffer if N==0

	except when buff_static was/will be used
	maybe add trailinbg_nul 

	TODO zero out memory, realloc, and resize

	TODO flag in bits -- along with CSET

		zero above
		zero data
	
	maybe relaxed clear when realloc and free, DTOR
	or add this to a list of things to clear
*/
bool	blk1::resize_bytes( unsigned N )	// to exact N bytes
{
#ifdef MAX_resize_limit
	/*
		NULL pointers lead to resize_bytes on silly values

		Other problems will cause buffer to grow beyond 500K
		or whatever limit you decide (50K is borderline likely)

	*/
	if( N > MAX_resize_limit ) {
		THROW_dgb_fail( "blk1::resize_bytes(N) - but N is too much");
		// return FALSE;
	}
#endif

	if( N == nbytes_alloc ) return TRUE;


	void * B; // from buffer A to buffer B which has N bytes
	if( N == 0 ) {
		B = NULL;
	} else {
		if( N <= BLK1_N_STATIC )
		{
			N = BLK1_N_STATIC;
			B = buff_static;
		} else {
			B = malloc( N );
			/*
				if malloc failed - report it here,
				but dont throw, as all callers check!

				report it how ??
			*/
			if(!B)
			{
				float MB = 1024.0 * 1024 * 1024;
				float mb = float(N) / MB; // bytes to MB
				// mught be 36 bytes

				return FAIL("malloc( %d ) // %5.2f MB", N, mb );
				return FALSE;
			}
		}
	}
	/*
		Damage limitation, the calling code should have done this.
	*/
	if( nbytes_used > N )
		nbytes_used = N ;

	/*
		copy over old data, switch blocks, release old

		LURK: doesn overwrite buff if nbytes_used == 0 
		so clear() then DTOR doesnt do as expected?
		see zero_all

		LURK: convoluted logic,
		the if( nbytes_used ) is to optimise the memcpy ?

		LURK: only zero-ing nbytes_used allows other
		truncators of nbytes_used to not zero-out tail
 
	*/
	if( buff && B && nbytes_used ) {
		if( buff != B ) {
			memcpy( B, buff, nbytes_used );
		}
	}
	if( buff ) {
		if( buff != buff_static ) {
		//	memset( buff, 0, nbytes_alloc );
			my_bzero( buff, nbytes_alloc );
			free( buff );
		}
	}
	buff = (uchar *)B ;
	nbytes_alloc = N;
	/*
		To know that your alloc guesses look like ... add this code
		It prints:   0 51 127 241 412 669 1054 1632 2499 ...
		if(0) {
			e_print( "nbytes_alloc=%d\n", nbytes_alloc );
		}
	*/
	return TRUE;
}

/*!
	overwrite the used data area with NUL bytes
*/
void	blk1::zero_used()
{
//	memset( buff, 0, nbytes_used );
	my_bzero( buff, nbytes_used );
	// trailing_nul(); // iff uses nonzero filler
}

/*!
	overwrite the UN-used data area with NUL bytes
*/
void	blk1::zero_above()
{
//	memset( gap_addr(), 0, space_avail() );
	my_bzero( gap_addr(), space_avail() );
	// trailing_nul(); // iff uses nonzero filler
}

/*!
	overwrite the current data area with NUL bytes
*/
void	blk1::zero_all()
{
//	The static might be worth not zeroing // gcc optimising
//	memset( buff_static, 0, BLK1_N_STATIC );
	my_bzero( buff_static, BLK1_N_STATIC );

	if( buff != buff_static ) {
		if(nbytes_alloc) { // rare - when nbytes_alloc is zero (CTOR)
			// definitely ZERO out malloced blocks // dont free
			my_bzero( buff, nbytes_alloc );
		}

	}
	// trailing_nul(); // iff uses nonzero filler
}
/*!
	overwrite the buffer NUL bytes, clear(), and trailing_nul() for luck

	NB memory remains allocated, for similar size reuse
*/
void	blk1::scrub()
{
	zero_all(); // before releasing malloced area
	clear();
}

/*!
	useful to show blk contents on e_print dgb log
*/
void	blk1::dgb_dump(str0 msg) const
{
	buffer2 printer;
	printer.hex_dump_into_8859( buff, nbytes_used );
	printer.drop_trailing_eoln();
	INFO( "%s ...\n# DATA # %s", (STR0) msg, (STR0) printer );
}

/*!
	useful to show blk contents on e_print dgb log
*/
void	blk1::dgb_max_str(int max, u8 * msg)
{
	int n = nbytes_used;
	if(n>max) n = max;
	buffer2 printer;
	printer.hex_dump_into_8859( buff, n );
	printer.drop_trailing_eoln();
	INFO( "%s ...\n# DATA # %s", (STR0) msg, (STR0) printer );
}

/*!
	ensure that string buffer is NUL terminated,
	WITHOUT including it in the nbytes_used area.
*/
bool	blk1::trailing_nul()
{
	if(!get_space(1)) return false;
	// actually, throwing an exception would be a better idea
	// or overwriting the last byte of the buffer
	// as no code is going to check the return value from trailing nul
	// however unlikely it is to only fail here, though ...
	*gap_addr() = 0;
	return true;
}

/*!
	restart the buffer, but retain the malloc's buffer
*/
void	blk1:: clear_fn()
{
	nbytes_used = 0;
	trailing_nul(); // incase zero_all used '+' symbol
}

/*!
	delete the n leading bytes

	LURK: this leaves bytes un-scrubbed (yawn)
*/
void	blk1::del_copy_down( int n )
{
	if(n<=0) {
		if( n == 0 ) {
			if(1) INFO("n == 0");
		} else {
			FAIL("n == %d", n );
		}
		return;
	}
	if( nbytes_used < (unsigned int) n ) { // sign mix
		FAIL("n = %d nbytes_used = %d", n, nbytes_used );
		clear();
		return;
	}
	uchar * p2 = &buff[n];
	nbytes_used -= n;
	if( nbytes_used <= 0 )
	{
		clear();
		return;
	}
	INFO("n = %d nbytes_used = %d", n, nbytes_used );
	// memmove( buff, p2, nbytes_used ); // correct
	memmove( buff, p2, nbytes_used + 1 ); // poss pre-existing trailing NUL
	// if there was one before, replace it
	// because it is not part of the nbytes_used, so not copied
	// or add one to above memmove (we know there is a gap)
	// trailing_nul();
}


/*!
	put STR0 without any convertion
*/
bool	blk1::put_ASCII( const char * s ) // ASCII STR0 // ie _bytes
{
	if(!s) {
		put_ASCII("null)");
		return FAIL("(null)");
	}
	return put_n_bytes( strlen(s), s );
}

/*!
	delete the (n bytes) leading upto p2 (not incl)
*/
void	blk1::del_copy_down( uchar * p2 )
{
	del_copy_down( p2 - buff );
}

/*!
	set the buffer to a string value
*/
bool blk1::set( str0 s )
{
	set( s, s.str_len() );
	return true;
}

/*!
	set the buffer to a string value, with (hidden) extra NUL for luck!
*/
bool blk1::set( u8 * P, int len )
{
	nbytes_used = 0;
	return put_nn_bytes( len,  P );
}

bool blk1:: set( str0 s, int len )
{
	nbytes_used = 0;
	put_n_bytes( len,  s.str );
	trailing_nul();
	return true;
}

/*
	This is really annoying,

/media/X3/YEAR/src/libs_apps/src/lib_base2/js_1/j_cx.cxx(85):
error: more than one user-defined conversion from "buffer1" to "str0" applies:
            function "blk1::operator str0()"
            function "str0::str0(buffer1 &)"
                return buf; // function return type is str0
                       ^

	It was annoying when I had to create tha multiple declarations,
	It is annoying that its not a inor error
	It is annoying that I dont know which I want to be correct

	str0 should be 'dumb' compared with buffer2
	str0::str0( buffer1 & buf ) 
*/

blk1::operator str0()
{
	trailing_nul();
	return (str0) (char *) buff;
}

blk1::operator char *()
{
	trailing_nul();
	return (char *) buff; // signed
}

blk1::operator STR0() {

	// call of .. is ambiguous ... looks like CTOR of str0
	trailing_nul();
	return (STR0) buff;
 //	return (STR0) (str0) (char *) buff;
}

blk1::operator STR0() const // not really const
{
	blk1 * that = const_cast<blk1 *>( this );
	// call of .. is ambiguous ... looks like CTOR of str0
	that->trailing_nul();
	return (STR0) that->buff;
 //	return (STR0) (str0) (char *) buff;
}

bool blk1:: operator == ( const char * rhs ) const
{
	return IS_SAME == cmp( rhs );
}

bool blk1:: operator == ( const str0 & rhs ) const
{
	return IS_SAME == cmp( (STR0) rhs );
}

bool blk1:: operator == ( const blk1 & rhs ) const
{
	return IS_SAME == cmp( rhs );
}

IS_DIFF_t blk1:: cmp( const char * rhs ) const
{
	p0p2 lhs_p0p2 ( buff, nbytes_used );
	return (IS_DIFF_t) lhs_p0p2.cmp( rhs );
}

IS_DIFF_t blk1:: cmp( const p0p2 & rhs_p0p2 ) const
{
	p0p2 lhs_p0p2 ( buff, nbytes_used );
	return (IS_DIFF_t) lhs_p0p2.cmp( rhs_p0p2 );
}

IS_DIFF_t blk1:: cmp( const blk1 & rhs ) const
{
	p0p2 lhs_p0p2 ( buff, (int) nbytes_used ); // limits size
	u8 * rhs_p0 = (u8*) rhs.buff; // un const cast
	p0p2 rhs_p0p2 ( rhs_p0, rhs.nbytes_used );

	return (IS_DIFF_t) lhs_p0p2.cmp( rhs_p0p2 );
}

bool blk1:: replace_nul_bytes( u8 c1 )
{
	unsigned int N = nbytes_used;
	u8 * P = buff;
	for(unsigned int i=0; i<N; i++ ) {
		if(*P == ASCII_NUL) *P = c1;
		P++;
	}
	return true; // cannot fail
}

uchar blk1:: get_last_byte() // NUL if empty
{
	unsigned int POS = nbytes_used - 1;
	if( POS < 0 ) {
		WARN("empty buffer");
		return ASCII_NUL;
	}
	return buff[ POS ];
}
