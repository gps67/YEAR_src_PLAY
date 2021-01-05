#include "run_report.h"

// class data_set;
#include "str0.h"
#include "e_print.h"

/*!
	When PDFLIB is not available at compile time,
	this file provides stub functions

	Except these - which are plain table reports
	bool run_report_1( data_set * dset )
	bool run_report_2( data_set * dset )
	bool run_report_3( data_set * dset )
	bool run_report_4( data_set * dset )
*/


bool run_pdf_VatTrack( data_set * dset, str0 filenamepdf )
{
	e_print("NO PDF run_pdf_VatTrack( dset, %s )\n", (STR0)filenamepdf );
	return false;
}

bool run_pdf_Cash_Cat( data_set * dset, str0 filenamepdf )
{
	e_print("NO PDF run_pdf_Cash_Cat( dset, %s )\n", (STR0)filenamepdf );
	return false;
}


bool run_report_2_pdf( data_set * dset, str0 filenamepdf )
{
	e_print("NO PDF run_report_2_pdf( dset, %s )\n", (STR0)filenamepdf );
	return false;
}

bool run_report_3_pdf( data_set * dset, str0 filenamepdf )
{
	e_print("NO PDF run_report_3_pdf( dset, %s )\n", (STR0)filenamepdf );
	return false;
}

/////////////
// I just added these to fill the linkage void ///


bool run_pdf_CH(ACCTS::data_set*, str0, ACCTS::data_set*, ACCTS::data_set*)
{
	e_print("NO PDF \n" );
	return false;
}

namespace ACCTS {
 class table;
};
// class ACCTS::table;

bool run_pdf_Table(ACCTS::table*, str0)
{
	e_print("NO PDF \n" );
	return false;
}


