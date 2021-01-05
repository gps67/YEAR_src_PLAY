#include "dset_api.h"

#include <string.h> // memcpy
#include "tm_parts.h"
#include "my_hash.h"
 
dset_api_print::dset_api_print(
	FILE * _file,
	int _ncols
) {
	f = _file;
	ncols = _ncols;
}

bool dset_api_print::merge_floats(
	mem_line_spec * spec,
	time_t t0,
	int n,
	float * flts
) {
	if( !flts) {
		fprintf(stderr, "dset_api_print::print_obj_stat() NULL\n");
		return FALSE;
	}
	time_t t2 = t0 + n * spec->dt;
	tm_parts tm;
	// tm.local_from_time( t0 );
	tm.UST_from_time( t0 );
	fprintf( f, "{\n" );
	fprintf( f, "   type obj_stat_vnn\n");
	fprintf( f, "   site %s\n", spec->site.str );
	fprintf( f, "   host %s\n", spec->host.str );
	fprintf( f, "    cmd %s\n", spec->cmd.str );
	fprintf( f, "    opt %s\n", spec->opt.str );
	fprintf( f, "    dev %s\n", spec->dev.str );
	fprintf( f, "   attr %s\n", spec->attr.str );
	fprintf( f, "     dt %d\n", spec->dt );
	fprintf( f, "    avg %s\n", spec->avg.str );
	fprintf( f, "  time0 {%s}\n", tm.str_fmt1() );
	tm.UST_from_time( t2 );
	fprintf( f, "  time2 {%s}\n", tm.str_fmt1() );
	fprintf( f, "     t0 %ld\n", (long)t0 );
	fprintf( f, "     t2 %ld\n", (long)t2 );
	fprintf( f, "   data {\n\t");

	int cl = ncols;
	if(0)
	{
		for( int r = 0; r < n; r++ ) {
			if( cl-- < 1 ) {
				printf("\n\t");
				cl = ncols -1;
			}
			printf(" %6.2f", flts[r] );
		}
	} else { // SKIP WHEN DEBUGGING
		float avg = 0.0;
		for( int r = 0; r < n; r++ ) {
			avg += flts[r];
		}
		if(n) avg = avg/n;
		printf(" (( %d values average %6.2f ))\n\t", n, avg);
		for( int r = 0; r < n; r++ ) {
			if( cl-- < 1 ) {
				break;
				cl = ncols -1;
			}
			printf(" %6.2f", flts[r] );
		}
	}
	fprintf( f, "\n");
	fprintf( f, "   }\n");
	fprintf( f, "}\n");

	return TRUE;
}
