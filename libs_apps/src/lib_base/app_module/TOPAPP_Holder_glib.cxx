#include "TOPAPP_Holder_glib.h"
#include "TOPAPP_Task.h"

#include <glib.h>
#include "e_print.h"
#include "dgb.h"

#include "dgb_fail.h" 
#include "post_object.h" // SEN_TRY macros
#include <locale.h>
#include <unistd.h>
#include "misc.h"

using namespace APPS;

/*!
	constructor - remove GTK words from argv, gtk_init, prep_env, etc
*/
TOPAPP_Holder_glib::TOPAPP_Holder_glib(
	int * p_argc,
	char *** p_argv,
	bool is_non_gui
)
: TOPAPP_Holder() // which does not exist -- yet
{
	gdb_sigaction( ** p_argv ); // sets progname_argv0 and ...
	main_app = NULL;
	gtk_closed = false;
	g_not_gtk = false; // default to gui
	loop = NULL;
	setlocale(LC_ALL,"");

	g_not_gtk = is_non_gui;

	// cannot call this virtual function in the CTOR !!!
	if(is_non_gui)
		call_gtk_or_glib_init( p_argc, p_argv ); // OR glib_init
}

/*!
	destructor - gtk_main_quit()
*/
TOPAPP_Holder_glib::~TOPAPP_Holder_glib()
{
	call_gtk_or_glib_quit();
}

bool TOPAPP_Holder_glib:: call_gtk_or_glib_init(int * p_argc, char *** p_argv)
{
	return call_glib_init(p_argc,p_argv);
}

bool TOPAPP_Holder_glib:: call_gtk_or_glib_quit()
{
	return call_glib_quit();
}

/*!
	CTOR-II - gtk_main_init()

	this is going to need GTK linkage anyway ...
	but this is a step towards a dual system
*/
bool TOPAPP_Holder_glib::call_glib_init(
	int * p_argc,
	char *** p_argv
)
{
	errno_drop();
	errno_zero_fn();
	INFO("HERE is it gui: g_not_gtk = %d", g_not_gtk);
	if( g_not_gtk ) {
		// OK
		// the point is - that gtk is not linktime available
		// but a gtk program can be non gui
		// eg accts runs reports
	} else {
		WARN("This is a GUI object, with non GUI init");
	}
	GMainContext *context = NULL; // use default
	gboolean is_running = true; // almost meaningless ?
	loop = g_main_loop_new(context, is_running);
//	e_print_init_gtk_text(this); // GTK ??
	e_print_init_console(); // provided in libbase2/gtk_text
	errno_zero(); // some init file wasnt there
	return true;
}

/*!
	hold the main_app, so that it is deleted before main_quit
*/
void TOPAPP_Holder_glib::app_register( TOPAPP_Task * app )
{
	app->debug_track_pointer();
	INFO("app = %p", app );
	if(main_app) {
		WARN("task is a sub-task - refactor code to not call this %p", app);
	} else
		main_app = app;
}

/*!
*/
void TOPAPP_Holder_glib:: task_attach( TOPAPP_Task * task )
{
	task->debug_track_pointer();
	wait_more_for( task );
}

/*!
*/
void TOPAPP_Holder_glib:: task_detach( TOPAPP_Task * task )
{
	task->debug_track_pointer();
	wait_less_for( task );
}


/*!
*/
void TOPAPP_Holder_glib:: app_destroyed()
{
	call_gtk_or_glib_quit();
}

/*!
	delete all held objects before calling gtk_main_quit
*/
bool TOPAPP_Holder_glib::call_glib_quit()
{
	tell_main_app_to_forget_link_to_here();
	if(!gtk_closed)
	{
		gtk_closed = true;
		// actually, windows might cause callbacks on OBJ ??
		win_list.rezero();
		main_app = NULL;
		if(g_not_gtk) {
			if(1) STEP("Calling g_main_quit\n");
			g_main_loop_quit(loop);
			return true;
		} else {
			if(1) {
				FAIL("Calling g_main_quit - but is GTK\n");
				g_main_loop_quit(loop);
				return true;
			}
			return FALSE_dgb_fail("GLIB not GTK");
		}
	}
	return FAIL("already called");
}

