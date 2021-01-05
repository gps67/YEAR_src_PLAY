#include "rep_pdf_Reports.h"
// NOT // #include "rep_pdf_CH.h"

// CALLER HAS ALREADY DONE THIS
//	if(!run_report_1( dset )) return 1;
//	if(!run_report_2( dset )) return 1;
//	if(!run_report_3( dset )) return 1;
//	if(!run_report_4( dset )) return 1;
//
// ie notice the difference between _pdf (here) and internal prep reports (above)

using namespace PDF_GEN;
using namespace ACCTS;


/*
	report1 was Categs, but I've just hijacked it to be VatRep
*/
bool run_pdf_VatTrack( data_set * dset, str0 filenamepdf )
{
	obj_hold<pdf_ctxt> PDF_CTXT = new pdf_ctxt();
//	pdf_ctxt PDF_CTXT;
	obj_hold< rep_pdf_VatTrack > mydoc =
		 new rep_pdf_VatTrack( *PDF_CTXT, dset, filenamepdf );
	if( !PDF_CTXT->run_wrapped_report( mydoc ))
	{
		printf("# APP: exiting after PDFlib exception!\n");
		return false;
	}
	return true;
}

bool run_pdf_Cash_Cat( data_set * dset, str0 filenamepdf )
{
	
	obj_hold<pdf_ctxt> P_PDF_CTXT = new pdf_ctxt();
	pdf_ctxt & PDF_CTXT = * P_PDF_CTXT;
//	obj_hold< rep_pdf_Cash_Cat > mydoc =
	obj_hold< rep_pdf_0 > mydoc = // BASE CLASS USED !!
		 new rep_pdf_Cash_Cat( PDF_CTXT, dset, filenamepdf );
	if( !PDF_CTXT.run_wrapped_report( mydoc ))
	{
		printf("# APP: exiting after PDFlib exception!\n");
		return false;
	}
	return true;
}


bool run_pdf_CH(
	data_set * dset,
	str0 filenamepdf,
	data_set * _dset_curr,
	data_set * _dset_prev
)
{
	obj_hold<pdf_ctxt> P_PDF_CTXT = new pdf_ctxt();
	pdf_ctxt & PDF_CTXT = * P_PDF_CTXT;
	obj_hold< rep_pdf_0 > mydoc = // BASE CLASS USED !!
		 new_rep_pdf_CH( PDF_CTXT, dset, filenamepdf, _dset_curr, _dset_prev );
	if( !PDF_CTXT.run_wrapped_report( mydoc ))
	{
		printf("# APP: exiting after PDFlib exception!\n");
		return false;
	}
	return true;
}

bool run_pdf_Table( table * tbl, str0 filenamepdf )
{
	data_set * dset = tbl->dset;

	obj_hold<pdf_ctxt> P_PDF_CTXT = new pdf_ctxt();
	pdf_ctxt & PDF_CTXT = * P_PDF_CTXT;
	obj_hold< rep_pdf_table > mydoc =
		 new rep_pdf_table( PDF_CTXT, dset, tbl, filenamepdf );
	if( !PDF_CTXT.run_wrapped_report( mydoc ))
	{
		printf("# APP: exiting after PDFlib exception!\n");
		return false;
	}
	return true;
}
/////////////////////////
