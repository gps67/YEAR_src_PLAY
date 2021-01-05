
#include "misc.h"
#include "blk1.h"
#include "vm18flt_tbl.h"

/*
	This is a demo of dyn_array (which is a strange thing).
	The number of reserved rows is initially set high.
	Each iteration finds get_space(1) is OK

	Each row is added WITHOUT updating nbytes_used!
	Instead one single (int nrow) is maintained and (nrows_reserved)
	The data is poked directly in (ising operator[] but no bounds checks)

	You must update nbytes_used before any re-alloc!
	so that the correct amount is copied
*/

vm18flt_tbl::vm18flt_tbl( int expect ) {
	nrow = 0;
	nrow_reserved = 0;
	clear_row();

	for(int c=0; c<N18; c++) {
		col[c].get_space_for_total( expect );
	}
}

vm18flt_tbl::~vm18flt_tbl() {}

void vm18flt_tbl::clear_row() {
	for(int c=0; c<N18; c++) {
		row[c] = -1.0;
	}
}

void vm18flt_tbl::update_nbytes_used() {
	for(int c=0; c<N18; c++) {
		col[c].extend_to( nrow );
	}
}

BOOL vm18flt_tbl::reserve(int at_least) {
	if(at_least <= nrow_reserved)
		return TRUE;
	/*
		here nbytes_used MUST be updated, incase of realloc
		above it isnt necessary, because sufficient space

		After LIMIT reached, this increments by 1
		but blk1 jumps semi-exponentially.
		Either fetch space_avail (least) or jump at_least by 10
		or get caller to reserve the correct (jumping) amount)
	*/
	update_nbytes_used();
	for(int c=0; c<N18; c++) {
		if( ! col[c].get_space_for_total( at_least ) )
			return FALSE;
	}
	nrow_reserved = at_least;
	return TRUE;
}

BOOL vm18flt_tbl::add_row() {
	if(nrow == nrow_reserved) { // FULL
		reserve( nrow + 20 );	// realloc less
	}
	if( !reserve( nrow + 1 ) )
		return FALSE;
	// there is lots of offs calcs, which is repeated :-(
	// except operator[] is inlined - so might be a common expr :-)
	for(int c=0; c<N18; c++) {
		col[c][nrow] = row[c];
		// float * P = (float *)(blk[c].buff + offs);
		// *P = row[c];
		row[c] = -1.0;
	}
	// nbytes_used isnt updated, under this umbrella (nrow)
	nrow++;
	// clear_row(); // done as row[c]=-1;
	return TRUE;
}
