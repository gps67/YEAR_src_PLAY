#include "rpt_help_RPT_base.h"
#include "enum_flags.h" // ROW_show_sums

using namespace ACCTS;

bool help_RPT_base::create_new_row_123()
{
	row_inst * row = tbl->find_row_add_fake( _key );
	spec_key         ->set_from_edit( row, *_key );
	return write_row_back_123( row );
}


bool help_RPT_base::write_row_back_123( row_inst * row )
{
	if(!row) return false;

	bool ok = true;
	if(! write_data_both( row ) ) ok = false; // NOT key
	if(! write_data_trans( row ) ) ok = false; // NOT acct
	if(! write_data_xfer( row ) ) ok = false; // NOT date
	if(! write_data_plus( row ) ) ok = false;
	if(! write_back_vats( row ) ) ok = false;
	

	// should I be writing back sums when there are none ?
	if(!write_row_back_sums( row )) return false; 

	return ok;
}

/*!
	write the sum columns back into the RPT table

	each sum can be a value or a subtotal (or empty),
*/
bool help_RPT_base::write_row_back_sums( row_inst * row )
{
	if(!row) return false;

	if( row_show_sums )
	{
		// build tree picks up this flag and sets bg-colour
		row->row_flags |= ROW_show_sums;
	}

	// write back the running total (its also what the pdf displays)
	int nc = spec_sums.N();
	for( int i=0; i< nc; i++ )
	{
		col_spec_amnt * spec = spec_sums[i];
		if( row_show_sums )
		{
			spec->set_amnt( row, _sums[i].sum );	// SUBTOTAL
		}
		else if( _sums[i].used )
		{
			spec->set_amnt( row, _sums[i].val ); // VALUE
			_sums[i].step();
		}
		else {
			// leave blank or zero , when cell was unused
		}
	}

	row_show_sums = false;
	return true; // optimistic
}
