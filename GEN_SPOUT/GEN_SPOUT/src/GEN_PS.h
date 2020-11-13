#ifndef GEN_PS_H
#define GEN_PS_H

#define IF_NOT if(0) 

#include "GEN_TOP.h"

namespace SPOUT {

 struct GEN_PS_t {

	// too many places to put this - so base class
	GEN_DOC_ARGS_t DOC_ARGS;
	GEN_PAGE_t PAGE;

	// output GEN_PS writes to a buffer
	buffer2 out;

	// output has pathbuilder helpers
	bool in_path;
	
	// per document flag
	// naff trick where first showpage does not showpage
	// so can call showpage at START of page
	// so same counter is incremented

	bool FOUND_FIRST_showpage;

	GEN_PS_t() {
		// CTOR
		out.get_space( 1024 * 32 ); // 32K first malloc // out buffer
		in_path = false;
//		in_page = false;
		FOUND_FIRST_showpage = false;
	}

	bool comment( str0 cmnt ) {
		return out.print("%% %s\n", (STR0) cmnt ); // with leading SP
	}

	bool printf( STR0 fmt, ... ) { // ASIF // GEN_PS.printf( fmt, ... )
		// standard printf code
		va_list args;
		va_start( args, fmt );
		// into_utf8 = false // from what ? // meaning what ?
		bool ok = out.vprint( false, fmt, args );
		va_end( args );
		return ok;
	}
	
	bool put_EOLN()
	{
		return out.put_LF();
	}

	bool cmd_line( STR0 fmt, ... ) { // ASIF // GEN_PS.printf( fmt, ... )

		// standard printf code
		va_list args;
		va_start( args, fmt );
		// into_utf8 = false // from what ? // meaning what ?
		bool ok = out.vprint( false, fmt, args );
		va_end( args );

		// printf + EOLN
	 if(0) // check syntax // compiles OK //
		ok = ok && put_EOLN(); // looks like bitwise - but is && ??
	//	ok &&= put_EOLN(); // bad syntax
		ok &= put_EOLN(); // looks like bitwise - but is && ??
		return ok;
	}

	bool hdr_comment( STR0 fmt, ... ) {
		out.put_byte('%');

		// PRESUME output is plain text partline
		// standard printf code
		va_list args;
		va_start( args, fmt );
		// false = conv_8859_to_utf8 // upgrade somehow
		bool ok = out.vprint( false, fmt, args );
		va_end( args );

		put_EOLN();
		return ok;
	}

	// dsc_
	bool dsc_comment( STR0 fmt, ... ) {
		// Document Structure Conventions
		// % % NOSP Word 
		// % % NOSP Word COLON SP ...
		// COLON considered part of keyword
		// cross refer to PPD PS printer desc
		out.put_2_bytes('%', '%'); // double % 
		// printf of %  would be %%
		// printf of %% would be %%%%

		// PRESUME output is plain text partline
		// standard printf code
		va_list args;
		va_start( args, fmt );
		// it needs a FILTER option to distinguish vprint
		// false = conv_8859_to_utf8 // upgrade somehow
		bool ok = out.vprint( false, fmt, args );
		va_end( args );

		put_EOLN();
		return ok;
	}

	/*
		%% name : SP value EOLN
	*/
	bool hdr_name_value( STR0 name, STR0 val ) {
		INFO("%s: %s", name, val );
		return dsc_comment("%s: %s", name, val );
	}

	bool hdr_blank_line() { return hdr_comment(""); }
	bool cmd_blank_line() { return cmd_line(""); }

	bool hdr_PageSize_device( XY_t & wh )
	{
		cmd_line("<</PageSize [ %d %d ]>> setpagedevice", (int) wh.x, (int) wh.y);
		return true;
	}

