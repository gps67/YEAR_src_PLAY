#ifndef pdf_base_H
#define pdf_base_H

#include <stdio.h>
#include <stdlib.h>

class pdf_base;

#include "pdf_base0.h"
#include "pdf_styles.h"
#include "axes_calc.h"
#include "str0.h"
// #include "pdf_pen_base.h"

#include "pdf_ctxt.h"

namespace PDF_GEN {

class pdf_pen_base;

struct pdf_WH_t {
	float W;
	float H;

	pdf_WH_t()
	: W(0)
	, H(0)
	{
		// INIT NULL gets complex when f64 is invoked
	}
	void FLIP_WH() {
		float T = W; W = H; H = T;
	}
	bool PRINT_WH(buffer2 & buf, const char * fmt_WH = 0 ) {
		if(!fmt_WH) fmt_WH = "{ %f %f }";
		buf.print(fmt_WH, W, H );
		return true; // goto_RET_OKAY_ZERO _OKAY_ZERO
	}
};
typedef pdf_WH_t  WH_t; // proper naming arguments // press go //

//!	
/*!
	pdf_base is the base class of your PDF document generator
	adds (20) styles
*/
class pdf_base : public pdf_base0
{
 public:

	bool landscape;

	~pdf_base();
	pdf_base( pdf_ctxt & ctxt );

	/*!
	*/
	Axes_Calc axes;

	/*!
		the current settings

		// only a pointer because pdf_obj_js base
	*/
	obj_hold<Style_Settings> style_settings;

	pdf_pen_base * pen_cache;

	/*!
		the current mode is the type of drawing that is going on,
		this sort of follows the PDF mode, and some further reading is required
		as I am still getting occasional message like:

// PDF: FATAL: function 'PDF_setfont' must not be called in 'path' scope        

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
		mode_both
	} Mode;

	/*!
		The current Mode
	*/
	Mode mode;

	bool goto_mode( Mode m );
	bool goto_mode_none();

	void unset_current_pen( pdf_pen_base * pen )
	{
		if( pen_cache == pen ) set_current_pen_fn( NULL );
	}

	void set_current_pen( pdf_pen_base * pen )
	{
		if( pen_cache != pen ) set_current_pen_fn( pen );
	}
	void set_current_pen_fn( pdf_pen_base * pen );

	pdf_WH_t page_WH;
	/*!
		device units - 72 *
	*/
	// float page_width;
	#define page_width page_WH.W

	/*!
		device units - 72 *
	*/
	// float page_height;
	#define page_height page_WH.H

	/*!
		pdf_base has had PDF_CTXT available for pen creation ETC,
		but now wants to do something more complex in a setjmp environ,
		so PDF_CTXT calls this virtual function. You can only have
		one function per class, but you can setup any vars,
		and easily override the VTBL.

		The alternative is to move away from setjmp to exception
		handling, which calls setjmp for every PDF function!
	*/
	virtual bool run_wrapped_report() = 0;


	bool open_file(
		str0 filename,
		str0 title,
		str0 author,
		str0 creator
	);
	void close_file();

	// A£ is an external const but PDF might have its own pixel point 
	bool set_WH_A3( pdf_WH_t & WH_A3 ); 
	bool set_WH_A4( pdf_WH_t & WH_A4 ); 

	bool page_open;
	bool begin_page_WH( pdf_WH_t WH );
	bool begin_page();
	bool begin_page_a4();
	bool begin_page_a4_landscape();
	bool begin_page_a3_landscape();
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
