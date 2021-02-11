
// #pragma interface
/*
	I use too many inline functions
	they cause multiple definitions for each includer of .h
	so I put proagma interface into each .h
	and then pick a file where the inlines will be outlined
	and include a list of pragme inplementation - as follows
*/

#pragma implementation "cset_bit_map.h"
#pragma implementation "cset_byte_map.h"
#pragma implementation "dgb_fluff.h"
#pragma implementation "dgb_print.h"
#pragma implementation "dgb_print_v9.h" // Y
#pragma implementation "perr.h"
#pragma implementation "scan_nl.h"
#pragma implementation "own_str.h"

#include "dgb_fluff.h"
#include "dgb_print.h"
#include "dgb_print_v9.h"
#include "perr.h"
#include "cset_bit_map.h"
#include "cset_byte_map.h"
#include "scan_nl.h"
#include "own_str.h"

int some_var;
