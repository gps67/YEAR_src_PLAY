#ifndef report1_pdf_H
#define report1_pdf_H


#include "rep_pdf_0.h"
#include "rpt_names.h"

using namespace PDF_GEN;
using namespace ACCTS;

/*!
	Vat Track: no_into, VatSum
*/
class rep_pdf_VatTrack : public rep_pdf_0
{
 public:
	rep_pdf_VatTrack(
		pdf_ctxt & _pdf,	// PDF_GEN_CTXT_SESS
		data_set * _dset,	// dset CSV_ACCTS //
		str0 filenamepdf	// ACCTS_REPORT_NAME.pdf // VAT_TRACK
	)
	: rep_pdf_0(
		_pdf,		// GEN_PDF_mid_SESS
		_dset,		// CSV_ACCTS_DATA_as_DSET
		filenamepdf	// VFS_FILENAME.pdf // as STR0 // AKEY //
	)
	{
	}
	virtual bool run_wrapped_report();
};


/*!
	Something New - PL2 - base?
*/
class rep2_pdf : public rep_pdf_0
{
 public:
	rep2_pdf( pdf_ctxt & _pdf, data_set * _dset, str0 filenamepdf )
	: rep_pdf_0(
		_pdf,
		_dset,
		filenamepdf
	)
	{
	}
	virtual bool run_wrapped_report();
};


/*!
	Primary: intro, CashBook, Categorised
*/
class rep_pdf_Cash_Cat : public rep_pdf_0
{
 public:
	rep_pdf_Cash_Cat( pdf_ctxt & _pdf, data_set * _dset, str0 filenamepdf )
	: rep_pdf_0(
		_pdf,
		_dset,
		filenamepdf
	)
	{
	}
	bool intro();
	virtual bool run_wrapped_report();
};

/*!
	a single table as a file

	Title is tablename
	Its always portrait
	Sorted by keyfield (why not as found?)
	Intended for simple sheets in a hurry
*/
class rep_pdf_table : public rep_pdf_0
{
	
 public:
	obj_hold<str2> tablename;
	obj_hold<str2> title;
	obj_hold<str2> keyfield;
	obj_hold<str2> totcol_name;
	bool full_key_path;
	int interior_depth;

	rep_pdf_table(
		pdf_ctxt & _pdf,
		data_set * _dset,
		table * _tbl,
		str0 _filenamepdf
	);

	virtual bool run_wrapped_report();
};

/*!
	factory to create a report
	without knowing the TYPE rep_pdf_CH

	implemented in rep_pdf_CH.cxx
*/
rep_pdf_0 *
new_rep_pdf_CH(
	pdf_ctxt & _pdf,
	data_set * _dset,
	str0 filenamepdf,
	data_set * _dset_curr,
	data_set * _dset_prev
);

#endif

