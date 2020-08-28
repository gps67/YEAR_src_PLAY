
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

#include "SPOUT_ink_rgb.h"
#include "SPOUT_XY.h"

#define IF_NOT if(0) 

using namespace SPOUT;

/*!
	test_SPOUT is not on the heap
	so it uses N++ Alloc PRE_ALLOC

	print a document to filename.ps 
	bounce document to filename.pdf
	TODO STREAM_FILE_to_PIPELINE ARGV_ENV_CTXT

	needs to overwrite NPAGES in header

	METHOD
		MMAP -or- PADDING 

		buildup_the output buffer using disk blocks
		into stream from txt_component
		-or-
		fixed size 4M mosaic 
		-or-
		===== ZONE ===== MOVES == header rewrites upto 4K
	
	METHOD
		TWO PASS Document Builder

			or as many as required
			or local rewrite
			or parametised what until when

		MMAP 4K PAGED HEAP -- as single file with holes

			SEGMENT 
			_type == ".elf64" #'.elf32' 
			_type == elf_segments
			_type == vfs_payloads 
			_type == PAGE_OF_256_ITEMS + DATA[u24]
			_type == array [ u8_u8 ] of CODE_POINT in u24_SEGMENT

		PASS 
			GEN TOC 
			GEN_Page_No

		PHONE_BOOK

			COLLECT semi-printed pages or scrolls or ...
			many u24 SEGMENTS eg 3 of 16_M used
			many u16 SEGMENTS eg OFFS 60 of 64_K used
			many u24 SEGMENTS eg 300 of u16_idx_item
			many u32 SEGMENTS CLARIFIES u24 spilling to u32

		CALC_SHEETS

			SCRIPT runs over inter-connected SEGMENTS # api events

			CACHE_DATASET OLD_VALS NEW_VALS rerun_to_update

			api_events
				SIGNAL UPDATE ITEM # OLD_VAL # GET NEW_VAL

		api_events
			the_GET_EA_GET_VAL_ script macros


 REGEN TREE using SCRIPT LAYER

 	CODE for get() // into ENQ_ACK
	WADE CODE PRE_BUILD convert TOKENISE at compile time
	SCRIPT runs MAKE over each EXPR it uses
	api_events runs the minimum, snapshot(bit), expands to mostly, excess



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
class test_SPOUT_base // : public obj_ref
{
 public:
	buffer2 out;

	buffer2 DOC_Title; // easier than STR1
	buffer2 DOC_Creator; // easier than STR1
	buffer2 DOC_; // easier than STR1

	int pages;
	int pg_no;

	// GC PEN
	
	XY_t page_wh;

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
//	 return NULL_means_EOT; 

	test_SPOUT_base()
	{
		out.get_space( 1024 * 32 );
		pages = 0;
		pg_no = 0; // pg_no = idx = ITEM = N++ //
		page_wh.set( 595, 842 );
		in_path = false;
		in_page = false;
	}

	bool comment( str0 cmnt ) {
		return out.print("%% %s\n", (STR0) cmnt ); // with leading SP
	}

	bool cmd_line( STR0 fmt, ... ) {
		va_list args;
		va_start( args, fmt );
		bool ok = out.vprint( false, fmt, args );
		va_end( args );
		out.put_LF();
		return ok;
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

	bool dsc_comment( STR0 fmt, ... ) {
		// Document Structure Conventions
		// % % NOSP Word 
		// % % NOSP Word COLON SP ...
		// COLON considered part of keyword
		// cross refer to PPD PS printer desc
		out.put_byte('%');
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

	bool hdr_blank_line() { return hdr_comment(""); }
	bool cmd_blank_line() { return cmd_line(""); }

	bool hdr_PageSize_device( XY_t & wh )
	{
		cmd_line("<</PageSize [ %d %d ]>> setpagedevice", (int) wh.x, (int) wh.y);
		return true;
	}

	bool file_header()
	{
		// must know Pages == 2 before starting output!!

		// see page 617 of PLRM2
		// Appendix G
		// Document Structuring and Conventions 
		// Version 3.0
		// see 2.4.2.Program Structure
		// see page 627 of PLRM2
		hdr_comment("!PS-Adobe-3.0");
		dsc_comment("Pages: %d", pages );

		if(DOC_Title) {
			dsc_comment("Title: %s", (STR0) DOC_Title );
		}

		if(DOC_Creator) {
			dsc_comment( "Creator: %s", (STR0) DOC_Creator );
		}

		// double %% becomes % in vprintf // or use hdr2_
		// hdr2_comments adds %% and /n // and vprint fmt
		// hdr_comments adds % and /n // and vprint fmt
IF_NOT		dsc_comment("DocumentMedia: Default %f %f () ()", page_wh.x, page_wh.y );

//		dsc_comment( "LanguageLevel: 2" );
//		dsc_comment( "CreationDate: 2020-08-24" );


		dsc_comment( "EndComments" );

		cmd_blank_line();
		dsc_comment( "BeginProlog" );
		dsc_comment( "EndProlog" );
		cmd_blank_line();
		dsc_comment( "BeginSetup" );

	// THIS ONE // set the page size
		hdr_PageSize_device( page_wh );
		// cmd_line("<</PageSize [ %d %d ]>> setpagedevice", page_wh.x, page_wh.y );

		dsc_comment( "EndSetup" );
		cmd_blank_line();
		return true; // or ask out if it has had mem errors

		// Page
		// BeginPageSetup
		// EndPageSetup
		// PageTrailer
		
		// Trailer
		// EOF
	}

// RTFM says Page_begin or something

	bool page_header( XY_t _wh, int pg_no )
	{
		page_wh = _wh;

	/*
		PLRM2 pg 628
	*/

