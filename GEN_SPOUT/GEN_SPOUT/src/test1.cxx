
// #include <stdlib.h>
#include <unistd.h> // write
// #include <string.h>

#include <stdarg.h> // ...

/*
	TODO REDO pipes on WIN32, g_spawn, but without glib

	Cant find it, but I had print | tar -xf -
	Need to grow that on WIN32 (win64) 
	Need to avoid depending on it on UNIX
	SO write PS to a file, normalise the PS ps2ps, ps2pdf, view_pdf

*/

#include "dgb.h"
#include "buffer2.h"

#include "util_buf.h" // blk_write_to_file( out, filename )
#include "dir_name_ext.h" // dir_name_ext( filename ) .ext = ; mk_near_path_name

#include "SPOUT_STUBS.h"
#include "SPOUT_argv_decoder.h"

#define IF_NOT if(0) 

using namespace SPOUT;

struct ink_rgb
{
	float r;
	float g;
	float b;

	ink_rgb &  set_white()
	{
		r = 0.0;
		g = 0.0;
		b = 0.0;
		return * this;
	}

	ink_rgb &  set_blue()
	{
		r = 0.0;
		g = 0.0;
		b = 1.0;
		return * this;
	}

	ink_rgb &  set_green()
	{
		r = 0.0;
		g = 1.0;
		b = 0.0;
		return * this;
	}

	ink_rgb &  set_black()
	{
		r = 1.0;
		g = 1.0;
		b = 1.0;
		return * this;
	}

	ink_rgb()
	{
		set_blue();
	}

};

/*!
	test_SPOUT is not on the heap

	this will get split and renamed
	test_SPOUT
	SPOUT_basics -- comments phrases strings pages 
	SPOUT_assists -- path builder, 
	SPOUT_plus -- 

	we will not be using TREE of code as an ARG because I am lazy
	the gnerator does all it's own pre-calc
	and output dumb values, or predefined code sequences

	I dont really want any CALC matrix but dpi 720 vs dpi 72 ??
	ie then use whole decimal numbers ... maybe

	It is also tricky to have { int ptsz; } ?
*/
class test_SPOUT // : public obj_ref
{
 public:
	buffer2 out;
	int pages;

	// GC PEN
	
	int pg_no;
	int page_w;
	int page_h;
	//	if(!page_header( page_w, page_h, pages++ )) return FAIL_FAILED();


	ink_rgb rgb;

	bool in_path;
	bool in_page;

	bool list_end()
	{
		return true; // yes it is list_end // bool yes call now
		// EMPTY CODE REQUIRED
		//  DEBUG can log MB tracked vars calls 
		// this is supposed to never get called
		// because it install itself as NULL or NEAR_NULL
		// caller optimised out NULL hoping this code is empty
	}
//	bool list_end(); //  API_VECTOR() HERE = UDEF // UDEF overrides NULL
//	bool list_end(); //  { @MODULE_UDEF @SESS @CTXT } // routing Names
//	bool list_end(); //  ASYNC_SIGNAL_in_sequence_tho // or pre-numbered
//	// LIST = { ... ITEM ... } 
//	// for ALLOC use { ITEM = N ++ }
//	// ASM get N
//	// ASM set ITEM
//	// ASM incr N
//	// ASM Net N
//	// 
//	// ASM set ARGV ITEM [VIA]
//	// ASM ret ITEM
//	// LINK tell_API_expect_cache_API_alloc_idx_in_list
//	// API tell_API_expect_cache_API_alloc_idx_in_list_or_heap_or_mmap_or_
//	// API new_idx = N++; // ALLOC_ID claims this ASM // WHEN_CHECKS_SURVEYS


	// NULL => use_default() // this ctxt // ctxt.sess // ctxt.udef 
	// NAME <= 
//	bool list_end(); //  /* w, page_h, pages++ */ )) // return FAIL_FAILED();
//	 return NULL_means_EOT; 

	test_SPOUT()
	{
		out.get_space( 1024 * 32 );
		pages = 0;
		pg_no = 0; // pg_no = idx = ITEM = N++ //
		page_w = 595;
		page_h = 842;
		page_w = 595;
		page_h = 842;
		in_path = false;
		in_page = false;
	}

	bool comment( str0 cmnt ) {
		return out.print("%% %s\n", (STR0) cmnt ); // with leading SP
	}

