#ifndef pdp_blob_H
#define pdp_blob_H

#include "post_array.h"
#include "ints.h"
#include "t_v.h"
#include "zblk.h"

/*!
	pdb_blob is a generic BLOB, but subclasses are used for normal C++
	reasons to indicate the TYPE of the data.
*/
class pdb_blob : public  NS_POST::ArrayOfByte
{
 public:
	pdb_blob( int n, void * data );
	CLASSINFO( pdb_blob, NO_REFS );
	static pdb_blob * mk_blob( NS_POST::storage * sto, int n, void * buff );
};

/*!
	pdb_blobz is a generic zlib compressed BLOB,
	but subclasses are used for normal C++
	reasons to indicate the TYPE of the data.

	pdb_blobz is not registered with pdb, but does zlib
*/
class pdb_blobz : public pdb_blob
{
 public:
	CLASSINFO( pdb_blobz, NO_REFS );
	bool un_compress_bytes(
		int expected_byte_count,
		void * buff
	);
	pdb_blobz( int n, void * data ) : pdb_blob( n, data ) { };
};

/*!
	pdb_blob_fltz is registered with pdb. It is a zlib compressed
	array of n floats, as used by SAR_DATA segments.

	To ensure that you comply with NS_POST requirements, call the
	static mk_XXX functions 
*/
class pdb_blob_fltz : public pdb_blobz
{
 public:
	CLASSINFO( pdb_blob_fltz, NO_REFS );
	pdb_blob_fltz( int n, void * data );

  static
	pdb_blob_fltz * mk_blob_fltz_from_compressed(
		NS_POST::storage * sto,
		int nbytes,
		void * data
	);

  static
	pdb_blob_fltz * mk_blob_fltz(
		NS_POST::storage * sto,
		int n,
		float * data
	);

	bool un_compress(
		int expected_count,
		float * buff
	);

};

/*!
	pdb_blob_tvz is registered with pdb. It is a zlib compressed
	array of n of (Time,Value) pairs, as used by SAR_DATA T_V segments.

	To ensure that you comply with NS_POST requirements, call the
	static mk_XXX functions 
*/
class pdb_blob_tvz : public pdb_blobz
{
 public:
	CLASSINFO( pdb_blob_tvz, NO_REFS );
	pdb_blob_tvz( int n, void * data );

  static
	pdb_blob_tvz * mk_blob_tvz_from_compressed(
		NS_POST::storage * sto,
		int nbytes,
		void * data
	);

  static
	pdb_blob_tvz * mk_blob_tvz(
		NS_POST::storage * sto,
		int n,
		t_v * data
	);

	bool un_compress(
		int expected_count,
		t_v * items
	);
};

#endif
