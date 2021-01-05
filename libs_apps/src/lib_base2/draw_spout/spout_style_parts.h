#ifndef spout_style_parts_H
#define spout_style_parts_H

#include "spout_obj_ref.h"

#include "str0.h"
// #include "spout_base_0.h"
#include "dgb_fail.h"

namespace SPOUT_GEN {

/*!
	A Part_Style Colour is a colour that can be used in stroke/fill/both.

	SPOUT doesnt appear to cache the colour, but my code does (appear to).
	SPOUT does cache SPOT colours.

	Style_fg_stroke::spout_gen() calls SPOUT_setcolor, as does _fill ...
*/
struct Part_Style_Colour : public spout_obj_ref
{
	/*!
		The spot colour name is used by top end printers,
		(and not the r/g/b values)
	*/
	str0 spot_name;

	/*!
		ROM allocated "rgb" "gray" or "CNMNMNC"
	*/
	str0 RGB_or_GRAY; 
	/*!
		non-high-end printers, without colour separation,
		use the r/g/b values. Also non-spot colour specs.
	*/
	float red;
	float green;
	float blue;
	//
	float tint;
	float gray;

	Part_Style_Colour(
		str0 _spot_name,
		str0 _RGB_or_GRAY,
		float _red,
		float _green,
		float _blue,
		float _tint,
		float _gray
	)
	: spot_name( _spot_name )
	, RGB_or_GRAY( _RGB_or_GRAY )
	, red( _red )
	, green( _green )
	, blue( _blue )
	, tint( _tint )
	, gray( _gray )
	{
	}

	Part_Style_Colour(
		float _red,
		float _green,
		float _blue
	)
	: spot_name( "UNUSED" )
	, RGB_or_GRAY( "rgb" )
	, red( _red )
	, green( _green )
	, blue( _blue )
	, tint( 0 )
	, gray( 0 )
	{
	}

};


// int spout_font_int =
// SPOUT_findfont( _spout->spout, _font_name, "host", 0 );
// DUNNO maybe selector exprs for SESS fonts_flag_host

#define SPOUT_findfont(spout,fontname,one,two) -1
#if 0
int xSPOUT_findfont(
	SPOUT_t * spout,
	str0 _font_name,
	str0 PLUS_one = (STR0)NULL,	
	str0 PLUS_TWO = (STR0)NULL 
	// I think this is wrong, but pdflib ? who cares ? (they dont)
) {

	FAIL("UNWRITTEN");
	return 0;
};
#endif

/*!
	A Font_Base is a fonts scalable outline, which can be scaled to
	point height. It also provides descender/ascender/capheight
	which need to be multiplied by point-height.

	See Style_Font_Size for a scaled font. There can only be ONE
	current Style_Font_Size, but you can have several Part_Style_Font_Base
	loaded.

	A FONT loader in a different unicerse is d_text PRINT_HEAD
	/home/gps/YEAR/src/PLAY/GFX_lib_WAX/libWAX/src

*/
class Part_Style_Font_Base : public spout_obj_ref
{ public:
	int spout_font_int;

	float descender;	// multiply by point height
	float  ascender;
	float capheight;

	/*!
		wheres my AFM loader?
		Thats Adobe Font Metrics btw
		get_bounding_bbox_of_glyph(u8) // (utf8_u32_u64)
		// UTF8 can be switched with // EAID u16
		// decode(u16) u8_u8 FILTER is NOT A glyph // is a _t //
		// LHS(u8) RHS(u8_ASCII_VALUE) // += CODE_PAGE_known

		where is my VFS Layout?
		/home/gps/YEAR/tars/SCRATCH_ZONE_19-2020-02-14.tgz/utar://SCRATCH_ZONE_19/SPOUT/DIFF_vfs/

		// gc //
		/home/gps/YEAR/src/PLAY/GFX_lib_WAX/libWAX/src/X_Draw.h

		WAX::X_Draw += display window gc // += API

	*/
	Part_Style_Font_Base( SPOUT_t * _spout, str0 _font_name )
	{
		descender = 0.0; // used as flag!
		ascender = 0.0;
		capheight = 0.0;
		// 0 == not embedded -- into SPOUT file
		spout_font_int = SPOUT_findfont( _spout, _font_name, "host", (STR0)NULL /* was 0 */ );
		if( -1==spout_font_int) THROW_dgb_fail( "bad-font" );
	}
/*
	// page 59 says to avoid setting the config file try:
	SPOUT_set_parameter( _spout->spout, "FontAFM",     "filename.afm" );
	SPOUT_set_parameter( _spout->spout, "FontOutline", "filename.pfa" );
*/

};

}; // NAMESPACE
#endif