	//
	// this might be virtual, or one of a range
	//
	bool file_header(
//		GEN_DOC_ARGS_t & DOC_ARGS,
//		GEN_PAGE_t & PAGE
	) {
		// must know Pages == 2 before starting output!!
		// TEMPLATE POSTSCRIPT suggested by groff template text
		// example_ps // man -Tps ascii | less
		// example_test_SPOUT_test1.ps // .pdf // GEN_POSTSCRIPT
		// src/PLAY/GEN_SPOUT/GEN_SPOUT/eg_less_troff_
		// viewer_show_example_template // man -Tps ascii | less
	/* It says

		%!PS-Adobe-3.0
		%%Creator: groff version 1.22.4
		%%CreationDate: Sun Nov  1 20:43:07 2020
		%%DocumentNeededResources: font Times-Roman
		%%+ font Times-Bold
		%%+ font Courier
		%%+ font Times-Italic
		%%DocumentSuppliedResources: procset grops 1.22 4
		%%Pages: 3
		%%PageOrder: Ascend
		%%DocumentMedia: Default 595 842 0 () ()
		%%Orientation: Portrait
		%%EndComments
		%%BeginDefaults
		%%PageMedia: Default

	*/

		// see page 617 of PLRM2
		// Appendix G
		// Document Structuring and Conventions 
		// Version 3.0
		// see 2.4.2.Program Structure
		// see page 627 of PLRM2
		hdr_comment("!PS-Adobe-3.0");

		// PUTTING TO FRONT for a while
		if(DOC_ARGS.DOC_Title) {
			hdr_name_value("Title", (STR0) DOC_ARGS.DOC_Title );
		}

////		%%Creator: SPOUT version -1
		dsc_comment("%s: %s version %s", "Creator", 
			(STR0) DOC_ARGS.DOC_Creator,
			(STR0) DOC_ARGS.DOC_Creator_version
		);

////		%%CreationDate: Sun Nov  1 20:43:07 2020
		tm_parts tm;
		hdr_name_value("CreationDate", (STR0)tm.str_pdf_now() );

		// standard list of fonts // expand
		hdr_name_value("DocumentNeededResources", "font Times-Roman");
		dsc_comment("+ font Times-Bold");
		dsc_comment("+ font Times-Italic");
		dsc_comment("+ font Courier");

		// hdr_name_value but decimal not string
		dsc_comment("Pages: %d", DOC_ARGS.pages );

//		%%PageOrder: Ascend
		hdr_name_value("PageOrder", "Ascend" );

//		what is the extra 0
//		%%DocumentMedia: Default 595 842 0 () ()
		dsc_comment("DocumentMedia: Default %.0f %.0f 0 () ()",
			PAGE.page_wh.x, 
			PAGE.page_wh.y );

//		%%Orientation: Portrait
		hdr_name_value("Orientation", "Portrait" );


		// ASIDE dumpted here for vprint users
		// double %% becomes % in vprintf // or use hdr2_
		// hdr2_comments adds %% and /n // and vprint fmt
		// hdr_comments adds % and /n // and vprint fmt
// IF_NOT

		hdr_name_value("LanguageLevel", "2" );

		dsc_comment( "EndComments" );
		cmd_blank_line();

		dsc_comment( "BeginProlog" );
		dsc_comment( "EndProlog" );
		cmd_blank_line();

		dsc_comment( "BeginSetup" );

	// THIS ONE // set the page size
		// "<</PageSize [ %d %d ]>> setpagedevice", PAGE.page_wh.x, PAGE.page_wh.y );
		hdr_PageSize_device( PAGE.page_wh );

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

	bool fontname_ptsz( STR0 fontname, int ptsz )
	{
		return
		 cmd_line("/%s findfont %d scalefont setfont\n",
		  fontname,
		  ptsz
		 );
	}

	bool times_ptsz( int ptsz )
	{
		return fontname_ptsz( "Times-Roman", ptsz );
	}

#if 0
		return fontname_ptsz( "Times-Roman", ptsz );
		return fontname_ptsz( "Times-Bold", ptsz );
		return fontname_ptsz( "Times-Italic", ptsz );
		return fontname_ptsz( "Courier", ptsz );
		// Times- Courier- 
		// -Roman -Bold -Italic
#endif

	bool parenth_text( str0 text )
	{
		if(!text) {
			// out.print("()");
			return PASS("but empty text");
		}
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
			cmd_word("newpath");
		} else {
			WARN("already in_PATH");
		}
		return true;
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
	//	// DOCUMENT HEAD TABLES SEGMENT(".bss") 
	//	// 
	//	// START MODE DRAW_STUFF
	//	// 
	//	newpath_by_now(); // AUTO GEN newpath() // call now
	//	GEN routes out "newpath" was done at top of zone
	//
	//	// line_color // FGBG settings ... // GEN_SECTIONS_LISTS
	//	GEN r g b setrgbcolor
	//	
	//	GEN newpath // was done at top of zone
	//	GEN CSR // HERE // FILE FUNC ARGV CALLER 
	//	GEN X Y moveto // the one and only // this function
	//	GEN X Y lineto // STAR
	//	GEN X Y lineto // STAR
	//	GEN closepath // back to first_pos xy // 
	//
	//	GEN %d genlinewidth
	//	GEN stroke

	//	this first ZERO(0,0) is first (last) XY_POINT on path
	//	or start from (X,Y) // XY _t //
	//	
	//	but no need for MOVE ?
		newpath_by_now(); // extra tracer
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

	bool no_showpage_but_begin_first_page(
//		GEN_DOC_ARGS_t & DOC_ARGS,
//		GEN_PAGE_t & PAGE
	) {
		return showpage();
	//	return showpage(DOC_ARGS,PAGE);
	}

//	bool page_header( GEN_PAGE_t & PAGE )
	bool page_header( )
	{

	/*
		PLRM2 pg 628
	*/

#if 0

		dsc_comment( "PageSize: [ %f %f ]", PAGE.page_wh.x, PAGE.page_wh.y );
		cmd_line( "/PageSize [ %f %f ]", PAGE.page_wh.x, PAGE.page_wh.y );
#endif

		cmd_blank_line();
		STR0 label = "PAGE_ONE";
		if(PAGE.page_no == 1)
			label = "PAGE_TWO";
		dsc_comment( "Page: (%s), %d", label, PAGE.page_no );

		hdr_blank_line();
IF_NOT		dsc_comment( "PageBoundingBox: 0 0 %d %d",
			(int) PAGE.page_wh.x,
			(int) PAGE.page_wh.y );
		dsc_comment( "BeginPageSetup");

		// BEGIN PAGE CODE HERE

		hdr_blank_line();
		dsc_comment( "EndPageSetup");
		hdr_blank_line();
IF_NOT		dsc_comment( "BoundingBox: %d %d %d %d",
			0, 0, (int) PAGE.page_wh.x, (int) PAGE.page_wh.y );
IF_NOT		dsc_comment( "DocumentMedia: %d %d () ()", (int) PAGE.page_wh.x, (int) PAGE.page_wh.y );

		dsc_comment( "EndComments" );
		cmd_blank_line();
//		dsc_comment( "BeginProlog" );
		return true; // or ask out if it has had mem errors
	}

	bool showpage(
	//	GEN_DOC_ARGS_t & DOC_ARGS,
	//	GEN_PAGE_t & PAGE
	) {
		PAGE.page_no = DOC_ARGS.pages ++ ; // MATCH idx = N ++ // scope
		// PAGE.page_no == 0 or more u24 or u4 or see item_expr
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
	//	pages_plus_on_showpage(DOC_ARGS,PAGE); // AUTO CONNECT bool plumbing //
		pages_plus_on_showpage(); // AUTO CONNECT bool plumbing //
		return true;
	}
	bool pages_plus_on_showpage( 
	//	GEN_DOC_ARGS_t & DOC_ARGS,
	//	GEN_PAGE_t & PAGE
	) {
	//	return on_showpage(PAGE);
		return on_showpage();
	}
//	bool on_showpage( GEN_PAGE_t & PAGE )
	bool on_showpage( )
	{
	//	if(!page_header( PAGE )) return FAIL_FAILED();
		if(!page_header( )) return FAIL_FAILED();
		return true;
		/*
			The previous PAGE has finished
			maybe some cache builders mid flow

		*/
	}

	bool	closepath() {
		cmd_word("closepath"); // path exists until next newpath
		end_in_PATH(); // but we soon need another newpath
		// there is stilll "fill" to do // outline etc
		return true;
	}

	bool	fill() { return cmd_word("fill"); }

	bool write_to_stdout()
	{
		write(1, out.buff, out.nbytes_used);
		return true;
	}
	
	bool write_to_file( STR0 name_1 ) {
		return blk_write_to_file( out, (STR0) name_1 );
	}

 }; // class
}; // namespace

#endif
