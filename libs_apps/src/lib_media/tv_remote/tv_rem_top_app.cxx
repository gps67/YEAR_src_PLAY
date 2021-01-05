#include "tv_rem_top_app.h"
#include "w_menu1.h"
#include "c_tree_tbl_two.h"
#include "w_vpaned.h"
#include "w_table.h"
#include "w_entry.h"
#include "w_dialog_msg.h"
#include "w_btn_plus.h"
#include "tv_panel.h"
using namespace TV_REM;
using namespace APPS;

/*
///////////////////////////////////////////////////////////////
void C_menu_help_about( GtkWidget * abtn, tv_rem_top_app * app )
{

	w_dialog::msg_failed(
		app->main_window,
		"Free Software",
		"\n"
		"tv_rem\n"
		"\n"
		"This is Free Software,\n"
		"Licensed under the GPL and LGPL\n"
		"http://www.information-cascade.co.uk\n"
		"http://freshmeat.net\n"
		"http://www.gnu.org\n"
	);
}
///////////////////////////////////////////////////////////////
*/

/*!
	destructor - 
*/
tv_rem_top_app::~tv_rem_top_app()
{
	e_print("~tv_rem_top_app();\n");
	main_window = NULL;
	e_print("~tv_rem_top_app() DONE;\n");
}

/*
///////////// use default
w_menu_bar * tv_rem_top_app::make_menu_bar()
/////////////
*/


/*!
	constructor - assemble the active window
*/
tv_rem_top_app::tv_rem_top_app(
	TOPAPP_Holder_gtk * holder,
	const char * first_file
)
: TOPAPP_Task_Window_Major( holder, "tv_rem", 770, 700, first_file, false )    
{                                  

	/*
		should really have a callback that does:
		app_area = mk_main_panel(); // filename set
	*/
	tvpanel = new tv_panel();
	app_area->pack_big( tvpanel );
	// app_area->pack( tvpanel );
	main_window->show();

	// Start program with the About BOX
	if(0)  C_menu_help_about( NULL, this );
}
