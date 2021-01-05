#ifndef dset_api_segs_map_H
#define dset_api_segs_map_H

#include "dset_api.h"
#include "dyn_array.h"

struct type_t0_t2 : public GRP_sar_data
{
	time_t t0;
	time_t t2;
};

/*
	This is map of the segs available for a SINGLE line.
	When used as a dset_api, the LINE is presumed to be KNOWN
*/
class dset_api_segs_map : public dset_api
{
 public:
	~dset_api_segs_map(); 
	dyn_array<type_t0_t2> t0_t2;
	int N()
	{
		return t0_t2.N();
	}
	void clear()
	{
		t0_t2.rezero();
	}
	time_t lowest()
	{
		if( !N() ) return 0;
		return t0_t2[0].t0;
	}
	time_t highest()
	{
		int n = N();
		if( !n ) return 0;
		return t0_t2[n-1].t2;
	}
	void split( time_t t1 );

	dset_api_segs_map()
	: t0_t2()
	{
	}

/*
	bool merge_spec( mem_line_spec * spec )
	{
		if(!spec) return FALSE;
		return TRUE;
	}
*/

  virtual
        bool list_segs_to_dset(
                dset_api * dset,
                mem_line_spec * spec,
                time_t t0,
                time_t t2
        );
        bool listed_seg(
                mem_line_spec * spec,
                time_t t0,
                time_t t2
        );

	//! join adjacent segments
	void merge_all();
	void split_eras( int era_width );
	//! 
	void split_weeks() {
		split_eras( 60 * 60 * 24 * 7 );
	}

};

#endif
