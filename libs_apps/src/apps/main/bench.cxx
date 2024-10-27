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
	char * filename = NULL;
	if( argc == 2 ) {
		filename = argv[1];
	}
	{
		TOPAPP_Holder_gtk holder(&argc, &argv);
//		new BENCH::bench_top_app( &holder, filename );
		new bench_top_app( &holder, filename );
		holder.call_gx_main_and_exit();
	}
	ref_tally.print_leaked_report();
	return 0;
}

// DOXYGEN Main Page

/*!

*/
