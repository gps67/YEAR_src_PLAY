
#include "pdb_blob.h"
REGISTER( pdb_blob );
REGISTER( pdb_blobz );
REGISTER( pdb_blob_fltz );
REGISTER( pdb_blob_tvz );

#include "post_array.h"

#if 0
	// this was OK with gcc-3.5
	// and it makes more sense to me (with namespace qualification)
	REGISTER_TEMPLATE( NS_POST::array_of_ptr<pdb_blob_fltz> );
#endif

#if 1
	// this compiled and linked and ran
	// why MUST it be in NS_POST namespace ??
namespace NS_POST {
	REGISTER_TEMPLATE( array_of_ptr<pdb_blob_fltz> );
};
#endif

#include "e_print.h" // e_print

/*!
	construct a pdb_blob from n bytes of src data, see mk_blob
*/
pdb_blob::pdb_blob( int n, void * src )
: NS_POST::ArrayOfByte( n )
{
	memcpy( (void *)body(), src, n );
}

/*!
	construct a pdb_blob from n bytes of src data,
	and use the NS_POST specific new operator.
*/
pdb_blob * pdb_blob::mk_blob( NS_POST::storage * sto, int n, void * data )
{
	return new( self_class, *sto, n - 1) pdb_blob ( n, data);
}

/*!
	construct a pdb_blob_fltz from n BYTES of data
*/
pdb_blob_fltz::pdb_blob_fltz( int n, void * data )
: pdb_blobz( n, data )
{
}

/*!
	construct a pdb_blob_tvz from n BYTES of data
*/
pdb_blob_tvz::pdb_blob_tvz( int n, void * data )
: pdb_blobz( n, data )
{
}


/*!
	static function that PROPERLY calls the NS_POST new function
*/
pdb_blob_fltz * pdb_blob_fltz::mk_blob_fltz_from_compressed(
	NS_POST::storage * sto,
	int nbytes,
	void * data
) {
	return new( self_class, *sto, nbytes - 1) pdb_blob_fltz ( nbytes, data);
}

/*!
	static function that PROPERLY calls the NS_POST new function
*/
pdb_blob_fltz * pdb_blob_fltz::mk_blob_fltz(
	NS_POST::storage * sto,
	int n,
	float * data
)
{
	static zblk z; // NOT THREAD SAFE
	z.clear();

	if( !z.compress( (char *) data, n * sizeof(*data) ) ) {
		e_print("pdb mk_blob_fltz() z.compress failed\n" );
		return NULL;
	}
	return mk_blob_fltz_from_compressed(
		sto,
		z.buff.nbytes_used,
		z.buff.buff
	);
}

/*!
	static function that PROPERLY calls the NS_POST new function
*/
pdb_blob_tvz * pdb_blob_tvz::mk_blob_tvz_from_compressed(
	NS_POST::storage * sto,
	int nbytes,
	void * data
) {
	return new( self_class, *sto, nbytes - 1) pdb_blob_tvz ( nbytes, data);
}

/*!
	static function that PROPERLY calls the NS_POST new function,
	and also compresses the data for you
*/
pdb_blob_tvz * pdb_blob_tvz::mk_blob_tvz(
	NS_POST::storage * sto,
	int n,
	t_v * data
) {
	static zblk z; // NOT THREAD SAFE
	z.clear();

	if( !z.compress( (char *) data, n * sizeof(*data) ) ) {
		return NULL;
	}
	return mk_blob_tvz_from_compressed(
		sto,
		z.buff.nbytes_used,
		z.buff.buff
	);
}

// un_compress

/*!
	base class extracts data for you, but you are expected
	to know exactly how many bytes the data contains.
*/
bool pdb_blobz::un_compress_bytes(
	int expected_nbytes,
	void * flts
) {
	uLongf actual_nbytes = expected_nbytes; // zlib stops here, exact?
	int zlen = get_size();
	void * zdata = body();
	int t = uncompress(
		(Bytef*) flts,
		& actual_nbytes,
		(Bytef*) zdata,
		zlen
	);
	if( t != Z_OK ) {
		fprintf(stderr, "uncompress fail %d\n", t );
		return FALSE;
	}
	if( actual_nbytes != (uLongf) expected_nbytes ) {
		fprintf(stderr, "un_compress: expected %d bytes got %ld FAIL\n",
			expected_nbytes, actual_nbytes );
		return FALSE;
	}
	return TRUE;
}

/*!
	extract exactly expected_count floats from the (entire) blob
*/
bool pdb_blob_fltz::un_compress(
	int expected_count,
	float * flts
) {
	int expected_nbytes = expected_count * sizeof( flts[0] );
	return un_compress_bytes( expected_nbytes, flts );
}

/*!
	extract the entire blob into exactly expected_count T_V pairs
*/
bool pdb_blob_tvz::un_compress(
	int expected_count,
	t_v * items
) {
	int expected_nbytes = expected_count * sizeof( items[0] );
	return un_compress_bytes( expected_nbytes, items );
}
