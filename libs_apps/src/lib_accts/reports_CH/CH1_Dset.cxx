#include "CH1_Dset.h"

using namespace ACCTS;

  bool CH1_Dset:: save_file( str0 filename )
  {
	if(!dset) return FAIL("NULL dset");
	if(!dset->file_save( filename ))
		return FAIL_FAILED();
	return PASS("saved %s", (STR0) filename );
  }

  bool CH1_Dset:: save_play()
  {
	buffer1 b1;
	return save_file( str_CH_name( b1, year2, e_CH1_archive_csv_play ) );
  }

  bool CH1_Dset:: save_draft() // semi live // manually copied over
  {
	buffer1 b1;
	return save_file( str_CH_name( b1, year2, e_CH1_archive_csv ) );
  }

  bool CH1_Dset:: load_file( str0 filename )
  {
	dset = new data_set();
	if(!dset) return FAIL_FAILED();
	if(!dset->file_load( filename ))
		return FAIL_FAILED();

	tbl_CH1 = dset->get_table("CH1");
	if(!tbl_CH1) return FAIL("need table called CH1");

	/*
		the loaded file MUST have a valid column for this year
		that will include DEFAULT numbers for shares
		but smudge over them anyways
	*/
	if(!uses_column()) { return FAIL_FAILED(); } // create if absent
	if(!uses_column()) { return FAIL_FAILED(); } // retain is exists

	return true;
  }

  bool CH1_Dset:: load_draft()
  {
	buffer1 b1;
	return load_file( str_CH_name( b1, year2, e_CH1_archive_csv ) );
  }

  bool CH1_Dset:: load_play()
  {
	buffer1 b1;
	return load_file( str_CH_name( b1, year2, e_CH1_archive_csv_play ) );
  }

  bool CH1_Dset:: init_from_year2( int _year2 ) // _and_load
  {
	year2 = _year2;

	col_spec_PREV = NULL;
	col_spec_CURR = NULL;
	col_spec_DESC = NULL;

	if(!load_draft()) return FAIL_FAILED();
	if(!save_play()) return FAIL_FAILED();

	PASS("DONE");
	return true;
  }

  /*
	add a new column for this CURR year Y2011_2012
	PREV column must exist, there is no way to build from scratch
	(the rows already exist, as do the IO LR and desc fields)
	init to all zeros, then splice in share values 100
  */
  bool CH1_Dset:: uses_column()
  {
	// names are used to obtain SPEC's (which also hold names)
	buffer1 col_PREV;// Y2010_11 // stored in CH1_names which doesnt change
	buffer1 col_CURR;// Y2011_12 // stored in CH1_names which doesnt change
	mk_CH_name( col_PREV, year2, e_CH1_col_PREV );
	mk_CH_name( col_CURR, year2, e_CH1_col_CURR );

	str0 col_DESC = "desc"; // hardcoded
	col_spec_DESC = (col_spec*) tbl_CH1->must_get_col_spec( col_DESC );

	// need to add a new column to the LEFT of previous year
	// OK if it already exists ?
	col_spec_PREV = (col_spec_amnt*) tbl_CH1->must_get_col_spec( col_PREV );
	if(!col_spec_PREV) {
		return FAIL("previous years data MUST exist (%s)", 
			(STR0) col_PREV );
	}

	col_spec_CURR = (col_spec_amnt*) tbl_CH1->add_type_col_before(
		"amount",	// col_spec type
		 col_CURR,	// Y2011_12 new col name
		 col_PREV	// Y2010_11 add new before this old
	);
	if(!col_spec_CURR) return FAIL_FAILED();
	
	PASS("DONE");
	return true;
  }

  /*
	init the new col to all zeros, then splice in share values 100
  */
  bool CH1_Dset:: zero_column()
  {
	if(!uses_column()) { return FAIL_FAILED(); } // create if absent

/*
	DONE BY USES COLUMN

	// need to add a new column to the LEFT of previous year
	// OK if it already exists ?
	if(!col_spec_PREV) {
		return FAIL("previous years data MUST exist (%s)", col_PREV );
	}
	if(!col_spec_CURR) {
		return FAIL("current years data MUST exist (%s)", col_CURR );
	}
*/

	amnt_t amnt_zero;
	amnt_zero.set_zero();
	amnt_zero.round_for_CH1(); // was already, but test anyway
	int n = tbl_CH1->rows_list.N();
	for( int r = 0; r < n; r++ ) {
		row_inst * row = tbl_CH1->rows_list[ r ];
		col_spec_CURR -> set_amnt( row, amnt_zero );
	}

	set_CURR_row_val_rounded( "NFS_SH_s", "1000.0" );
	set_CURR_row_val_rounded( "NFS_SH_t",  "100.0" );
// -2.22 goes to -3.0 which may be wrong but will be invisible
//	set_CURR_row_val_rounded( "PL1_a",  "-2.22" );

	PASS("DONE");
	return true;
  }

// ------------------------------------------------------------



