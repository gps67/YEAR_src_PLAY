#ifndef CH_Names_H
#define CH_Names_H

#include "str1.h"

namespace ACCTS {

enum CH_Name_Tag
{
	e_zero = 0,
	e_CH1_col_PREV,
	e_CH1_col_CURR,
	e_CH1_archive_csv,
	e_CH1_archive_csv_play,
	e_CH1_pdf,
	e_CH1_pdf_play,
	e_csv_CURR_PL2,
	e_mm3_PREV,
	e_mm3_CURR,
	e_csv_PREV,
	e_csv_CURR,
	e_csv_CAT_adj,
	e_PL2_CashCat_pdf,
	e_PL2_VatTrack_pdf,
	e_last
};

extern
bool mk_CH_name( buffer1 & buf, int year2, CH_Name_Tag tag );
extern
const char * str_CH_name( buffer1 & buf, int year2, CH_Name_Tag tag );

extern 
bool CH_Name_Tag_show_test(int year2);

}; // namespace
#endif
