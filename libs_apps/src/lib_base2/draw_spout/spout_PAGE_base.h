#ifndef spout_base_H
#define spout_base_H

#include "spout_STUBS.h"

#include <stdio.h>
#include <stdlib.h>

class spout_PAGE_base;

// #include "spout_base_0.h"
#include "spout_styles.h"
#include "axes_calc.h"
#include "str0.h"
// #include "spout_pen_base.h"

// #include "spout_ctxt.h"

namespace SPOUT_GEN {

class spout_pen_base;

//!	
/*!
	spout_PAGE_base is the base class of your SPOUT document generator

		all the way from API to PDF // uses ps_to_pdf

		PAGE is what we are working on with SPOUT.CSR.PRINT_HEAD.PAGE
		PAGE is SPOUT.CSR.PRINT_HEAD.PAGE
		PAGE is SPOUT.DOC.PAGE

		DRAW is SPOUT.DRAW
		DIAG is GIT EDIT SCRIPT UDEF // or ROM Module
	
	SPOUT is a PS printer 

		PHRASE_BOOK API DRAW PLUS

		SPOUT += FONT // auto_add one_new_instance
		SPOUT += FGBG // RGBA_32 
	
	SPOUT is a PAGE _Layout

		PALETTE of FGBG PEN INK PAPER ... whose list is this ... 
		CSR is PRINT_HEAD
		CSR += _Layout // RUN DATE GIT DATE FILE LINE 

	SPOUT offers INDEX scripts EXPR's and MACROS

		FILTER suggests MATCHED ...
		DISPLAY has strange API requirements
		REDRAW optimised XYWH VIEW 

	It brings a multi-layered list of (ATTR) = (.type .name .cache ...)
	
	 ITEM exists on mention
	 TYPE exists on declare _t 
	 SESS exists on MMAP_FILE // shared DATA except SESS . fields

	 PAGE += _Layout
	 PAGE += _Draw
	 PAGE += _
	 PAGE += _DIAG
	
	SPOUT is an output stream -ed document 

	  Layout 
	  _PAGE_Layout _PAGE_
	  _DRAW_Layout _DRAW_ _Layout

	adds (20) styles

	spout_PAGE_base is a multi faced page Layout base class
	spout_csr is _base _sess // _dpi // _diag //

	_Layout {
	  ... Sprites Steps Fonts Lines ... 
	}

	_Layout {
	  ... PEN INK PAPER BOOK SHELF UDEF ...
	  ... PEN DRAW XYWH XPOS TREE ZONE ...
	  ... PRINT_PAGE
	  ... PRINT_HEAD ...
	  ... PRINT_PEN ...
	  ... csr ...
	}

	// typedef spout_PAGE_base _plus; // plus is %s_%s UDEF PLUS
	// typedef spout_PAGE_base spout_pen_plus; // plus is %s_%s UDEF PLUS
	// typedef spout_PAGE_base spout_page_plus; // plus is %s_UDEF 

*/
class spout_PAGE_base : public spout_obj_ref
{
 public:
 	// SPOUT is an output PAGE _Layout

	~spout_PAGE_base();
	spout_PAGE_base();
	// spout_PAGE_base( spout_ctxt & ctxt );

	bool landscape;

	/*!
		UNIT Point_72_dpi
	*/
	Axes_Calc axes;

	/*!
		the current settings

		// only a pointer because spout_obj_js base
	*/
	obj_hold<Style_Settings> style_settings;

	/*!
		Retain _pen _ink _fgbg _and_so_on
	*/
	spout_pen_base * pen_cache;

	/*!
		the current mode is the type of drawing that is going on,
		this sort of follows the SPOUT mode, and some further reading is required
		as I am still getting occasional message like:

// SPOUT: FATAL: function 'SPOUT_setfont' must not be called in 'path' scope        

		This should be replaced with the function that will be called:
			fill_pending, ...

	*/
	typedef enum {
		mode_none,
		mode_line,
		mode_fill,	
		mode_text_fill,	//text
		mode_text_line,
		mode_text_fill_line,
		mode_both // ??
	} Mode;

	/*!
		The current Mode

		we envelope calling text draw MACROS
		the cache does that
		csr is print_head
		etching font_lines

		SETTER = void goto_mode(Mode mode)
	*/
	Mode mode;

	void goto_mode( Mode m ); // fflush(stdout) // nudge

	void unset_current_pen( spout_pen_base * pen )
	{
		if( pen_cache == pen ) set_current_pen_fn( NULL );
	}

	void set_current_pen( spout_pen_base * pen )
	{
		if( pen_cache != pen ) set_current_pen_fn( pen );
	}
	void set_current_pen_fn( spout_pen_base * pen );

	/*!
		device units - 72 *
	*/
	float page_width;

	/*!
		device units - 72 *
	*/
	float page_height;

	/*!
		spout_PAGE_base has had SPOUT_CTXT available for pen creation ETC,
		but now wants to do something more complex in a setjmp environ,
		so SPOUT_CTXT calls this virtual function. You can only have
		one function per class, but you can setup any vars,
		and easily override the VTBL.

		The alternative is to move away from setjmp to exception
		handling, which calls setjmp for every SPOUT function!
	*/
	virtual bool run_wrapped_report() = 0;


	void open_file(
		str0 filename,
		str0 title,
		str0 author,
		str0 creator
	);
	void close_file();

	bool page_open;
	void begin_page();
	void begin_page_a4();
	void begin_page_a4_landscape();
	// start new page ONLY if required
	void now_need_page()
	{
		if(!page_open) begin_page();
	}
	void now_need_landscape( bool _landscape = true );
	void now_need_portrait( bool _portrait = true ) { now_need_landscape( !_portrait ); }
	void end_page();
	void next_page();
	void axes_set();

	virtual void on_page_top()
	{
		// set y_pos = 0;
	}

	Style_Font_Face_Size * curr_font();

};

}; // NAMESPACE
#endif

