#include "misc.h"
#include "amnt_t.h"
#include "date_glib.h"
#include "dset.h"


#include "w_widget0.h"
//# #include <gtk/gtk.h>

// #include "w_file_sel.h"
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

// --------------------------------------------------
class Argv_scanner
{
public:
	int argc;
	char ** argv;
public:
	bool usage_error;

	Argv_scanner( int _argc, char ** _argv )
	{
		usage_error = false;
		argc = _argc;
		argv = _argv;
	}

	virtual bool register_program_name()
	{
		if(argc < 1 ) {
			return FAIL("progname missing from argv");
		}

		gdb_sigaction( argv[0] ); // sets progname_argv0
		argv++;
		argc--;
		return true;
	}

	char * pop_arg()
	{
		if(argc < 1 ) {
			FAIL("CODE ERROR");
			return NULL;
		}
		char * s = argv[0];
		argc--;
		argv++;
		return s;
	}

	virtual bool on_usage_error() {
		usage_error = true;
		return false;
	}

	/*
		cmd can be --varname value if you like
	*/
	bool match_cmd( str0 cmd, str0 cmd_desc )
	{
		// does cmd match ?
		// absent arg will not match NULL
		if( cmd != (STR0) argv[0] ) return false;

		if( argc < 1 ) {
		 argc = 0; // already is
		 on_usage_error();
		 return FAIL("CODE ERROR expected %s // **** ",
			(STR0) cmd
			);
		}

		// remove cmd from argv
		argc--;
		argv++;

		return true;
	}

	/*
		--load-load-save f1 f2 f3
	*/
	bool match_cmd_arg_arg_arg( str0 cmd, str0 cmd_desc, str1 & arg1, str1 & arg2, str1 & arg3 )
	{
		if( cmd != (STR0) argv[0] ) return false;

		str0 argdesc = "3args";
		str0 arg; // temp

		if( argc < 1 ) { // cmd itself
		 argc = 0; // already is
		 arg = ""; // const
		 on_usage_error();
		 return FAIL("CODE ERROR expected %s arg // %s",
			(STR0) cmd,
			(STR0) argdesc
			);
		}
		// remove cmd from argv
		argc--;
		argv++;

		// check for absent arg
		// should commit to eating the two words
		if( argc < 3 ) {
		 arg = ""; // const
		 on_usage_error();
		 return FAIL("expected 3 args // %s",
			(STR0) cmd,
			(STR0) argdesc
			);
		}

		// check against --next-flag-where-arg-should-be
		arg = argv[0];
		if( arg.has_prefix("--") ) {
		 // leave --flag on argv
		 on_usage_error();
		 return FAIL("expected %s arg // (got %s) // %s",
			(STR0) cmd,
			(STR0) arg,
			(STR0) argdesc
			);
		}
		// remove arg
		argc--;
		argv++;
		arg1 = arg;

		// check against --next-flag-where-arg-should-be
		arg = argv[0];
		if( arg.has_prefix("--") ) {
		 // leave --flag on argv
		 on_usage_error();
		 return FAIL("expected %s arg // (got %s) // %s",
			(STR0) cmd,
			(STR0) arg,
			(STR0) argdesc
			);
		}
		// remove arg
		argc--;
		argv++;
		arg2 = arg;

		// check against --next-flag-where-arg-should-be
		arg = argv[0];
		if( arg.has_prefix("--") ) {
		 // leave --flag on argv
		 on_usage_error();
		 return FAIL("expected %s arg // (got %s) // %s",
			(STR0) cmd,
			(STR0) arg,
			(STR0) argdesc
			);
		}
		// remove arg
		argc--;
		argv++;
		arg3 = arg;

		return true;
	}

	/*
		cmd can be --varname value if you like
	*/
	bool match_cmd_arg( str0 cmd, str0 cmd_desc, str1 & arg, str0 argdesc )
	{
		// does cmd match ?
		// absent arg will not match NULL
		if( cmd != (STR0) argv[0] ) return false;

		if( argc < 1 ) { // cmd itself
		 argc = 0; // already is
		 arg = ""; // const
		 on_usage_error();
		 return FAIL("CODE ERROR expected %s arg // %s",
			(STR0) cmd,
			(STR0) argdesc
			);
		}
		// remove cmd from argv
		argc--;
		argv++;

		// check for absent arg
		// should commit to eating the two words
		if( argc < 1 ) {
		 arg = ""; // const
		 on_usage_error();
		 return FAIL("expected %s arg // %s",
			(STR0) cmd,
			(STR0) argdesc
			);
		}

		// check against --next-flag-where-arg-should-be
		arg = argv[0];
		if( arg.has_prefix("--") ) {
		 // leave --flag on argv
		 on_usage_error();
		 return FAIL("expected %s arg // (got %s) // %s",
			(STR0) cmd,
			(STR0) arg,
			(STR0) argdesc
			);
		}
		// remove arg
		argc--;
		argv++;

		return true;
	}

