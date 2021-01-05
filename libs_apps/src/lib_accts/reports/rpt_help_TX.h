#ifndef rpt_help_TX_H
#define rpt_help_TX_H

#include "rpt_help_tbl.h"

namespace ACCTS {

/*!
	help_TX_base is the superset of TRANS and XFER in one
	
	it is also the base class of help_REPORT (which writes but
	doesnt read)
*/
class help_TX_base : public help_tbl
{
 public:
	obj_hold<col_spec>	 spec_key;	// both
	obj_hold<col_spec_date>	 spec_date;		// XFER
	obj_hold<col_spec_date>	 spec_date_tax;			// TRANS
	obj_hold<col_spec_date>	 spec_date_pay;			// TRANS
	obj_hold<col_spec>	 spec_cat;	// both
	obj_hold<col_spec_amnt>	 spec_amnt;	// both
	obj_hold<col_spec>	 spec_vat;			// TRANS
	obj_hold<col_spec>	 spec_acct;			// TRANS
	obj_hold<col_spec>	 spec_acct1;		// XFER
	obj_hold<col_spec>	 spec_acct2;		// XFER
	obj_hold<col_spec>	 spec_ref;	// both
	obj_hold<col_spec>	 spec_img;	// both
	obj_hold<col_spec>	 spec_desc;	// both

	obj_hold<str2>	  _key;			// both
	date_glib	  _date;			// XFER
	date_glib	  _date_tax;				// TRANS
	date_glib	  _date_pay;				// TRANS
	row_inst	* _cat;			// both
	row_inst	* _acct;				// TRANS
	row_inst	* _acct1;			// XFER
	row_inst	* _acct2;			// XFER
	row_inst	* _ref;			// both
	str2		* _img;			// both
	str2		* _desc;		// both
	amnt_t  	  _amnt;		// both
	row_inst	* _vat;					// TRANS

	help_TX_base();

	// tbl was set by late_init
	bool get_specs_both();
	bool get_specs_trans();
	bool get_specs_xfer();
	bool get_specs_all();

	bool check_specs_both();
	bool check_specs_trans();
	bool check_specs_xfer();
	bool check_specs_plus();
	bool check_specs_all();

	bool fetch_data_both( row_inst * row );
	bool fetch_data_trans( row_inst * row );
	bool fetch_data_xfer( row_inst * row );

	obj_hold<col_spec_date_rel>	spec_date_tax_rel;	// PLUS
	obj_hold<col_spec_date_rel>	spec_date_pay_rel;	// PLUS
	obj_hold<col_spec_amnt>		spec_amnt_xfer;		// PLUS
	obj_hold<col_spec_amnt>		spec_amnt_vat;		// PLUS
	obj_hold<col_spec_amnt>		spec_amnt_pre;		// PLUS

	amnt_t	  _amnt_xfer;
	amnt_t	  _amnt_vat; // is sum of euro and uk
	amnt_t	  _amnt_pre; // is sum of euro and uk

	bool write_data_both( row_inst * row );
	bool write_data_trans( row_inst * row );
	bool write_data_xfer( row_inst * row );
	bool write_data_plus( row_inst * row );

	void clear_TX();

	// TX_base is used by RPT_base TRANS and XFER

};

/*!
	help_TRANS reads from the TRANS table,
	and feeds copy_TRANS_to_REPORT
*/
class help_TRANS : public help_TX_base
{
 public:

 //
	bool get_specs();
 //
	bool fetch_data( row_inst * row );
	bool create_new_row() { return false; }
	bool write_row_back() { return false; }
	void inittrans();

	help_TRANS( data_set * dset );
};

/*!
	loads data row from an XFER table row
*/
class help_XFER : public help_TX_base
{
 public:

	bool get_specs();
 	bool fetch_data( row_inst * row );

 //	bool create_new_row();
 //	bool write_row_back();
	help_XFER( data_set * dset );
};

}; // NAMESPACE
#endif

