
#include "mm3_file_data.h"
#include "mm3_file_tbl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/*
	does everything
	all the TCL output format is here, and scan_transaction
	doesn't do repeating transactions
	skips a few fields that I don't use (macro's and reconciliations)
*/

#define NB( note )

void decl_tbl(
	const char * name,
	const char * key1,
	const char * fields
)
{
	printf("%s, %s\n", name, fields );
}

char * mm3_file_tbl::mk_img( int year, int mm, int dd, char * cat )
{
	struct stat st;
	char * absent = "-";
	//////// image file name
	char fname[90];
	char fnameb[90];
	static char iname[30];
	const char * fmt = "/tmp/%4.4d-%4.4d/img/%2.2d%2.2d%s%s.png";
	sprintf( fname, fmt,
		year_start,
		year_start+1,
		mm,
		dd,
		cat,
		""
	);
	sprintf( fnameb, fmt,
		year_start,
		year_start+1,
		mm,
		dd,
		cat,
		"b"
	);
	if( 0!=stat( fname, &st ) )
	{
		if(0) fprintf( stderr, "Absent %s\n", fname );
		return absent;
	}
	if( 0==stat( fnameb, &st ) )
	{
		fprintf( stderr, "TOO MANY %s ***********\n", fname );
		// fileb is PRESENT so conflict
		return absent;
	}
	sprintf( iname, "%2.2d%2.2d%s", mm, dd, cat );
	if(0) fprintf( stderr, "FOUND %s ", iname );
	return iname;
}

void mm3_file_tbl::print_transaction_tbl( a_trans & t )
{
	char buf[90];

	print_tbl_name( SPL_trans );

	sprintf( buf, "0x%3.3X", t.trans_id);
	sprintf( buf, "+"); // use AUTO
	print_tbl_field( buf );

	sprintf( buf, "%4.4d-%2.2d-%2.2d", t.year,t.mm,t.dd );
	print_tbl_field( buf );

	sprintf( buf, "%4.4d-%2.2d-%2.2d", t.year2,t.mm2,t.dd2 );
	print_tbl_field( buf );

	print_tbl_field( t.cat, 2 );

	sprintf( buf, "%c", t.acct );
	print_tbl_field( buf );

	// add scan image name column
	sprintf( buf, "%s", mk_img( t.year, t.mm, t.dd, t.cat.str ));
	print_tbl_field( buf );

#ifdef WITH_MARKS
	sprintf( buf, "%c", t.mark0 );
	print_tbl_field( buf );
	sprintf( buf, "%c", t.mark );
	print_tbl_field( buf );
#endif
	print_tbl_field( t.ref, 6 );
	print_tbl_field( t.desc, -19 );
/*
	print_tbl_field( t.amnt, 8 );
	print_tbl_field( t.vat2, 1 );
*/
/*
	print_tbl_field(6,	t.ref);
	printf(",  ");
	print_tbl_field(-19,t.desc);
*/
	print_tbl_field( t.amnt, 9 );
	print_tbl_field( t.vat2);
	print_tbl_eoln();
}

void mm3_file_tbl::print_transfer_tbl( a_trans & t )
{
	char buf[90];

	print_tbl_name( SPL_xfer );

	if(1)
		sprintf( buf, "+" );
	else
		sprintf( buf, "0x%3.3X", t.trans_id);
	print_tbl_field( buf );

	sprintf( buf, "%4.4d-%2.2d-%2.2d", t.year,t.mm,t.dd );
	print_tbl_field( buf );

/*
	// same again
	print_tbl_field( buf );
*/

	print_tbl_field( t.cat, 2 );

	sprintf( buf, "%c", t.acct );
	print_tbl_field( buf );

	sprintf( buf, "%c", t.acct2 );
	print_tbl_field( buf );

	// add scan image name column
	sprintf( buf, "%s", "-" );
	print_tbl_field( buf );

#ifdef WITH_MARKS
	sprintf( buf, "%c", t.mark0 );
	print_tbl_field( buf );
	sprintf( buf, "%c", t.mark );
	print_tbl_field( buf );
#endif
	print_tbl_field( t.ref, 6 );
	print_tbl_field( t.desc, -19 );
/*
	print_tbl_field( t.amnt, 8 );
	print_tbl_field( t.vat2, 1 );
*/
/*
	print_tbl_field(6,	t.ref);
	printf(",  ");
	print_tbl_field(-19,t.desc);
*/
	print_tbl_field( t.amnt, 9 );
	print_tbl_eoln();
}

void mm3_file_tbl::print_tbl()
{

	infos.add_info( "title", title );

	decl_tbl( SPL_info, "key, value" );
	for ( int i = 0; i<infos.n_info; i++)
	{
		a_info * item = infos.list[i];
		print_tbl_name( SPL_info );
		print_tbl_field( item->key, 9 );
		print_tbl_field( item->desc );
		print_tbl_eoln();
	}
	print_tbl_blank_line();

	decl_tbl( "ERA", "dotdot, key, desc" );
	for ( int i = 0; i<eras.n_era; i++)
	{
		a_era * item = eras.list[i];
		print_tbl_name( "ERA" );
		print_tbl_field( item->path, 8 );
		print_tbl_field( item->key, 10 );
		print_tbl_field( item->desc );
		print_tbl_eoln();
	}
	print_tbl_blank_line();

	decl_tbl( "REF", "dotdot, key, desc" );
	// could do this
	print_tbl_blank_line();

	decl_tbl( SPL_cat, "dotdot, key, desc" );
	for ( int i = 0; i<cats.n_cat; i++)
	{
		a_cat * item = cats.list[i];
		print_tbl_name( SPL_cat );
		print_tbl_field( item->path, 2 );
		print_tbl_field( item->key, 2 );
		print_tbl_field( item->desc );
		print_tbl_eoln();
	}
	print_tbl_blank_line();

	decl_tbl( SPL_acct, "key, amount, desc" );
	for ( int i = 0; i<accts.n_acct; i++)
	{
		a_acct * item = accts.list[i];
		print_tbl_name( SPL_acct );
		print_tbl_field( item->key );
		print_tbl_field( item->amnt, -7 );
		print_tbl_field( item->desc );
		print_tbl_eoln();
	}
	print_tbl_blank_line();

	decl_tbl( SPL_note, "key, desc" );
	for ( int i = 0; i<notes.n_note; i++)
	{
		a_note * item = notes.list[i];
		print_tbl_name( SPL_note );
		print_tbl_field( item->key, 2 );
		print_tbl_field( item->desc );
		print_tbl_eoln();
	}
	print_tbl_blank_line();


	decl_tbl( SPL_vat, "key, rate, desc" );
	for ( int i = 0; i<vats.n_vat; i++)
	{
		a_vat * item = vats.list[i];
		print_tbl_name( SPL_vat );
		print_tbl_field( item->key );
		print_tbl_field( item->amnt, 5 );
		print_tbl_field( item->desc );
		print_tbl_eoln();
	}
	print_tbl_blank_line();

	decl_tbl( SPL_trans, "key, date_tax, date_pay, cat, acct, img, " 
		HDR_MARKS "ref, desc, amnt, vat" );
	for ( int i = 0; i<trans.N; i++)
	{
		a_trans * item = trans.list[i];
		print_transaction_tbl( *item );
	}
	print_tbl_blank_line();

	decl_tbl( SPL_xfer, "key, date, cat, acct1, acct2, img, ref, desc, amnt" );
	for ( int i = 0; i<xfers.N; i++)
	{
		a_trans * item = xfers.list[i];
		print_transfer_tbl( *item );
	}
	print_tbl_blank_line();
}
