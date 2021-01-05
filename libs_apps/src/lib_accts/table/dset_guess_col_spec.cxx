#include "tbl_col_spec_plus.h"
#include "dset.h"

namespace ACCTS {

/*!
	This is to be SCRIPTED - it guesses the colun types from the name
*/
col_spec * data_set::guess_col_spec( table * tbl, str2 * colname )
{
// if(no_guessing_as_it_prevents_load) return new col_str1( colname );
//
// this is where a sripting environment rules -

	data_set * dset = tbl->dset;
	col_spec * ret_spec = NULL;

	if(0) INFO("table.colname %s.%s", (STR0) * tbl->name, (STR0) *colname );

	/*
		this doesnt cater for key fields which
		are date or other, presume string keys
	*/
	if( colname->is_same_as( "key" ) ) {
		col_spec * spec =  new col_spec_str2_key( colname );
		tbl->key_spec = spec;
		return spec;
	}

	/*
		this doesnt cater for dotdot fields which are date
		or other, presume string keys

		NB This doesnt always work as expected:
		eg ACCTS{ key, dotdot, desc, amnt_start, amnt_end }
		the interior nodes becomes LEAVES, with zero amnt_start
		which are then added to their own tree's !!
	*/
	if( colname->is_same_as( "dotdot" ) ) {
		col_spec * spec =  new col_spec_link_dot_dot( colname );
		tbl->group_spec = spec;
		return spec;
	}

	if( tbl->name->is_same_as( "XFER" ) )
	{
		if( colname->is_same_as("cat" ) ) {
			table * tbl2 = dset->find_add( "CAT" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
		if( colname->is_same_as("acct" ) 
		 || colname->is_same_as("acct1" )
		 || colname->is_same_as("acct2" )
		)
		{
			table * tbl2 = dset->find_add( "ACCT" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
		if( colname->is_same_as("ref" ) ) {
			table * tbl2 = dset->find_add( "REF" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
	}

	if( tbl->name->is_same_as( "NWATRANS" ) )
	{
		// added for NWA_Parse
		if( colname->is_same_as("refk2" ) ) {
			table * tbl2 = dset->find_add( "REF" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
	}

	if( tbl->name->is_same_as( "INFO" ) ) {
		if( colname->is_same_as("key" ) ) {
		}
		if( colname->is_same_as("value" ) ) {
		}
	}

	if( tbl->name->is_same_as( "TRANS" ) )
	{
		if( colname->is_same_as("cat" ) ) {
			table * tbl2 = dset->find_add( "CAT" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
		if( colname->is_same_as("cat2" ) ) {
			WARN("DO WHAT WITH TRANS.cat2 ??? mostly unused");
		}
		if( colname->is_same_as("vat_era" ) ) {
#warning "This crashed somewhere else when VAT_ERA table didnt exist (ERA does)"
// could maybe be ... list of fields for table (none other than key?)
			// table * tbl2 = dset->find_add( "VAT_ERA" );
			table * tbl2 = dset->find_add( "ERA" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
		if( colname->is_same_as("vat" ) ) {
			table * tbl2 = dset->find_add( "VAT" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
		if( colname->is_same_as("acct" ) 
		 || colname->is_same_as("acct1" )
		 || colname->is_same_as("acct2" )
		)
		{
			table * tbl2 = dset->find_add( "ACCT" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
		/* better as plain text desc
		if( colname->is_same_as("ref" ) ) {
			table * tbl2 = dset->find_add( "REF" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
		*/
	}

	if( tbl->name->is_same_as( "ACCT" ) )
	{
		if( colname->is_same_as( "group" ) ) {
			PASS(" LINKING ACCT: group/path");
			table * tbl2 = dset->find_add( "ACCT_group" );
			return new col_spec_row_link_key( colname, tbl2 );
		}
	}

	if( tbl->name->is_same_as( "CAT" ) )
	{
		if( colname->is_same_as("path" ) ) {
			PASS("LINKING CAT: path");
			col_spec * spec =  new col_spec_link_dot_dot( colname );
			tbl->group_spec = spec;
			return spec;
		}
	}

	if( tbl->name->is_same_as( "VAT" ) )
	{
		if(
			colname->is_same_as("rate")
		 ||	colname->is_same_as("amount")
		) {
			ret_spec =  new col_spec_percent( colname );
			return ret_spec;
		}
	}


	if(
		colname->is_same_as("amount")
// OK	 ||	colname->is_same_as("amnt") 
// OK	 ||	colname->is_same_as("amnt_closing") 
// OK	 ||	colname->is_same_as("amnt_pre_vat") 
// OK	 ||	colname->is_same_as("amnt_vat") 
	 ||	colname->is_same_as("excluded_vat_report") 
	 ||	colname->is_same_as("credit") 
	 ||	colname->is_same_as("debit") 
	 ||	colname->is_same_as("balance") 
	 ||	colname->is_same_as("turnover") 
	 ||	colname->is_same_as("tax_income") 
	 ||	colname->is_same_as("tax_paid") 
	 ||	colname->is_same_as("ni_er") 
	 ||	colname->is_same_as("ni_ee") 
	 ||	colname->is_same_as("ni_tot") 
	 ||	colname->is_same_as("cred") 
	) {
		return new col_spec_amnt( colname );
	}

	if( colname->is_same_as("img") )
	{
		return new col_spec_img_id( colname );
	}

	if( tbl->name->has_lex_substr( "ERA", "ERA_", "_ERA_", "_ERA") )
	{
		if( colname->is_same_as("path" ) ) {
			PASS("LINKING ERA: path");
			col_spec * spec =  new col_spec_link_dot_dot( colname );
			tbl->group_spec = spec;
			return spec;
		}
	}

	if( tbl->name->is_same_as( "WEEK" ) 
	 || tbl->name->is_same_as( "MONTH" ) ) // P11 wage-slips
	{
		const char * names[] = {
	//		"week",
// DATE			"date_end",
	//		"WHO",
			"wk_tax_cred_days",
			"wk_tax_cred",
	//		"TaxCode",
			"wk_gross",
			"wk_ni_er",
			"wk_ni_ee",
			"wk_tax",
			"wk_nett",
		};
		int n = sizeof( names) / sizeof( names[0] );
		for( int i=0; i<n; i++ ) {
			if( colname->is_same_as( names[i] ) ) {
				return new col_spec_amnt( colname );
			}
		}
	}

	if( colname->has_lex_substr( "amnt", "amnt_", "_amnt_", "_amnt" ))
	{
		return new col_spec_amnt( colname );
	}

	if( colname->has_lex_substr( "gross", "gross_", "_gross_", "_gross"))
	{
		return new col_spec_amnt( colname );
	}

#define NOT_new_str2_static(S) new str2(S)

	if( colname->has_lex_substr( "date", "date_", "_date_", "_date" ))
	{
		table * era_tbl = NULL;
		static obj_hold<str2> s_ERA  = NOT_new_str2_static( "ERA" );
		static obj_hold<str2> s_ERA2 = NOT_new_str2_static( "HIDDEN_ERA" );
		static obj_hold<str2> s_KEY  = NOT_new_str2_static( "era" );
		static obj_hold<str2> s_GRP  = NOT_new_str2_static( "path" );
		static obj_hold<str2> s_DESC = NOT_new_str2_static( "DESC" );
		era_tbl = dset->get_table( s_ERA );
		if(!era_tbl) {
			era_tbl = dset->get_table( s_ERA2 );
		}
		if(!era_tbl) {
			col_spec * spec1 = new col_spec_str2( s_KEY );
			col_spec * spec2 = new col_spec_str2( s_GRP );
			col_spec * spec3 = new col_spec_str2( s_DESC );
			col_spec * spec =  new col_spec_link_dot_dot( s_GRP );
			era_tbl = dset->find_add_table( s_ERA2 );
			// era_tbl = dset->find_add_fake_table( s_ERA2 );
			era_tbl->add_col( spec1 );
			era_tbl->add_col( spec2 );
			era_tbl->add_col( spec3 );
			era_tbl->desc_spec = spec3;
		}
		return new col_spec_date( colname, era_tbl );
	}

	/*
		Y2003_04 is the column name for that year
		every value is an amnt. 

		This is a kludge for not having a second field type row in csv
	*/
	if( colname->has_prefix( "Y202" ) )
	{
		return new col_spec_amnt( colname );
	}
	if( colname->has_prefix( "Y201" ) )
	{
		return new col_spec_amnt( colname );
	}
	if( colname->has_prefix( "Y200" ) )
	{
		return new col_spec_amnt( colname );
	}
	if( colname->has_prefix( "Y199" ) )
	{
		return new col_spec_amnt( colname );
	}

	if( !tbl->desc_spec )
	{
		if(
			colname->is_same_as("desc") 
		 ||	colname->is_same_as("description")
		) {
			ret_spec = new col_spec_str2( colname );
			tbl->desc_spec = ret_spec;
			return ret_spec;
		}
	}

	WARN("UNKNOWN table.colname %s.%s", (STR0) * tbl->name, (STR0) *colname );
	ret_spec = new col_spec_str2( colname );
	return ret_spec;
}
}; // NAMESPACE
