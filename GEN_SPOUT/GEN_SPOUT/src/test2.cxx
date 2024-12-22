
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
#include "tm_parts.h" // str_pdf()

#include "SPOUT_STUBS.h"
#include "SPOUT_argv_decoder.h"

#include "SPOUT_ink_rgb.h"
#include "SPOUT_XY.h"
#include "GEN_PS.h"

#define IF_NOT if(0) 

using namespace SPOUT;

/*!

	class test_SPOUT_base // : public obj_ref // SPEC_t SPEC; // CTXT //

		
		a PDF Postscript DOCUMENT from SPOUT


	class test_SPOUT_base // : public obj_ref // SPEC_t SPEC; // CTXT //

		a DOCUMENT _t

	class SPOUT : public test_SPOUT_base

		GEN_POSTSCRIPT _FGBG _DRAW

		GEN_PDF // += TABLE of { NAME VALUE } // PAIR[s]

		GEN_SEGMENT // += elf_layout_

		GEN_SEGMENT_DATA_WASM

		a DOCUMENT ( DATA, CODE ) // link %s "LEX_name" // _Layout
		# beyond SPOUT:: // CODE_LAYER_MACRO base64 within comments
		# SEGMENT // ELF_FILE :: TABLES SEGMENTS LOADERS SCHEME
		# UDEF // Item_t ITEM; // CMNT // PSG MATCHED //
		# CDEF // DECL NEW_VAR ARGV
		# DATA // VAR_POOL NAME DATA // LINK_LOCN
		# LOCN // LINK_LOCN_t LINK // LOCN NAME {SCRIPT}
		# ITEM // NEW_VAR // auto_build CTOR( ARGV )
		# VAR // TYPE_t NAME // line_tail_is_CTOR

		" %s_t %s " "TYPENAME" "VARNAME" // line_tail

			ITEM.DATA.SCRIPT 
			field_type //_t STRING_TOKEN // EXPR_near_TOKENS
			field_name //_t STRING_TOKEN // str1 or buffer or ...
			field_value //_t DATA

		GEN_FONT { glyph_t & glyphs[CSET]; // complex CSET_ARRAY // }
		// PSG MATCH ...
		// glyph_t & glyph = glyphs[idx] // idx = TABLE[ BYTE ] 
		// idx_t idx // auto_t varname // alias LHS RHS {CODEPOINT}
		// CSET_INFO_t CSET_INFO // ASCII_CTRL _ASCII_GLYPH UTF8_GLYPH
		// ASCII_CTRL
		//_ASCII_GLYPH
		// ASCII_CSET // CSET_t {CSET} // KEY_NAME // sparse_vars 
		// 8859_CSET // CSET_t {CSET} // KEY_NAME // sparse_vars 
		// CSET_TABLE // TABLE[idx] => OBJ { ... }
		// UTF8_GLYPH // draw {SCRIPT} {ARGV} {VARS}
		// 8859_GLYPH // LEX IDENT STARTES WITH DIGIT // pfx_NNNN_rhs
		// UTF8_GLYPH_SUBSET // draw {SCRIPT} {ARGV} {VARS} // is_a_font
		// FONT_GLYPH // FT2_INFO // 

		GEN_DRAW { DIAG_t DIAG } {
		 // uses VARS // autoimport via here attachment point
		 // join SCOPE // SELF + CTXT + IMPORT + STO + CONNECT + CALL
		 // 
		}

		GEN_SPOUT_Glyph {
		 # FONT # FONT_METRIC #
		 # TABLE ASCII_8859_UTF8_CSET_used
		 # TABLE PARSER_TOKEN MODULE {module}
		 # TABLE SEGMENT_TOC
		 # TBL SEGMENT_IDX_DATA_SPEC # DATA = N_BYTES_STO
		 # LEX OPT # u8 bytes[N] # BIND = by_placement( N_BYTPES_STO { N BYTES }
		 # API #
		 # FUNC_is_a_form_of_TABLE
		 gylph_t & glyph = font.select_glyph(idx) // _alias //
		 #
		 # DRAW #
		 OK == draw at  VARS // 
		 // VARS += XYZ DRAWABLE FGBG SURFACE SEGMENT SPEC
		 draw glyph at XYZ on SURFACE -etc- VARS
		 enq attr of glyph # ascent descent tick baseline_0 ...
		
		}


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

	GEN_PS
	GEN_Document // _t _pdf

	#if 0
		class test_SPOUT_base 
		: public obj_ref

			PTR_to_OBJ 
			REF_to_OBJ // eg idx_OBJ
			REF_COUNT_of_OBJ // ie ref_count or
			REF_KEPT_of_OBJ // during SESS
			// accumulate BATCH of UPDATED VALUES

			OPTION ref_count
			OPTION ref_keep
			OPTION GC_mark_as_active
			OPTION report all external edits as on_CHANGE
			OPTION report on_CHANGE to SELF # if NO_CHANGE return OK;
			OPTION report on_CHANGE to SELF # if NO_CHANGE WARN_CALLER
			OPTION LOG BOTHERED_for_NO_CHANGE_count

		OPTION obj_ref_counted_base // single BIND counts to 1 // 
		 OPTION DEBIND

		 SPOUT is GEN_PS 
		 SPOUT is GEN_STO_PARSE_RUN _Module

		 	_Module provides TEMPLATE for Module
		 	_Module provides TEMPLATE for USAGE // in_SAMPLE

		 SPOUT helps to prepare PAGE
		 SPOUT helps to prepare DTP PAGE BLOCK Z_TEXT LAYOUT PHRASE ITEM
		 SPOUT helps to prepare DIAG for BLOCK
		 SPOUT helps to prepare FOTO for BLOCK //
		 // labels borders ballons index_use // AUTO_ADJUST_TO_FRAME //

		 AUTO_ADJUST _to_FRAME

		 	OPTION transparent borders
		 	OPTION transparent main_area

			OPTION stretch_SELF_TO_FRAME
			OPTION scroll_SELF_within_FRAME // slightly used added

			OPTION change internal layout to suit FRAME
			OPTION enquire what does it mean to suit FRAME
			eg FRAME wants to resize to 100% SCREEN 
			eg FRAME wants to task track in SESSION

		REQUIRE LEX - to WORK as EXPECTED
		REQUEST LEX - triggers BENCH to search for LEX PROVIDERS
		SUGGEST LEX - triggers LIBR which claims all LEX USAGE
		PROVIDE LEX - LIBR provides LEX via API

			CT_RT architecture
			short-circuits N nested layers
			but that list of layers is available in CT_RT _NOW

			SECOND SCAN over TEXT 
			now using full PSG VIEW

				ironically it works near blind
				it is basically quick with locks and threads

			USAGE ARGV "{ PROVIDE LEX }"

				note how "ARGV" was applied to following "DATA"
				and DATA is ITEM, ARGV_of_ITEM_PHRASE_ITEM

		API LEX

			most of the time it is strong typing
			LEX_SPELLING is unique # OPTION is_unique_spelling
			LEX_SPELLING is confused # OPTION is_repeat_spelling
			// accidental alias but independent before confusion
			// AUTO_MERGE resolves a MINI namespace as an FSM //
			// we go for full separation of namespaces
			// we got our outer namespace as a tagged_this_year
			// within that we call stuff what we like, ASCII, utf8
			// CSET_BYTE _ASCII _LATIN _UTF8 _UTF8_FONT_one_latin
			// CSET CIDENT99 // RTFM // 

			MATCH SPELLING

				FILTER can BIND to UC_lc 
				FILTER can BIND to ALIAS tree

				exact match

				P0_N	// current design // P0 may be API_P0
				P0_P2	// MAGNET_MATCH P0_N from P2 = P0+N
				STR	// uses P0_N
				STR0	// requires second copy with a NUL

			MATCH SPELLING ANYSTR // P0_N_STR _CSET _from_SESS

				STR must now contain CSET
				STR may direct use of MMAP_stream_FILE

				CT_RT uses XPOS += DIR FILE OFFS
				CT_RT uses XPOS += DIR FILE LINE CPOS // glyph
				CT_RT uses XPOS += DIR FILE LINE BPOS // byte

				i64 {
				 opcode "{ XPOS += DIR FILE LINE BPOS }"
				 MINI_MACHINE buffer OFFS_in_FILE_of_BOLN CPOS
				 FLAG AVAR as updated_but_no_NEW_VAL
				 FLAG AVAR as ZAP_OLD_VAL // forgotten API

				 	on_DELETE OLD_VAL 
					on_RELEASE AVAR("OLD_VAL")
					on_LOG_ALL_EDITS_before_MERGE_EDITS
					on_LOG_ALL_EDITS_after_MERGE_EDITS

					// _one // STREAM
					// _two // MMAP_FILE // unaligned
					_VFS unnaligned_TEXT_in_HEAP_of_FILES
					_VFS_FILE_SEGMENTS eg ISO in ISO_in_EXT

				 opcode "{ XPOS += DIR FILE LINE BPOS }"

				  XPOS

				  DIR
				  FILE

				  LINE		u23 or switch up

				  BPOS		127 or switch to _two

	MACRO 
		OFFS_of_LINE
		OFFS_of_LINE_plus_BPOS

		EDGE_CHECK
		OFFS_of_NEXT_LINE // or OUTSIDE or LINE_P2
		
		P0 = OFFS_of_LINE // add_weight interpret as P0 of P0P2
		P2 = OFFS_of_EOF // add_weight interpret as P2 of P0P2


				}


		
	#endif
*/
class test_SPOUT_base // : public obj_ref
{
 public:

