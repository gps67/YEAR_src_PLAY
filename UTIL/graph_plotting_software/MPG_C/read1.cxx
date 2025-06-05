#define FILE_IN "mpg.txt"

#define BUFF_SIZE 1024

#include <stdio.h>
#include <errno.h>
#include <cstring>

// typedef int bool;


struct line_data { // CSV_SCHEMA CSV_LINE_DATA P0P2_t P0P2

	// DB: MPG_C // database of petrol used milage when maybe where
	// 2025-04-13, 28369,  23.08, 30.90, Y Full Poole
	// YEAR_MM_DD COMMA_in_CSV - milage %s litres_E10 %f pounds %F<F3>


	int year;
	int mm;
	int dd;

	// Tue Feb // lib_base/time/tm_parts{ .cxx .h }
	//

	float miles;
	float litres;
	float cost;
	char note[ BUFF_SIZE ];

	void clear() {
		year = 0;
		mm = 0;
		dd = 0;
		miles = 0.0;
		litres = 0.0;
		cost = 0.0;
		note[ 0 ] = 0 ;
	}

	void show2() {
		char datestr[16];
		sprintf(datestr, "%4.4d-%2.2d-%2.2d", year, mm, dd ); 
		printf("# SHOW2" );
		printf(" %s,", datestr );
		printf(" %6.0f,", miles );
		printf(" %5.2f,", litres );
		printf(" %5.2f,", cost );
		printf(" %s", note );
		printf("\n");
	}


	bool line_in_line( char * buff ) {
		// FMT starts here
		// create a TREE of SPEC // needs to provide API_LISTS
		// PICK individual TYPES 
		// and add SUBTYPE // *
		// and add_NAME "%s" for AVAR( ARGV1 ) // ambiguous argv[1] 
		// NUMERIC _doube _float _fixed_32_USAGE __USAGE int str
		// NUMERIC preferred range [0 [30
		// NUMERIC preferred range [0 [30  [30K 
		// NUMERIC IDX aligned to ARRAY[N] of _1_2_4_8_BYTES
		// ARRAY [ N ] of WORD_TYPE_1_2_4_8_t // OMIT _t // EMIT _t
		// WHY = IDX_grow_to_u15 _or_N _N_check_DONE
		// we MUST "{ check: 0 <= idx < N }"
		// using ":" to split TEXT into TWO at ":" // CALC_P0P2 
		// STEP_CSR_to_nect_LEX_or_GAP //
		// GAP 
		// SET SESS.CSR_one.XPOS.P0 = .CSR_one.XPOS.P0 
		// CXX compiler will find something for ABSENT.feature_field_EXPR
		// _ // "_" overwrites "." // AVOID_REPEATS ABSORB // MERGE
		// _ // "_" overwrites "." // AVOID_ABSENT ABSENT // PROVIDE
		// PROVIDE STO for a Modules AVAR for A_CONST
		// the compiler has already scanned all the STR0 exprs
		// output includes VAR_POOL several FSM machines with FSM_VARS
		// VARS should be allocated from Module_VARS_ROM_CONSTS
		// COMPILER may infer whatever it likes from it's own GEN
		// turbo compilers SET_OPTION_WITHIN_COMMENT // parse comments
		// AVAR NUMERIC double const readonly
		// AVAL NUMERIC double const readonly // AVAL // ERA_EDGE on change
		// ERA_EDGE on AVAL_CHANGED // ie recompile test distribute now
		// ERA_SCOPE_VAR_POOL_one // usually and _two // default AUTO //
		// DEFAULT AUTO USEFUL // edit SETTINGS add_OPTION_DONT USEFUL
		// that might be a sublevel of useful, nested zones
		// Module sees the world as a CXX SCRIPT or ARGV or ARGV_SCRIPT
		// that means YOUR APP or LANG behaves like ARGV_SCRIPT_THREE



		// EXPR
		// PREDICT KNOW will be able NEXT_P0P2.P0 = HERE.P2
		// GEN a specific CSV parser for SCANF
		// this one works for MPG_CSV
		// as would sqlite, reload from SOURCE, tcc style
		// keeps PROJECT_TASK_BACKUP as diff on SQL_LOAD_DUMP
		// FIELD TYPE NAME idx_in_ARGV
		// YEAR MM DD
		// float decimal hex etc with preferred widths eg "0x%04d" // NEEDS LEX EDGE eg SP //
		// convert each RET_VAL into an i64_ITEM // waste but easy if simple POINTER to N_BYTES of SESS_HEAP_idx_OBJ_is_NAMED_DOUBLE_with_some_ATTRS
		/*
		#ifdef CT_RT_PHASE_xFF
		ITEM_t_HERE ITEM : TRAY_ITEM_BASE {
			//
			// PICK // GEN code then does it's stuff
			// TRAY_one_ITEM_BASE 
			// TRAY_two_ITEM_BASE 
			// difference is how items are allocated
			// eg ROM will never delete
			// eg MALLOC needs FREE
			// eg MMAP_ALLOC_system_two // for ITEM into HEAP within SESS_AVAR
			// 
			// SCRIPT

			nested_sub_class
			struct MEM_OBJ_CACHE : MEM_OBJ_BASE_TYPE { // moderately expensive 
			// lots of shared code and instant upgrade

				// i64_WORD
				// i8_i8_OPCODE
				// i16_HEAP_ITEM i4_HEAP i12_ITEM
				// i8_i24
s
			}

			LIST of FIELD 
			 NAME
			 NAME_as_idx // if not IDX in FIELDS
			 TYPE

		} ;
		#endif
		*/
		clear();
		static const char * fmt = "%d-%d-%d, %f, %f, %f,%n";
		int pos = 0;
		int t = sscanf( buff, fmt, 
			& year, & mm, & dd,
			& miles,
			& litres,
			& cost,
			& pos //  note
		);
		if(t<6) {
			show2();
		}
		char * eos = buff + strlen( buff );
		eos--;
		while((eos >= buff) && ((*eos == '\n') || (*eos == '\r'))) {
			*eos = 0;
			eos--;
		}
		// snprintf( note, BUFF_SIZE, "'%d/%d %s'", t, pos, buff+pos );
		snprintf( note, BUFF_SIZE, "%s", buff+pos );
		return true;
	}

