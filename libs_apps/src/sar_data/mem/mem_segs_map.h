#ifndef mem_segs_map_H
#define mem_segs_map_H

#include "dset_api.h"
#include "dyn_array.h"

struct type_t0_t2 : public GRP_sar_data
{
	time_t t0;
	time_t t2;
};

class mem_segs_map : public dset_api
{
 public:
	dyn_ary<type_t0_t2> t0_t2;
	
};

#endif
