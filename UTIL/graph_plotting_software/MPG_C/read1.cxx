#define FILE_IN "mpg.txt"

#define BUFF_SIZE 1024

#include <stdio.h>
#include <errno.h>
#include <cstring>

// typedef int bool;



struct line_data {
	int year;
	int mm;
	int dd;
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
