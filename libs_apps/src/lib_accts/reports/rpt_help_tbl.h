#ifndef rpt_help_tbl_H
#define rpt_help_tbl_H

#include "obj_hold.h"
#include "dset.h"
#include "tbl_col_spec_plus.h"
#include "dgb_fail.h"
#include "tbl_col_spec_date_rel.h"

#include "rpt_tbl_iter.h"

namespace ACCTS {
/*
	Design change - again
	A more things unravel, I cant escape making the fields hardcoded
	(with a dynamic 'any-others' list). As a feeler, and patch whilst
	that isnt so, the table/rows are loaded into these structures.
	In future the IO_CSV will load into structures, where row_inst
	is derived from/points to the fixed structure. Each field
	will be registered with a get/set function (etc)

	Interestingly JAVA reflection could build the get/set list,
	python could do something, and C++ would require a CORE fixed list.
*/

class map_acct : public GRP_lib_accts
{
	map_acct();
};

/*!
	help_tbl helps with loading, saving and iterating rows.
*/
class help_tbl : public GRP_lib_accts
{
 public:
	table * tbl;
	tbl_iter iterator;

	bool already_existed;

	virtual ~help_tbl();
	help_tbl();
//	help_tbl( table * _tbl );
//	help_tbl( data_set * dset, str0 tablename );
	bool late_init( data_set * dset, str0 tablename );
	bool open_tbl( data_set * dset, str0 tablename );
	void set_tbl( table * _tbl );

	void iterator_restart();
	bool iterator_get_row();
	bool fetch_row( str0 key );
	bool fetch_row_or_fail( str0 key );
	bool iterator_finished();
	virtual bool fetch_data( row_inst * row ) = 0;
	virtual bool get_specs() = 0;
};

/*!
*/
class help_REF : public help_tbl
{
 public:

	col_spec 	* spec_dotdot;
	col_spec 	* spec_key;
	col_spec 	* spec_desc;

	row_inst	* _dotdot;
	str2		* _key;
	str2		* _desc;

	bool get_specs();
 	bool fetch_data( row_inst * row );

 //	bool create_new_row();
 //	bool write_row_back();
	help_REF( data_set * dset );
};

/*!
*/
class help_CAT : public help_tbl
{
 public:

	col_spec 	* spec_dotdot;
	col_spec 	* spec_key;
	col_spec 	* spec_desc;

	row_inst	* _dotdot;
	str2		* _key;
	str2		* _desc;

	bool get_specs();
 	bool fetch_data( row_inst * row );

 //	bool create_new_row();
 //	bool write_row_back();
	help_CAT( data_set * dset );
};

/*!
*/
class help_ERA : public help_tbl
{
 public:

	col_spec 	* spec_dotdot;
	col_spec 	* spec_key;
	col_spec 	* spec_desc;

	row_inst	* _dotdot;
	str2		* _key;
	str2		* _desc;

	bool get_specs();
 	bool fetch_data( row_inst * row );

	help_ERA( data_set * dset );
};

/*!
*/
class help_VAT : public help_tbl
{
 public:

	col_spec	* spec_key;
	col_spec_percent * spec_rate;
	col_spec 	* spec_desc;

	str2		* _key;
	per_cent_t	  _rate;
	str2		* _desc;

	bool get_specs();
 	bool fetch_data( row_inst * row );

 //	bool create_new_row();
 //	bool write_row_back();
	help_VAT( data_set * dset );
};

/*!
*/
class help_ACCT : public help_tbl
{
 public:

	col_spec	* spec_key;
	col_spec_amnt	* spec_initbal;
	col_spec	* spec_desc;
	col_spec_amnt	* spec_end_bal;

	str2		* _key;
	amnt_t		  _initbal;
	str2		* _desc;

	bool get_specs();
 	bool fetch_data( row_inst * row );

 //	bool create_new_row();
 //	bool write_row_back();
	help_ACCT( data_set * dset );

	bool write_back_end_bal( row_inst * acct, const amnt_t & amnt );

	void copy_all_balances(); // changes aiteration
};

/*!
*/
class help_INFO : public help_tbl
{
 public:

	col_spec 	* spec_key;
	col_spec 	* spec_value; // VALUE but still as string

	str2		* _key;
	str2		* _value;

	bool get_specs();
 	bool fetch_data( row_inst * row );

 //	bool create_new_row();
 //	bool write_row_back();
	help_INFO( data_set * dset );
	bool must_fetch_value( str0 & val, str0 key );
	bool fetch_value_default( str0 & val, str0 key, str0 deflt ) ;
	bool must_fetch_date( date_glib & dt, str0 key );
	bool must_fetch_date_year_start( date_glib & dt );
	bool must_fetch_date_next_year_start( date_glib & dt );
};

}; // NAMESPACE
#endif
