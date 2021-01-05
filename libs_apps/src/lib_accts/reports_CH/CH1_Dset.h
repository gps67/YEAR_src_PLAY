#ifndef CH_Dset_H
#define CH_Dset_H

#include "CH1_Names.h"
#include "dset.h"
#include "str1.h"
#include "amnt_t.h"
#include "tbl_col_spec_plus.h"
#include "rpt_tbl_iter.h"

namespace ACCTS {
	class data_set;
	class table;
};

namespace ACCTS {
struct CH1_Dset
{

	// will probably want to change variables stored ?
	// which will change size of this structure
	// and also effect user of this
	//
	// or dont go adding too many fields, without recompile !deps
	//
	int year2;
	// CH1_Names CH1_names;
	// bool mk_CH_Name( buf, year2, tag );


	// CH1_filename;
	// rpt_filename;
	// PL2_filename;
	// csv_PREV;
	// csv_CURR;

	obj_hold<data_set> dset;
	obj_hold<table> tbl_CH1;

	obj_hold<col_spec_amnt> col_spec_PREV;
	obj_hold<col_spec_amnt> col_spec_CURR;
	obj_hold<col_spec> col_spec_DESC;

	tbl_iter iter;
	obj_hold<col_spec> spec_CH1_key;
	obj_hold<col_spec_amnt> spec_CH1_CURR;

	bool init_from_year2( int _year2 );
	bool uses_column();
	bool zero_column();

	// private
	bool load_file( str0 filename );
	bool save_file( str0 filename );

	bool load_draft(); // always load last years live file
	bool load_play(); // mostly unused, restart from start
	bool save_play(); // always save pre-draft play copy
	bool save_draft(); // mostly unused, do manual copy

	bool calc_sums();

  /*
	dset.tbl_CH1[ rowkey ] = "100.00";
  */
  bool set_CURR_row_val_rounded( str0 rowkey, str0 valstr ) // "NFS_SH_s", "1000.0" )
  {
	amnt_t val;
	if(! val.set( valstr )) return FAIL_FAILED(); // never fails !!
	return set_CURR_row_val_rounded( rowkey, val );
  }

  bool set_CURR_row_val_rounded( str0 rowkey, amnt_t val ) // "NFS_SH_s", 1.23
  {
	amnt_t val2 = val;
	val2.round_for_CH1();
	return set_CURR_row_val( rowkey, val2 );
  }

  /*
	dset.tbl_CH1[ rowkey ] = 1.23; // NB rint before 
  */
  bool set_CURR_row_val( str0 rowkey, amnt_t val ) // "NFS_SH_s", 1.23 )
  {
	// note that valstr is AMOUNT because CURR is spec of all numbers
	// add some code to table* for LR IO desc key etc
	row_inst * row = tbl_CH1->find_row( rowkey );
	if(!row) return FAIL_FAILED();
	if(! col_spec_CURR -> set_amnt( row, val ))
		return FAIL_FAILED();
	return true;
  }

  bool get_CURR_row_val( str0 rowkey, amnt_t & val ) // "NFS_SH_s", var )
  {
	row_inst * row = tbl_CH1->find_row( rowkey );
	if(!row) return FAIL("row_key = '%s'", (STR0) rowkey );
	// NO if(!row) return FAIL_FAILED();
	if(! col_spec_CURR -> get_amnt( row, val ))
		return FAIL_FAILED();
	return true;
  }

  bool get_CURR_row_val_rounded( str0 rowkey, amnt_t & val ) // "NFS_SH_s", var )
  {
	if(!get_CURR_row_val( rowkey, val )) return FAIL_FAILED();
	val.round_for_CH1();
	return true;
  }

  bool get_PREV_row_val( str0 rowkey, amnt_t & val ) // "NFS_SH_s", var )
  {
	row_inst * row = tbl_CH1->find_row( rowkey );
	if(!row) return FAIL_FAILED();
	if(! col_spec_PREV -> get_amnt( row, val ))
		return FAIL_FAILED();
	return true;
  }

  bool get_PREV_row_val_rounded( str0 rowkey, amnt_t & val ) // "NFS_SH_s", var )
  {
	if(!get_PREV_row_val( rowkey, val )) return FAIL_FAILED();
	val.round_for_CH1();
	return true;
  }

  // use these

  bool get_PREV( str0 rowkey, amnt_t & val ) {
	return get_PREV_row_val_rounded( rowkey, val );
  }

  bool get_CURR( str0 rowkey, amnt_t & val ) {
	return get_CURR_row_val_rounded( rowkey, val );
  }

  bool set_CURR( str0 rowkey, amnt_t & val ) {
	return set_CURR_row_val_rounded( rowkey, val );
  }


	bool iter1_use_CH1() {
		if(!tbl_CH1) return FAIL("NULL CH1");
		spec_CH1_CURR = col_spec_CURR; // already set
		spec_CH1_key = tbl_CH1 -> must_get_col_spec("key");
		if(! spec_CH1_key ) return FAIL_FAILED();
		if(! spec_CH1_CURR ) return FAIL_FAILED();
		iter.set_tbl( tbl_CH1 );
		return true;
	}
	str2 * iter1_get_CH1_row_key() { 
		return spec_CH1_key -> get_str2_for_csv( iter.row );
	}
	void iter1_set_current_row_CURR_col_to_rounded_amnt( amnt_t amnt ){
		amnt.round_for_CH1(); // no need
		spec_CH1_CURR -> set_amnt( iter.row, amnt );
	}
	bool iter1_check_ok()
	{
		return true;
	}
	bool iter1_get_row(){
		return iter.get_row();
	}

	bool get_DESC( str0 row_key, obj_hold<str2> & desc ) {
		if(!col_spec_DESC) return FAIL_FAILED();
		row_inst * row = tbl_CH1->find_row( row_key );
		if(!row) {
			return FAIL_FAILED(); // desc str2 unchanged 
		}
		desc = col_spec_DESC->get_str2_for_display( row );
		if(!desc) return FAIL_FAILED();
		return true;
	}

};

}; // namespace
#endif
