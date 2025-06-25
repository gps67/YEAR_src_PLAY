
#include "mm3_file_data.h"

//
// Copy constructor - should be non pointer ?
void a_trans::set_from( a_trans & t1 ) {
	reset();
	trans_id = t1.trans_id;
	year = t1.year;
	mm = t1.mm;
	dd = t1.dd;
	year2 = t1.year2;
	mm2 = t1.mm2;
	dd2 = t1.dd2;
	cat = t1.cat;
	acct = t1.acct;
	acct2 = t1.acct2;
	mark0 = t1.mark0;
	mark = t1.mark;
	ref = t1.ref;
	desc = t1.desc;
	amnt = t1.amnt;
	vat2 = t1.vat2;
}
//
void a_trans::reset(void) {
	trans_id = 0;
	year = 0;
	mm = 0;
	dd = 0;
	year2 = 0;
	mm2 = 0;
	dd2 = 0;
	cat.clear();
	acct = ' ';
	acct2 = '-'; // special nul value for unused
	mark0 = ' ';
	mark = ' ';
	ref.clear();
	desc.clear();
	amnt.clear();
	vat2.clear();
}

cat_list::cat_list( void ) {
	n_cat = 0;
	const char * DNIXOT = "DNIXOT";
	if(0)
	{
		DNIXOT = "DNIXOT";
		DNIXOT = "/";
		list[ n_cat++] = new a_cat( DNIXOT, "-",  "Categories" );
	} else
	{
		DNIXOT = "-";
	}
	list[ n_cat++] = new a_cat( "D", DNIXOT,  "Drawings" );
	list[ n_cat++] = new a_cat( "T", DNIXOT,  "Tax" );
	list[ n_cat++] = new a_cat( "O", DNIXOT,  "Outgoings" );
	list[ n_cat++] = new a_cat( "X", DNIXOT,  "XFER" );
	list[ n_cat++] = new a_cat( "I", DNIXOT,  "Income" );
	list[ n_cat++] = new a_cat( "N", DNIXOT,  "Notes" );
}
bool cat_list::add_cat( own_str & abb, own_str & d ) {
	if( n_cat >= MAX_CAT ) return FALSE;
	// fixup path
	char path[13] = { 0, 0, 0 };
	char * abb_str = abb.str;
	char NIXOT = 'O';
	// disallow n3 other than N3 for NIXOT main groups
	if( abb_str[1] ) {
		sprintf( path, "%c", abb_str[0] );
	} else {
		path[0] = 0;
		char c0 = abb_str[0];
		switch( c0 ) {
		 case 'D':
		 case 'N':
		 case 'I':
		 case 'X':
		 case 'O':
		 case 'T':
			return true; // already added
			path[0] = '-';
			sprintf( path, "DNIXOT" );
		break;
		 default:
			path[0] = 'O'; // every other single letter is OUTGOINGS
		}
	}

	list[ n_cat++] = new a_cat( abb, &path[0],  d );
	return TRUE; // presume allocs worked
}
bool cat_list::lookup_key( own_str & k, int & pos ) {
	for( int i = 0; i< n_cat; i++ ) {
		if(0==strcmp(k.str, list[i]->key.str)) {
			pos = i;
			return TRUE;
		}
	}
	return FALSE;
}
const char * cat_list::key_to_desc( own_str & k ) {
	int pos;
	if(lookup_key( k, pos )) 
		return list[pos]->desc.str;
	else
		return "NOT FOUND";
}

void  mm3_file_data::fixup_merge_xfer( a_trans & xfer )
{
	// try to rejoin an xfer with the tax_date transaction
	for( int i=0; i<trans.N; i++ )
	{
		a_trans & t = * trans.list[i];
		if(
			( xfer.acct == t.acct )
		 &&	( xfer.amnt == t.amnt )
		 &&	( xfer.ref  == t.ref  )
		)
		{
			fprintf(stderr, "MERGE acct %c/%c ref %s amnt %s date %d-%2.2d-%2.2d\n",
				xfer.acct,
				xfer.acct2,
				xfer.ref.str,
				xfer.amnt.str,
				xfer.year,
				xfer.mm,
				xfer.dd2
			);
			t.year2 = xfer.year;
			t.mm2   = xfer.mm;
			t.dd2   = xfer.dd2;
			t.acct  = xfer.acct2;
			return;
		} else if(
		(
			( xfer.acct2 == t.acct )
		 &&	( xfer.amnt.is_negative( t.amnt ))
		 &&	( xfer.ref  == t.ref  )
		)) {
			fprintf(stderr, "MERGE2 acct %c\\%c ref %s amnt- %s date %d-%2.2d-%2.2d\n",
				xfer.acct2,
				xfer.acct,
				xfer.ref.str,
				t.amnt.str,
				xfer.year,
				xfer.mm,
				xfer.dd2
			);
			t.year2 = xfer.year;
			t.mm2   = xfer.mm;
			t.dd2   = xfer.dd2;
			t.acct  = xfer.acct;
			return;
		} else if(
		(
		 (	( xfer.acct2 == t.acct )
		 ||	( xfer.acct == t.acct ) )

		 && 	( xfer.ref  == t.ref  )

		 && (	( xfer.amnt.is_negative( t.amnt ))
		 ||	( xfer.amnt == t.amnt ) )

		)) {
			fprintf(stderr, "MIS-MATCH MERGE3 acct %c\\%c ref %s amnt- %s date %d-%2.2d-%2.2d\n",
				xfer.acct,
				xfer.acct2,
				xfer.ref.str,
				t.amnt.str,
				xfer.year,
				xfer.mm,
				xfer.dd2
			);
			// t.year2 = xfer.year;
			// t.mm2   = xfer.mm;
			// t.dd2   = xfer.dd2;
			// t.acct  = xfer.acct;
			// return;
		}

	}
	xfers.append( & xfer );
}
