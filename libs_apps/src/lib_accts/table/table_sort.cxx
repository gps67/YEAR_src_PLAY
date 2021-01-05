
#include "is_diff.h"
#include "tbl_col_spec.h"
#include "obj_list.h"
#include "tbl_row_inst.h"
#include "table.h"
#include <typeinfo>

#include "sort_spec_base.h"

#define VERBOSE if(0)

//! The number of col_specs that can be sorted by (other edits ARE required)
#define N4 4

namespace ACCTS {

//! how to sort an obj_list<row_inst> by upto N4 col_specs
/*!
	A sort_spec_one is a sort_spec with N4 col_specs (to sort by)

	obj_list_base::sort() calls: obj_list_sorter::sort()
	which repeatedly calls:  this->do_cmp( row1, row2 )
	which calls: spec[i]->cmp_rr( row1, row2 );

	Actually it provides (obj_ref *) not (row_inst *),
	and the cast is done by lying about the MFN_do_cmp
	signature of the member function (pointer to).

*/
class sort_spec_one : public sort_spec_base
{
 public:

	/*!
		provide the pointer to the member function, which
		PRESUMES all objects in the list of pointers are
		row_inst.

		This is only called once per sort, the function it
		returns is called hundreds of times.
	*/
  virtual
	MFN_do_cmp get_do_cmp()
	{
		return (MFN_do_cmp) &sort_spec_one::do_cmp;
	}

	/*!
		We are sorting a list of rows by member fields.
		You can have N4 field selectors (all ascending).
	*/
	obj_hold<col_spec> spec[N4];

	/*!
		A created sort_spec can remain and be reused,
		possibly even edited. You dont have to provide
		all of the field selectors, but dont leave
		a NULL gap.
	*/
	sort_spec_one(
		col_spec * _spec1,
		col_spec * _spec2 = NULL,
		col_spec * _spec3 = NULL,
		col_spec * _spec4 = NULL
	)
	{
		spec[0] = _spec1;
		spec[1] = _spec2;
		spec[2] = _spec3;
		spec[3] = _spec4;
	}

	/*!
		When there is only one sort_column, there is a risk
		of several rows being equal. Its nice to add one more
		which is the original input sequence or primary key
		(which is supposed to be unique). If all N4 selectors
		are used (and not repeats), its less likely that
		you will notice any equal values.

		do_cmp NOW adds an extra row_sort comparison
		but its possible that there is a conflict (surrounding
		code didnt set row_sort on load, or -1 or something)
	*/
	void add_default( col_spec * _spec1 )
	{
		if( ! _spec1 ) return;
		for( int i = 0; i<N4; i++ )
		{
			if( spec[i] == _spec1 ) return;
			if( !spec[i] ) {
				spec[i] = _spec1;
				return;
			}
		}
		if(1) e_print("sort_spec_one::add_default() - no room\n");
	}

	/*!
		This function is called hundreds of times,
		and it calls the virtual col_spec->cmp_rr(r1,r2)
		which does the actual work.
	*/
	IS_DIFF_t do_cmp( row_inst * row1, row_inst * row2 )
	{
		IS_DIFF_t diff = IS_SAME;
		// pointer comparison - not C++ tricks (HOW_TO_BE_SURE: dunno)
		if( row1 == row2 ) return IS_SAME;
		if( !row1 ) return IS_LESS;
		if( !row2 ) return IS_MORE;
		// neither row1 nor row2 is NULL
		for( int i = 0; i<N4; i++ )
		{
			if( !spec[i] ) break;
			diff = spec[i]->cmp_rr( row1, row2 );
			if( diff != IS_SAME ) return diff;
		}
		// still here? use the row_sort fallback

		if( row1->row_sort < row2->row_sort) return IS_LESS;
		if( row1->row_sort > row2->row_sort) return IS_MORE;

		// still here - really odd!
		e_print( "# WARN # do_cmp(row1,row2) cannot distinguish them!\n" );

		return IS_SAME;
	}
};

void table::sort_by_col_spec(
	col_spec * spec0,
	col_spec * spec1,
	col_spec * spec2,
	col_spec * spec3
)
{
	if(0) gdb_break_point();
        if(0) e_print("sort_by_col_spec %s - %s\n",
		spec0->name->sget(),
		typeid(*spec0).name()
	 );
	
	obj_hold<sort_spec_one>	sort_spec; sort_spec = new sort_spec_one(
		spec0, spec1, spec2, spec3
	);
 if(0)
 {
	/*
		Add the extra sort field (if theres room).
		key_spec is a reasonable default, and unique.
		Most tables set their own date fields (TAX or PAY)
	*/
	sort_spec->add_default( col_spec_sort_1 );
	sort_spec->add_default( col_spec_sort_2 );
	sort_spec->add_default( key_spec );
 }
	/*
		sort the rows of the table, using the prepared sort_spec
	*/
	rows_list.sort( sort_spec );

	// sort_spec gets auto deleted (obj_hold)
}

}; // NAMESPACE
