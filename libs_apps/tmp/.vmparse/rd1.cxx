/*
	rd1.c is the MAIN	
	it holds HARDCODED passwords, and SITE_NAME SQL-DB-NAME, etc
	much like a script or data file would ;-)

	rd1 takes a list of filenames where ... each file ...
	line1 is a special header, that contains the date and hostname
	data is `vmstat 60 1440`
	the file was zlib compressed

	The file is parsed, and each 'line_id' is registered (SQL)
	The data is accumulated in a memory store
	The data is dumped to SQL disk

	Overlapping segments are NOT deleted (HERE), but that is the idea

	TimeZone is PRESUMED to match (collector == parser), but is
	accounted for (fixable). Data is spliced into WEEKS at SUN:2400:GMT

	vmstat is OK, but `sar -AM 60 1400` is better (vmstat -d not added)
	TODO: add the sar parser!
	TODO: Linux sar is completely different, but BINARY API possible!
	NOTE: Time Series (every Minute) is used here, bdf daily elsewhere.
	NOTE: site is supposed to be 'ABC_READING' not '-' (READING==TOWN)
	NOTE: HPUX1020 was the prototype, others will need adjusted parsers
	HINT: parse all then edit SQL.TABLE.LINE[id].sitename!
	TODO: add site(ABC_BUILDING) as a parameter
	NOTE: split YEAR/Decade using line_id somehow
*/

#define FIXED_CMD	"vmstat1020"
#define FIXED_SITE	"-"		// "ABC_BUILDING"

// #include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

// #include <stdlib.h>
#include <stdio.h>
// #include <time.h>
// #include <errno.h>

#include "misc.h"
//#include "array_base_1248.h"

#include "tz.h"
#include "vm18flt_tbl.h"
#include "tm_parts.h"
#include "line1.h"
#include "vmparse_1020.h"
// #include "file_line.h"
#include "zfile_line.h"
#include "line_store_tbl.h" // multiple classes
#include "line_data_seg.h" // multiple classes

#include "zblk.h" // 
#include "sql_line_id.h" // 
#include "sql_seg.h" // 

int zparse_file( line_dset & dset, const char * zfilename ) {
	// int fd;
	char * line;
	line1 line1;
	zfile_line in;

	if( !in.open( zfilename ) ) return errno;

	if( !(line = in.getline()) ) {
		fprintf(stderr, "ERROR Empty File! Missing first line \n");
		in.close();
		return 99;
	}
	// could be vmstat_1020, sar_1020, vmstat_sol7, sar_sol7, ...
	// though they might have similar line1 layouts?
	// scripting goes here to call this ??
	if( !line1.parse_line1( line ) ) {
		fprintf(stderr, "ERROR vmstat_1020 file expected\n%s", line );
		in.close();
		return 98;
	}
	// fprintf(stderr,"%6d %s\n", line1.t0, line );

	vmparse_1020 vmparse;	// parses the strings
	vm18flt_tbl tbl;	// receives plain data from vm-parse
	vmparse.z_parse( line1, in, tbl ); // needs line1 hints + hdrs
	in.close();		// was closed on eof anyway 

	// now put vmstat data into dset
	// this isnt underneath because it might vary (site)
	line_id_spec spec1;
	line_store * linedata;

 if(1) {
	int ncols = 20;
	char ** names = vmparse.colnames();
//	maybe add here or later ??
//	char ** names_opt = vmparse.colnames_opt();
//	char ** names_attr = vmparse.colnames_attr();
	for( int c = 0; c < N18; c++ ) {

		// our spec1 is not retained - a copy is
//		spec1.site.set( "-" );			// LURK // ERRM
		spec1.site.set( FIXED_SITE );			// LURK // ERRM
		spec1.host.set( line1.hostname );
		spec1.dev.set( "-" );
//		spec1.cmd.set( "vmstat1020" );
		spec1.cmd.set( FIXED_CMD );
		spec1.opt.set( "-" );
		spec1.attr.set( names[c] );
		spec1.avg.set( "-" );	// raw data (ints!) isnt averaged
		spec1.dt = line1.dt;

// could re-process line_alias later
// could also figure when attr == opt "." something

//		spec1.opt.set( names_opt[c] );
//		spec1.attr.set( names_attr[c] );

		linedata = dset.find_line( & spec1 );
		if( 0==linedata->data.N ) {
			int T0 = line1.t0 - (60*60*24*7);
			int T2 = T0 + (60*60*24*7)*100; // 2 yrs?
			SQL_seg sql( & dset.sql_link );
			sql.load_weeks(
				linedata,
				& dset,
				T0, T2 );
		}
		linedata->add_data(
			line1.t0,
			tbl.nrow,
			tbl.get_col( c )
		);

	}
 }
	return 0;
}

int main( int argc, char ** argv )
{

	if(0) {
		zblk z;
		if(  z.test() ) return 0;
		return 99;
	}

	char * filename;
	char * zfilename;
	/* the first tzset is done manually  -else ? GMT GMT ? */
	TZ.set_TZ( "GB" );

	line_dset dset;
	sql_selector sel = PostgreSQL;
#define MySQL MySql
//	sql_selector sel = MySQL;
	char * host = "localhost";
	char * user = "obj_stat_gather";
	char * pass = "gather99";
	char * db = "obj_stat_1";
	if(!dset.mysql_connect( sel, host, 0, user, pass, db )) {
		return 98;
	}

	filename = "vmstat_medprod_0731";
	zfilename = "vmstat_medprod_0731.gz";
	// was
	// 6 seconds for 100 files (144)
	// 2 seconds per month ! (1 host)
	// 10 * 30 = 18 secs per month  (no gz)
	// zparse_file( zfilename );
	// 2.9 seconds for 7 files (1440) (read not print)
	// 3.4 seconds for 7 files (1440) (read and print)
	for( int i = 1; i< argc; i++ ) {
		zparse_file( dset, argv[i] );
	}
	// dset.print_all( stdout );
	dset.store_all( );
	if(0) dset.print_all( stdout );
}
