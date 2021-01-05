#ifndef sar_parser_floats_H
#define sar_parser_floats_H

#include <misc.h>
#include "sar_parser_parts.h"

//! Holds all the tbl_col_floats...
struct sar_parser_floats : public GRP_sar_parse
{

	tbl_collect * tbl;	// needed to add dynamic cpus + disks

// SYS
	sar_a	a;
	sar_b	b;
	sar_c	c;
	sar_m	m;
	sar_v	v;
	sar_w	w;
	sar_y	y;

	sar_u	* u_system; // sum of both
	sar_q	* q_system; // sum of both

// cpu - dunamic number
// two opt groups (-u -q) but BOTH are presumed to be input

	dyn_array<sar_u *> u_ary;
	dyn_array<sar_q *> q_ary;

// disk - dynamic number

	dyn_array<sar_d *> d_ary;

	// tbl_collect outlives sar_parser_floats (thanks to _wrap)
	// that allows use of tbl_collect as a return value

	~sar_parser_floats(); // deletes REFERENCES to all columns data

	sar_parser_floats( tbl_collect * _tbl ) 
	:
		tbl( _tbl ),

		a( _tbl ),
		b( _tbl ),
		c( _tbl ),
		m( _tbl ),
		v( _tbl ),
		w( _tbl ),
		y( _tbl ),

		u_system( NULL ),
		q_system( NULL ),

//		u( _tbl ),	// Dynamic number 
//		q( _tbl ),	// Dynamic number 
//		d( _tbl ),	// Dynamic number 

		u_ary(),
		q_ary(),
		d_ary() 
	{
		tbl = _tbl;
	}

	sar_u *  lookup_cpu_u( int i );
	sar_q *  lookup_cpu_q( int i );
	sar_d *  lookup_disk( char * dev );

};

#endif
