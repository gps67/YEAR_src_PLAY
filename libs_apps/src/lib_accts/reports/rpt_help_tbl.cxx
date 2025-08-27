// #include "report2.h"
// #include "tbl_col_spec_plus.h"
// #include "dgb_fail.h"
// #include "tbl_col_spec_date_rel.h"

#include "rpt_help_tbl.h"

namespace ACCTS {

//	virtual
	help_tbl::~help_tbl()
	{
		tbl = NULL;
	}

	help_tbl::help_tbl()
	{
		tbl = NULL;
	}

	bool help_tbl::open_tbl( data_set * dset, str0 tablename )
	{
		if(!dset) return false;
		tbl = dset->get_table( tablename );
		if( tbl )
		{
		//	e_print(" Table %s already existed\n", (STR0) tablename );
			already_existed = true;
		}
		else
		{
			already_existed = false;
			tbl = dset->find_add_fake_table( tablename );
		}
		if( tbl ) iterator.set_tbl( tbl );
		return tbl;
	}

	bool help_tbl::late_init( data_set * dset, str0 tablename )
	{
		open_tbl( dset, tablename );
		get_specs();
		return true;
	}

	void help_tbl::set_tbl( table * _tbl )
	{
		tbl = _tbl;
		iterator.set_tbl( tbl );
	}