	bool row_step( line_data & next ) {
		float d_litres = next.litres;
		float d_miles = next.miles - miles;
		float d_km = d_miles * 1.60934;
		if(d_miles < 1 ) {
			printf(" # SKIPPING # d_miles < 1 # \n");
			return false;
		}
		char datestr[16];
		sprintf(datestr, "%4.4d-%2.2d-%2.2d", next.year,next.mm,  next.dd ); 
		float p_per_mile = 100.0 * next.cost / d_miles; 
		float gallons = d_litres / 4.54609; // litres in a gallon
		float pounds_per_gallon = next.cost / gallons ;
		float pounds_per_litre = next.cost / d_litres ;
		float mpg = d_miles / gallons;
		float d_km_100 = (d_km/100.0); // per 100 km
		float l_100 = d_litres / d_km_100; // litres per 100 km
		printf(" %s", datestr );
		printf(" %6.0f %3.0f %6.0f", miles, d_miles, next.miles );
		printf(" %5.1f p_mile", p_per_mile );
		printf(" %5.2f £_litre", pounds_per_litre );
		printf(" %5.2f £_gallon", pounds_per_gallon );
		printf(" %5.1f mpg", mpg );
		printf(" %5.1f lp100", l_100 );
		printf(" %s", next.note );
		printf("\n");
		return true;
	}

	void printer() {
		printf("%4.4d-%2.2d-%2.2d", year, mm, dd );
		printf("\n");
	}

};

// 2015-09-11, 91034, 31.11, 33.51, To Jim



