#if 1

/*
         Mq { cpu runq-sz %runocc swpq-sz %swpocc }
         Mu { cpu %usr %sys %wio %idle }
         a { iget/s namei/s dirbk/s }
         b { bread/s lread/s %rcache bwrit/s lwrit/s %wcache pread/s pwrit/s }
         c { scall/s sread/s swrit/s fork/s exec/s rchar/s wchar/s }
         d { device %busy avque r+w/s blks/s avwait avserv }
         m { msg/s sema/s }
         q { runq-sz %runocc swpq-sz %swpocc }
         v { text-sz ov proc-sz ov inod-sz ov file-sz ov }
         w { swpin/s bswin/s swpot/s bswot/s pswch/s }
         y { rawch/s canch/s outch/s rcvin/s xmtin/s mdmin/s }
 
*/

#include <stdio.h>
#include "misc.h"
#include "str0.h"
#include "dgb_fail.h"
#include "sar_parser.h"
#include "vmparse_1020.h"
#include "dset_api.h"
#include "post_object.h" // SEN_TRY macros
#include "misc.h" // proname_argv0
#include "cfg_demo_1.h" // sql connection parameters etc
#include "dset_api_MySQL.h" // sql connection parameters etc

#ifdef WIN32
#warning unported WIN32 kludge to make it compile to see it link
#define __try try
#define __except except
#undef SEN
// #undef SEN_ACCESS_VIOLATION_HANDLER
//#define SEN_TRY
// #define SEN_ACCESS_VIOLATION_HANDLER()
#endif


enum Parse_File_Type
{
	unknown,
	sar_hp10,
	sar_hp11,
	vmstat_hp10,
	vmstat_hp11,
	excess
};

struct Parse_File_Type_Var : public GRP_sar_parse
{
	Parse_File_Type file_type;

	bool set_from_str( str0 str )
	{
		if( str == "vmstat_hp11" )
		{
			file_type = vmstat_hp11;
			return true;
		}
		else if( str == "vmstat_hp10" )
		{
			file_type = vmstat_hp10;
			return true;
		}
		else if( str == "sar_hp11" )
		{
			file_type = sar_hp11;
			return true;
		}
		else if( str == "sar_hp10" )
		{
			file_type = sar_hp10;
			return true;
		}
		else
		{
			file_type = unknown;
			return true;
		}
		return false;
	}

	str0 get_name()
	{
		switch( file_type )
		{
			case unknown:		return "unknown";
			case sar_hp10:		return "sar_hp10";
			case sar_hp11:		return "sar_hp11";
			case vmstat_hp10:	return "vmstat_hp10";
			case vmstat_hp11:	return "vmstat_hp11";
			case excess:		;
		}
		return "unknown";
	}

	void incr()
	{
		file_type = (Parse_File_Type) ((int)file_type+1);
	}
};

str0 prog_name;
// odd not in VARS
CFG_DEMO::kf_cfg_demo_1 cfg_demo;	// global ?

/*
bool send_col_to_dset(
	line_spec * spec,
	int T0,
	int T2,
	int nrow,
	float * flts,
	dset_one & dset
) {
// resolve columns into SQL registered columns -- presuming sql
	line_store * linedata;
	linedata = dset.find_line2( spec );
// load background from SQL
	dset.sql_link->load_weeks(
		linedata,
		T0, T2
	);
// write new data on top of background
	linedata->add_data(
		T0,
		nrow,
		flts
	);
	return TRUE;
}

bool send_tbl_to_dset(
	dset_tbl  & tbl,
	dset_one & dset
) {
	if( !tbl.ncol ) return TRUE;
	int nrow = tbl.data[0]->N();
	time_t T0 = tbl.time0;
	time_t T2 = tbl.spec1.dt * nrow + T0;
	for( int col = 0; col < tbl.ncol; col++ ) {
		if(! send_col_to_dset(
			// the line spec
		&	tbl.data[col]->spec,
			T0,
			T2,
			nrow,
			tbl.data[col]->base(),
			dset
		)) return FALSE;
	}
	return TRUE;
}

bool zparse_sar_file(
	dset_one & dset,
	str0 site_name,
	str0 zfilename
) {
	// create a vanilla tbl, from the two key parameters (site,cmd)
	str0 cmd_name = "sar";
	dset_tbl tbl( (STR0) site_name, (STR0) cmd_name );

	// create a sar_parser, and run it over the file
	sar_parser parser( &tbl );
	if(! parser.z_parse( tbl, (STR0) zfilename ) ) return FALSE;
	// I'm not sure if the parser can be reused, or the tbl

	send_tbl_to_dset( tbl, dset );
	return TRUE;
}
*/

int bad_usage()
{
	Parse_File_Type_Var file_type;
	fprintf( stderr,
		"USAGE: %s "
		"[--sitename sitename] "
		"[--filetype filetype] "
		"[--postfile filename] "
		"FILES\n",
		(STR0) prog_name
	);
	fprintf( stderr, "Where filetype is one of ... \n\n" );
	file_type.file_type = unknown;
	file_type.incr();
	for(
		;
		file_type.file_type < excess;
		file_type.incr()
	) {
		fprintf( stderr, " --filetype %s\n", (STR0) file_type.get_name() );
	}
	fprintf( stderr, "\n" );
	fflush(0);
	return 22;
}

