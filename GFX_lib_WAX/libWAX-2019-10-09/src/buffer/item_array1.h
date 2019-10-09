#ifndef ITEM_ARRAY_1
#define ITEM_ARRAY_1

struct item_array1 : public buffer1 {

// IE //	unsigned nbytes_alloc;
// IE //	unsigned nbytes_used;
// IE //	char * buff;
// ADDS:
		unsigned item_size;

// array of item_size (item type implied elsewhere see item_array2 template)

/**/	item_array1( unsigned item_size1, int N = 0 ): buffer1( N*item_size1 )
{
	item_size = item_size1;		// eg 4 for u32 or char * in argv
}
bool	resize( unsigned N )		// to exactly N items
{
	unsigned nb = N * item_size;
	return blk1::resize( nb );
}
bool	get_space( unsigned N )		// for at least N items
{
	unsigned nb = N * item_size;
	return blk1::get_space( nb );
}
char *	item_addr( unsigned i )		// you cast (your_type*)
{
	return & buff[ i * item_size ];
}
unsigned item_count( void )
{
	return nbytes_used / item_size;
}
bool	put_n_item( unsigned n, char * bytes ) // surrounding template-api
{
	return put_nn_bytes( n * item_size, bytes );
}
};

/*
	used to build argv
*/

#endif