	GEN_PS_t PS;


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
	//	PS.out.get_space( 1024 * 32 ); // 32K first malloc // out buffer
		PS.DOC_ARGS.pages = 0;
		PS.PAGE.page_no = 0; // page_no = idx = ITEM = N++ //
		PS.PAGE.page_wh.set( 595, 842 );
//		PS.PAGE.in_path = false;
//		PS.PAGE.in_page = false;
		
		// CTOR done elsewhere // PS( out );
	}

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
		PASS("system('%s')", (STR0) cmd );
		return true;
	}

	/*!
		TEST: write TEXT to file, call pdfview
	*/
	bool write_normalise_pdf_view() // get .out TEXT
	{
		// decide on the filenames, call ps2pdf then viewer
		// actually it is already in buf
		// TODO print headers then main text
		STR0 BASENAME = "/tmp/test1_tmp_";
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

		// save blk as PS file
		PS.write_to_file( (STR0) name_1 );

		// shared cmd ARGV as STR0
		buffer2 cmd;

	 if(1) { // test normalising PS though some
		cmd.print("ps2ps %s %s", (STR0) name_1, (STR0) name_2);
		if(!system_cmd( cmd ) ) return FAIL_FAILED();
	}

	 if(1) { // gen PDF
		cmd.clear();
		cmd.print("ps2pdf %s %s", (STR0) name_1, (STR0) name_3);
		if(!system_cmd( cmd ) ) return FAIL_FAILED();
	}

	 if(0) { // view PS ascii
		cmd.clear();
		cmd.print("cat %s", (STR0) name_1);
		if(!system_cmd( cmd ) ) return FAIL_FAILED();
	 }

	 if(1) { // view PDF pages
		cmd.clear();
		cmd.print("mupdf %s", (STR0) name_3);
		if(!system_cmd( cmd ) ) return FAIL_FAILED();
	 }

		return true;
	}

};

