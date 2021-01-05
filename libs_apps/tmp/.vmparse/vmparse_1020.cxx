
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

// #include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <time.h>
// #include <errno.h>

#include "misc.h"
//#include "array_base_1248.h"

#include "vm18flt_tbl.h"
#include "tm_parts.h"
#include "zfile_line.h"		// gz gets line reader
#include "line1.h"		// header info
#include "vmparse_1020.h"

char ** vmparse_1020::colnames() {

	static char * colnames_ary[] = {
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

	static char * coldescs_ary[] = {
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
int vmparse_1020::scan_line_1020( vm18flt_row & v, char * line )
{
	char * ptn_i;
	char * ptn_s;

	//      "  0  1  2  3  4  5  6  7  8  9  -  1  2  3  4  5  6  7";
	ptn_i = " %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i";
	ptn_s = " %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s";

/*

         procs           memory                   page                              faults       cpu
    r     b     w      avm    free   re   at    pi   po    fr   de    sr     in     sy    cs  us sy id
    2     2     0    24895   37870    1    0     0    0     0    0     0    173   1347   145   8  2 90
    3     1     0    24895   37638    0    0     0    0     0    0     0    238   1588   295  93  7  0
	
*/

		if (18 == sscanf( line, ptn_i, 
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
		) ) {
			if(0) {
				for( int c=0; c<N18; c++ ) {
					fprintf(stderr," v[%d]==%d\n", c, v[c] );
				}
				fprintf(stderr,"\n" );
			}
			return 1;
		} else return 0;
}

int vmparse_1020::matches_header1( char * str )
{
	char * ptn;
	int t;
	char sp;
	ptn = " procs memory page faults%c cpu ";
	t = sscanf( str, ptn, &sp );
	return t==1;
} 

int vmparse_1020::matches_header2( char * str )
{
	char * ptn;
	int t;
	char sp;
	ptn = " r b w avm free re at pi po fr de sr in sy cs us sy%c id ";
	t = sscanf( str, ptn, &sp );
	return t==1;
} 

void vmparse_1020::z_parse(
	line1 &line1,
	zfile_line &in,
	vm18flt_tbl & tbl )
{

	char * line;

	tbl.reserve( line1.nn ); // 1440 usually
	while( line = in.getline() ) {
		// fgets( line, 1024, file ) ) 
		// each line is either data or header
		// line store held by in.bufferr
		if (scan_line_1020( tbl.row, line ) ) {
			tbl.add_row();
		} else {
			if( matches_header1( line )) continue;
			if( matches_header2( line )) continue;
			fprintf(stderr,"ERROR %s\n", "----------------------------" );
			fprintf(stderr,"ERROR %s", line );
		}
	}
	tbl.update_nbytes_used(); // no need to
	in.close(); // ??
}
