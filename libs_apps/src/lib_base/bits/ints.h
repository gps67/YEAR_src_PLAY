#ifndef ints_H
#define ints_H	// all these are direct C word types and friends

#include "misc.h"	// bool
#include "is_diff.h"	// is_same, is_more, is_less
#include "GRP_base_class.h" // base classes ONLY defined here

#ifdef WIN32
	// conflict with MINGW somewhere
	// typedef int int32;
#else
	typedef int int32; // CXX standard says 32 bits is plenty for u24_idx
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

	typedef unsigned int uint; // ALIAS u32 bit
	typedef unsigned int uns; // u32 u32 even on u64

/*
	64 bit will vary more than others an need some clever #include
	ie most COMPILERS like u64_WORD  _PTR fits in WORD _IDX_fits_in_u24

	after all those MACROS, u64 is WORD_DIRECT "CXX long long"

		CXX_LANG will PARSE out "long long" to builtin "u64_cpu"

			CXX_LANG is a NEAR COG (center_of_gravity)
			
			  PARSE is detected by SUBLEX { Activates SCRIPT ARGS }


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

#if 0
	TOKEN SLIGHT_of_HAND

		these disappear

			u64_lohi -> "unsigned long long"
	
	ON HILO use _CACHED_SWAPB

		C COMPILER tracks VALUES_USED ... etc ...
		ASM inline reduces it to single ASM over single register
		POSSIBLE NEED to extend u8_to_u64_setting_extra_bytes
		POSSIBLE NEED to extend u8_to_i64_setting_extra_bytes
		POSSIBLE NEED to extend u8_to_u64_keeping_extra_bytes
		POSSIBLE NEED to extend u8_to_i64_keeping_extra_bytes


		Also u48_PAYLOAD always need a shift, and a SWAPB API

			u64_hilo_t hilo; 

			typedef u64_WORD_hilo u64_WORD; // _t //
			typedef u64_WORD_hilo_t u64_WORD_t; // _t //

	 converts to RHS 
	  FILTER -is- all_MACROS_dropped _to_VALUE_u64_cpu
	   u64_cpu == "u64_cpu_lohi" // ALIAS "u64_lohi" // ALIAS DIALECT // %s //
	   // %s // u64 == "int" // PICK CSR // { %s is INT } //
	   // { %s is INT } // MATCH PARSE SCRIPT WORD VALUE EA API_EA
	   // API_EA // { void * same_as_caddr_t } // { DETECT_SCRIPT } //
	   // API_CALL // array idx of N of ITEM_TYPE 

	   MACRO goes TRANSPARENT // transparent_MACRO //

	   	use the WELL_KNOWN uns int u8 u64 u256 nbytes_SEGMENT
	
	MACRO EVAL STEP

	   u64_t == "u64_lohi" // DIALECT is generalising via PICK(PICK)

	   u64_CT
	   u64_RT 

	   SWAPB alternates ON_OFF_PICK COMPILER generates enacted inline calls

	   # OPTION # GEN CALL ARGS # GEN_PSG_API # ARGV # ARGS # OBJV # 

	   COMPILER will MATCH
	   int
	   long
	   short
	   char byte
	   u16_lohi
	   u16_hilo
	   u16 -> u16_lohi // after all those macros at CT "u8_u8" "_t"

	   u48 -> "u64_WORD >> 16" // TOP_FILL_ZERO // TOP_FILL_SIGN //
	   u64 -> "u64_WORD_in_CPU" // PICK %s of ALIAS ITEM EXPR
	   u64 -> WORD_%s LOHI _HILO _CPU

	RT_DATA 

		COMPILER expands all the MACROS 
		by running VALUES through MACHINE through RUN_ALL_EXPR
		// GEN_ALL // += GEN_LOHI 

	CT_CODE

		prerun SCRIPT over DATA
		CT collects all GEN(what_if) // SPEC += COLLECTABLE
		CT can pick token bindings from SYMBOL or KNOWN_VALUE("SAMPLE")
		CT has full benefit of API_CLASS_FUNCS_BYTES_NBYTES_SEGMENT

		SWAPB_DOES_SWAP via GEN_TECHNIQUE CACHED_GETTER
		SWAPB_DOES_ASIS // _SWAPB ASIS //
		int_t -> "u64_WORD" -> "unsigned long"

		This code is run as ASIF_WORD direct no SWAPB used
		That might be a STEP within a SCRIPT_of_STEPS

		After all SWAPB is done, whats left is DIRECT VALUE u64_WORD

			u64_WORD_lohi .. CXX defines it from _cpu == _lohi
			u64_WORD_lohi .. CXX defines it from _cpu == _hilo {
				SWITCH OPTION API_is_WORD_VALUE // not VAR
				SWITCH not nbytes API_GETTER // _with_CACHE
				SWITCH API_GETTER_in_register // _is_ _in_
				OPTION DECODE API GETTER WORD_DIRECT // TREE steps
			}

	ie MACROS do calls to GETTER MACROS  50_50 GET_DIRECT GET_API_MODULE.*

		NULL pointer calls DIRECT or GEN_DIRECT

		GEN DIRECT can still be stepped down to _CACHED_COPY
		(most func calls do that ATOM_is_ATOM _in_VAR)







	TOKEN SLIGHT_of_HAND converts to RHS 
	GEN u64_cpu -> u64_lohi // DETECTED as HAPPENING in lohi_X86_u64_lohi
	GEN u64_cpu -> u64_lohi // DETECTED as SIMULATED into lohi_X86_u64_hilo
	GEN u64_cpu -> u64_lohi // DETECTED as CODED into lohi_X86_u64_hilo
	GEN u64_cpu -> u64_lohi // DETECTED as WORD u64_cpu // WITH // u64_lohi
	// 
#endif

// everyone uses ints.h
// to check, the base classes are defined via here

#endif
