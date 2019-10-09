#ifndef BLK1_H
#define BLK1_H
#include <string.h>
#include "misc.h"
#include "str0.h"

typedef unsigned char uchar;
#define BLK1_N_STATIC 60 // stack allocated

/*!
	A dynamic buffer for strings, arrays or streams, byte-based
	and can contain binary data.
*/
struct blk1 : public GRP_lib_base
{

	unsigned nbytes_alloc;	// dynamic buffer
	unsigned nbytes_used;	// current fit
	uchar * buff;		// dynamic buffer
	uchar   buff_static[BLK1_N_STATIC]; // stack allocated

	/**/	blk1( int N = 0 );
	/**/	~blk1( void );

	blk1( const blk1 & b );
	blk1( const str0 & b );

	blk1 & operator = ( const char * s )
	{
		set(s);
		return * this;
	}

	blk1 & operator = ( str0 s )
	{
		set(s);
		return * this;
	}

	bool set( str0 s );
	bool set( str0 s, int len );

	/*!
		restart the buffer, but retain the malloc's buffer
	*/
	void	clear()
	{
		nbytes_used = 0;
	}

	operator bool()
	{
		return nbytes_used;
	}

	/*!
		realloc (down) to the exact used space
	*/
	bool	trim_size()
	{
		return resize( nbytes_used );
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
	bool	resize( unsigned N );
	void	del_copy_down( uchar * p2 );
	void	del_copy_down( int n );

	/*
		blk1 used for append
	*/

	bool	put( const blk1 & b )
	{
		return put_nn_bytes( b.nbytes_used, b.buff );
	}

	/*!
		fast put_byte for when you KNOW you have enough space
	*/
	void	raw_put_byte( uchar byte )
	{
		buff[nbytes_used++] = byte;
	}
	/*!
		append a byte to the buffer
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
	operator str0()
	{
		trailing_nul();
		return (str0) (char *) buff;
	};

};

#endif