int main( int argc, char ** argv )
{
	const char * filename = FILE_IN;
	if(argc>1) filename = argv[1];
	if(0==strcmp(filename,"-")) filename = NULL;
	FILE * fin = stdin;
	if( filename ) {
		fin = fopen( filename, "r" );
		printf(" # FILENAME # %s #", filename );
	}
	if(!fin) {
		perror( FILE_IN );
		return errno;
	}

	char buff[ BUFF_SIZE ];
	line_data row_prev;
	line_data row;
	int line_no = 0;
	while( char * s = fgets( buff, BUFF_SIZE, fin ) ) {
		// printf("## %s", buff );
		line_no ++;
		if( line_no == 1 ) continue ; // headers

		if( buff[0] == '#' ) {
			printf(" # SKIPPING # COMMENT ## %s", buff );
			printf("##\n" );
			continue ; 
		}

		if( !row.line_in_line( buff )) {
			printf("## BLANK LINE ?? ##\n");
			continue;
		}

		if( line_no == 2 ) {
			row_prev = row;
			printf(" # SKIPPING # FIRST DATA LINE ## %s\n", buff );
			// \n not doubled here not in buff
			continue ; // cant calc
		}

		// row.printer();
		if(!row_prev.row_step( row )) continue;
		row_prev = row;
	}


}
// --EOT--
#if 0
// man 3 printf OUTPUT_CHANNEL FMT_STR 
// STR -vs- STR0 // KNOW can use STR0 as implementation of STR
// AVAR_STR_one SPEC STR_one AVAR_one
// ARGV each mention creates AVAR at that CODE_POINT XPOS is also TPOS
// TPOS is an EA of TREE_TWIG_or_LEAF // TWIG holds XPOS // LEVEL_UP XPOS //
// XPOS becomes an imported AVAR from LIBR into SESS ...
// TREE // NOT SECURE but UDEF IDX and STR_P0P2 STR_STR0 
// AVAR = STR // VALUE == STR // == SAME_EA or SAME_EA_EXPR MATCH_ALIAS_EA
// AUTO PTR * & // use a precise match of CXX syntax // small list of tricks
// pragma tell GCC to see this as a MACRO NAME ARGS GET_RET_VAL_from_EXPR
// EXPR // PSG_TREE SPEC INST DECL XPOS // EXPR_TREE // EXPR_STRUCT // PSG_ABC
// PSG_LEX // We re-apply PSG within LEX as PARSE_LEX // FSM FSM_POS XPOS
// PSG_LEC LEX in PSG // I'd prefer LEX is always a1_a2_star_UTF8 a1 a2
// a1 // ASCII A-Z a-z     _ // APPLY _%s then it is all a2 // a2_plus a2
// a2 // ASCII A-Z a-z 0-9 _ // PLUS is STAR but at least one 
// STAR permits ZERO // build FSM for this as DIAG
// glyph && is_IDENT_a2 // WEIGHT and HINT from TEXT found
// AUTO apply DIALECT // click to USWD //

