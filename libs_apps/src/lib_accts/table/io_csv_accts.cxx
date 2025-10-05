#include "io_csv_accts.h"
#include "dset.h"                                                          

namespace ACCTS {

/*!
	constructor - simple
*/
io_csv_accts::io_csv_accts( data_set * _dset )
: io_csv_in()
{
	dset = _dset;
}

/*!
	destructor - plain
*/
io_csv_accts::~io_csv_accts()
{
}

/*!
	CSV has found a single line
	This is not the title line, it could be, ask the table itself

	YES this is rediculous, particularly the repeat lookup of tbl.
	THAT is a TODO for the HT code, as we do allow lines to appear
	in ANY order, eg from a JS script source. In this case,
	the CSV is a (compressed, crypted) MUX of table lines.

	Generic CSV does not HAVE to have header lines (chaos tho)
*/
bool io_csv_accts::got_line_1()
{
	if(1)	print_fields();
	if( !nfield ) return FALSE;
	char * tblkey = fields[0];
	table * tbl = NULL;
	tbl = dset->find_add( tblkey );
	tbl->csv_line_in( this );
	return TRUE;
}
}; // NAMESPACE
