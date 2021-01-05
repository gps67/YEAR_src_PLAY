#include "misc.h"
#include "amnt_t.h"
#include "date_glib.h"
#include "dset.h"

#include "w_widget0.h"
#include "w_file_sel.h"
#include "accts_top_app.h"

#include "io_csv_accts.h"
#include "run_report.h"
#include "rep_pdf_RUN_C.h"

// T_MY_HOLDER

// #include "w_win_top.h"
#include "TOPAPP_Holder_gtk.h"
#include "dir_name_ext.h"
#include "fork_progs.h"
#include "dgb_fail.h"

#define default_CH1_archive_csv "tmp_CH1_archive.csv"

int main_test1( str0 filename)
{
	obj_hold<data_set> dset1;	
	io_csv_accts io1( dset1 );
	if(1)
		// SAME // if( !dset1->file_load( filename ) ) return 1;
		io1.load_file( filename );
	else {
		io1.load_line_x("T1, K1, F2");
		io1.load_line_x("T1, a, 1");
		io1.load_line_x("T1, b, 2");
		io1.load_line_x("CAT, cat, path, desc");
		io1.load_line_x("CAT, c1,  -,    desc1");
		io1.load_line_x("CAT, c2, c1,    desc2");
	};

	return 0;
}

bool run_pdf_vat_track( str0 filename_csv ) // filename_csv
{
	obj_hold<data_set> dset1 = new data_set();	
	if( !dset1->file_load( filename_csv ) ) return false;

	// io_csv_accts io1( dset1 );
	// if(!io1.load_file( filename_csv ))
	// 	return false;

	if(!run_report_1( dset1 )) return false;
	if(!run_report_2( dset1 )) return false;
	if(!run_report_3( dset1 )) return false;
	if(!run_report_4( dset1 )) return false;

	str1 vat_track_pdf;
	
	dir_name_ext file_named_X( filename_csv );
	buffer2 nm;
	nm.put( file_named_X.name ) ;
	nm.put( "_Vat_Track" ) ;
	dir_name_ext vtk( filename_csv );
	vtk.name.set( nm );
	vtk.ext.set( "pdf" );
	vtk.mk_full_path_name();
	vat_track_pdf = vtk.full_path_name;

	e_print("Generating: '%s'", (STR0) vat_track_pdf );
	if(! run_pdf_VatTrack( dset1, vat_track_pdf ))
		return false;
	fork_xpdf( vat_track_pdf );
	return true;
}

bool run_reports( str0 filename_csv, str0 filename_CH1_archive_csv )
{
	INFO("file %s", (STR0) filename_csv );
	obj_hold<data_set> dset1 = new data_set();	
	if( !dset1->file_load( filename_csv ) ) return false;

	// io_csv_accts io1( dset1 );
	// if(!io1.load_file( filename_csv ))
	// 	return false;

	if(!run_report_1( dset1 )) return false;
	if(!run_report_2( dset1 )) return false;
	if(!run_report_3( dset1 )) return false;
	if(!run_report_4( dset1 )) return false;
	/*
		run a separate phase
		1011.csv -> 1011_PL2.csv # different CAT tree, sums done

		tmp_CH1_archive_draft.csv += 1011_PL2.csv

		1011_CH1_draft.pdf <= from archive ?
	*/
	str1 filename_html;
	str1 filename_pdf;

 // return 0;

	dir_name_ext file_named_X( filename_csv );
	file_named_X.ext = "html";
	file_named_X.mk_full_path_name();
	filename_html.set( file_named_X.full_path_name );

	file_named_X.ext = "pdf";
	file_named_X.mk_full_path_name();
	filename_pdf = file_named_X.full_path_name;

	// i THINK its ok to mix HTML and PDF -- clue ?
	if(!run_pdf_Cash_Cat( dset1, filename_pdf )) return false;

	int page = 20; // varies with user sessions - make a static config
	page = 1; // varies with user sessions - make a static config
	fork_xpdf_page( filename_pdf, page ); 
	// added bonus - comes up in landscape mode

	if(!run_html_Cash_Cat( dset1, filename_html )) return false;
//	fork_netscape( filename_html );

	return true;
}


