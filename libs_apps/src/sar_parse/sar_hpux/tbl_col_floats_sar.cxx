#include <stdio.h>
#include "tbl_col_floats_sar.h"
#include "tbl_collect.h"

/*!
	constructor - dev is NULL
*/
tbl_col_floats_sar::tbl_col_floats_sar(
	tbl_collect * tbl,
	const char * opt,
	const char * attr
)
: tbl_col_floats_wrap( tbl, opt, NULL, attr )
{
}

/*!
	constructor - asks tbl to create a new table and point to it
*/
tbl_col_floats_sar::tbl_col_floats_sar(
	tbl_collect * tbl,
	const char * opt,
	const char * dev,
	const char * attr
)
: tbl_col_floats_wrap( tbl, opt, dev, attr )
{
}