int missing_arg( str0 opt )
{
	fprintf( stderr, "%s: %s expects a parameter\n", (STR0) prog_name, (STR0) opt );
	return bad_usage();
}

int main( int argc, char ** argv )
{
#if 0
#warning update POST - sort out the SEN_TRY_CATCH_EXCEPT macros OK
// # error fatal warning
#else
    SEN_TRY { 
	bool ok = TRUE;
     try {

	TZ.set_TZ( "GB" );


	// set default values before checking the command line
	str0 site_name = getenv( "SAR_SITE_NAME" );
	Parse_File_Type_Var file_type;
	str0 post_file = "/tmp/dpb_sar_test";
	str0 data_default_sar    = "../samples/eg_sar_data/sar_test_data.gz_2";
	str0 data_default_vmstat = "../samples/eg_sar_data/sar_vmstat.gz";

	// parse argv, note shift to 0 based with progname removed

	prog_name = argv[0];
	gdb_sigaction( argv[0] ); // sets progname_argv0 and ...
	argv++;
	argc--;

	// I think cfg_demo has STATIC refs to sections and vars
	// gdb_invoke(true);
	cfg_demo.ref_static(); // do not free statially allocated obj on ref0
	cfg_demo.late_init();
	// init_from_NOISE init_from_ZERO late_init P0P2 // _expr
	// init_from_DATA NBytes_t DATA; DATA = STDIN NBytes_chunk_of_stream
	cfg_demo.set_DEMO_values(); // ,y test settings
//	cfg_demo.file_name=("/tmp/cfg_demo.kf_");
//	cfg_demo.save_file_();
	cfg_demo.save_file("/tmp/cfg_demo.kf_");
	// gdb_invoke(true);

	dset_api_MySQL * dset = new dset_api_MySQL();
	if(!  dset ) {
		FAIL_FAILED(); 
		return 1;
	}
	if(!  cfg_demo.connect_mysql( *(dset->mysql) ) ) {
		FAIL_FAILED(); 
		return 1;
	}

	while( ( argc ) && ( argv[0][0] == '-' ) )
	{
		str0 opt = argv[0];
		argc --;
		argv ++;
		if( opt == "--sitename" )
		{
			if( argc < 2 )
				return missing_arg( opt );
			site_name = argv[0];
			argc --;
			argv ++;
			continue;
		} else if( opt == "--filetype" )
		{
			if( argc < 2 )
				return missing_arg( opt );
			if( !file_type.set_from_str( argv[0] ))
			{
				fprintf(stderr, "bad --filetype '%s'\n", argv[0] );
				return bad_usage();
			}
			argc --;
			argv ++;
			continue;
		} else if( opt == "--postfile" )
		{
			if( argc < 2 )
				return missing_arg( opt );
			post_file = argv[0];
			argc --;
			argv ++;
			continue;
		} else
		{
			if( opt != "--help" )
				fprintf( stderr, "unrecognised parameter %s\n", (STR0) opt );
			return bad_usage();
		}
	}

	if( ! site_name ) {
		fprintf(stderr, "You must set ENV var $SAR_SITE_NAME to COM_Buildin or use --sitename COM_Buildingg\n");
		return bad_usage();
	}

	// for debugging set a default filename
	char * argv2[2];
	if( !argc ) {

		if(1)
		{
			file_type.file_type = vmstat_hp10;
			argv2[0] = data_default_vmstat;
		} else {
			file_type.file_type = sar_hp11;
			argv2[0] = data_default_sar;
		}
		argv2[1] = NULL;
		argc = 1;
		argv = argv2;
	}

//	dset_api_pdb * dset = new dset_api_pdb( post_file );

	// process all files named on the command line 
	switch( file_type.file_type )
	{
	 case sar_hp11:
	 {
		for( int i = 0; i< argc; i++ ) {
			STEP("Loading file %s ...\n", argv[i] );
			if( !sar_parser::zfile_to_dset(
				site_name,
				"sar",
				argv[i],
				dset
			))
			{
				ok = FALSE;
			}
		}
		STEP("Loaded file %d ...\n", ok );

	 }
	 break;
	 case vmstat_hp10:
	 {
		for( int i = 0; i< argc; i++ ) {
			STEP("Loading file %s ...\n", argv[i] );
			if( !vmparse_1020::zfile_to_dset(
				site_name,
				"vmstat",
				argv[i],
				dset
			))
			{
				ok = FALSE;
			}
		}
	 }
	 break;
	 default:
	 {
		fprintf(stderr,"for file parser for file_type %s\n", (STR0) file_type.get_name() );
		return bad_usage();
	 }
	}

	// write the data in ASCII format to stdout
	// does not exist // if(1) dset.print_all( stdout );

	// write the data to the DBMS
	// DUNNO // dset->store_all();
	STEP("deleting dset\n" );
	fflush(0);

	delete dset;


     } catch ( dgb_fail * ex ) {
	ex->caught();
	delete ex;
	ok = FALSE;
     } catch ( const char * ex_str ) {
	WARN("PLAIN STRING EXCEPTION '%s'\n", ex_str );
	ok = FALSE;
     }

	ref_tally.print_leaked_report();

	if( ok )
		return 0;
	else
		return 1;

    } SEN_ACCESS_VIOLATION_HANDLER(); 
#endif
    return EXIT_FAILURE;
}
#else
#include "dgb_fail.h"
int main(int argc, char ** argv )
{
	FAIL("commented out");
	return 1;
}
#endif
