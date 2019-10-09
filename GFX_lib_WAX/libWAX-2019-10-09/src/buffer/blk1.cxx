#include "blk1.h"
#include "misc.h"
#include "e_print.h"

/*!
	constructor - eg to 50 bytes
*/
/**/	blk1::blk1( int N )
{
	nbytes_alloc = 0;
	nbytes_used = 0;
	buff = 0;
	resize( N );
}

/*!
	constructor - copy
*/
/**/	blk1::blk1( const blk1 & b )
{
	nbytes_alloc = 0;
	nbytes_used = 0;
	buff = 0;
	resize( b.nbytes_used + 1 );
	put( b );
}

/*!
	constructor - copy
*/
/**/	blk1::blk1( const str0 & s )
{
	nbytes_alloc = 0;
	nbytes_used = 0;
	buff = 0;
	int len = s.str_len();
	resize( len + 1 );
	put( s );
}

/*!
	destructor - frees memory
*/
/**/	blk1::~blk1( void )
{
	nbytes_used = 0;
	resize( 0 );		// releases memory
}

/*!
	get_space(n) ensures that there are n BYTES available to write into
	You call the inline get_space(n) macro, it calls this.
	Repeat the logic to allow direct calls.
*/
bool	blk1::get_space_fn( unsigned space )	// add padding (first time exact)=0)
{
	unsigned required = nbytes_used + space; // what we need
	if( nbytes_alloc >= required )
		return TRUE;			// already got it
	/*
		0 51 127 241 412 669 1054 1632 2499 ...
	*/
	int N = ((nbytes_alloc * 3)/2) + space + 50;	// ask for more (semi exp) 
	// N = nbytes_alloc + space;		// test exact grow (slow)
	if( resize( N ) ) return TRUE;		// got more space
	N = nbytes_alloc + space;		// try ask for exact
	return resize( N );
}

/*!
	resize to EXACTLY N bytes, free buffer if N==0

	except when buff_static was/will be used
*/
bool	blk1::resize( unsigned N )	// to exact N bytes
{
	if( N == nbytes_alloc ) return TRUE;

#if 0
	if( N <= BLK1_N_STATIC )
	{
		if( !buff )
		{
			nbytes_alloc = BLK1_N_STATIC;
			buff = buff_static;
			return true;
		}

		if( buff == buff_static )
		{
			nbytes_alloc = BLK1_N_STATIC;
			return true;
		}

		if( nbytes_used > N ) nbytes_used = N;
		if( nbytes_used ) memcpy( buff_static, buff, nbytes_used );
		free( buff );
		buff = buff_static;
		nbytes_alloc = BLK1_N_STATIC;

		return true;

		// That still leaves increasing buffer escaping buff_static
	}
#endif

	void * B;
	if( N == 0 ) {
		B = NULL;
	} else {
		if( N <= BLK1_N_STATIC )
		{
			N = BLK1_N_STATIC;
			B = buff_static;
		} else {
			B = malloc( N );
			if(!B) return FALSE;
		}
	}
	/*
		Damage limitation, nbytes_used unchanged
	*/
	if( nbytes_used > N )		// calling code should release first
		nbytes_used = N ;	// base class damage limitation
	/*
		copy over old data, switch blocks, release old
	*/
	if( buff && B && nbytes_used ) {
		if( buff != B )
			memcpy( B, buff, nbytes_used );
	}
	if( buff ) {
		if( buff != buff_static )
			free( buff );
	}
	buff = (uchar *)B ;
	nbytes_alloc = N;
	/*
		To know that your alloc guesses look like ... add this code
		It prints:   0 51 127 241 412 669 1054 1632 2499 ...
		if(0) {
			uchar s[100];
			sprintf( s, "nbytes_alloc=%d\n", nbytes_alloc );
			write(2,s, strlen(s));
		}
	*/
	return TRUE;
}

/*!
	ensure that string buffer is NUL terminated,
	WITHOUT including it in the nbtes_used area.
*/
bool	blk1::trailing_nul()
{
	if(!get_space(1)) return false;
	*gap_addr() = 0;
	return true;
}

/*!
	delete the n leading bytes
*/
void	blk1::del_copy_down( int n )
{
	if(n<=0) return;
	uchar * p2 = &buff[n];
	nbytes_used -= n;
	if( nbytes_used <= 0 )
	{
		clear();
		return;
	}
	memmove( buff, p2, nbytes_used );
}

/*!
	delete the (n bytes) leading upto p2 (not incl)
*/
void	blk1::del_copy_down( uchar * p2 )
{
	del_copy_down( p2 - buff );
}

bool blk1::set( str0 s )
{
	nbytes_used = 0;
	set( s, s.str_len() );
	return true;
}

bool blk1::set( str0 s, int len )
{
	put_n_bytes( len,  s.str );
	trailing_nul();
	return true;
}
