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
#include "amix_main.h"
#include "dir_name_ext.h"


#include "cfg_demo_1.h"
#include "sql_link_MySQL.h"

struct SPOUT_session {
	// curr_pen // _inks_from_ _curr_pallete _inks
	// curr_paper // DPI FONTS FGBG ANIM DRAW
	// curr_pages // entire document export_to_index _files
	// curr_user // BENCH _link_to_BENCH_as_USER_t // TTY // x509 claim
	// curr_data // from file // mmap // segment in shared file // Layout
	// curr_sess += SESS
	// SESS += SPOUT_session * SESSION("name")

	// DEFAULT STATE = mid EVAL EXPR DATA // eg mid get_attr("attr")
	// THIS is probably local to a caller 
};

bool cmd_SPOUT(int argc, char ** argv )
{
	SPOUT_session sess; // ALLOC on stack until return; // DTOR //
	return FAIL("// SWITCH TO SESSION");
	// thats where I want ARGV = _t { argc argv } // struct VECT ARGV[ 2 ]
	// "[0[2" // TOKEN
	// if ARGS.is_empty() { ... } 
	::CFG_DEMO::kf_cfg_demo_1 cfg;
	// SWITCH TO SESSION
	cfg.late_init();
	if(cfg.load_file_()) {
		INFO("loaded");
	} else {
		INFO("defaulted");
		cfg.set_DEMO_values();
		cfg.save_file_();
	}

	sql_link_MySQL sql;
	if(cfg.connect_mysql( sql )) {
		INFO("connected");
	} else {
		return FAIL_FAILED();
	}


	return PASS("does");
}

bool cmd_cfg_demo(int argc, char ** argv )
{
	::CFG_DEMO::kf_cfg_demo_1 cfg;
	// cfg_demo { ... load_file_from_MySQL save_file_into_SQL ... }
	//  POOL = SESS_CONN_CFG _pool {
	//	HERE
	//  } 
	//  SESS += VAR_POOL "cfg_demo" // filename %s.%s ".cfg" // ".%s" "cfg"
	// AUTO filename .wrap "cfg" "cfg_demo" "cfg_two" 
	cfg.late_init();
	if(cfg.load_file_()) {
		INFO("loaded");
	} else {
		INFO("defaulted");
		cfg.set_DEMO_values();
		cfg.save_file_();
	}

	sql_link_MySQL sql;
	if(cfg.connect_mysql( sql )) {
		INFO("connected");
	} else {
		return FAIL_FAILED();
	}


	return PASS("does");
}

bool cmd_play_code(int argc, char ** argv )
{

	const char * default_argv[] = {
	 "cfg_demo",
	};
	if( argc == 0 ) {
		INFO("Using builtin default argv");
		argv = (char **) default_argv;
		argc = sizeof( default_argv ) / sizeof( default_argv[0]);
	}

	str0 cmd1 = argv[0];
	argv ++;
	argc --;

	INFO("invoking-cmd1 %s", (STR0) cmd1 );

	if( cmd1 == "cfg_demo" ) return cmd_cfg_demo( argc, argv );
	if( cmd1 == "SPOUT" ) return cmd_SPOUT( argc, argv );
	if( cmd1 == "HT_main" ) return 0==HT_main(); // ( argc, argv );
	if( cmd1 == "amix" ) return amix_cmd(argc, argv);

	// holder does this // setlocale(LC_ALL,"");
	//	TOPAPP_Holder_gtk holder(&argc, &argv);
	//	new gtk_text_top_app( & holder, filename );
	//	e_print_show();
	//	holder.call_gx_main_and_exit();
	// }
	return FAIL("unrecognised cmd '%s'", (STR0)cmd1 );

}


int main(int argc, char ** argv )
{
	// gdb_invoke();
	dir_name_ext pathname;
	// lots todo // pathname.test1();
	if( !pathname.decode_filename( argv[0] )) {
		FAIL_FAILED();
		return 1;
	}
	pathname.test_print();
	if( argc == 1 ) {
		FAIL("Usage %s cmd ...", argv[0] );
		return 6;
	}
	str0 cmd0 = (STR0) pathname.name;
	INFO("Invoked as '%s'", (STR0)cmd0 );
	argc --;
	argv ++;

	// all roads lead to cmd_play_code( argc, argv ) // after SHIFT cmd0
	// but does not have to, could interpret argv0 as cmd0
	// could also call it with argv0 yet to be checked
	// but it is special as variations in ./name /path/name etc 
	// still no sign of SYMB_LINK to KNOWN_EXE_NAME
	// but this is to interpret ARGS to do different tests
	//
	// convert BOOL to EXIT_ZERO +- errno
	bool ok1;
	     if( cmd0 == "play_code" ) ok1 = cmd_play_code( argc, argv );
	else if( cmd0 == "exec_name" ) ok1 = cmd_play_code( argc, argv );
	else {
		WARN("expected 'play_code' got '%s'", (STR0)cmd0);
		ok1 = cmd_play_code( argc, argv );
	}
	ref_tally.print_leaked_report();
	if( ok1 ) {
		return 0;
	}
	FAIL_FAILED();
	if( errno ) return errno;
	return 22;
}
