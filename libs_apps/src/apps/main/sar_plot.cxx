//#include "amnt_t.h"
//#include "date_glib.h"
//#include "dset.h"

#include "w_widget0.h"
//#include "file_sel.h"
#include "sar_top_app.h"

// BOTH needed, but double include causs {} bug
// #include "post_file.h" // post_exe_name
#include "post_object.h" // SEN_TRY macros

//#include "TOPAPP_Holder.h"

int main(int argc, char ** argv )
{
    SEN_TRY { 
	// see post_file.cxx - to invoke gdb on the correct file
	str0 prog_name
	// NS_POST::post_exe_name 
	= argv[0];

	{
		APPS::TOPAPP_Holder_gtk holder(&argc, &argv);

		char * filename = (char *) "/tmp/dpb_sar_test";
		if( argc == 2 ) {
			filename = argv[1];
		}

		// debugging show me now
		e_print_show();

		new sar_top_app( &holder, filename );
		
		holder.call_gx_main_and_exit();
	}
	ref_tally.print_leaked_report();
	return 0;
    } SEN_ACCESS_VIOLATION_HANDLER(); 
    return 0;
}

// DOXYGEN Main Page

/*!

*/