#if 0

		dsc_comment( "PageSize: [ %f %f ]", page_wh.x, page_wh.y );
		cmd_line( "/PageSize [ %f %f ]", page_wh.x, page_wh.y );
#endif

		cmd_blank_line();
		STR0 label = "PAGE_ONE";
		if(pg_no == 1)
			label = "PAGE_TWO";
		dsc_comment( "Page: (%s), %d", label, pg_no );

		hdr_blank_line();
IF_NOT		dsc_comment( "PageBoundingBox: 0 0 %d %d",
			(int) page_wh.x,
			(int) page_wh.y );
		dsc_comment( "BeginPageSetup");

		// BEGIN PAGE CODE HERE

		hdr_blank_line();
		dsc_comment( "EndPageSetup");
		hdr_blank_line();
IF_NOT		dsc_comment( "BoundingBox: %d %d %d %d",
			0, 0, (int) page_wh.x, (int) page_wh.y );
IF_NOT		dsc_comment( "DocumentMedia: %d %d () ()", (int) page_wh.x, (int) page_wh.y );

		dsc_comment( "EndComments" );
		cmd_blank_line();
//		dsc_comment( "BeginProlog" );
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

	bool xy_cmd( XY_t & xy, STR0 cmd ) 
	{
		xy.xy_print_3d( out );
		return out.print( "%s\n", cmd );
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

	bool xy_lineto( XY_t & xy ) 
	{
		newpath_by_now(); // extra tracer
		return xy_cmd( xy, "lineto" );
	}

	bool xy_moveto( XY_t & xy ) 
	{
	//	newpath_by_now(); // AUTO GEN newpath() // call now
	//	but no need for MOVE ?
		return xy_cmd( xy, "moveto" );
	}

	bool xy_str( XY_t & xy, STR0 str ) 
	{
	//	newpath_by_now(); // AUTO GEN newpath() // call now
	//	but no need for MOVE ?
		xy_cmd( xy, "moveto" );
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
		if(!page_header( page_wh, pg_no )) return FAIL_FAILED();
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

	bool	closepath() { return cmd_word("        closepath"); }
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

};

class test_SPOUT :public test_SPOUT_base // : public obj_ref
{
 public:
	bool test1()
	{
		page_wh.set( 50, 30 ); // set page size before
	// do this at END of document first !!
	// KEEP vars_api of SCRIPT
	//  vars_api auto_keep scripts varso
	//   vars_api_pre_compile # run with samples list # cache 
	//    var_cache_pre_built += ... exprs used ...
	//	STRUCT { ... } // lucky C++ SCRIPT Name
	//	 NODE_ID = ITEM_ID = LOCN_ID
	//     Name {

		// magic opinion Pages == 2
		pages = 2;

		DOC_Title = "SET DOC Title";
		DOC_Creator  = "SPOUT";

	//	}; // Name
	//	Export Name to API as struct PTR 
	//	ALIAS PTR REF
	//	ALIAS REF PTR Name { ... } // Name provides { ... }


	// DIVERTS to "callable_get_file_header()"
	// BECAUSE pages == N_total after last alloc done and dusted
	//

		if(!file_header()) return FAIL_FAILED();
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
	//	if(!page_header( page_wh, ++ )) return FAIL_FAILED();

		if(!list_end( /* page_wh, pg_no */ )) return FAIL_FAILED();

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

		XY_t BL( 0,         0 );
		XY_t TR( page_wh.x, page_wh.y ); 
		XY_t BR( 0,         page_wh.y );
		XY_t TL( page_wh.x, 0 );

		xy_moveto( BL );
		xy_lineto( TR );
		xy_lineto( BR );
		xy_lineto( TL );
#if 0
		// last point is the first // becomes closed loop
		xy_moveto( BL );
#else
		// last point is the first // becomes closed loop
		closepath(); // CALL it NOW
#endif
		setlinewidth( 10 );
		stroke();

		newpath();

		XY_t XY_20_20(30, 30);
		XY_t XY_30_30(30, 30);
		XY_t XY_30_15(30, 15);
		xy_str( XY_30_30, "PAGE ONE"); // page[1] //
		times_ptsz( 10 );
		xy_str( XY_30_15, "with backslash-ed parenths () ");

		showpage();
		page_wh.set( 30, 50 ); // set page size before

		// newpath(); // somewhere_API PATH_name.CTOR( CTXT, CODE, DATA )
		// { XY_TREE } // STEP = { XY ID } // ID = idx_of_step_in_list //
		times_ptsz( 20 );
		xy_str( XY_20_20, "PAGE2 TWO");
		showpage();

	//	if(! out.print("THIS IS TEXT\n") ) return FAIL_FAILED();
	//	if(! write_to_stdout() ) return FAIL_FAILED();
		if(! write_normalise_pdf_view() ) return FAIL_FAILED();
		return true;
	}

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

