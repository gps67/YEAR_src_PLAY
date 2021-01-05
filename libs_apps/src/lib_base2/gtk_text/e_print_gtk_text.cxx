// #include "g_text_buffer.h"

#include <stdarg.h>
#include "TOPAPP_Task_Window_Minor.h"
#include "w_text_view.h"
#include "w_win_top.h"
#include "w_vbox.h"
#include "e_print_io.h"

#include "stdio.h" // FAIL_in_FAIL

#include "buffer2.h" //
#include "dgb.h" // TRACE - careful though

//static obj_hold<obj_ref> hold_e_print_view_top_win;

using namespace APPS;

class e_print_io_gtk_text;

/*!
	problem with top level window - close calls destroy
	but obj_hold holds something
*/
class E_Print_View : public TOPAPP_Task_Window_Minor
{
 public:
	obj_hold<w_text_view> 	text_view;

	~E_Print_View()
	{
	}

	E_Print_View(TOPAPP_Holder * holder)
	: TOPAPP_Task_Window_Minor(
		holder, // NULL, // TOPAPP_Holder
		"E_Print_View",
		400, 500
		// filename // default NULL filename with false compression
	)
	{
		errno_zero();
//		// need to debug ref_count and call_destroy and ... delete
//		hold_e_print_view_top_win = this;
// gdb_invoke(true);
	//	// obj-name set to w_title by base class
	//	debug_set_obj_name("E_print_view"); // use title
	if(0)
	// if it is set ..
		main_window->debug_set_obj_name("E_print_view should use title"); // use title
	// AAGGHHH - this is not the w_win_top ...
		debug_track_pointer();

		has_btn_hide = true; // now working
		use_standard_layout(); // late_init, called from FINAL class!

		text_view = new w_text_view(); // no shared tags
		app_area->pack_big( text_view->mk_scroll_wrap() );
		if(0) {
			// WARN would only want to print to e_print
			// WARN("e_print is usually hidden, on startup");
			main_window->show();
		}
		text_view->set_font1("Courier 12");
		// text_view -> test1();
		errno_zero(); // already was
	}

//	/*!
//		The default is from 8859, add from_utf8 later
//	*/
//	void e_print_from_8859( const char * fmt, va_list args )
//	{
//		txt_buff.vprint( fmt, args );
//		text_view->append_8859( txt_buff );
//		txt_buff.clear();
//		// scroll_to_endpos();
//	}

};

void C_g_print_io_str( const char * msg )
{
	if( !e_print_io ) {
		// fprintf ... ?
		return;
	}
	e_print_io -> write_UTF8( msg );
}

bool FAIL_in_FAIL(const char * s )
{
	fprintf(stderr, "## FAIL_in_FAIL ## %s\n", s );
	fflush(0);
	return false;
}

class e_print_gtk_text : public e_print_io_INTERFACE
{
	obj_hold<E_Print_View>	panel; // or actual window ...
//	obj_hold<TOPAPP_Task_Window_Minor> window;
	bool also_write_to_gui;
	bool also_write_to_console;
	bool also_write_to_logfile;
	FILE * logfile;
	const char * logfilename;

	// UTF8 is the only gtk CSET
 public:
#if 0
	e_print_gtk_text()
	: e_print_io_INTERFACE()
	{
		also_write_to_console = true;
	}
#endif

	e_print_gtk_text(TOPAPP_Holder * holder = NULL )
	: e_print_io_INTERFACE()
	{
		also_write_to_gui = false;
		also_write_to_console = true;
		also_write_to_logfile = false;
		logfilename = NULL;
		logfile = NULL;
		if(!init_logfile()) {
			FAIL_clear_error();
		}
		if(holder) check_panel(holder);
	}

	bool init_logfile()
	{
		if( logfile ) return false;
#ifdef WIN32
		logfilename = "C:\\Temp\\log_e_print.txt";
#else
		logfilename = "/tmp/log_e_print.txt";
#endif
		if( logfilename ) {
			INFO("using logfile '%s'", logfilename );
			// above line will possibly fflush mix output
			logfile = fopen(logfilename, "w");
			if(!logfile) {
			// return DIAG_FAIL("DIAG_FAIL_SAYS: fopen(%s,w)", logfilename );
			 return FAIL("fopen(%s,w)", logfilename );
			}
		}

		also_write_to_logfile = true;
		return true;
	}