/*!
	test_SPOUT_base 
*/
class test_SPOUT :public test_SPOUT_base // : public obj_ref
{
 public:
	bool test1()
	{
		PS.PAGE.page_wh.set( 50, 30 ); // set page size before
		PS.PAGE.page_wh.set( 594, 841 ); // just shy of ...
		PS.PAGE.page_wh.set( 500, 700 ); // within screen
	// do this at END of document first !!
	// KEEP vars_api of SCRIPT
	//  vars_api auto_keep scripts varso
	//   vars_api_pre_compile # run with samples list # cache 
	//    var_cache_pre_built += ... exprs used ...
	//	STRUCT { ... } // lucky C++ SCRIPT Name
	//	 NODE_ID = ITEM_ID = LOCN_ID
	//     Name {

		// magic opinion Pages == 2
		PS.DOC_ARGS.pages = 2;

		PS.DOC_ARGS.DOC_Title = "ABC_SUMMARY";
		PS.DOC_ARGS.DOC_Creator  = "SPOUT";
		PS.DOC_ARGS.DOC_Creator_version  = "-1";

	//	}; // Name
	//	Export Name to API as struct PTR 
	//	ALIAS PTR REF
	//	ALIAS REF PTR Name { ... } // Name provides { ... }


	// DIVERTS to "callable_get_file_header()"
	// BECAUSE pages == N_total after last alloc done and dusted
	//

		if(!PS.file_header()) return FAIL_FAILED();
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

		if(!PS.no_showpage_but_begin_first_page()) return FAIL_FAILED();
	//	if(!page_header( PAGE.page_wh, ++ )) return FAIL_FAILED();

		if(!list_end( /* PAGE.page_wh, PAGE.page_no */ )) return FAIL_FAILED();

		PS.comment("This is a modified comment");
		PS.times_ptsz( 20 );

		PS.setrgbcolor( PS.PAGE.rgb.set_green() );
		PS.setrgbcolor( PS.PAGE.rgb.set_blue() );
		PS.setrgbcolor( PS.PAGE.rgb );
		// want the above 3 cached
		// list_end() style // 
		// next command does goto_its mode // clears previous mode
		// first each setrgbcolour sets mode ACTIVE_BIT_NOW( bitmask )
		// each leaves reside WANT RGB, it has changed, most recent bit
		// last knows by something else, dislodging, 
		// flush now or later
		// for now avoid repeat calls

		PS.setlinewidth( 10 );

		XY_t BL( 0,         0 );
		XY_t TR( PS.PAGE.page_wh.x, PS.PAGE.page_wh.y ); 
		XY_t BR( 0,         PS.PAGE.page_wh.y );
		XY_t TL( PS.PAGE.page_wh.x, 0 );

		PS.xy_moveto( BL );
		PS.newpath();
		PS.xy_lineto( TR );
		PS.xy_moveto( BR );
	//	PS.xy_lineto( BR );
		PS.xy_lineto( TL );
#if 0
		// last point is the first // becomes closed loop
		PS.xy_moveto( BL );
#else
		// last point is the first // becomes closed loop
		PS.closepath(); // CALL it NOW
#endif
		PS.stroke();

		PS.newpath();

		XY_t XY_20_20(30, 30);
		XY_t XY_30_30(30, 30);
		XY_t XY_30_15(30, 15);
		PS.xy_str( XY_30_30, "PAGE ONE"); // page[1] //
		PS.times_ptsz( 10 );
		PS.xy_str( XY_30_15, "with backslash-ed parenths () ");

		PS.showpage();
		PS.PAGE.page_wh.set( 30, 50 ); // set page size before

		// newpath(); // somewhere_API PATH_name.CTOR( CTXT, CODE, DATA )
		// { XY_TREE } // STEP = { XY ID } // ID = idx_of_step_in_list //
		PS.times_ptsz( 20 );
		PS.xy_str( XY_20_20, "PAGE2 TWO");
		PS.showpage();

	//	if(! out.print("THIS IS TEXT\n") ) return FAIL_FAILED();
	//	if(! write_to_stdout() ) return FAIL_FAILED();
		if(! write_normalise_pdf_view() ) return FAIL_FAILED();
		return true;
	}

}; // class
using namespace SPOUT;

/*!
	bool SPOUTER_test1( argv_decoder & ARGS )

	 TEXT_BLOCK_HERE CMNT_TEXT

	 // test1 creates an inst of test_SPOUT

	 // SPOUTER // test_SPOUT // _t 

	 // USES API %s.test1() || FAILED // IE OK IFNOT IF_OK()
*/
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