	bool hdr_comment( STR0 fmt, ... ) {
		out.put_byte('%');
		va_list args;
		va_start( args, fmt );
		// false = conv_8859_to_utf8 // upgrade somehow
		bool ok = out.vprint( false, fmt, args );
		va_end( args );
		out.put_LF();
		return ok;
	//	return out.print("%%%s\n", (STR0) cmnt ); // with leading SP
	}

	bool cmd_line( STR0 fmt, ... ) {
		va_list args;
		va_start( args, fmt );
		bool ok = out.vprint( false, fmt, args );
		va_end( args );
		out.put_LF();
		return ok;
	}

	bool hdr_blank_line() { return hdr_comment(""); }
	bool cmd_blank_line() { return cmd_line(""); }

	bool file_header( int w, int h, int pages )
	{
		hdr_comment("!PS-Adobe-3.0");
//		hdr_comment("!PS");
		buffer2 buf;


//		hdr_comment("<</PageSize [ %d %d ]>> setpagedevice", w, h );
//		hdr_comment("/PageSize [ %d %d ] setpagedevice", w, h );

		// double %% becomes % in vprintf
		// hdr_comments adds % and /n // and vprint fmt
IF_NOT		hdr_comment("%%DocumentMedia: Default %d %d () ()", w, h );

		hdr_comment( "%%Creator: SPOUT" );
//		hdr_comment( "%%LanguageLevel: 2" );
//		hdr_comment( "%%CreationDate: 2020-08-24" );

		hdr_comment("%%Pages: %d", pages );

		hdr_comment( "%%EndComments" );
		hdr_comment( "%%BeginProlog" );
		hdr_comment( "%%EndProlog" );
		hdr_comment( "%%BeginSetup" );
	// THIS ONE // set the page size
		cmd_line("<</PageSize [ %d %d ]>> setpagedevice", w, h );
		hdr_comment( "%%EndSetup" );
		return true; // or ask out if it has had mem errors
	}

	bool hdr_PageSize( int w, int h )
	{
		buffer2 buf;

		return true;
	}

