#include "dset_api_segs_map.h"

#include "time_calc_zone.h"


dset_api_segs_map::
~dset_api_segs_map()
{
}

bool dset_api_segs_map::list_segs_to_dset(
	dset_api * dset,
	mem_line_spec * spec,
	time_t t0,
	time_t t2
) {
	if(!dset) return TRUE;
	int n = N();
	int i;
	for(i=0; i<n; i++ )
	{
		dset->listed_seg( spec, t0_t2[i].t0, t0_t2[i].t2 );
	}
	return TRUE;
} 

bool dset_api_segs_map::listed_seg(
	mem_line_spec * spec,
	time_t t0,
	time_t t2
) {
	// of course spec matches this one!!
	// ignore overlaps, but drop duplicates
	int n = N();
	int i;
	for(i=0; i<n; i++ )
	{
		if( t0_t2[i].t0 < t0 ) continue;
		if( t0_t2[i].t0 > t0 ) break;
		if( t0_t2[i].t2 > t2 ) return TRUE;
		    t0_t2[i].t2 = t2;
		return TRUE;
	}
	if(0) e_print("t0_t2( %ld, %ld )\n", t0, t2 );
	t0_t2.ins( i );
	t0_t2[i].t0 = t0;
	t0_t2[i].t2 = t2;
	return TRUE;
}

#include "tm_parts.h"
void dset_api_segs_map::merge_all()
{
	int i = 1;
	while( i < N() )
	{
		// presume sorted, allow overlap !!
		if( t0_t2[ i-1 ].t2 >= t0_t2[ i ].t0 )
		{
		 if(0) {
			tm_parts tm1;
			tm_parts tm2;
			tm_parts tm3;
			tm_parts tm4;
			tm1.UST_from_time( t0_t2[ i-1 ].t0 );
			tm2.UST_from_time( t0_t2[ i-1 ].t2 );
			tm3.UST_from_time( t0_t2[ i   ].t0 );
			tm4.UST_from_time( t0_t2[ i   ].t2 );
			e_print("seg -t0--- %s \n", tm1.str_fmt1() );
			e_print("seg --t2-- %s \n", tm2.str_fmt1() );
			e_print("seg --t0-- %s \n", tm3.str_fmt1() );
			e_print("seg ---t2- %s \n", tm4.str_fmt1() );
			e_print("\n" );
		 }
			t0_t2[ i-1 ].t2 = t0_t2[ i ].t2 ;
			t0_t2.del( i );
		} else
			i++;
	}
}

void dset_api_segs_map::split_eras(int era_width)
{
	obj_hold<dset_api_segs_map> dest;
	int n = N();
	for( int i=0; i<n; i++ )
	{
		time_t t0 = t0_t2[i].t0;
		time_t t2 = t0_t2[i].t2;
		while( t0 < t2 )
		{
			time_calc_zone era( t0_t2[i].t2, era_width );
			if( t2 < era.T2 ) {
				dest->listed_seg( NULL, t0, t2 );
				t0 = t2;
			} else {
				dest->listed_seg( NULL, t0, era.T2 );
				t0 = era.T2;
			}
		}
	}
	n = dest->N();
	t0_t2.rezero();
	t0_t2.get_space( n );
	t0_t2.extend_to( n );
	for( int i=0; i<n; i++ )
	{
		t0_t2[i].t0 = dest->t0_t2[i].t0;
		t0_t2[i].t2 = dest->t0_t2[i].t2;
	}
}

void dset_api_segs_map::split( time_t t1 )
{
	int n = N();
	for( int i=0; i<n; i++ )
	{
		if( t0_t2[i].t2 <= t1 ) continue;
		if( t0_t2[i].t0 >= t1 ) return;
		t0_t2.ins(i);
		t0_t2[i].t0 = t0_t2[i+1].t0;
		t0_t2[i].t2 = t1;
		t0_t2[i].t0 = t1;
	//	t0_t2[i].t2; // unchanged
		return;
	}
}