int bool_to_retcode( bool ok )
{
	if( ok ) 
		return 0;
	if( errno )
		return errno;
	return 1;
}

int main(int argc, char ** argv )
{
	/*
		errm the entire argv processing is awful
		as is the use of --verb-action on the command line
		a --flag is not a --cmd
		getopts? sucessive cmd args? but not today ?
	*/
	{

		char * filename = NULL;

		gdb_sigaction( argv[0] ); // sets progname_argv0
		argv++;
		argc--;

		// char * argv2[ argc+1+1+10 ];
		char * argv2[ 20+1+1+10 ];

		int argc2 = 0;
		argv2[ argc2++ ] = (char *)progname_argv0;

		bool _run_reports = false;
		bool _run_reports_CH = false;
		bool _call_func_with_arg = false;
		bool _pdf_vat_track = false;
		bool _load_save_quit = false;
		bool _gdb_text = false;
		bool _gdb_gui = false;

		str1 CH1_archive_csv;
		str1 func_name;
		str1 func_arg;


		while( argc > 0 )
		{
			str0 arg = argv[0];
			argv++;
			argc--;
			if( arg[0] != '-' )
			{
				filename = (char *)arg;
				argv2[ argc2++ ] = (char *)arg;
				continue;
			}
			if( arg == "--in" )
			{
				filename = argv[0]; // POSS NULL
				argv++;
				argc--;
				continue;
			}
			if( arg == "--CH1-archive" )
			{
				CH1_archive_csv = argv[0];
				argv++;
				argc--;
				continue;
			}
			if( arg == "--pdf-vat-track" )
			{
				_pdf_vat_track = true;
				continue;
			}
			if( arg == "--run-reports" )
			{
				_run_reports = true;
				continue;
			}
			if( arg == "--call_func_with_arg" )
			{
				if(argc<2) {
					THROW_dgb_fail("argv wrong");
					return 22;
				}
				_call_func_with_arg = true;
				func_name = argv[0];
				func_arg = argv[1];
				argv++;
				argv++;
				argc--;
				argc--;
				continue;
			}
			if( arg == "--run-reports-CH" )
			{
				_run_reports_CH = true;
				continue;
			}
			if( arg == "--load-save-quit" )
			{
				_load_save_quit = true;
				continue;
			}
			if( arg == "--gdb-text" )
			{
				gdb_invoke( false );
				_gdb_text = true;
				continue;
			}
			if( arg == "--gdb-gui" )
			{
				gdb_invoke( true );
				_gdb_gui = true;
				continue;
			}


		}
		argv2[ argc2 ] = NULL;


	// done immed to catch CLI parsing code functions
	// if( _gdb_text ) { gdb_invoke( false ); } else
	// if( _gdb_gui ) { gdb_invoke( true ); }

	/*
		problems with PDF closing and reopening,
		or tables, or something. So need to call this prog
		for each pdf output. Only one gets run per call
	*/
		if(!CH1_archive_csv) {
			// "tmp_CH1_archive.csv"
			CH1_archive_csv = default_CH1_archive_csv; 
		}

		if( _load_save_quit )
		{
			obj_hold<data_set> dset1 = new data_set();	
			if( !dset1->file_load( filename ) ) return 1;
			if( !dset1->file_save( filename, false ) ) return 1;
			return 0;
		}
		else if( _pdf_vat_track )
			return bool_to_retcode( run_pdf_vat_track( filename ));
		else if( _run_reports )
			return bool_to_retcode( run_reports(
				filename,
				CH1_archive_csv
			));
		else if( _run_reports_CH )
			return bool_to_retcode( run_report_5_CH(
				filename,
				CH1_archive_csv
			));
		else
		{
			char ** p_argv2 = (char **)&argv2;
			TOPAPP_Holder_gtk holder(&argc2, &p_argv2);
			new accts_top_app( &holder, filename );
			holder.call_gx_main_and_exit();
		}
	}
	ref_tally.print_leaked_report();
	return 0;
}

