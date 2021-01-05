#include "sar_parser_temps.h"

#include <stdlib.h>
#include <string.h>

sar_parser_floats::~sar_parser_floats()
{
	// each floats belongs to tbl_collect - detatch - dont delete !
	// No problem - tbl_col_floats_wrap - does nothing ;-)
	// delete each dyn_array of sar_u sar_q sar_d

	int n = d_ary.N();
	for( int d=0; d<n; d++ )
	{
		sar_d * disk = d_ary[d];
		d_ary[d] = NULL;
		delete disk;
	}
	n = u_ary.N();
	for( int u=0; u<n; u++ )
	{
		sar_q * c_q = q_ary[u];
		sar_u * c_u = u_ary[u];
		q_ary[u] = NULL;
		u_ary[u] = NULL;
		delete c_q;
		delete c_u;
	}
	// the rest just collapses
}

// create new col_group at position i
sar_u *  sar_parser_floats::lookup_cpu_u( int i )
{
	if( true ) // create cpu_all FIRST
	{
		if( !u_system) 
		{
			const char * buff = "cpu_all";
			u_system = new sar_u( tbl, buff );
			q_system = new sar_q( tbl, buff );
		}
	}
	if( i == -1 )
	{
		return u_system;
	}

	// text gives parser the CPU number
	if( i > 10 ) {
		fprintf(stderr, "soft limit of 10 CPU's\n" );
		assert( i <= 10 );
	}
	int n = u_ary.N();
	if( i >= n ) {
		u_ary.extend_to_fill( i+1, NULL );
		q_ary.extend_to_fill( i+1, NULL );
	}
	for( int j = 0; j<=i; j++ ) {
		if( !u_ary[ j ] ) {
			// cpu0 cpu1 ... named here
			char buff[10];
			sprintf( buff, "cpu%d", i );
			u_ary[i] = new sar_u( tbl, buff );
			q_ary[i] = new sar_q( tbl, buff );
		}
	}
	return u_ary[i];
}

sar_q *  sar_parser_floats::lookup_cpu_q( int i )
{
	lookup_cpu_u( i );
	if( i == -1 ) return q_system;
	return q_ary[i];
}

sar_d *  sar_parser_floats::lookup_disk( char * dev )
{
	int n = d_ary.N();
	for( int d=0; d<n; d++ )
	{
		// use busy as sample
		if( d_ary[d]->busy.spec().dev.is_same_as( dev )) {
			return d_ary[d];
		}

		assert( d < 1000 ); // SOFT runaway limit
	}
	d_ary.extend_to_fill( n+1, NULL );
	d_ary[n] = new sar_d( tbl, dev );
	return d_ary[n];
}

