#ifndef FLOATS_SAR_H
#define FLOATS_SAR_H

#include "tbl_col_floats_wrap.h"

class tbl_collect;

/*!
	tbl_col_floats_sar is a _wrap which is effectively a
	POINTER to the real tbl_col which is held by the tbl.
*/
class tbl_col_floats_sar : public tbl_col_floats_wrap {
public:
	tbl_col_floats_sar(
		tbl_collect * tbl,
		const char * opt,
		// dev = "-",
		const char * name
	);
	tbl_col_floats_sar(
		tbl_collect * tbl,
		const char * opt,
		const char * dev,
		const char * name
	);
};
#endif
