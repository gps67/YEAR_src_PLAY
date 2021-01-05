#ifndef vm18flt_tbl_H
#define vm18flt_tbl_H

#define N18 18
typedef float vm18flt_row[N18];

#include <string.h>
#include "misc.h"
#include "blk1.h"
#include "dyn_array.h"

typedef dyn_array<float> flt_col;

class vm18flt_tbl {
public:
	vm18flt_row row;
//	blk1 blk[N18];
	flt_col col[N18];
	int nrow;
	int nrow_reserved;

	vm18flt_tbl( int expect=0 );
	~vm18flt_tbl();
	void clear_row();
	void update_nbytes_used();
	bool reserve(int at_least);
	bool add_row();
	float * get_col( int c ) {
		return col[c].ary_base();
	}
};

#endif
