#ifndef ints_byte_order_H
#define ints_byte_order_H

 class WORD; // imports from CORE level // GAIN hand_written type_case

 class u32_WORD; // ALIAS u32_hilo data;
 class u32_abcd; // ALIAS u32_hilo data;
 class WORD_abcd; // ALIAS u32_hilo data;
 class u32_hilo; // ALIAS u32_hilo data;

 /*!
	Network_Byte_Order
		Supposedly, all data on disk is hilo 
	
	Module WORD
		knows about compiler MACROS for SWAPB
	
	WORD {
		u32_hilo DATA;
	}
	
	SHEET {
	 EXPR WORD { hilo }
	 EXPR WORD { liho }
	 EXPR WORD { cdHL }
	 EXPR WORD { LHcd }
	 EXPR WORD { lohi }
	 }

	TYPE FLAG {
		HILO
		ABCD
		dcba
		LOHI
	}

	ON LOHI {
		u32_cpu
		 value =
		  lohi_FROM_hilo(
		    u32_hilo data_in_file
		  )
		u32_hilo data_in_file = = lohi_FROM_hilo( u32_cpu value )
	}

	ON HILO {
	// the other include_ON.h
	}

	SWAPB IN SITU {
		u32_cpu lohi = INPUT;
		SWAPB lohi RENAME hilo {
			// soon end scope INPUT hilo
			// soon start scope OUTPUT lohi
		}
		u32_hilo OUTPUT = hilo 
	}

	SWAPB IN SITU {
		u32_cpu hilo = INPUT;
		SWAPB hilo RENAME lohi {
			// soon end scope INPUT lohi
			// ASM SWAP_BYTES_in_REGISTER( LOW8 )
			// ASM SWAP_BYTES_in_MEMORY( via REGISTER )
			// soon start scope OUTPUT hilo
		}
		u32_lohi OUTPUT = lohi 
	}

 */
	WORD {
		u32_lohi DATA;
	}
 */
 struct u32_hilo {
  public:
  	/* INTERFACE
		LHS = RHS.get()
		ITEM = LHS
		bool OK = LHS.set( VAL ) // u32_lohi // export LOCAL to MODULE
		LHS = (u32) get(); // __GETTER__ DATA_DIRECT & LINK;
  		void set( u32 RHS ); // RHS acting as both LOCN and LEAF
	*/
	/* USAGE TOKEN USAGE SHEET
		u32_lohi // export LOCAL to MODULE
		bool OK = LHS.set( VAL ) // u32_lohi // export LOCAL to MODULE
	*/

  	u32 get();
	 /* ALIASES */
		u32 get();
		u32 get_cpu();
		u32 get_as_cpu();
	 /* PICK
	 	u32 hiLo_ from_WORD_abcd;	// probably best place for SWAPB
	 */

  	void set(u32 val); // DIALECT // RETURN bool OK;
	 /* ALIASES */
  		void set(u32 val);
  		void set(u32_cpu val);
  		void set_cpu(u32 val);
  		void set_cpu(u32_cpu val);

  	void set_from_cpu(u32 val);
  	void set_from_cpu(u32 val);

 class u32_hilo; // ALIAS u32_hilo hilo;


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
	typedef long long i64;
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
struct GRP_lib_base2_js  : public GRP_lib_base2 {};
struct GRP_lib_base2_gtk : public GRP_lib_base2 {};

struct GRP_lib_inet {};
struct GRP_lib_media {};
struct GRP_lib_play {};
struct GRP_sar_data {};
struct GRP_sar_parse {};
struct GRP_sar_plot {};

#endif

