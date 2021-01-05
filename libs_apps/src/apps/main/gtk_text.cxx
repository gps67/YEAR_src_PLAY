//#include "amnt_t.h"
//#include "date_glib.h"
//#include "dset.h"

#include "w_widget0.h"
//#include "w_file_sel.h"
#include "gtk_text_top_app.h"

int main(int argc, char ** argv )
{
	char * filename = NULL;
	if( argc == 2 ) {
		filename = argv[1];
	}
	{
		TOPAPP_Holder_gtk holder(&argc, &argv);
//		gtk_init(&argc, &argv);
		new gtk_text_top_app( & holder, filename );
		gtk_main();
	}
	ref_tally.print_leaked_report();
	return 0;
}

// DOXYGEN Main Page

/*!

*/
