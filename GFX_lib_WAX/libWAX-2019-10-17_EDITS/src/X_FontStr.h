#ifndef X_FontStr_H
#define X_FontStr_H

#include "buffer1.h"

// We rely on this having been included already ?
// #include "X_STUBS.h"

namespace WAX {

 struct X_FontStr 
 {
	buffer1 full_name;	// 1
	buffer1 foundry;	// 2
	buffer1 family;		// 3
	buffer1 weight;		// 4 medium bold ?NORM? [demibold]
	buffer1 slant;		// 5 roman italic oblique
	buffer1 set_width;	// 6 normal
	buffer1 unknown;	// 7
	buffer1 pixels;		// 8 PIXEL_SIZE
	buffer1 point_10;	// 9 point_size in tenths
	buffer1 dpi_h;		// 10 RESOLUTION_X
	buffer1 dpi_v;		// 11 RESOLUTION_Y
	buffer1 spacing;	// 12 m==mono p==prop
	buffer1 width_10;	// 13 AVERAGE_WIDTH
	buffer1 cset;		// 14 may include - ? iso8859-1

	void init_all( const char * val);

	void init_star() {
	 init_all( "*" );
	}

	const char * join();
	void print_fields();

// PRE_PARSED into TOKENIXED usage_here SUBLEX of VAR accessed via FUNC CALL

	void set_bold_on() { set_weight_bold(); }
	void set_bold_off() { set_weight_normal(); }

// Layout gets hinted to one_line these functions to COMPILE_able PRE_PARSED
// VIEW fades-out stray_details leaves_head_lines note_attached_says

	void set_weight( const char * str ) { weight = str; }
	void set_weight_bold() { set_weight("bold"); }
	void set_weight_medium() { set_weight("medium"); }
//	void set_weight_normal() { set_weight("normal"); }
	void set_weight_normal() { set_weight("medium"); }

	void set_slant_italic() { slant = "i"; }
	void set_slant_roman() { slant = "r"; } // ie normal

	void set_width_normal() { set_width = "normal"; } // helvetica

// run FSM + STO + _MACHINE_ + CXX_Class + SCRIPT
// API_MAPPING local_var repeats_upstream PKT_IN PKT_TO

	bool parse( const char * str ); // a vary narrow syntax csv('-') //X11

// only using C++ for global variable names, dpi_h_CMNT = "floating_in_space"; 
// also using C with classes
// also using SCRIPT FILTER
// GEN_CODE {
//  HERE
// }


//	void set_dpi_150() { dpi_h = "150"; dpi_v = "150"; } // difference ?

	void set_dpi_150() { set_dpi("150"); }
	void set_dpi_100() { set_dpi("100"); }
	void set_dpi_75() { set_dpi("75"); }

	void set_dpi(const char * dpi) {
	 dpi_h = dpi;
	 dpi_v = dpi;
	}

	void set_dpi(float dpi) {
	 dpi_h.clear();
	 dpi_h.printf("%d", (int) (dpi*10) );
	 dpi_v.set( dpi_h );
	}

	void set_point(const char * pt) {
	 point_10.clear();
	 point_10.printf("%s0", pt );
	}

	void set_point(float pt) { // point_10 = pt * 10 // as_str
	 point_10.clear();
	 point_10.printf("%d", (int) (pt*10) );
	}

	void set_cset_latin1() { cset = "iso8859-1"; }

	void set_usable_defaults();

	void set_courier();
	void set_helvetica();
	void set_scalable(); // put zero as 

 };

}

#endif


