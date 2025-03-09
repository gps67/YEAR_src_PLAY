//#include "amnt_t.h"
//#include "date_glib.h"
//#include "dset.h"

#include "w_widget0.h"
//#include "w_file_sel.h"
#include "bench_top_app.h"
#include "TOPAPP_Holder_gtk.h"
#include <locale.h>

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
		holder.call_gx_main_and_exit();
	}
	ref_tally.print_leaked_report();
	return 0;
}

// DOXYGEN Main Page

/*!

*/