	void help_tbl::iterator_restart()
	{
		iterator.restart();
	}
	bool help_tbl::iterator_get_row()
	{
		if(! iterator.get_row() )
		{
			// clear();
			return false;
		}
		if(! fetch_data( iterator.row ) )
			return FALSE_dgb_fail("fetch data");
		return true;
	}
	bool help_tbl::fetch_row( str0 key )
	{
		if(!tbl) return false;
		row_inst * row = tbl->find_row( key );
		if(!row) return false;
		return fetch_data( row );
	}
	bool help_tbl::fetch_row_or_fail( str0 key )
	{
		if(fetch_row( key )) return true;

		if(!tbl) return FAIL("NULL TABLE key=%s", (STR0) key );;

		return FAIL("Absent table.field %s.%s",
			(STR0) tbl->get_name(),
			(STR0) key
		);
	}
	bool help_tbl::iterator_finished()
	{
		return iterator.finished();
	}

///////////////

bool help_ACCT::get_specs()
{
	spec_key     =                  tbl ->must_get_col_spec( "key" );
	spec_initbal = (col_spec_amnt*) tbl ->must_get_col_spec( "amount" );
	spec_desc    =                  tbl ->must_get_col_spec( "desc" );
	spec_end_bal = (col_spec_amnt*) tbl ->must_get_col_spec( "amnt_closing" );

	spec_initbal -> is_summable = true;
	spec_end_bal -> is_summable = true;

	if(!spec_key    ) return FALSE_dgb_fail("NULL spec_key");
	if(!spec_initbal) return FALSE_dgb_fail("NULL spec_initbal");
	if(!spec_desc   ) return FALSE_dgb_fail("NULL spec_desc");
	if(!spec_end_bal) return FALSE_dgb_fail("NULL spec_end_bal");

	return true; // optimistic
}

help_ACCT::help_ACCT( data_set * dset )
: help_tbl()
{
	late_init( dset, "ACCT" );
}

bool help_ACCT::fetch_data( row_inst * row )
{
	_initbal.set_zero();
	_key   = spec_key     -> get_str2_for_edit( row );
	_desc  = spec_desc    -> get_str2_for_edit( row );
	         spec_initbal -> get_amnt( row, _initbal );
	if(!_key) return FAIL("NULL _key for row");
	return true; // optimistic
}

bool help_ACCT::write_back_end_bal( row_inst * acct, const amnt_t & amnt )
{
	if( ! spec_end_bal )
		return FALSE_dgb_fail("NULL spec_end_bal");
	if( ! acct )
		return FALSE_dgb_fail("NULL acct");
	// e_print("write_back_end_bal %s %s\n",
	// 	(STR0) acct->debug_row_name(),
	// 	(STR0) amnt.get_nocurr_str()
	// );
	return spec_end_bal -> set_amnt( acct, amnt );
}

void help_ACCT::copy_all_balances()
{
	if( ! spec_end_bal )
		return_VOID_dgb_fail("NULL spec_end_bal");
	iterator_restart();	// presume .. something
	while( iterator_get_row() )
	{
		spec_end_bal -> set_amnt( iterator.row, _initbal );
	}
}


////////////////////


help_CAT::help_CAT( data_set * dset )
: help_tbl()
{
	late_init( dset, "CAT" );
}

bool help_CAT::get_specs()
{
	spec_dotdot  = tbl ->must_get_col_spec( "dotdot" );
	spec_key     = tbl ->must_get_col_spec( "key" );
	spec_desc    = tbl ->must_get_col_spec( "desc" );
	if(!spec_key    ) return FALSE_dgb_fail("NULL spec_key");
	if(!spec_dotdot ) return FALSE_dgb_fail("NULL spec_dotdot");
	if(!spec_desc   ) return FALSE_dgb_fail("NULL spec_desc");
	return true;
}

bool help_CAT::fetch_data( row_inst * row )
{
	_key    = spec_key     -> get_str2_for_edit( row );
	_desc   = spec_desc    -> get_str2_for_edit( row );
	_dotdot = spec_dotdot  -> get_linked_row( row );
	if(!_key) return FAIL("NULL _key for row");
	return true; // optimistic
}


////////////////////


help_REF::help_REF( data_set * dset )
: help_tbl()
{
	late_init( dset, "REF" );
}

bool help_REF::get_specs()
{
	spec_dotdot  = tbl ->must_get_col_spec( "dotdot" );
	spec_key     = tbl ->must_get_col_spec( "key" );
	spec_desc    = tbl ->must_get_col_spec( "desc" );
	if(!spec_key    ) return FALSE_dgb_fail("NULL spec_key");
	if(!spec_dotdot ) return FALSE_dgb_fail("NULL spec_dotdot");
	if(!spec_desc   ) return FALSE_dgb_fail("NULL spec_desc");
	return true;
}

bool help_REF::fetch_data( row_inst * row )
{
	_key    = spec_key     -> get_str2_for_edit( row );
	_desc   = spec_desc    -> get_str2_for_edit( row );
	_dotdot = spec_dotdot  -> get_linked_row( row );
	if(!_key) return FAIL("NULL _key for row");
	return true; // optimistic
}


////////////////////


help_ERA::help_ERA( data_set * dset )
: help_tbl()
{
	late_init( dset, "ERA" );
}

bool help_ERA::get_specs()
{
	spec_dotdot  = tbl ->must_get_col_spec( "dotdot" );
	spec_key     = tbl ->must_get_col_spec( "key" );
	spec_desc    = tbl ->must_get_col_spec( "desc" );
	if(!spec_key    ) return FALSE_dgb_fail("NULL spec_key");
	if(!spec_dotdot ) return FALSE_dgb_fail("NULL spec_dotdot");
	if(!spec_desc   ) return FALSE_dgb_fail("NULL spec_desc");
	return true;
}

bool help_ERA::fetch_data( row_inst * row )
{
	_key    = spec_key     -> get_str2_for_edit( row );
	_desc   = spec_desc    -> get_str2_for_edit( row );
	_dotdot = spec_dotdot  -> get_linked_row( row );
	if(!_key) return FAIL("NULL _key for row");
	return true; // optimistic
}


////////////////////


help_VAT::help_VAT( data_set * dset )
: help_tbl()
{
	late_init( dset, "VAT" );
}

bool help_VAT::get_specs()
{
	spec_key     =                    tbl ->must_get_col_spec( "key" );
	spec_rate   = (col_spec_percent*) tbl ->must_get_col_spec( "rate" );
	spec_desc    =                    tbl ->must_get_col_spec( "desc" );
	if(!spec_key  ) return FALSE_dgb_fail("NULL spec_key");
	if(!spec_rate ) return FALSE_dgb_fail("NULL spec_rate");
	if(!spec_desc ) return FALSE_dgb_fail("NULL spec_desc");
	return true;
}

bool help_VAT::fetch_data( row_inst * row )
{
	_key    = spec_key   -> get_str2_for_edit( row );
	_desc   = spec_desc  -> get_str2_for_edit( row );
	          spec_rate  -> get_percent( row, _rate );
	if(!_key) return FAIL("NULL _key for row");
	return true; // optimistic
}


////////////////////


help_INFO::help_INFO( data_set * dset )
: help_tbl()
{
	late_init( dset, "INFO" );
}

bool help_INFO::get_specs()
{
	spec_key     = tbl ->must_get_col_spec( "key" );
	spec_value    = tbl ->must_get_col_spec( "value" );
	if(!spec_key    ) return FALSE_dgb_fail("NULL spec_key");
	if(!spec_value   ) return FALSE_dgb_fail("NULL spec_value");
	return true;
}

bool help_INFO::fetch_data( row_inst * row )
{
	_key    = spec_key     -> get_str2_for_edit( row );
	_value   = spec_value    -> get_str2_for_edit( row );
	if(!_key) return FAIL("NULL _key for row");
	return true; // optimistic
}

bool help_INFO::must_fetch_value( str0 & val, str0 key ) 
{
	if(!fetch_row_or_fail( key ))
		return FAIL_FAILED();
	val = _value;
	return true;
}

bool help_INFO::fetch_value_default( str0 & val, str0 key, str0 deflt ) 
{
	if(!tbl) {
		val = deflt;
		FAIL("NULL tbl");
		return true;
	}
	if(fetch_row( key )) {
		val = _value;
		return true;
	}
	val = deflt;
	return true;
}

bool help_INFO::must_fetch_date( date_glib & dt, str0 key )
{
	str0 date_str = "1999-12-31"; // looks suspect ?
        if(!must_fetch_value(date_str,key))
		return FAIL_FAILED();
	date_str = _value;
	if(!dt.set_year_mm_dd( date_str ))
		return FAIL("badly formed date %s", (STR0) date_str );;
	return true;
}

bool help_INFO:: must_fetch_date_year_start( date_glib & dt )
{
	return must_fetch_date( dt, "year_start" );
}

bool help_INFO:: must_fetch_date_next_year_start( date_glib & dt )
{
	return must_fetch_date( dt, "next_year_start" );
}

// 
}; // NAMESPACE
//////////////////////////
