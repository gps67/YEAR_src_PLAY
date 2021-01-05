#ifndef ints_H
#define ints_H


#include "misc.h"	// bool
#include "is_diff.h"	// is_same, is_more, is_less

#ifdef WIN32
	// conflict with MINGW somewhere
	// typedef int int32;
#else
	typedef int int32;
#endif


typedef int delme;

/*
	TODO: rewrite these properly

	hmmm now I want u8_t u8; // use name of type as a VARNAME
	this works when u3 u5 are varnames u24 etc
*/

	typedef unsigned   char u8; // used by imported code ! but other?
	typedef            char i8;
	typedef          short i16;
	typedef unsigned short u16;
	typedef            int i32;
	typedef unsigned   int u32;

	typedef unsigned int uint;
	typedef unsigned int uns;

/*
	64 bit will vary more than others an need some clever #include
*/
	typedef          long long i64;
	typedef unsigned long long u64;


/*
	common macro to find the number of items in a declared array
*/
#define N_ARY(ary) (sizeof(ary)/sizeof(ary[0]))

/*
	flt80 is an 80 bit i386 float

	I now realise that float and double are supported by the FPU
	(not just libc moving bits around), but flt80 might
	be left in FPU registers.

	BUG: avoid two inline parameters with g++ though !!
*/
	typedef long double flt80;

/*
	Most things include this file, so this is where I define
	the empty structs that different libraries use as base classes
	to indicate which group they belong to (DOXYGEN)
*/

struct GRP_apps {};
struct GRP_lib_accts {};

struct GRP_lib_base {};
struct GRP_lib_base_fd   : public GRP_lib_base {};
struct GRP_lib_base1_glib : public GRP_lib_base {};

struct GRP_lib_base2 {};
struct GRP_lib_base2_x11 : public GRP_lib_base2 {};
struct GRP_lib_base2_pdf : public GRP_lib_base2 {};
// maybe move this to SPOUT_GEN namespace
struct GRP_lib_base2_spout : public GRP_lib_base2 {};
struct GRP_lib_base2_js  : public GRP_lib_base2 {};
struct GRP_lib_base2_gtk : public GRP_lib_base2 {};

struct GRP_lib_inet {};
struct GRP_lib_media {};
struct GRP_lib_play {};
struct GRP_sar_data {};
struct GRP_sar_parse {};
struct GRP_sar_plot {};

#endif
