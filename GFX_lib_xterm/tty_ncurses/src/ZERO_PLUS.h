#ifndef ZERO_PLUS_H
#define ZERO_PLUS_H

namespace ZERO_PLUS {

	// coordinate system of tty
	//
	//	typedef int NUM; // int or float or int64 or double 
	//	// ncurses is int character cells zero_top_left 25x80 51x80
	//
	// coordinate system of x11
	//  ONE OF
	//   DTP_AFM_1000
	//   DTP_dpi_150 // SCREEN +- SQUINTY
	//   DTP_pdf_unit = float & unit = SCRIPT_unit_pallette_item_THREE
	//   DTP THREE = this_DOC_pdf_style_by_name("anystr")
	//   DTP THREE = lookup("DIAG_SIZE_unit_is_72_dpi");
	//   GEN "DIAG_SIZE_unit_is_72_dpi" from "dpi = 72;"
	//   GEN detects "dpi = 72;" ALSO "set_dpi(dpi)" ALSO "adj_dpi_ONE(1.5)"
	//   GEN presents shortlist of well known values
	//   DTP_pdf_float_pt
	//   DTP_pdf_float_mm
	//   DTP_pdf_float_unit
	//   DTP_pdf_float_glyph
	//   DTP_pdf_int_pt
	//   DTP_pdf_int_mm
	//   DTP_pdf_int_unit
	//   DTP_pdf_int_glyph
	//   ZERO.set_dpi( 72 ); // also the default UNIT
	//   ZERO.unit.set_dpi( 7 ); // int glyph_step_unit
	//   ZERO.unit.set_mode_glyph_step_unit(); // dpi == 5
	//
	// coordinate system of pdf
	//
	//  float xpos_pt;
	//  float ypos_pt;
	//
	//  typedef float NUM;
	//

	// namespace usgae
	//
	// DTP_pdf_int_pt // is a parallel namespace to 
	// DTP_pdf_int_unit // is a parallel namespace to 
	//
	// dont import * do call namespace.CTOR and the rest follows behind NS
	//
	// all of the template XY<float> combinations
	// typedef XY_<int_pixel_pos> XY; // ZERO_top_left
	// 

 typedef int NUM;

 struct XY {
  int x;
  int y;
 };
 struct WH {
  int w;
  int h;
 };
 struct P0P2 {
  int P0;
  int P2;
 };
		
};

#endif
