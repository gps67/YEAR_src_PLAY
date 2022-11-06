MK_ext_lib/README.txt

	# TODO LGX=LGX of { LGX WIN32 ARM HANDSET } 
	# TODO E6320=$(shell uname -n)

	LGX=LGX of { LGX WIN32 ARM HANDSET } 
	box_name=$(shell uname -n)

        # seems ~ does not work in Makefiles # $(HOME)
        DIR_libs_apps=$(HOME)/YEAR/src/libs_apps
        DIR_libs_apps_build=$(DIR_libs_apps)/build_$(LGX)_$(box_name)

        INCLS += -I$(DIR_libs_apps)/include

	# vector to specific version # VFS lib => $(EXPR ARGV) # EXPR
        LINKS += -Wl,--rpath=$(DIR_libs_apps_build)/lib

	# everything is output in the . directory .o .so .exec
	# so runtime looking here gets latest

        LINKS += -Wl,--rpath=$(DIR_libs_apps_build)/
        LINKS += -L$(DIR_libs_apps_build)
        LINKS += -lbase

2022-11-04 Nov Fri MK_ext_lib

	README.txt	HELLO
	Makefile_eg*	the OLD style 

	 style A

	 	-I $incl_dir 
        	LINKS += "-Wl,--rpath=$(DIR_libs_apps_build)/lib"
		Makefile_eg_1*	the OLD style 

	 style B

	 	LIST_CPLUS_INCLUDE_PATH += $incl_dir // add to LIST //
        	LINKS += "-Wl,--rpath=$(DIR_libs_apps_build)/lib"
		Makefile_eg_2*	the NEW style 

			details here 

				reasons for change
				details
				suggest STYLES += " style C " // SUBLEX
				suggest STYLES += " ARGS " // ARGV_SPEC // API
				suggest STYLES += " ARGV " // ARGV_SPEC // API
				suggest STYLES += " API " // ARGV // API
				suggest STYLES += " OOP " // OBJV // API
				suggest STYLES += " CXX " // OBJV // API // VIEW

			OFF_TOPIC

				MODULE SCRIPT
				DATA_DECL SCRIPT
				SUBLEX_MATCH
				LOCAL_DIALECT // trust operator // as_used //
				VIEW_SCRIPT 
				 // the creation of this %s_%s NODE 
				 NODE = VIEW SCRIPT %s_%s GEN_SUBLEX "%s_%s"
				 PSG += " %s_%s " // INTENTIONAL_EXTRA_SPACE ->
				 PSG += " %s " // INTENTIONAL_EXTRA_SPACE
				 MACRO { "VIEW" "SCRIPT" "VIEW_SCRIPT" ... }
				 // ... // grafts a MOUNT POINT with OPTS + VARS
				 // ... // SUBLEX_new_obj_id // LEX_API // obj_id

			u8 obj_idx
			u8 obj_pool

				OBJ_t & STO.GET_OBJ_pool_idx( pool, idx ) {
					OBJ_t & OBJ = POOL[pool].ITEM[idx] ;//

AUTO_SPLICE_TEMPLATE_WRITER

	" ;// " PLACEHOLDERS for ANY_SYNTAX that BENCH adds to VALS VARS EXPRS
	NEAR " ; "
	NEAR " // "
	NEAR " // CMNT " ## MATCH ## CACHE_MATCH ## PARSE_MATCH ## FUNC VARS
	NEAR " %s " ## MATCH ## CACHE_MATCH ## PARSE_MATCH ## FUNC VARS
	NEAR " %s " ## MATCH ## ANYSTR ## BOUNDS_CHECKING_OR_NOT 
	NEAR "%s" ## MATCH ## " %s " ## SPELLING uses INTENTIONAL_SPACE

				// SCHEME function returns THIS or NEAR IDX EXPR
					return OBJ;
				}
	
	 style C

	 	STEP == ARGV // CMNT // CODE_POINT // SESS_STEP //

		OBJ_t & ARGV_GET %d // CODE_POINT // TELLS // 

		STR0 ARG = ARGV[ %d ] // UPTO ARGC // VARIETY_of_

		EA_EXPR_STR0 _t // const char * STR0 = ANYSTR //

		ANYSTR VAL = EXPR ARGS 
		ANYSTR VAL = EXPR ARG0
		ANYSTR VAL = EXPR ARG1 
		ANYSTR VAL = EXPR ARG2 
		ANYSTR VAL = EXPR ARG3 # REASSIGNED TO "EXPR_ARGV" # lose -1
		ANYSTR VAL = EXPR ARGV # OBJ_t * OBJ = GETER_FUNC( i64 idx )
	
	/*
		i64 idx is a massive over RANGE beyond u8_idx
		u8_idx is overreach for u5_idx // POSS from FAST_LOOKUP[byte]
		u8 LOOKUP( u8 u5_idx ) // u5_from_BYTE_TABLE
		u8 LOOKUP( u8 u5_idx ) // u5_from_u5_BYTE_TABLE // u5_mask //
		u8 CACHED( u5 ) // u5 &= b_0001_1111 // 0x1F // FIVE_BIT //
		WORD_64 TABLE[32]; // EIGHT_t seen as WORD_PAIR u32_u32 in u64
		WORD_64 can DUAL_MODE and GEN SOURCE 

			GEN SOURCE

			GEN SOURCE

		PLUS_SPEC_CODE_POINT can add ANYTHING to CT 
		EA_SYSTEM can add CT to RT
		EA_SYSTEM can add RT to CT

	*/

			OVERLAP // not OVERREACH help yourself //

