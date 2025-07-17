//#include "amnt_t.h"
//#include "date_glib.h"
//#include "dset.h"

//	bench_MAIN
//	bench_MAIN _SESS // see bench_top_app // TODO
//	bench_MAIN_SESS // CURR_XPOS_in_SCRIPT CURR_STACK_FRAMES CURR_SCOPE
//
//	TRICK SESS is what holder holds
//	SESS is what holder WILL hold when it arrives
//	TOP_APP_holder_gtk CACHE_SESS_holder
//
		TOPAPP_Holder_gtk holder(&argc, &argv);

		new bench_top_app( &holder, filename );
		holder.call_gx_main_and_exit();
//
//
//	SESS is a global variable // with some builtin FIELDS SPEC
//	THREAD is a global variable //
//	SESS is an outer variable // BIND and then act BATON_RELAY
//	THREAD is an outer variable // CT_RT reserved C_VAR "%s" as SCOPE
//
//	CPU_REGISTER_20 // above 16 // 20 - 30 // CPU_REGISTER_99
//	16 reserver registers of 32 actual registers i5
//	8 reserver registers of 16 or 256 i4_PLUS u3_idx GROW_99 128 256 32K
//	8 reserver registers of 16 or 256 i4_MINUS M1 M2 M3 M4 M5 M6 M7 M8
//
//	OPTION can handle u8_idx knowing CPU does that
//	OPTION can handle u16_idx knowing CPU does that
//	OPTION can handle u32_idx knowing CPU does that
//	OPTION can handle u64_idx knowing CPU pretends to do that
//	OPTION NOT handle u64_idx u63_is_the_LIMIT
//	
//
//	P0 is BYTE ALIGNED
//	P0 not WORD ALIGNED
//	eg create ARGV by sraering with CSET_PREPPED PARSED as ARGV
//	eg create ARGV by replacing SP1 with NUL and tracking P0P2
//	ie ARGV_WORD _is ANY_TEXT with NUL added at P2[i] // outside P0P2
//	CHEEKY uses NEXT items space to store that NUL byte [P0 BYTE_A ... [P2
//	OVERLAY prepare INIT_VAL from KEPT_COPY then STEP_LIST
//	STEPS += STEP += BYTE_A BYTE_B // but not WORD ALIGNED
//	STEPS += STEP += BYTE_ABCD _EFGH _EIGHT_t STEP_i32_i32
//	WORD_one is unique and drifts to CPU_32 _handling_PAIR_i32_i32
//	iterate though replacing every SP1 with ASCII_NUL BYTE and tracking P0P2
//	XPOS is a small world with own coins and counts and lexicons
//	SCRIPT is in language ARGV_SCRIPT // UDEF "if" // HEAP LIST_STRUCT lookup
//	OPTIMISER first STEP is LOAD INIT_VAL from nbytes in ROM at XPOS
//
//	eg create ARGV by writing out each STR0



#include "w_widget0.h"
//#include "w_file_sel.h"
#include "bench_top_app.h" // SESS is here and CPU_AVAR
#include "TOPAPP_Holder_gtk.h"
#include <locale.h>


#if 0
EIGHT_t SESS; // SESS is in a CPU_WORD that EVAL turns into a PTR in a CPU_regi
SESS_t SESS; // SESS is in a GROUP that shares a CPU_register EA_STRUCT

DECODE_SESS() { i8_i8 _OPCODE ... }
#endif

// using namespace APPS;

//! tv_rem
int main(int argc, char ** argv )
{
	// holder does this // setlocale(LC_ALL,"");

	// bench FILENAME // edit or run filename
	// it is the main SCRIPT
	char * filename = NULL; // More complex ARGV sets filename init NULL
	if( argc == 2 ) {
		filename = argv[1];
	}

	// _SESS holder parses ARGV // filename remains NULL // ARGV_holder 
	{
		// generic parse and replace // rewrites ARGV // ALLOC_HERE
		// bench_top_app // parses holder.ARGV // rewrites ARGV
		// MODEL_2 // instead of rewrite use PAIR of ARGV _one _two
		// ARGV_PARSER // has own ALLOC subsystem // fast_exit //
		// Most GENERIC holder // UDEF done by bench //
		TOPAPP_Holder_gtk holder(&argc, &argv);

		// UDEF_TASK_TOOL("bench") // LOCN += ADIR_BENCH_SESS_DATA //
		// bench_REC // += DIARY // += LOGS DONE // 
		// bench_DVD //
		// bench_WEB_TV //
		// bench IDE libs_apps_PEER // 
		// bench JACK // AUDIO JACK // MEDIA AUDIO // VFS ROM LIBR //
		// bench ...
//		new BENCH::bench_top_app( &holder, filename );
		// bench_top_app // the logic of the APPLET and BENCH SESS
		// get rid of filename // run ARGV as SCRIPT // SCRIPT $FILE
		// "${DIALECT}" // Q2 retains single word-ness even is_TREE
		// new throws away the retval // or make bool
		// holder holds bench_top_app // which is the TYPE_NAME
		new bench_top_app( &holder, filename );
		// SELF = CTOR( SELF, ARGS_TAIL ) // this //
		// ARGS_TAIL has access to SESS OK // $1 is holding AVAR SELF 
		holder.call_gx_main_and_exit();
	}
	ref_tally.print_leaked_report();
	return 0;
}

// DOXYGEN Main Page

/*!

*/