	/*
		cmd can be --varname value if you like
	*/
	bool match_cmd_args(
		str0 cmd,
		str0 cmd_desc,
		str1 & arg1,
		str0 arg1_desc,
		str1 & arg2,
		str0 arg2_desc
	)
	{
		// does cmd match ?
		// absent arg will not match NULL
		if( cmd != (STR0) argv[0] ) return false;

		// safety
		// NO //  arg1 = ""; // const
		// NO //  arg2 = ""; // const

		if( argc < 1 ) {
		 argc = 0; // already is
		 on_usage_error();
		 return FAIL("CODE ERROR matched %s arg arg // %s",
			(STR0) cmd,
			(STR0) cmd_desc
			);
		}
		// remove cmd from argv
		argc--;
		argv++;

		// check for absent arg
		// should commit to eating the two words
		if( argc < 2 ) {
		 on_usage_error();
		 return FAIL("expected %s arg arg // %s %s",
			(STR0) cmd,
			(STR0) arg1_desc,
			(STR0) arg2_desc
			);
		}

		// check ARG1 against --next-flag-where-arg-should-be
		arg1 = argv[0];
		if( arg1.has_prefix("--") ) {
		 // leave --flag on argv
		 on_usage_error();
		 return FAIL("expected %s arg // (got %s) // %s",
			(STR0) cmd,
			(STR0) arg1,
			(STR0) arg1_desc
			);
		}
		// remove arg
		argc--;
		argv++;

		// check ARG2 against --next-flag-where-arg-should-be
		arg2 = argv[0];
		if( arg2.has_prefix("--") ) {
		 // leave --flag on argv
		 on_usage_error();
		 return FAIL("expected %s arg // (got %s) // %s",
			(STR0) cmd,
			(STR0) arg2,
			(STR0) arg2_desc
			);
		}
		// remove arg
		argc--;
		argv++;

		return true;
	}

	/*
		match flag_var() ... should have var_type mechanism
	*/

}; // Argv_scanner
// --------------------------------------------------

struct CH1_layout {
	str1 CH1_archive_csv;
};

extern
bool run_CH1_YEAR2( int year2 );

extern
bool run_CH1_YEAR2_ZERO( int year2 );

extern
bool run_CH1_check1(int year2);

bool run_CH1_2010_2011() { return run_CH1_YEAR2( 2011 ); }
bool run_CH1_2011_2012() { return run_CH1_YEAR2( 2012 ); }
bool run_CH1_2012_2013() { return run_CH1_YEAR2( 2013 ); }
bool run_CH1_2013_2014() { return run_CH1_YEAR2( 2014 ); }

