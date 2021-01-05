#ifndef report0_H
#define report0_H

namespace ACCTS {
	class data_set;
};
using namespace ACCTS;
#include "str0.h"

extern bool run_report_1( data_set * dset );
extern bool run_report_2( data_set * dset );
extern bool run_report_3( data_set * dset );
extern bool run_report_4( data_set * dset );
// completely different
extern bool run_report_5_CH( str0 filename_year_csv, str0 filename_CH1_csv );

extern bool run_CH1_YEAR2( int year2 );
extern bool run_CH1_check1( int year2 );

// #include "rep_pdf_RUN_C.h"

#endif

