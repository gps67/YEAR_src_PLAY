#ifndef rep_pdf_RUN_C_H
#define rep_pdf_RUN_C_H

namespace ACCTS {
	class data_set;
};
using namespace ACCTS;
#include "str0.h"

extern bool run_html_Cash_Cat( data_set * dset, str0 filenamepdf );

extern bool run_pdf_VatTrack( data_set * dset, str0 filenamepdf );
extern bool run_pdf_Cash_Cat( data_set * dset, str0 filenamepdf );

extern bool run_pdf_Table( table * tbl, str0 filenamepdf );

extern bool run_pdf_CH(
	data_set * dset,
	str0 filenamepdf,
	data_set * dset_curr,
	data_set * dset_prev
);
extern bool run_report_3_pdf( data_set * dset, str0 filenamepdf );
#endif

