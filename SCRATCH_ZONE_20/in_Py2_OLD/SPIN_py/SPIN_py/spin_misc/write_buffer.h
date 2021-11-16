#ifndef SPIN_WRITE_BUFFER_H
#define SPIN_WRITE_BUFFER_H

#include "app_types.h"

#include "buffer/misc.h"
#include "buffer/blk1.h"
#include "buffer/buffer1.h"

#include "P_utils.h"
 

struct SPIN_write_buffer : PY_Object_base
{
	P_ref_base down_stream;
	buffer1 buff;		/* buffer collects it */
	int lo;		/* reset to this initial size */
	int hi;		/* force flush when growing beyond this */

	SPIN_write_buffer()
	: PY_Object_base( &SPIN_write_buffer_Type )
	, down_stream()
	{
		lo = 1000;
		hi = 10000;
	}

	~SPIN_write_buffer()
	{
	}

};

/* EXPORTED */

extern SPIN_write_buffer * as_SPIN_write_buffer( PyObject * elem );

SPIN_write_buffer * new_SPIN_write_buffer(
	PyObject *downstream,
	int lo,
	int hi
);

#endif
