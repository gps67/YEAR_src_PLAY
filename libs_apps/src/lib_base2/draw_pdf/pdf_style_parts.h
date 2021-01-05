#ifndef pdf_style_parts_H
#define pdf_style_parts_H

#include "str0.h"
#include "pdf_base0.h"
#include "dgb_fail.h"

namespace PDF_GEN {

/*!
	A Part_Style Colour is a colour that can be used in stroke/fill/both.

	PDF doesnt appear to cache the colour, but my code does (appear to).
	PDF does cache SPOT colours.

	Style_fg_stroke::pdf_gen() calls PDF_setcolor, as does _fill ...
*/
struct Part_Style_Colour : public pdf_obj_ref
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


/*!
	A Font_Base is a fonts scalable outline, which can be scaled to
	point height. It also provides descender/ascender/capheight
	which need to be multiplied by point-height.

	See Style_Font_Size for a scaled font. There can only be ONE
	current Style_Font_Size, but you can have several Part_Style_Font_Base
	loaded.
*/
class Part_Style_Font_Base : public pdf_obj_ref
{ public:
	int pdf_font_int;

	float descender;	// multiply by point height
	float  ascender;
	float capheight;

	/*!
	*/
	Part_Style_Font_Base( pdf_base0 * _pdf, str0 _font_name )
	{
		descender = 0.0; // used as flag!
		ascender = 0.0;
		capheight = 0.0;
		// 0 == not embedded -- into PDF file
		pdf_font_int = PDF_findfont( _pdf->pdf, _font_name, "host", 0 );
		if( -1==pdf_font_int) THROW_dgb_fail( "bad-font" );
	}
/*
	// page 59 says to avoid setting the config file try:
	PDF_set_parameter( _pdf->pdf, "FontAFM",     "filename.afm" );
	PDF_set_parameter( _pdf->pdf, "FontOutline", "filename.pfa" );
*/

};

}; // NAMESPACE
#endif
