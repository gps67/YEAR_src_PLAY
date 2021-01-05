//#include "amnt_t.h"
//#include "date_glib.h"
//#include "dset.h"

//#include "w_widget0.h"
//#include "w_file_sel.h"
//#include "tv_rem_top_app.h"

//#include "TOPAPP_Holder_gtk.h"

#include "w_widget0.h"
#include "gtk_text_top_app.h"
#include <locale.h>

#include "obj_ref.h"
#include "HT_main.h"

int main(int argc, char ** argv )
{
	// holder does this // setlocale(LC_ALL,"");
	char * filename = NULL;
	if( argc == 2 ) {
		filename = argv[1];
	}
	{
	//	TOPAPP_Holder_gtk holder(&argc, &argv);
	//	new gtk_text_top_app( & holder, filename );
	//	e_print_show();
		HT_main();
	//	holder.call_gx_main_and_exit();
	}
	ref_tally.print_leaked_report();
	return 0;
}