// FMT
// %*X width hex_val // FFF_width == 3
// MAGIC calling operator==(lhs,rhs) permits the 
// MAGIC calling operator==(/* lhs==this , */ rhs ) const  permits break const

	LHS == THIS
	RHS == THAT
	OP_CODE == "operator==" 
	PARSE "{ LHS == RHS }"

	CT_EXPR 
	RT_EXPR 

	CT_KNOW
	RT_KNOW

	LHS == CT
	RHS == RT

	XPOS is P0P2
	XPOS is ARGV  GAP LHS GAP == GAP RHS GAP // GAP_ZERO_is_OKAY // PUNCT //
	XPOS is PARSER_CSR_one = lookup (one)
	CT_RT batch writes PSG_LEX from DECL and USED

	PSG LHS == RHS
	LHS_t LHS
	RHS_t RHS
	SPELLING == // original 
	SPELLING operator== // made_into_C_IDENT special_case_C++_IDENT
	HEAP_ITEM operator== // Probably AVAR acting as A_CONST_VAL AVAL

	CMP( LHS, RHS ) // OPTS

		is_SAME // PUS is same EA // 
	//	is_DIFF // not is_SAME
		is_LESS
		is_MORE
	
	EA_EXPR from decode_i64

		write a bunch of BITFIELD MACROS
		using UNIQUE_SPELLING as EXACT_MATCH_STR
		eg N_BYTES read_write_api /* ROM */ 
	
	<- NESTED CODE IN DATA ->

		use the locally sourced LIBR .o ELF_CODE.o.LIb.LIBR.o
		ELF.o // VFS_already presents N_BYTES ELF_SEGMENT as AFILE.o
		SPEC_t SPEC += SPEC_EXPR // decode i64 i8_i8 i16_i32(PICK_SPEC()))
		SPEC_t SPEC += SPEC_EXPR // decode i64 i8_i8 i16_i32(AVAR)
		SPEC_t SPEC += SPEC_EXPR // decode i64 i8_i8 i16_i32(LOCN)
		SPEC_t SPEC += SPEC_EXPR // decode i64 i8_i8 i16_i32(ITEM)
		SPEC_t SPEC += SPEC_EXPR // decode i64 i8_i8 i16_i32(EXPR)
		SPEC_t SPEC += LIBR_LIBR // SPEC_LIBR // ASIF_DATA // WHEN_ACTN

		SPEC_t AVAR_NAME

			A STRONG THEME is %s_%s LEX_JOIN LEX_SPLIT LEX_EDGE
			MATCH %s_%s_%s // etc // INTERLEAVE // LABEL NAME DATA //

	ANY_STR
	ANYSTR

		lookup(ANYSTR) // avoid lex split, since KNOW own DECL HERE

			RET_VAL = lookup(anystr)

		ANYSTR is N_BYTES P2 == P0 + nbytes // glue point cost gain

			N_BYTE
			N_BYTES
			N_bytes
			n_bytes
			nbytes

			BYTES
			PTR_to_BYTES
			EA_EXPR += "{ BASE P0 }"
			EA_EXPR += "{ BASE P0 SPEC_t SPEC  }"
			EA_EXPR += "{ BASE P0 SPEC_t SPEC  } ..."
			EA_EXPR += "{ DIALECT  _two }" // NULL %s LHS // _two //
			EA_EXPR += "{ XPOS_t  CODE_POINT }"
			EA_EXPR += "{ BASE P0 }"

		XPOS + nbytes + BYTES_VALUES

			CODE_POINT
				SCRIPT
				 LINE
			HEAP_ITEM
				EA
				EA_EXPR
			LOCN_ITEM
				DIAG // GEN FSM to DECODE i64
			EA_as_NAME
			NAME_for_EA

			NAME_as_ANYSTR
			ANYSTR_as_NAME

		DATA is N_BYTES 

			DECODE SEGMENT_DATA as FILE_MIME_TYPE
			ELF_Layout
			EXE_ASM
			PAGES_of_SAMPLE_VALUE_POOL
			MIME_TYPE_png
			MIME_TYPE_pdf
			MIME_TYPE_cxx

		EA_as_ZONE_HEAP_ITEM_EXPR_ACTION 

			LANG TAGS adds extra grip to DIALECT
			CSV LAYOUT // MUX CSV_TABLES // single data_set //
			COMPLEX OBJECT LAYOUT based on lookup(%s) TABLES
			unlimited FIELD_VARS as long as all unique
			or MATCH_EXACT
			or MATCH_ALIES
			or MATCH_CMP_RET_VAL enum  is_LESS is_SAME is_MORE
			or MATCH_CMP_RET_VAL enum 
			-PICK->
			is_LESS
			is_SAME
			is_MORE
			is_M1
			is_SIGN_FFFF
			is_DECODE_i64
				i8_i8
					BYTE_A
					BYTE_B

					PAIR_AB_BA // in LOW BITS of CPU_WORD
					QUAD_i32 // half half , high half
					EIGHT_i64 // _LOHI hightest is lowest

						because comes first A B of 8
						because DRAW ABCD EFGH
						because DRAW DCBA HGFE
						because DRAW HGFE DCBA


#endif