int main(int argc, char ** argv )
{
	/*
		errm the entire argv processing is awful
		as is the use of --verb-action on the command line
		a --flag is not a --cmd
		getopts? sucessive cmd args? but not today ?
	*/
	{

		str1 filename;

		Argv_scanner argv_scanner( argc, argv );

		argv_scanner.register_program_name();
		// gdb_sigaction( argv[0] ); // sets progname_argv0
		// argc--; argv++;

		// char * argv2[ argc+1+1+10 ];
		char * argv2[ 20+1+1+10 ];

		int argc2 = 0;
		argv2[ argc2++ ] = (char *)progname_argv0;

		// accts by default opens a grid gui on the csv
		// but running reports doesnt want that default
		bool then_do_grid = true;

		CH1_layout CH1_layout;
		str1 func_name;
		str1 func_arg;


		while( argv_scanner.argc > 0 )
		{

			if( argv_scanner.match_cmd_arg(
				"--in",
				"The input csv",
				filename,
				"The input csv"
			)) continue;

			if( argv_scanner.match_cmd(
				"-",
				"The input csv is stdin"
			)) {
				filename = "-";
				continue;
			}

			if( argv_scanner.match_cmd_arg(
				"--CH1-archive",
				"sets the filename used later",
				CH1_layout.CH1_archive_csv,
				"The CH1 archive of several years attrs vals"
			)) continue;

			if( argv_scanner.match_cmd_arg(
				"--pdf-vat-track",
				"run the over-detailed vat tracking report",
				filename,
				"csv filename"
			))
			{
			return bool_to_retcode( run_pdf_vat_track( filename ));
				then_do_grid = false;
				continue;
			}

			if( argv_scanner.match_cmd_arg(
				"--load-save-quit",
				"normalise the csv",
				filename,
				"the csv" ))
			{
				then_do_grid = false;
			obj_hold<data_set> dset1 = new data_set();	
			if( !dset1->file_load( filename ) ) return 1;
			if( !dset1->file_save( filename, false ) ) return 1;
			return 0;
				continue;
			}

			str1 filename2;
			str1 filename3;
			if( argv_scanner.match_cmd_arg_arg_arg(
				"--load-load-save",
				"merge two into third",
				filename,
				filename2,
				filename3
				))
			{
				then_do_grid = false;
			obj_hold<data_set> dset1 = new data_set();	
			if( !dset1->file_load( filename ) ) return 1;
// gdb_invoke(true);
// this doesnt work because the column headers must match exactly
// need a command called COPY_COLUMN
			if( !dset1->file_load( filename2 ) ) return 1;
			if( !dset1->file_save( filename3, false ) ) return 1;
			return 0;
				continue;
			}

			if( argv_scanner.match_cmd_arg(
				"--run-reports",
				"run the Cat and Cash pdf reports",
				filename,
				"the csv file and also pdf basename"
			))
			{
				then_do_grid = false;
			return bool_to_retcode( run_reports(
				filename,
				CH1_layout.CH1_archive_csv
			));
				continue;
			}

/*

			if( argv_scanner.match_cmd_arg(
				"--run-reports-CH",
				"the CH1 kaboodle",
				filename,
				"..."
			))
			{
				then_do_grid = false;
			return bool_to_retcode( run_report_5_CH(
				filename,
				CH1_layout.CH1_archive_csv
			));
				continue;
			}
*/

			str0 arg = argv[0];
			argv++;
			argc--;


			str1 Y;
			if( argv_scanner.match_cmd_arg(
				"CH1_YEAR2_ZERO",
				"clear the years I am currently working on",
				Y,
				"YEAR2"
			))
			{
				int year2 = 2012;
				Y.as_int( year2 );
				run_CH1_YEAR2_ZERO( year2 );
				return 0;
				continue;
			// must copy temp file to live file for next year !!!
			//	run_CH1_YEAR2_ZERO( 2012 );
			//	run_CH1_YEAR2_ZERO( 2013 );
			}

			if( argv_scanner.match_cmd(
				"CH1_2010_2011",
				"Run CH1 suite with hardcoded values"
			))
			{
			if(0)
				if(! run_CH1_2010_2011() )  
					 return bool_to_retcode(FAIL_FAILED());

			 // a very confused test on the differences only
			 // probably writes the new values to disk
			 // opens two pdf windows cos it does it twice !!
			 if(1)
				if(! run_CH1_check1(2011)) 
					 return bool_to_retcode(FAIL_FAILED());
				 return bool_to_retcode(PASS("DONE"));
				
				continue;
			}

			if( argv_scanner.match_cmd(
				"CH1_2011_2012",
				"Run CH1 suite with hardcoded values"
			))
			{
				// WISH // E1 || return 22; // G++ needs expr
				return bool_to_retcode(
					run_CH1_2011_2012() 
				);
				continue;
			}

			if( argv_scanner.match_cmd(
				"CH1_2012_2013",
				"Run CH1 suite with hardcoded values"
			))
			{
				// WISH // E1 || return 22; // G++ needs expr
				return bool_to_retcode(
					run_CH1_2012_2013() 
				);
				continue;
			}

			if( argv_scanner.match_cmd(
				"CH1_2013_2014",
				"Run CH1 suite with hardcoded values"
			))
			{
				// WISH // E1 || return 22; // G++ needs expr
				return bool_to_retcode(
					run_CH1_2013_2014() 
				);
				continue;
			}

			if( argv_scanner.match_cmd(
				"--gdb-text",
				"..."
			))
			{
				gdb_invoke( false );
				continue;
			}

			if( argv_scanner.match_cmd(
				"--gdb-gui",
				"..."
			))
			{
				gdb_invoke( true );
				continue;
			}

			argv2[ argc2++ ] = argv_scanner.pop_arg();

		}
		argv2[ argc2 ] = NULL;


	/*
		problems with PDF closing and reopening,
		or tables, or something. So need to call this prog
		for each pdf output. Only one gets run per call
	*/
		if(!CH1_layout.CH1_archive_csv) {
			// "tmp_CH1_archive.csv"
			CH1_layout.CH1_archive_csv = default_CH1_archive_csv; 
		}

		if( then_do_grid )
		{
			// gtk args are not yet handled by argv_scanner
			// maybe it could have its own iterating list
			char ** p_argv2 = (char **)&argv2;
			TOPAPP_Holder_gtk holder(&argc2, &p_argv2);
			new accts_top_app( &holder, filename );
			holder.call_gx_main_and_exit();
		}
	}
	ref_tally.print_leaked_report();
	return 0;
}

