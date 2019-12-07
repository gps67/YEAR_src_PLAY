#include "test_SHAPE_BASE.h"
#include "test_SHAPE_BASE.h"

	// Layout( CSR ) // pass it for LINES COLS etc
	// it can expect most values to be in place, eg title

	bool SHAPE_BASE:: set_XYWH( XYWH_t _XYWH )
	{
		XYWH = _XYWH;
		return true;
	}

