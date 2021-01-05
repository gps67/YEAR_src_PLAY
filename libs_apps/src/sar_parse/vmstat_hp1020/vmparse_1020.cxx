#include <stdio.h>
#include <string.h>

#include "misc.h"

#include "tm_parts.h"
#include "zfile_line_in.h"		// gz gets line reader
#include "vmstat_wrap_line1.h"		// header info
#include "vmparse_1020.h"
#include "dset_api.h"

#define N18 18

/*!
	constructor - create all the column attributes - fixed N18
*/
vmparse_1020::vmparse_1020(
	const char * site_name, 
	const char * cmd_name,
	const char * hostname,
	time_t t0,
	int dt,
	int nrows
)
: tbl( site_name, cmd_name )
{
	/*
		its OK to do this before creating the cols,
	*/
	fprintf(stderr,"hostname='%s'\n", hostname );
	tbl.set_host_in_all_cols( hostname );
	tbl.set_t0_in_all_cols( t0 );
	tbl.set_dt_in_all_cols( dt );

	// not a VTL func
	const char ** names = colnames();

	// create the store for the data
	for( int c=0; c<N18; c++ )
	{
		const char * opt = "-";
		const char * dev = "-";
		const char * attr = names[c];
		tbl.create_col( opt, dev, attr );
		/*
			The attr should be split and also the descr
			should be originated from here (create_col)
		*/
	}
	tbl.get_space( nrows ); // 1440 usually
}

/*!
	destructor - delete tbl
*/
vmparse_1020::~vmparse_1020()
{
	// tbl gets default delete, which removes each col
}

/*!
	an array of attr names - expand later to use descr
*/
const char ** vmparse_1020::colnames() {

	static const char * colnames_ary[] = {
		"procs.r",	
		"procs.b",
		"procs.w",
		"mem.avm",
		"mem.free",
		"page.re",
		"page.at",
		"page.pi",
		"page.po",
		"page.fr",
		"page.de",
		"page.sr",
		"faults.in",
		"faults.sy",
		"faults.cs",
		"cpu.us",
		"cpu.sy",
		"cpu.id",
		NULL
	};

	static const char * coldescs_ary[] = {
	"procs.r",	"Procs in read queue",
	"procs.b",	"Procs blocked WIO",
	"procs.w",	"Procs waiting for system to not be overswapped",
	"mem.avm",	"Page active virtual memory",
	"mem.free",	"Page freelist",
	"page.re",	"Page reclaims",
	"page.at",	"Page Addr Trans faults",
	"page.pi",	"Pages mapped in",
	"page.po",	"pages mapped out",
	"page.fr",	"pages freed (per sec)",
	"page.de",	"Anticipated short term memory shortfall",
	"page.sr",	"Pages scanned by clock algorithm, per second",
	"faults.in",	"interrupts (devices)",
	"faults.sy",	"system calls",
	"faults.cs",	"context switches",
	"cpu.us",	"user",
	"cpu.sy",	"sys (+hi-pri)",
	"cpu.id",	"idle",
		NULL
	};

	return colnames_ary;
}

/*!
	parse a single data line (or return false), add to tbl
*/
bool vmparse_1020::scan_line_1020( char * line )
{
	const char * ptn_i;
	const char * ptn_s;
	int v[N18];

	//      "  0  1  2  3  4  5  6  7  8  9  -  1  2  3  4  5  6  7";
	ptn_i = " %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i";
	ptn_s = " %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s";

/*
         procs           memory                   page                              faults       cpu
    r     b     w      avm    free   re   at    pi   po    fr   de    sr     in     sy    cs  us sy id
    2     2     0    24895   37870    1    0     0    0     0    0     0    173   1347   145   8  2 90
    3     1     0    24895   37638    0    0     0    0     0    0     0    238   1588   295  93  7  0
*/

		if (N18 != sscanf( line, ptn_i, 
			&v[0],
			&v[1],
			&v[2],
			&v[3],
			&v[4],
			&v[5],
			&v[6],
			&v[7],
			&v[8],
			&v[9],
			&v[10],
			&v[11],
			&v[12],
			&v[13],
			&v[14],
			&v[15],
			&v[16],
			&v[17]
		) )
			return false;
		if( !tbl.prep_next_row() ) return false;
		for( int c=0; c<N18; c++ )
		{
			* tbl.data[c]->ptr() = v[c];
		}
		if(0) {
			fprintf(stderr," line = %s\n", line );
			for( int c=0; c<N18; c++ ) {
				fprintf(stderr," v[%d]==%d\n", c, v[c] );
			}
			fprintf(stderr,"\n" );
		}
		return true;
}

/*!
	accuratly match a noise line
*/
bool vmparse_1020::matches_header1( char * str )
{
	const char * ptn;
	int t;
	char sp;
	ptn = " procs memory page faults%c cpu ";
	t = sscanf( str, ptn, &sp );
	return t==1;
} 

/*!
	accuratly match a noise line
*/
bool vmparse_1020::matches_header2( char * str )
{
	const char * ptn;
	int t;
	char sp;
	ptn = " r b w avm free re at pi po fr de sr in sy cs us sy%c id ";
	t = sscanf( str, ptn, &sp );
	return t==1;
} 

/*!
	iterate over every line of file
*/
bool vmparse_1020::z_parse( zfile_line_in &in )
{
	bool ok = true;
	char * line;

	while( (line = in.getline()) )
	{
		if (scan_line_1020( line ) ) {
			// OK
		} else {
			if( matches_header1( line )) continue;
			if( matches_header2( line )) continue;
			ok = false;
			fprintf(stderr,"ERROR %s\n", "----------------------------" );
			fprintf(stderr,"ERROR %s", line );
		}
	}
	return ok;
}


// STATIC
/*!
	parse the zfilename, EXPECTING it to comply, send to dset_api
*/
bool vmparse_1020::zfile_to_dset(                         
	const char * site_name,                                         
	const char * cmd_name,          // vmstat
	const char * zfilename,                                         
	dset_api * dset                                                 
)
{
	if( !cmd_name )
		cmd_name = "vmstat";

	// create the file access, auto close on return
	zfile_line_in in;
	if( !in.open( zfilename ) )
	{
		perror( zfilename );
		return false;
	}

	// read in the first line
	char * line;
	if( !(line = in.getline()) )
	{
		perror( zfilename );
		return false;
	}

	// extract the added info from the first line
	vmstat_wrap_line1 line1;
	if( !line1.parse_line1( line ) )
	{
		fprintf(stderr, "parse_line1 returned false # %s # \n", line );
		return false;
	}
								
	// create a parser, setup its spec
	vmparse_1020 parser(
		site_name,
		cmd_name,
		line1.hostname,
		line1.t0,
		line1.dt,
		line1.nn
	);
								
	// run the parser over the file, into its tbl_collect
	if(! parser.z_parse( in ) )
	{
		
		fprintf(stderr, "z_parse returned false # %s # \n", in.repeat_get_line() );
		return FALSE;
	}
								
	// send the data downstream
	parser.tbl.send_to_dset( dset );
								
	// delete the parser and the tbl_collect (on exit)
								
	return TRUE;
}