	bool page_header( int w, int h, int pg_no )
	{
		page_w = w;
		page_h = h;

#if 0

		buf.clear();
		hdr_comment( buf.print("%%PageSize: [ %d %d ]", w, h );
		hdr_comment( (STR0) buf );

		buf.clear();
		hdr_comment( buf.print("/PageSize [ %d %d ]", w, h );
		cmd_line( (STR0) buf );
#endif

		hdr_comment( "%%Page: %d", pg_no );
IF_NOT		hdr_comment( "%%PageBoundingBox: 0 0 %d %d", w, h );
		hdr_comment( "%%BeginPageSetup");

		// BEGIN PAGE CODE HERE

		hdr_blank_line();
		hdr_comment( "%%EndPageSetup");
IF_NOT		hdr_comment( "%%BoundingBox: %d %d %d %d", 0, 0, w, h );
IF_NOT		hdr_comment( "%%DocumentMedia: %d %d () ()", w, h );

		hdr_comment( "%EndComments" );
//		hdr_comment( "%BeginProlog" );
		return true; // or ask out if it has had mem errors
	}

	bool times_ptsz( int ptsz )
	{
		STR0 fontname = "Times-Roman";
		return
		 out.print("/%s findfont %d scalefont setfont\n",
		  fontname,
		  ptsz
		 );
	}

	bool parenth_text( str0 text )
	{
		if(!text) return PASS("but empty text");
		out.print("(");
		const char * S = (STR0) text;
		while( char c = *S++ ) {
		 switch(c) {
		  case '(' :
		  case ')' :
		  	// no need if balanced
			// \( \) if not
			// I'm not counting them, zap them all
		  	out.print("\\%c", c);
		  break;
		   default:
		   	out.put_byte(c);
		 }
		}
		out.print(")");
		return true;
	}

	bool x_y_cmd( int x, int y, STR0 cmd ) 
	{
		return out.print( "%d %d %s\n", x, y, cmd );
	}

	bool x_cmd( int x, STR0 cmd ) // x isnt x
	{
		return out.print( "%d %s\n", x, cmd );
	}

	bool cmd_word( STR0 word )
	{
		return out.print( "%s\n", word );
	}

	bool in_PATH() { return in_path; }
	bool set_in_PATH() { return in_path = true; }
	bool end_in_PATH() { return in_path = false; }
	// called by closepath fill // NO auto-closepath.  no taa
	// intended use WRITE_LIST_PATH _N_ array then call closepath fill
	bool newpath()
	{
		if(!in_PATH()) {
			set_in_PATH();
		}
		return cmd_word("newpath");
	}

	bool newpath_by_now()
	{
		/*
			xy_moveto does NOT need this
			xy_lineto does 
		*/
		// an AUTO newpath
		if(!in_PATH()) {
			return newpath();
		}
		return true;
	}

	bool stroke()
	{
		return cmd_word("stroke");
	}

	bool xy_lineto( int x, int y ) 
	{
		newpath_by_now(); // extra tracer
		return x_y_cmd( x, y, "lineto" );
	}

	bool xy_moveto( int x, int y ) 
	{
	//	newpath_by_now(); // AUTO GEN newpath() // call now
	//	but no need for MOVE ?
		return x_y_cmd( x, y, "moveto" );
	}

	bool xy_str( int x, int y, STR0 str ) 
	{
	//	newpath_by_now(); // AUTO GEN newpath() // call now
	//	but no need for MOVE ?
		x_y_cmd( x, y, "moveto" );
		text_show( str );
		return true;
	}

	bool setlinewidth( int lw )
	{
		return x_cmd( lw, "setlinewidth" );
	}

	bool setrgbcolor( float r, float g, float b )
	{
		return out.printf("%4.2f %4.2f %4.2f setrgbcolor\n", r, g, b ); 
	}
	bool setrgbcolor( ink_rgb & rgb )
	{
		return setrgbcolor( rgb.r, rgb.g, rgb.b ); 
	}

	bool text_show( str0 text )
	{
		parenth_text( text );
		out.print(" show\n");
		return true;
	}

	bool no_showpage_but_begin_first_page() { return showpage(); }

	bool showpage()
	{
		pg_no = pages ++ ; // MATCH idx = N ++ // scope
		// pg_no == 0 or more u24 or u4 or see item_expr
		static bool FOUND_FIRST_showpage = false;
		if( FOUND_FIRST_showpage ) {
			cmd_word("showpage");
		} else {
			FOUND_FIRST_showpage = true; // but dont send it
		// NO	cmd_word("showpage"); // no previous to send
		/*
			The API says to call this for the first page
			without any showpage actually being emitter

			You could also call 
			PAGE_PLUS_ON_SHOWPAGE();

			At the end of every page, call showpage()
			Before the zero_th call pre_call

				ARGV 

				CODE
			
		*/
		// COMPILE // VECTOR_CALLED_on_SIGNAL // ACTION
		}
		pages_plus_on_showpage(); // AUTO CONNECT bool plumbing //
		return true;
	}
	bool pages_plus_on_showpage() { return on_showpage(); }
	bool on_showpage()
	{
		if(!page_header( page_w, page_h, pg_no )) return FAIL_FAILED();
		return true;
		/*
			The previous PAGE has finished
			maybe some cache builders mid flow

		*/
	}

	bool write_to_stdout()
	{
		write(1, out.buff, out.nbytes_used);
		return true;
	}

	bool	closepath() { return cmd_word("closepath"); }
	bool	fill() { return cmd_word("fill"); }

// --------------------------------------------------------- //

	bool system_cmd( buffer2 & cmd )
	{
		int t = 0;
		t = system((STR0) cmd );
		if( t != 0 ) {
			FAIL( "SYSTEM(%s)", (STR0) cmd );
			INFO( "system() returned %d", t );
			return FAIL_FAILED();
		}
	//	PASS("system(%s)", (STR0) cmd );
		return true;
	}

	bool write_normalise_pdf_view()
	{
		STR0 BASENAME = "/tmp/test1_tmp";
		dir_name_ext f_name;
		buffer2 name_1;
		buffer2 name_2;
		buffer2 name_3;
		name_1 = BASENAME;
		name_2 = BASENAME;
		name_3 = BASENAME;
		name_1.printf("1.ps");
		name_2.printf("2.ps");
		name_3.printf("3.pdf");

		blk_write_to_file( out, (STR0) name_1 );

		buffer2 cmd;
		cmd.print("ps2ps %s %s", (STR0) name_1, (STR0) name_2);
		if(!system_cmd( cmd ) ) return FAIL_FAILED();

		cmd.clear();
		cmd.print("ps2pdf %s %s", (STR0) name_1, (STR0) name_3);
		if(!system_cmd( cmd ) ) return FAIL_FAILED();

		cmd.clear();
		cmd.print("mupdf %s", (STR0) name_3);
		if(!system_cmd( cmd ) ) return FAIL_FAILED();

		return true;
	}

	
	bool test1()
	{
	// do this at END of document first !!
	// KEEP vars_api of SCRIPT
	//  vars_api auto_keep scripts varso
	//   vars_api_pre_compile # run with samples list # cache 
	//    var_cache_pre_built += ... exprs used ...
	//	STRUCT { ... } // lucky C++ SCRIPT Name
	//	 NODE_ID = ITEM_ID = LOCN_ID
	//     Name {
		page_w = 595;
		page_h = 842;
		page_w = 200;
		page_h = 200;
		int pages = 2;
	//	}; // Name
	//	Export Name to API as struct PTR 
	//	ALIAS PTR REF
	//	ALIAS REF PTR Name { ... } // Name provides { ... }


	// DIVERTS to "callable_get_file_header()"
	// BECAUSE pages == N_total after last alloc done and dusted
	//
		if(!file_header( page_w, page_h, pages )) return FAIL_FAILED();
	// PROVIDE {
	//   	callable_get_file_header() {
			// PRESUME INIT DONE and called from MAIN CALL
			// counted totals N = Pages_Total
			// 
	//		READ_all_cached_BUFFER( gen it now from a few vars )
	//		READ_all_cached_VARS( CONST_POOL )
	//		READ_all_cached_VARS( VAR_POOL[VAR_ITEM].api(ARGV) )
	//	}
	// const int N_pages = PARAMETER N // called at_end to build front_page
	// probably use some SPACES in a pre-printed header, copied own output
	// API FACT N_Pages = u24; // usually u4 // u8 CALLABLE // uses API ARGV
//-or-	// uses common code // recompile both ends //
//-or-	// VOCABU += PHRASE TOP_OF_NEW_PAGE Layout
	// }

		if(!no_showpage_but_begin_first_page()) return FAIL_FAILED();
	//	if(!page_header( page_w, page_h, ++ )) return FAIL_FAILED();

		if(!list_end( /* page_w, page_h, pg_no */ )) return FAIL_FAILED();

		comment("This is a comment");
		times_ptsz( 20 );

		newpath();
		setrgbcolor( rgb.set_green() );
		setrgbcolor( rgb.set_blue() );
		setrgbcolor( rgb );
		// want the above 3 cached
		// list_end() style // 
		// next command does goto_its mode // clears previous mode
		// first each setrgbcolour sets mode ACTIVE_BIT_NOW( bitmask )
		// each leaves reside WANT RGB, it has changed, most recent bit
		// last knows by something else, dislodging, 
		// flush now or later
		// for now avoid repeat calls

		xy_moveto( 0, 0 );
		xy_lineto( page_w, page_h ); 
		xy_lineto( 0, page_h );
		xy_lineto( page_w, 0 );
#if 0
		// last point is the first // becomes closed loop
		xy_lineto( 0, 0 );
#else
		// the last step does not need to be repeated if first
		// VIRTUAL call BUILTIN if NULL // BUILTIN = { ... HERE ... }
		// eg call this from list_end() signal
		// upgrade signal to PKT // fragment obvs // N_BYTES
		closepath(); // CALL it NOW
#endif
		setlinewidth( 10 );
		stroke();

		newpath();

		xy_str(30, 30, "PAGE ONE"); // page[1] //
		times_ptsz( 10 );
		xy_str(30, 15, "with backslash-ed parenths () ");

		showpage();

	//	CHANGE PAPER_WH // rotated_flat WH // rotated_flat HW_as_WH
	//	SWITCH MEDIA COST += ONE_PAGE_of_
	//	if(!page_header( w, page_h, pages++ )) return FAIL_FAILED();

		// newpath(); // somewhere_API PATH_name.CTOR( CTXT, CODE, DATA )
		// { XY_TREE } // STEP = { XY ID } // ID = idx_of_step_in_list //
		times_ptsz( 20 );
		xy_str( 50, 50, "PAGE2 TWO");
		showpage();

	//	if(! out.print("THIS IS TEXT\n") ) return FAIL_FAILED();
	//	if(! write_to_stdout() ) return FAIL_FAILED();
		if(! write_normalise_pdf_view() ) return FAIL_FAILED();
		return true;
	} // test1
};

bool SPOUTER_test1( argv_decoder & ARGS )
{
 	test_SPOUT SPOUTER;
	if(!SPOUTER.test1()) 
		return FAIL_FAILED();;
	return PASS("OK");
}

int main_FAILED() {
	FAIL_FAILED();
	if(errno) return errno;
	return 1;
}

int main( int argc, char ** argv, char ** envp ) {
	argv_decoder ARGS( argc, argv, envp );
	if(! SPOUTER_test1( ARGS ) ) return main_FAILED();
	return 0;
}