/* OUTDENT */	u2	/* AS_WAS */ [0123[4  

		u2	[0123[4 
		u2	[012[3 

				u2 b_00 = 0; // ARGC NONE
				u2 b_01 = 1; // ARGC ONE	
				u2 b_10 = 2; // ARGC SECOND
				u2 b_10 = 2; // ARGC ARGV

			if u2 == "ARGC_ARGV" // learn DIALECT CACHE

				u2 b_00 == 0; ARGC NONE // no added data
				u2 b_01 == 1; ARGC ONE	// one ITEM as ARG
				u2 b_10 == 2; ARGC SECOND // two ITEMS data
				u2 b_10 == 2; ARGC ARGV // 

		u3	[0123 4567 [ 8 
		u3	[0123 456[7 8_ ## EIGHT_and_on 8_ 
			RANGE LIMITED by -1 for FLAG_N

		u3	[0123 45[67 8_ ## u48_u16

			if u64 == "ARGC_ARGV" // %s %x // ... 

		u3	[0123 45[67 8_ ## u48_u16

			u48 = get_N() { return b_0100; // GET [4] }
			u48 = get_N() { return b_1111_1111; // N = -1 }
			u48 = get_N() { return b_1111_1111; // STR = EXPR_as_src}
			u48 = get_N() { u48 = WORD_64 >> 16; /* CMNT */ } 

				range of BASE classes
				u16_u48 // ALIAS u48_u8_u8 // loaded via LOHI
				u48_u16 // BITFIELD looks like BITS in WORD
				u48_u16 // BITFIELD saved as LOHI are FAST
				u24_u8 // BITFIELD when WORD_32 // range sub u32
	VALID			u16_u16_u32 // LOHI PARSER // picture of MEM
				u32_u16_u16 // BITFIELD in IN CPU REGISTER

		u8 u16 u32 u64
		u3 u5 u2 BITFIELDS within OPCODE

		u64	[[N 


	 style A

	 	-I $incl_dir 
        	LINKS += "-Wl,--rpath=$(DIR_libs_apps_build)/lib"

		DIR_NAME LOCN_t LOCN = "libs_apps_build"; // CTOR FUNC INIT(STR0)
		EXPR dir_name { EXPR ARGS ENV CTXT CODE_POINT ...

			SUBLEX += EXPR_STEP

				eg

			DIR_NAME == "%s_%s" lhs rhs
			DIR_NAME == "%s_%s_%s" lhs rhs ALSO
			DIR_NAME == "..." // PERMIT <newname> EXPR
			EXPR == "..." // PASTE CODE TEXT HERE // SEGMENT 


		enum MK_name_ARCH_token_pool {

		  OS_ARCH
		  	x86
			aarch64
			Darwin // ifeq ($(OS_ARCH),Darwin)
			linux-gnu

		 MOZ_PKG_PLATFORM := linux-$(TARGET_CPU)

		 	TARGET_CPU

				OS_ARCH == "win32" 
				OS_ARCH == "linux-%s " 

		  OS_ARCH
			WINNT 
			win32
			win64


		}

	 style B


	_MK_LIBS_APPS
	MK_LISTS_
	MK_SUFFIXES
	MK_with_libs_apps
	MK_with_libs_apps_@
