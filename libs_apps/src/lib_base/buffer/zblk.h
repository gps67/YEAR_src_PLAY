#ifndef zblk_H
#define zblk_H

/*
	zblk: compress a single mem block and hold the result
	grow buffer stretches to the largest ever needed

	know size from t0 t2 line.dt + type
	but prefer general multi-part processor?
	(particularly with list of blocks through SQL filter)
*/

#include <zlib.h>
#include "misc.h"
#include "blk1.h"

/*!
	A zblk is a blk1 which does compression/decompression.

	Maybe the two should be separated.
*/
struct zblk : public GRP_lib_base
{
	blk1	buff;
	int	srclen;
	zblk();
	~zblk();
	void clear() { buff.clear(); }
	bool compress( const char *src, uLong len);
	bool expand( blk1 & outbuff, uLong expected_len);

	bool test();

};

#endif
