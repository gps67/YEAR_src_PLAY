#include "tbl_col_spec_plus.h"
#include "table.h"
#include <typeinfo>

namespace ACCTS {

IS_DIFF_t col_spec::cmp_rr( row_inst * r1, row_inst * r2 )
{
	if( r1 == r2 ) return IS_SAME;
	if( ! r1 ) return IS_LESS;	// nil is less than something
	if( ! r2 ) return IS_MORE;	// nil is less than something

	// now what type is the sub class ??
	
	if(0) e_print( "col_spec::cmp_rr() - SUB CLASS MUST OVERRIDE THIS \n");
	gdb_break_point();

	obj_ref * cell1 = get_obj( r1 );
	obj_ref * cell2 = get_obj( r2 );

	if( cell1 == cell2 ) return IS_SAME;
	if( !cell1 ) return IS_LESS;
	if( !cell2 ) return IS_MORE;

	str2 * s1 = get_str2_for_display( r1 );
	str2 * s2 = get_str2_for_display( r2 );
	if(0) e_print("base_cmp_rr(%s,%s)\n", (STR0)s1->str, (STR0)s2->str );

	return s1->cmp( s2 );
}

IS_DIFF_t col_spec_amnt::cmp_rr( row_inst * r1, row_inst * r2 )
{
	cell_amnt * cell1 = (cell_amnt *) get_obj( r1 );
	cell_amnt * cell2 = (cell_amnt *) get_obj( r2 );

	if(0) e_print("amnt_cmp_pp(%p,%p)\n", cell1, cell2 );
	if( cell1 == cell2 ) return IS_SAME;
	if( !cell1 ) return IS_LESS;
	if( !cell2 ) return IS_MORE;
	if(0) e_print("amnt_cmp_rr(%s,%s)\n", (STR0)cell1->get_str2()->str, (STR0)cell2->get_str2()->str );

	return cell1->cmp( cell2 );
}

IS_DIFF_t col_spec_percent::cmp_rr( row_inst * r1, row_inst * r2 )
{
	cell_percent * cell1 = (cell_percent *) get_obj( r1 );
	cell_percent * cell2 = (cell_percent *) get_obj( r2 );

	if( cell1 == cell2 ) return IS_SAME;
	if( !cell1 ) return IS_LESS;
	if( !cell2 ) return IS_MORE;
	if(0) e_print("percent(%s,%s)\n", (STR0)cell1->get_str2()->str, (STR0)cell2->get_str2()->str );

	return cell1->cmp( cell2 );
}

IS_DIFF_t col_spec_date::cmp_rr( row_inst * r1, row_inst * r2 )
{
	cell_date * cell1 = (cell_date *) get_obj( r1 );
	cell_date * cell2 = (cell_date *) get_obj( r2 );

	if( cell1 == cell2 ) return IS_SAME;
	if( !cell1 ) return IS_LESS;
	if( !cell2 ) return IS_MORE;

	if(0) e_print("date_cmp_rr(%s,%s)\n", (STR0)*cell1->get_str2(), (STR0)*cell2->get_str2() );

	return cell1->cmp( cell2 );
}

IS_DIFF_t col_spec_str2::cmp_rr( row_inst * r1, row_inst * r2 )
{
	str2 * cell1 = (str2 *) get_obj( r1 );
	str2 * cell2 = (str2 *) get_obj( r2 );

	if( cell1 == cell2 ) return IS_SAME;
	if( !cell1 ) return IS_LESS;
	if( !cell2 ) return IS_MORE;

	if(0) e_print("str2_cmp_rr(%s,%s)\n", (STR0)*cell1, (STR0)*cell2 );

	return cell1->cmp( cell2 );
}
IS_DIFF_t col_spec_str2_key::cmp_rr( row_inst * r1, row_inst * r2 )
{
	str2 * cell1 = (str2 *) get_obj( r1 );
	str2 * cell2 = (str2 *) get_obj( r2 );
	if(0) e_print("str2_key_cmp_rr(%s,%s)\n", (STR0)*cell1, (STR0)*cell2 );

	if( cell1 == cell2 ) return IS_SAME;
	if( !cell1 ) return IS_LESS;
	if( !cell2 ) return IS_MORE;

	return cell1->cmp( cell2 );
}
// links are so imilar - made a common class
IS_DIFF_t col_spec_link::cmp_rr( row_inst * r1, row_inst * r2 )
{
	row_inst * cell1 = (row_inst *) get_obj( r1 );
	row_inst * cell2 = (row_inst *) get_obj( r2 );

	if( cell1 == cell2 ) return IS_SAME;
	if( !cell1 ) return IS_LESS;
	if( !cell2 ) return IS_MORE;

	table * tbl1 = get_linked_table();
	col_spec * spec2 = tbl1->group_spec; // check NULL

	if( spec2 ) {
		if( (cell1 == r1) && (cell2==r2) ) {
			if(0) e_print("recurion in cmp_rr\n" );
			gdb_break_point();
			return IS_SAME;
		}
		if(0) e_print("pivoting in %s '%s' -> cmp_rr(%s,%s)\n",
			typeid(*spec2).name(),
		(STR0)	spec2->get_name_str(),
		(STR0)	cell1->debug_row_name(),
		(STR0)	cell2->debug_row_name()
		 );
		if( tbl1 ) {
			str2 * k1 = tbl1->get_key_of_row( cell1 );
			str2 * k2 = tbl1->get_key_of_row( cell2 );
			if( k1 == k2 ) return IS_SAME;
			if( !k1 ) 
				return IS_LESS;
			if( !k2 ) 
				return IS_MORE;
			if(0) e_print("pivot comparing on %s %s\n",
			(STR0)	*k1,
			(STR0)	*k2
			);
			return k1->cmp( k2 );
		}
		// following presumes balenced leaves !
		return spec2->cmp_rr( cell1, cell2 );
	}
	if( tbl1 ) {

		str2 * k1 = tbl1->get_key_of_row( r1 );
		str2 * k2 = tbl1->get_key_of_row( r2 );
	if(0) e_print("link_cmp_rr(%s,%s)\n", (STR0)*k1, (STR0)*k2 );

		if( !k1 ) {
			return IS_SAME;
		}
		return k1->cmp( k2 );
	}
	if(1) e_print("link NO TBL is same(%s,%s)\n",
	(STR0)	r1->debug_row_name(),
	(STR0)	r2->debug_row_name()
	);
	gdb_break_point();
	return IS_SAME;
}
}; // NAMESPACE
