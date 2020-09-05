
#include "dgb.h"

#include "ft2.h"

// file:///nfs/NAS2/mnt/HD/HD_b2/2020/src_build_2020/DTP/freetype-2.10.2/docs/reference/ft2-basic_types.html#ft_bitmap

// https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_glyphslotrec

using namespace FT2;

 ft2 :: ft2() {
 	if(!init()) throw "FT2_INIT";
 }

 ft2 :: ~ft2() {
 	if(!done()) throw "FT2_DONE";
 }

 bool ft2 :: init() {
 	int err = FT_Init_FreeType( &library );  	
	if(err) {
		return FAIL("FT_Init_FreeType(&libr)");
	}
	PASS("FT_Init_FreeType");
	return true;
 }

 bool ft2 :: done() {
 	int err = FT_Done_FreeType( library );  	
	if(err) {
		return FAIL("FT_Done_FreeType(&libr)");
	}
	PASS("FT_Done_FreeType");
	return true;
 }

 bool ft2 :: face1_load_font( STR0 filename ) {
	static const int idx_0 = 0; // first font in file
 	int err = FT_New_Face( library, filename, idx_0, &face );  	
	if(err) {
		return FAIL("FT_New_Face()");
	}
	PASS("FT_new_face(%s)", filename );
	return true;
  // TODO:
  // load font over network or ...
  // FT_New_Memory_Face( library, font_file_data, font_file_size, idx, &face )
 }


#define int_from_fixed_6(x) ((x+32)>>6) // want set bit 5

 bool ft2 :: test1() {
 	STR0 font_file = 
	"/usr/share/fonts/truetype/liberation2/LiberationMono-Regular.ttf";
	int err;

 	if (!face1_load_font( font_file )) return FAIL_FAILED();

	// 64 fixed point <<6 // 16 ??
	// FT_Set_Char_Size( face, w0, h_16x64, dpi_h_300, dpi_v_300 )
	// ppem // pixels per em // 16 is 12pt ?
	int w0 = 0; // same as h
	int point_size = 36;
	int h_16x64 = point_size * 64;
	int dpi_x_300 = 100;
	int dpi_y_300 = 100;
	err = FT_Set_Char_Size( face, w0, h_16x64, dpi_x_300, dpi_y_300 );
	if( err ) return FAIL("FT_set_Char_Size()");

	// FT_Set_Pixel_Sizes( face, w0, h16 )
	// face->fixed_sizes // lists allowed

	int charcode = '@'; // 32 bit utf-32
	int glyph_index = FT_Get_Char_Index( face, charcode );

	int load_flags =  FT_LOAD_DEFAULT; // 0 // with bitmap
	err = FT_Load_Glyph(
		face,
		glyph_index,
		load_flags
	);
	if(err) return FAIL("FT_Load_Glyph()");

#define face_glyph_metrics face->glyph->metrics
// SEARCH REPLACE OPTION Optimising_Instance common_expr

//DIALECT
// FT_Pos face_glyph_metrics.%s field_name "horiBearingX";
//
//	EXPR = face->glyph->metrics;
//

	FT_Pos left  =        face->glyph->metrics.horiBearingX;
	FT_Pos right = left + face->glyph->metrics.width;
	FT_Pos top   =        face->glyph->metrics.horiBearingY;
	FT_Pos bottom = top - face->glyph->metrics.height;


	int glyph_rect_w = int_from_fixed_6(right - left) + 1;
	int glyph_rect_h = int_from_fixed_6(top - bottom) + 1;

	#if 0
	setFixedSize(
	 glyph_rect_w,
	 glyph_rect_h
	);
	m_glyphRect.width(),
	m_glyphRect.height());
	#endif

	WARN("TODO");

	return PASS("OK");
	return true;
 }


