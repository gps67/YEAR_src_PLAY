// THIS IS:  #include "rep_pdf_CH.h"
// Companies House report
// uses DIFFERETN dset

#include "rep_pdf_0.h"
#include "rpt_names.h"
#include "dset.h"

// now need to vary report depending on year ...
#include "date_glib.h"

// VIM is putting this file as UTF8 not 8859
// there is a VIM flag to specify it, but using octal
// PDF expects 8859
// £
#define CHR_POUND '\243' // OCT 243 DEC 163 HEX A3
#define STR_POUND "\243" // OCT 243 DEC 163 HEX A3

using namespace PDF_GEN;
using namespace ACCTS;

namespace ACCTS {

	/*!
		CH Companies House
	*/
	class rep_pdf_CH : public rep_pdf_0
	{
	 public:
		virtual ~rep_pdf_CH();
		virtual bool run_wrapped_report();

		data_set * dset_curr;
		data_set * dset_prev;

		//! accounting reference date (last day of year)
		date_glib ARD;

		str1 curr_year_end_long;
		str1 curr_year_end;
		str1 prev_year_end;

		table * INFO_tbl;
		col_spec * INFO_val_spec;
		str2 * INFO_attr( str0 attrname ); // never NULL

		str1 company_name;

	//	obj_hold<col_spec_amnt> curr_spec;
		col_spec_amnt * curr_spec;
		col_spec_amnt * prev_spec;
		col_spec      * desc_spec;
		col_spec      * io_spec;
		col_spec      * lr_spec;
		table * CH1_tbl;

		amnt_t curr_amnt;
		amnt_t prev_amnt;
		amnt_t show_curr_amnt;
		amnt_t show_prev_amnt;
		bool io_is_normal;
		bool lr_is_l;
		str2 * desc;
		bool get_row( str0 key );

		flt80 w_par; // pen->calc_string_width( ")" );
		flt80 x1;
		// flt80 x2;
		// flt80 x3;
		// flt80 x4;
		// flt80 x5;
		// flt80 x6;
		// flt80 x7;
		// flt80 x8;
		// flt80 x9;

		obj_hold<pdf_pen_text> pen_bold;
		flt80 h_normal;
		flt80 h_bold;
		flt80 h_titles;

		// current pen
		pdf_pen_text * pen;
		flt80 h;

		void set_pen_normal();
		void set_pen_titles();
		void set_pen_bold();
		void y_down_line();
		void y_down_lines( int lines );
		void y_down_line( int lines ) { y_down_lines(lines); }
		void blank_line( int lines = 1 ) { y_down_lines(lines); }

		buffer2 buff;

		bool init();

		rep_pdf_CH(
			pdf_ctxt & _pdf,
			data_set * _dset,
			str0 filenamepdf,
			data_set * _dset_curr,
			data_set * _dset_prev
		);

		void y_space_for_overlines( int overlines );
		void y_space_for_underlines( int underlines );
		void print_amnt_1( flt80 x_right, amnt_t &amnt, int overlines, int underlines );


		void x1_line( str0 text ); // current pen
		void x1_line_bold( str0 text );

		void x1_subtitle( str0 title );
		void L0_title( str0 title );

		void tab_ruler (
			flt80 _L1,
			flt80 _L2,
			flt80 _L3
		);

		bool title_page();
		bool contents_page();
		bool company_information_page();
		bool report_of_the_director_page();
		bool profit_and_loss_page();
		bool balance_sheet_page();
		bool notes_to_the_fs_pages();
		bool report_of_the_accountants_page();
		bool profit_and_loss_account_page();

		void page_header( str0 page_name );
		void page_footer( str0 notice, str0 page_number );

		void profit_and_loss_hdr();
		bool profit_and_loss_row(
			bool bold,
			str0 key,
			str0 notes,
			int overlines,
			int underlines
		);

		void balance_sheet_hdr();
		bool balance_sheet_row(
			bool bold,
			str0 key,
			str0 notes,
			bool left,
			int overlines,
			int underlines
		);

		void NFS_hdr( str0 title );
		bool NFS_row(
			str0 key,
			int overlines,
			int underlines
		);

		void PL2_hdr( str0 title );
		bool PL2_row(
			str0 key,
			bool left,
			int overlines,
			int underlines
		);

	}; // CLASS
}; // NAMESPACE
