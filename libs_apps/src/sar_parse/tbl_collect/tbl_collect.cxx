#include <stdio.h>
// #include "tm_parts.h"
#include "tbl_collect.h"

#include "dset_api.h"

/*!
	constructor - more data will be required for each col-spec, t0, dt,
	but that data may come later and be retro-fitted
*/
tbl_collect::tbl_collect(
	const char * site_name,
	const char *  cmd_name
){
	ncol = 0;
	time0 = 0;
	spec1.site.set( site_name );
	spec1.cmd.set( cmd_name );
	spec1.avg.set( "-" );		// or is that "1"
	spec1.linetype.set( "series" ); // SET HERE for pdb/other
//	spec1.set_dt( 0 ); 			// first realign uses unset dt
}


/*!
	create a new column with a new line-spec.

	The line-spec might be incomplete (dt not known until first sar data
	line, and the parser has already created the standard attr cols)

	The returned pointer is retained (borrowed) so the data top cannot move
	(but its dynamic array can)
*/
tbl_col_floats_base * tbl_collect::create_col(
	const char * opt,
	const char * dev,
	const char * attr
){
	/*
		How many data rows are held by the sibling cols.
		Are there any siblings?
	*/
	int nrow = 0;
	if( ncol )
		nrow = data[0]->N();

	tbl_col_floats_base * col = new tbl_col_floats_base(
		&spec1, opt, dev, attr
	);

	// col->t0 = time0;
	col->set_t0( time0 ); // this re-aligns to dt // else merge crashes

	/*
		With SAR a previously idle device can "appear" anytime.
		so set its history as zero, or some tracable value
	*/
	if( nrow )
		col->back_fill( nrow, 0.000421 );

	// add col to ring
	data.extend_by(1);
	data[ ncol ] = col;
	ncol++;
	
	return col;
}

/*!
	get space for lots of rows - call after creating the cols
*/
bool tbl_collect::get_space( int extra_rows )
{
	bool ok = true;
	for( int col = 0; col < ncol; col++ ) {
		if(! data[col]->get_space( extra_rows )) ok = false;
	}
	return ok;
}

/*!
	tbl_collect DOES own the data - sar_parser_temps does NOT
*/
tbl_collect::~tbl_collect()
{
	for( int i=0; i<ncol; i++ )
	{
		if(0) fprintf(stderr,"~tbl_collect() deleting col %d\n", i );
		tbl_col_floats_base * col = data[ i ];
		data[i] = NULL;
		delete col;
		
	}
}

/*!
	now that dt is known set it - NOTE that this means the linespec
	was created without dt being known (not searched for)
*/
void tbl_collect::set_dt_in_all_cols(int dt)
{
	spec1.dt = dt;
	if( !ncol ) 
		return;
	for( int col = 0; col < ncol; col++ ) {
		data[col]->set_dt(dt);
	}
}

/*!
	now that T0 is known, set it
*/
void tbl_collect::set_t0_in_all_cols(int t0)
{
	time0 = t0;
	if( !ncol ) 
		return;
	for( int col = 0; col < ncol; col++ ) {
		data[col]->set_t0(t0);
	}
}

/*!
	now that host is known, set it 
*/
void tbl_collect::set_host_in_all_cols(const char * host)
{
	spec1.host.set(host);
	if( !ncol ) 
		return;
	for( int col = 0; col < ncol; col++ ) {
		data[col]->spec.host.set(host);
	}
}

/*!
	now that time0 is known, set it 
*/
void tbl_collect::set_time0(time_t t0)
{
	time0 = t0;
	set_t0_in_all_cols(t0);
}

/*!
	extend all arrays by 1 BEFORE placing values there

	That means N will be 1 off, but you do have a slot to
	place data. You could also place data repeatedly, then
	sample every while
*/
bool tbl_collect::prep_next_row()
{
	if( !ncol ) 
		return TRUE;
	bool ok = TRUE;
	for( int col = 0; col < ncol; col++ ) {
		if( ! data[col]->prep_next() )
			ok = FALSE;
	}
	return ok;
}

/*!
	undo prep_next() 
	(EOF arrived instead of data.)
*/
bool tbl_collect::undo_prep_next_row()
{
	if( !ncol ) 
		return TRUE;
	bool ok = TRUE;
	for( int col = 0; col < ncol; col++ ) {
		if( ! data[col]->undo_prep_next() )
			ok = FALSE;
	}
	return ok;
}

/*!
	send a selected col to dset - some parsers might want
	to drop some data, if it has too many columns
*/
bool tbl_collect::send_col_to_dset( int col, dset_api * dset )
{
	if( (col < 0) || (col > ncol) ) {
		e_print("# ERROR send_col_to_dset() col/ncol %d/%d\n", col, ncol );
		return FALSE;
	}
	return data[col]->send_to_dset( dset );
}

/*!
	send all columns to dset
*/
bool tbl_collect::send_to_dset( dset_api * dset )
{
	bool ok = TRUE;
	for( int col = 0; col < ncol; col++ ) {
		if(! send_col_to_dset( col, dset )) ok = FALSE;
	}
	return ok;
}

// ----- THIS IS BEING MOVED TO dset_print ----

void tbl_collect::zxxx_prints(FILE * f)
{
	fprintf( f, "\n" );
	for( int i = 0; i < ncol; i++ ) {
		tbl_col_floats_base * fs = data[i];
		fprintf( f, "%d host %s cmd %s opt %s dev %s attr %-7s val %8.3f\n",
			i,
			fs->spec.host.str,
			fs->spec.cmd.str,
			fs->spec.opt.str,
			fs->spec.dev.str,
			fs->spec.attr.str,
			*fs->ptr()
		); 
	}
}

void tbl_collect::zxxx_dumps(FILE * f)
{
	fprintf( f, "\n" );
	for( int i = 0; i < ncol; i++ ) {
		tbl_col_floats_base * fs = data[i];
		int nrow = fs->N();
		fprintf( f, "%d site %s host %s cmd %s opt %s dev %s attr %-7s dt %d [%d] {",
			i,
			fs->spec.site.str,
			fs->spec.host.str,
			fs->spec.cmd.str,
			fs->spec.opt.str,
			fs->spec.dev.str,
			fs->spec.attr.str,
			fs->spec.dt,
			nrow
		); 
/*
		float * base = fs->base();
if(0)		for( int r = 0; r<nrow; r++ ) {
			if( !((r)%6)) {
				tm_parts tm;
				time_t t1 = time0 + r*spec1.dt;
				tm.local_from_time( t1 );
				fprintf( f, "\n%s", tm.str_hh_mm_ss() );
			//	fprintf( f, "\n%s", tm.str_fmt1() );
			}
			fprintf( f, "%8g ", base[r] );
		}
*/
		fprintf( f, "}\n" );
	}
}
