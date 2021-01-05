#ifndef SAR_PARTS_H
#define SAR_PARTS_H
/*
         Mq { cpu runq-sz %runocc swpq-sz %swpocc }
         Mu { cpu %usr %sys %wio %idle }
         a { iget/s namei/s dirbk/s }
         b { bread/s lread/s %rcache bwrit/s lwrit/s %wcache pread/s pwrit/s }
         c { scall/s sread/s swrit/s fork/s exec/s rchar/s wchar/s }
         d { device %busy avque r+w/s blks/s avwait avserv }
---      m { msg/s sema/s }
+++      m { msg/s sema/s select/s }
         q { runq-sz %runocc swpq-sz %swpocc }
         v { text-sz ov proc-sz ov inod-sz ov file-sz ov }
         w { swpin/s bswin/s swpot/s bswot/s pswch/s }
         y { rawch/s canch/s outch/s rcvin/s xmtin/s mdmin/s }
 
*/

#include <misc.h>
#include "tbl_collect.h" 
#include "tbl_col_floats_sar.h" // floats

typedef tbl_col_floats_sar floats;	// temp kludge

struct sar_u : public GRP_sar_parse
{
	floats usr, sys, wio, idle;

	sar_u( tbl_collect * tbl, const char * dev ) 
	:
		usr(  tbl, "u", dev, "usr" ),
		sys(  tbl, "u", dev, "sys" ),
		wio(  tbl, "u", dev, "wio" ),
		idle( tbl, "u", dev, "idle" )
	{
	}
};

struct sar_q : public GRP_sar_parse
{
	floats runqsz, runocc;	
	floats swpqsz, swpocc;		// two missing things	

	sar_q( tbl_collect * tbl, const char * dev ) 
	:
		runqsz( tbl, "q", dev, "runqsz" ),
		runocc( tbl, "q", dev, "runocc" ),
		swpqsz( tbl, "q", dev, "swpqsz" ),
		swpocc( tbl, "q", dev, "swpocc" )
	{
	}
};

struct sar_d : public GRP_sar_parse
{
	floats busy, avque, rw, blk, avwait, avserv;

	sar_d( tbl_collect * tbl, const char * dev ) 
	:
		busy  ( tbl, "d", dev, "busy" ),
		avque ( tbl, "d", dev, "avque" ),
		rw    ( tbl, "d", dev, "rw" ),
		blk   ( tbl, "d", dev, "blk" ),
		avwait( tbl, "d", dev, "avwait" ),
		avserv( tbl, "d", dev, "avserv" )
	{
	}
};

struct sar_b : public GRP_sar_parse
{
	floats bread, lread, rcache, bwrit, lwrit, wcache, pread, pwrit;

	sar_b( tbl_collect * tbl ) 
	:
		bread( tbl, "b", "bread" ),
		lread( tbl, "b", "lread" ),
		rcache( tbl, "b", "rcache" ),
		bwrit( tbl, "b", "bwrit" ),
		lwrit( tbl, "b", "lwrit" ),
		wcache( tbl, "b", "wcache" ),
		pread( tbl, "b", "pread" ),
		pwrit( tbl, "b", "pwrit" )
	{
	}
};

struct sar_w : public GRP_sar_parse
{
	floats swpin, bswin, swpot, bswot, pswch;

	sar_w( tbl_collect * tbl ) 
	:
		swpin( tbl, "w", "swpin" ),
		bswin( tbl, "w", "bswin" ),
		swpot( tbl, "w", "swpot" ),
		bswot( tbl, "w", "bswot" ),
		pswch( tbl, "w", "pswch" )
	{
	}
};

struct sar_c : public GRP_sar_parse
{
	floats scall, sread, swrit, fork, exec, rchar, wchar;

	sar_c( tbl_collect * tbl ) 
	:
		scall( tbl, "c", "scall" ),
		sread( tbl, "c", "sread" ),
		swrit( tbl, "c", "swrit" ),
		fork(  tbl, "c", "fork" ),
		exec(  tbl, "c", "exec" ),
		rchar( tbl, "c", "rchar" ),
		wchar( tbl, "c", "wchar" )
	{
	}
};

struct sar_a : public GRP_sar_parse
{
	floats iget, namei, dirbk;

	sar_a( tbl_collect * tbl ) 
	:
		iget ( tbl, "a", "iget" ),
		namei( tbl, "a", "namei" ),
		dirbk( tbl, "a", "dirbk" )
	{
	}
};

struct sar_y : public GRP_sar_parse
{
	floats rawch, canch, outch, rcvin, xmtin, mdmin;

	sar_y( tbl_collect * tbl ) 
	:
		rawch( tbl, "y", "rawch" ),
		canch( tbl, "y", "canch" ),
		outch( tbl, "y", "outch" ),
		rcvin( tbl, "y", "rcvin" ),
		xmtin( tbl, "y", "xmtin" ),
		mdmin( tbl, "y", "mdmin" )
	{
	}
};

struct sar_v : public GRP_sar_parse
{
	floats
		text, text_sz, text_ov,
		proc, proc_sz, proc_ov,
		inod, inod_sz, inod_ov,
		file, file_sz, file_ov;

	sar_v( tbl_collect * tbl ) 
	:
		text   ( tbl, "v", "text" ),
		text_sz( tbl, "v", "text_sz" ),
		text_ov( tbl, "v", "text_ov" ),
		proc   ( tbl, "v", "proc" ),
		proc_sz( tbl, "v", "proc_sz" ),
		proc_ov( tbl, "v", "proc_ov" ),
		inod   ( tbl, "v", "inod" ),
		inod_sz( tbl, "v", "inod_sz" ),
		inod_ov( tbl, "v", "inod_ov" ),
		file   ( tbl, "v", "file" ),
		file_sz( tbl, "v", "file_sz" ),
		file_ov( tbl, "v", "file_ov" )
	{
	}
};

struct sar_m : public GRP_sar_parse
{
	floats msg, sema, select;

	sar_m( tbl_collect * tbl )
	:
		msg(    tbl, "m", "msg" ),
		sema(   tbl, "m", "sema" ),
		select( tbl, "m", "select" )
	{
	}
};

#endif
