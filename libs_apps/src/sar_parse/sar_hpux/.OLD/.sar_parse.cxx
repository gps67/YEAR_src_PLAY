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

/*
#define FIXED_SITE	"FIRM_Building"		
	This is the name of the site,
	if you didnt set it before importing your data,
	you can simply 
		UPDATE line_id
		WHERE site="FIRM_Building"
		SET site="company_townname"
		;
	But next time you import data, you must get it right.
	The site must match or you get a new line_id.

	TODO: make this a getenv()
*/


#include <stdio.h>
#include "misc.h"
#include "dset_tbl.h"
#include "sar_parser.h"
#include "DB_sar.h"
#include "dset_one.h"


#include "line_store_tbl.h" // multiple classes
#include "line_data_seg.h" // multiple classes

#include "dset_one.h" // dset_one

// #include "sql_line_id.h" //
// #include "sql_seg.h" //
//

#include "post_object.h" // SEN_TRY macros

BOOL send_col_to_dset(
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

BOOL send_tbl_to_dset(
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

BOOL zparse_sar_file(
	dset_one & dset,
	const char * site_name,
	const char * zfilename
) {
	// create a vanilla tbl, from the two key parameters (site,cmd)
	const char * cmd_name = "sar";
	dset_tbl tbl( site_name, cmd_name );

	// create a sar_parser, and run it over the file
	sar_parser parser( &tbl );
	if(! parser.z_parse( tbl, zfilename ) ) return FALSE;
	// I'm not sure if the parser can be reused, or the tbl

	send_tbl_to_dset( tbl, dset );
	return TRUE;
}

int main( int argc, char ** argv )
{
    SEN_TRY { 

	// TZ.set_TZ( "GB" );
	/*

		ENSURE that you parse the data in the gathering timezone
		If that is cron it is usually localtime (guess from the
		closing mtime), which usually means 23:00 GMT!

	*/

//	sql_selector sel = MySQL;
//	sql_selector sel = PostgreSQL;
	sql_selector sel = Post; // handle SIGSEGV nostop noprint pass

	/*
		You do have a choice, but several fixes have been
		applied to the PostgreSQL side (which was tested with
		sar), and they need to be carried into the MySQL side.
		(Ditto for vmstat)

		The 'common code theory' falls down, because they need
		different schema.
	*/

	dset_one dset;	/*
		Connect to the database
		dset_one is the new name of line_store
		these password options should come from outside
	*/
	char * host = "localhost";
	char * user = "obj_stat_gather_user";
	user = "obj_stat_gather";
	char * pass = "gather99";
	char * db = "obj_stat_1";
	BOOL readonly = FALSE;
	// readonly = TRUE; // TEST
	if(!dset.sql_connect( sel, host, 0, user, pass, db, readonly )) {
		fprintf(stderr,"Connect to database failed\n");
		return 98;
	}

if(0)
{
// TEST - LOAD ALL - DUMP FILE
	dset.sql_link->tester( & dset ); // load all
	if(1) dset.print_all( stdout );
	return 0;
}

	const char * site_name = getenv( "SAR_SITE_NAME" );
	if( ! site_name ) {
		fprintf(stderr, "You must set ENV var $SAR_SITE_NAME to COM_Building\n");
		return(22);
		// exit(22); // doesnt call dset_one delete (its a sub func)
	}

	// process all files named on the command line 
	for( int i = 1; i< argc; i++ ) {
		zparse_sar_file( dset, site_name, argv[i] );
	}

	// write the data in ASCII format to stdout
	if(1) dset.print_all( stdout );
	fflush(0);

	// write the data to the DBMS
	dset.store_all();
	return 0;
    } SEN_ACCESS_VIOLATION_HANDLER(); 
    return EXIT_FAILURE;
}
/*!
\mainpage sar_plot Sar Parse

	sar_parse parses a text sar file and adds the data to the database.

	This version is specific to HPUX, but would work with
	most versions of SAR with some minor moficiations (not Linux though).

	The core data-types are repeated in different formats, one for the
	parser, one for the merged in-mem data, one for the POST database ...

	These multiple repeat formats were necessary, and now a target for reduction.
	Some work is required to modularise the code for generality

<HR>
\section parsing Parsing
	sar_parser takes a comressed (HPUX) file and parses it,
	building a dset_tbl table of columns of attributes found within
	the time-span. If a disk attribute is found half way through,
	NULL values are added from time-zero.

	\li zfile_line - gets a line from a compressed text file
	\li floats_base - collects a dyn_array<float> for one line
	\li floats_sar - same as floats_base, nicer constructor
	\li sar_a - holds the attribues from the '-a' part of sar
	\li sar_parser_temps - holds lots of floats_sar and builds a dset_tbl
	\li sar_parser - parses the input and update dset_tbl
	\li dset_tbl - collects a dyn_array<floats_base> rows-cols


	Sar produces a fixed number of attributes (columns) plus some for
	each disk and cpu device it finds, so there is a dynamic list of
	devices. When a device is silent then active, its values are
	back-filled with zero values. If its silent all day, a disk wont
	appear in the output from sar, and there will be a gap in the
	years accumulated data.

\section store_mem_row Parse Row

	Writing your own monitor (or parser) is very simple.
	For each attribute, create a floats_base myattr,
	and initialise it with its own name, hostname, site_name
	dt and collection table. (see sar_a).

	Then put a floating -point value into its variable, as
	just like sar_parser :: sys_line_4()

	Do likewise for the other attributes, then call
	tbl->prep_next_row(), which has a list of all attributes
	registered with it.

	You must repeat for exactly 'dt' (min-grain 10 seconds),

\section store_mem Store - Memory Collection
	Before the data is moved to the dset, it is collected into the
	tbl. This is an unnecessary interface. but it was probably
	advanced planning, going up a similar creek.

	\li floats_base - a generic collection metric of a float
	\li floats_sar - floats_base specialised for sar
	\li dset_tbl - a set of columns, with [0..[N line_ids

	After collection, the main() functions transfer the segments to
	the line_store in-memory database, which probably loads more
	databse from the SQL database, adds the new, and writes it back.

	In future, there is an issue with generating a common line_id,
	probably a pointer to a thing.

	(( Yes the duplication is again repeated with POST, and then again with
	non-POST wrappers of POST ... )).

\section store_mem Store - Memory
	All the parsed data data is accumulated in a dset_tbl, with NO
	reference to to the SQL database, until save time when data is LOADED
	from the database, to merge segments.

	\li line_id_spec - (site,host,cmd,opt,dev,attr,dt,avg) then db_id
	\li line_data_seg - (line_id,t0,t2,float data[])
	\li line_data_seg_list - list of N segments in the line
	\li line_store - SPEC + SEGS
	\li line_store_tbl - Hash Table of line_store
	\li dset_tbl - line_store_tbl + sql_link

	The segments in the memory store are merged by line_store :: add_data()
	but it retains breaks between weeks (Sat-Midnight-Sun UTC). New data
	overwrites old data, and all data is aligned to the nearest dt.
	
	When the mem store is saved to the database, all similar data
	from that week is loaded. to merge in the new data (Thinking about
	it, old data overwrites new data!)

\section store_db Store - DataBase
	DB_sar is a virtual base class (interface) that hides the
	difference between MySQL, PostgreSQL, ..., and the actual
	DB-schema used.

	\li DB_sar - virtual base class
	\li DB_sar_MySQL - MySQL implementation
	\li DB_sar_PGSQL - PostgreSQL implementation
	\li DB_sar_post - POST implementation
	\li sql_link_mysql - generic MySQL code
	\li sql_link_postgres - generic code
	\li sql_link_pg_lo - Large Object (BLOB) code

	PostgreSQL is an excellent system, but its handling of BLOBS
	is driving me up the wall. MySQL is much more natural.
	The differences require different table-schema, and different SQL
	statements, which is OK.  The problems are:
	\li BLOBS cant be DynamicText because of a 7.5K imit
	\li An extra indirection-pivot is required to access BLOB fields
	\li PostgrSQL creates a zillion 8K files in /var/postgres

	The recent BLURB says that PostgreSQL can now have large binary
	'TEXT' fields, but I cant quite see how to switch it on.

\section store_db_post Store - DataBase - POST
	POST is not an SQL/RDBMS, it is MMAP and c_struct (C++).

	To implement a DB to store lines/segs, it was necessary to
	repeat all the code for the in-mem database, slightly differently.

	\li pdb_root_type -
	\li ArrayOfByte - POST BLOB
	\li pdb_nblk - BLOB of floats
	\li pdb_t0_t2_nblk -
	\li pdb_ary_t0_t2_nblk - 
	\li pdb_line_list -
	\li pdb_line_and_segs -

\section sar_linux SAR and Linux
	This project parses an HPUX SAR text file, a quick step from Solaris,
	and others. I run it on a Linux laptop, and tested on HPUX,
	but there is currently no monitoring of Linux.

	Linux SAR is completely different, its text format is nothing
	like UNIX-SAR, and I see no reason to write a parser for it.
	A MUCH better solution for Linux would be to integrate the
	data-collection tool (eg sysstat) with the library code.

	This will happen, but there is some development which also
	has to be done. You could do it NOW, simply strip out the
	sar_parser class, and replace it with live monitors.

	The data you collect will be valid later, because it is
	stored in a DESIGNED generic format. The code interface
	is likely to be fairly similar, but the internal working
	will change (ironically to make it simpler). This will
	also reflect the needs of data collectors such as SNMP.

	The use of POST is also triggering a re-write, and the
	DB_sar class needs tidying up (its topsy). There are other
	requirements, such as collecting SAR (on UNIX) interactively,
	and propagating the data to a central monitor (maybe without
	SQL), plus of course retaining collected data over LAN outages,
	plus of course reporting and plotting.

	A new key feature will be to handle irregularly spaced (Time,Value)
	samples, as well as regularly spaced time-series data. This will be
	needed for 'df-daily' and unfiltered 'swapinfo' sources.

	This release gives you a foundation, where you can collect your
	years data, using only a shell script, and the system sar binary.

	



\section weak_points Hardcoded Weak Points
	\li The first 'dt' is used throughout. If necessary, edit the SAR file.
	\li dt has a min grain multiple of 10 seconds (sar_parser_temps)
	\li any parse error causes an exit (loss of data if you dont fix it)
	\li the stderr log output is noisy and messy
	\li the 'tdb' text dump goes to 2 decimal places
	\li all ints are converted to floats (24 bit precision)
	\li I've presumed that this is finite parse and exit not a long session
	\li Splitting on week boundries is good, until you want 4 weeks
	\li times are aligned to multiples of dt
*/
