#include "tbl_col_floats_base.h"
#include "tbl_collect.h"

#include "dset_api.h"

/*!
	constructor - copy spec1 but change opt,dev,attrname
*/
tbl_col_floats_base::tbl_col_floats_base(
	mem_line_spec * spec1,
	const char * opt,
	const char * dev,
	const char * attr
) {
	setup_opt_dev_attr( spec1, opt, dev, attr );
	ary.get_space( 60 * 24 ) ;
}

/*!
	used by constructor - copy spec1 but change opt,dev,attrname
*/
void tbl_col_floats_base::setup_opt_dev_attr(
	mem_line_spec * spec1,
	const char * opt,
	const char * dev,
	const char * attr
) {
	spec.copy_from( spec1 );
 
	const char * dev2 = "-";
	if( dev ) dev2 = dev;
 
	spec.dev.set( dev2 );
	spec.opt.set( opt );
	spec.attr.set( attr );
}

/*!
	used by constructor - copy tbl->spec1 but change opt,dev,attrname
*/
void tbl_col_floats_base::setup_opt_dev_attr(
	tbl_collect * tbl,
	const char * opt,
	const char * dev,
	const char * attr
) {
	setup_opt_dev_attr( &tbl->spec1, opt, dev, attr );
}

/*!
	send this segment to a dset (this class isnt a dset_api though)
*/
bool tbl_col_floats_base::send_to_dset( dset_api * dset )
{
	return dset->merge_floats( &spec, t0, N(), base() );
}