	bool check_panel( TOPAPP_Holder * holder =  NULL )
	{
		/*
			store or retrieve holder (first call must provide)
			and check non-null
		*/
		static TOPAPP_Holder * default_global_TOPAPP_Holder =  NULL;
		// save the holder
		if(!default_global_TOPAPP_Holder)
			default_global_TOPAPP_Holder = holder;

		// 99% of the time, check_panel is happy
		if(panel) return true;
		// else re-create the panel, using the holder

		if(!holder) holder = default_global_TOPAPP_Holder;
		if(!holder) {
			return FAIL_in_FAIL("holder cannot be NULL");
		}

		static int looping = 0;
		if( looping ) {
			// e_print (or other) within e_print setup ...
			// if there is an error, its too bad
			// we are not reporting ourselves!
			return FAIL_in_FAIL("e_print::check_panel() one loop detected");
			return false;
			return false;
		}
		looping ++;
		errno_zero();
		panel = new E_Print_View( holder );
		looping --;
		if(!panel){
			return FAIL_in_FAIL("panel cannot be NULL");
		}
		if(!panel->text_view){
			return FAIL_in_FAIL("panel->text_view cannot be NULL");
		}
		if(!panel->main_window){
			return FAIL_in_FAIL("panel->main_window cannot be NULL");
		}
		return true;
	}

	void show_unhide()
	{
		if(!check_panel()) return;
		// errm maybe
		panel->main_window->show();
		panel->main_window->raise_window();
	}

	void switch_gui_onoff( bool onoff )
	{
		if(!onoff) STEP("ON");
		also_write_to_gui = onoff;
		if(onoff) STEP("ON");
	}

	void switch_gui_onoff_toggle()
	{
		switch_gui_onoff( !also_write_to_gui );
	}


	bool write_BYTES( blk1 & blk ) {
		// apply heuristic ...
		return write_UTF8( blk );
	}

	bool write_ASCII( blk1 & blk ) {
		return write_UTF8( blk );
	}

	bool write_8859( blk1 & blk ) {
		buffer2 blk_utf8;
		if(! blk_utf8.put_nn_bytes_8859_to_utf8( blk.nbytes_used, blk.buff ))
		{
			// FAIL_in_FAIL("DIAG"); // but will only call us again ?
			return false;
		}
		return write_UTF8( blk_utf8 );
	}

	void app_prefers_8859( bool latin15 )
	{
		// e_print a warning ...
		prefers_8859 = false; // remains so too
		return; // unchanged
	}

	bool write_UTF8( blk1 & blk ) {
		if(!check_panel()) return false;
		if( also_write_to_gui ) {
			panel->text_view->append_utf8( (STR0) blk );
		}
		if( also_write_to_console ) {
			blk.trailing_nul();
			fprintf( stderr, "%s", (STR0) blk.buff );
			fflush(0);
		}
		if( also_write_to_logfile ) {
			blk.trailing_nul();
			fprintf( logfile, "%s", (STR0) blk.buff );
			fflush(0);
		}
		return true;
	}

	bool write_UTF8( STR0 str ) {
		if(!check_panel()) return false;
		if( also_write_to_gui ) {
			panel->text_view->append_utf8( str );
		}
		if( also_write_to_console ) {
			fprintf( stderr, "%s", str );
			fflush(0);
		}
		// above did go to file so this does ... what
		if( also_write_to_logfile ) {
			fprintf( logfile, "%s", str );
			fflush(0);
		}
		return true;
	}

/*
	bool set_as_glib_print_handler()
	{
		someone needs to set the global e_print_io
		so back to base class
	}
*/

	void e_print_show()
	{
		if(!check_panel()) return;
		panel->main_window->show();
		panel->main_window->raise_window();
	}

};

e_print_io_INTERFACE * new_e_print_gtk_text()
{
	return (e_print_io_INTERFACE *) new e_print_gtk_text();
}

void e_print_init_gtk_text( TOPAPP_Holder * holder )
{
	// why does this break output - I dont know
	// and why at the exact point when it does ...
	printf("\n#### e_print_init_gtk_text\n");
	e_print_gtk_text * io = new e_print_gtk_text( holder );
	// io -> show_unhide();
	io -> set_as_glib_print_handler();
}