/*!
	debug
*/
void TOPAPP_Holder_glib::debug_show_lists()
{
	INFO( "N(WIN/OBJ/keep) = %d/%d/%d",
		win_list.N(),
		OBJ_list.N(),
		OBJ_keep_list.N()
	);
	// return;
	int N=0;

#if 1
// incomplete type - delete w_win_top_stuff
	N = win_list.N();
	INFO( "%s[%d]", "win_list", N );
	for(int i=0; i<N; i++ ) {
		// works becuase .ptr is public, but get_VOID_ptr() could do same
		w_win_top * ptr1 = win_list[i].ptr;
		obj_ref0 * ptr = (obj_ref0 *) ptr1; // added knowledge
		ptr->debug_track_pointer();
	}
#endif

	N = OBJ_list.N();
	INFO( "%s[%d]", "OBJ_list", N );
	for(int i=0; i<N; i++ ) {
		OBJ_list[i]->debug_track_pointer();
	}

	N = OBJ_keep_list.N();
	INFO( "%s[%d]", "OBJ_keep_list", N );
	for(int i=0; i<N; i++ ) {
		OBJ_keep_list[i]->debug_track_pointer();
	}

}

/*!
	remove an Object (of almost any kind) that was keeping APP alive
*/
void TOPAPP_Holder_glib::wait_less_for( obj_ref0 * obj )
{
	obj->debug_track_pointer();

	OBJ_list.del_val( obj );
	if( OBJ_list.N() == 0 )
	{
		if( win_list.N() == 0 )
			call_gtk_or_glib_quit();
	}
	debug_show_lists();
}

/*!
	add an Object (of almost any kind) that will keep this thing alive
*/
void TOPAPP_Holder_glib::wait_more_for( obj_ref0 * obj )
{
	obj_hold<obj_ref0> temp2 = obj;
	OBJ_list.del_val_if_present( obj );
	OBJ_list.append( obj );
	debug_show_lists();
}

/*!
	add a sub-window to the list of toplevels
*/
bool TOPAPP_Holder_glib::window_attach( w_win_top * win )
{
	return FALSE_dgb_fail("GLIB not GTK");
}

/*!
	add a sub-window to the list of toplevels
*/
void TOPAPP_Holder_glib::keep_that( obj_ref0 * obj )
{
	obj_hold<obj_ref0> temp2 = obj;
	OBJ_keep_list.del_val_if_present( obj );
	OBJ_keep_list.append( obj );
	debug_show_lists();
}

/*!
	add a sub-window to the list of toplevels
*/
void TOPAPP_Holder_glib::drop_that( obj_ref0 * obj )
{
	OBJ_keep_list.del_val_if_present( obj );
	debug_show_lists();
}

/*!
	remove top-level window from keeplist
*/
bool TOPAPP_Holder_glib::window_detach( w_win_top * win )
{
	return FALSE_dgb_fail("GLIB not GTK");
}

/*
	As well as switch selecting glib/gtk/other, wrap with POST, catch, etc
*/
int TOPAPP_Holder_glib::call_gx_main_and_exit() // or return int 
{
 // SEN_TRY is a POST macro
// try {
	bool ok = TRUE;
	int ret = 0;
	try {
		e_print("-------------------- gx_main -------------------\n");
		if( g_not_gtk ) {
		 if(!call_glib_main()) ok = FALSE;
		 g_main_loop_run( loop );
		} else {
		 if(!Call_gtk_main()) ok = FALSE;
		// gtk_main();
		}
		e_print("-------------------- ret_main -------------------\n");

	} catch ( dgb_fail * ex ) {
		ex->caught();
		delete ex;
		ok = FALSE;
	} catch ( const char * ex_str ) {
		e_print("PLAIN STRING EXCEPTION '%s'\n", ex_str );
		ok = FALSE;
	}

// this is whats wanted - but MAIN APP HOLDER needs a bit of adjustment
// which might get solved when getopt progresses
////	if( main_app) main_app->pre_fast_exit();
	// fast exit
	if(!ok) {
		ret = 22;
		if(errno) ret = errno;
	}
	if(ret) WARN("_exit(%d)", ret );
	_exit(ret);

	// ref_tally.print_leaked_report();

	if( ok )
		return 0;
	else
		return 1;

// } // SEN_ACCESS_VIOLATION_HANDLER();                                           
#warning "I dont under stand this - post not working with gcc on WIN32"
//  catch(
//		GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION 
//		  && file::handle_page_access((GetExceptionInformation())-> 
//                             ExceptionRecord->ExceptionInformation) 
//	  ? EXCEPTION_CONTINUE_EXECUTION 
//	  : EXCEPTION_CONTINUE_SEARCH
// ) {}

 return EXIT_FAILURE;                                                        
}

bool TOPAPP_Holder_glib::call_glib_main()
{
	g_main_loop_run( loop );
	return true;
}

/*!
	This no_longer wraps with excepting, its caller does that
*/
bool TOPAPP_Holder_glib::Call_gtk_main()
{
	return FALSE_dgb_fail("GTK not compiled in");
		// gtk_main();
}
