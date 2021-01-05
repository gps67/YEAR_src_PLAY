#include "pdf_styles.h"
#include "pdf_base.h"
#include "pdf_base0.h"
#include "str0.h"
#include "dgb_fail.h"

#ifndef NULL
#define NULL 0
#endif

// use colour structs and findfontnames
#include "pdf_style_parts.h"

namespace PDF_GEN {

/////////////// Style_Atom

	/*!
		constructor - ensure that the SINGLE bit number is specified
	*/
	Style_Atom::Style_Atom( pdf_base0 * _pdf, ChangeFlag _change_flag )
	{
		pdf = _pdf;
		change_flag = _change_flag;
	}


// VIRTUAL
	/*!
		destructor - plain
	*/
	Style_Atom::~Style_Atom()
	{
		pdf = NULL;
	}


// VIRTUAL
	/*!
		create fonts and spots at start of document,
		without setting them as the current setting
	*/
	void Style_Atom::pre_setup()
	{
	}


// VIRTUAL
	/*!
		call pdf NOW to make this style current.
		Caller resets the cache.
	*/
	void Style_Atom::pdf_gen()
	{
	}


// VIRTUAL
	/*!
		This style has gone out of scope,
		and pre_setup will be called again (or not?)

		NB I dont use this, I require declaration at document start
	*/
	void Style_Atom::invalidate_delete()
	{
	}

/////////////// Style_Font_Face_Size

	/*!
		constructor - reuse a loaded font at scalefactor _pt.
	*/
	Style_Font_Face_Size::Style_Font_Face_Size(
		pdf_base0 * _pdf,
		Part_Style_Font_Base * _font,
		int _pt
	)
	: Style_Atom( _pdf, Change_Text_Size_Font )
	{
		font = _font;
		font_point = _pt;
	}

	/*!
		constructor - font might be reloaded (if PDFlib doesnt
		cache the requests itself).
	*/
	Style_Font_Face_Size::Style_Font_Face_Size(
		pdf_base0 * _pdf,
		str0 fontname,
		int _pt
	)
	: Style_Atom( _pdf, Change_Text_Size_Font )
	{
		font = new Part_Style_Font_Base( _pdf, fontname );
		font_point = _pt;
	}


// VIRTUAL
	/*!
		setfont, and if not already done, get font metrics

		NB must setfont to get metrics !!
	*/
	// mode = fill+stroke
	void Style_Font_Face_Size::pdf_gen()
	{
		PDF_setfont( pdf->pdf, font->pdf_font_int, font_point );
		if(!font->descender) {
			font->descender = PDF_get_value( pdf->pdf, "descender", 0 );
			font-> ascender = PDF_get_value( pdf->pdf,  "ascender", 0 );
			font->capheight = PDF_get_value( pdf->pdf, "capheight", 0 );
		}
	}

	/*!
		presumes that pdf_gen() has already been called!
	*/
	float Style_Font_Face_Size::get_descender()
	{
		return font_point * font->descender;
	}

	/*!
		presumes that pdf_gen() has already been called!
	*/
	float Style_Font_Face_Size::get_ascender()
	{
		return font_point * font->ascender;
	}

	/*!
		presumes that pdf_gen() has already been called!
	*/
	float Style_Font_Face_Size::get_capheight()
	{
		return font_point * font->capheight;
	}

	/*!
		extra - needed for layout
	*/
	float Style_Font_Face_Size::calc_string_width( str0 str )
	{
		return PDF_stringwidth(
			pdf->pdf,
			str,
			font->pdf_font_int,
			font_point
		);
	}

/////////////// Style_fg_stroke 

	/*!
		destructor - plain
	*/
	Style_fg_stroke::~Style_fg_stroke()
	{
		 colour = NULL;
	}

	/*!
		constructor - store colour
	*/
	Style_fg_stroke::Style_fg_stroke(
		pdf_base0 * _pdf,
		Part_Style_Colour * _colour
	)
	: Style_Atom( _pdf, Change_fg_stroke )
	{
		colour = _colour;
	}


// VIRTUAL
	/*!
		activation - set the STROKE colour
	*/
	void Style_fg_stroke::pdf_gen()
	{
		PDF_setcolor( pdf->pdf, "stroke",
			colour->RGB_or_GRAY,
			colour->red,
			colour->green,
			colour->blue,
			0
		);
	}

/////////////// Style_fg_fill 

	/*!
		constructor - store the colour settings
	*/
	Style_fg_fill::Style_fg_fill(
		pdf_base0 * _pdf,
		Part_Style_Colour * _colour
	)
	: Style_Atom( _pdf, Change_fg_fill )
	{
		colour = _colour;
	}


// VIRTUAL
	/*!
		activation - PDF_setcolor( FILL )
	*/
	void Style_fg_fill::pdf_gen()
	{
		PDF_setcolor( pdf->pdf, "fill",
			colour->RGB_or_GRAY,
			colour->red,
			colour->green,
			colour->blue,
			0
		);
	}

////////////////////////////// Style_Tray

	/*!
		constructor - empty
	*/
	Style_Tray::Style_Tray( pdf_base * _pdf )
	: pdf( _pdf )
	{
		set_NULL();
	}

	/*!
		destructor
	*/
	Style_Tray::~Style_Tray()
	{
		set_NULL();
	}

	void Style_Tray::set_NULL()
	{
		for( int slot = 0; slot < Style_Atom::Change_N; slot ++ )
		{
			want[ slot ] = NULL;
		}
		changes_mask = 0;
	}

	/*!
		HMMMM .... mostly unused
	*/
	void Style_Tray::copy_from( Style_Tray * tray2 )
	{
		for( int slot = 0; slot < Style_Atom::Change_N; slot ++ )
		{
			want[ slot ] = tray2->want[ slot ];
		}
		changes_mask = tray2->changes_mask;
	}

	/*!
		Set a slots Style_Atom. If that changes set the changes_mask bit.

		This is used to request style switches, also to build the
		styles to request.
	*/
	void Style_Tray::requesting_style( Style_Atom * style1 )
	{
		if( ! style1 ) return;

		int slot = style1->change_flag;
		if( want[ slot ] != style1 )
		{
			want[ slot ] = style1;
			changes_mask |= ( 1 << slot );
		}
	}

	/*!
		Accept a pointer or direct ...
	*/
	void Style_Tray::requesting_style( const Style_Tray & styles )
	{
		requesting_style( & styles );
	}

	/*!
		Set all slots from another style_tray, setting the changes_mask.

		This is used to request a style switch.
	*/
	void Style_Tray::requesting_style( const Style_Tray * styles )
	{
		if( !styles ) return;
		int slot = Style_Atom::Change_N;
		while( slot-- > 0 )
		{
			requesting_style( styles->want[ slot ] );
		}
	}

// Style_Tray // setup actual settings // shouldnt be here ??

	void Style_Tray::init_fill_rgb( float r, float g, float b )
	{
		Part_Style_Colour * colr = new Part_Style_Colour( r, g, b );

		Style_fg_fill * fg;
		fg = new Style_fg_fill( pdf, colr ); // leak colr

		requesting_style( fg );
	}
	void Style_Tray::init_line_rgb( float r, float g, float b )
	{
		Part_Style_Colour * colr = new Part_Style_Colour( r, g, b );

		Style_fg_stroke * fg;
		fg = new Style_fg_stroke( pdf, colr ); // leak colr

		requesting_style( fg );
	}
	void Style_Tray::init_font( str0 fontname, int pointsize )
	{
		Style_Font_Face_Size * font;
		font = new Style_Font_Face_Size( pdf, fontname, pointsize );
		requesting_style( font );
	}

//////////////// Style_Settings 

	/*!
		constructor, init want and curr to all NULL
	*/
	Style_Settings::Style_Settings( pdf_base * _pdf )
	: Style_Tray( _pdf )
	{
		for( int slot = 0; slot < Style_Atom::Change_N; slot ++ )
		{
			curr[ slot ] = NULL;
		}
	}

	/*!
		compare want==curr and apply changes curr=want,
		but limited to the required mask.
	*/
	void Style_Settings::flush_mask( int mask_used )
	{
		int required_mask = changes_mask & mask_used;
		if(!required_mask)
			return;	// no changes required for this use

		int slot = Style_Atom::Change_N;
		while( slot-- > 0 )
		{
			int mask = (1 << slot);
			if( required_mask & mask )
			{
				changes_mask &= ~mask; // clear bit 
				required_mask &= ~mask; // clear bit 
				Style_Atom * style0 = curr[ slot ];
				Style_Atom * style1 = want[ slot ];
				if( style0 != style1 ) // not changed twice
				{
					curr[ slot ] = style1;
					style1->pdf_gen();
				}
				if(!required_mask) break; // early exit
			}
		}
	}

	/*!
		HMMMM ... used where ?
	*/
	void Style_Settings::flush_all()
	{
		flush_mask( changes_mask );
	}

	/*!
		request updates that will effect text drawing
	*/
	void Style_Settings::flush_text()
	{
		flush_mask( 0
		 |	(1<<Style_Atom::Change_Text_Size_Font)
		 |	(1<<Style_Atom::Change_Line_Width)
		 |	(1<<Style_Atom::Change_Line_dashed)
		 |	(1<<Style_Atom::Change_Line_Other)
		 |	(1<<Style_Atom::Change_fg_fill)
		 |	(1<<Style_Atom::Change_fg_stroke)
		);
	}

	/*!
		request updates that will effect line drawing
	*/
	void Style_Settings::flush_line()
	{
		flush_mask( 0
		//	(1<<Style_Atom::Change_Text_Size_Font)
		 |	(1<<Style_Atom::Change_Line_Width)
		 |	(1<<Style_Atom::Change_Line_dashed)
		 |	(1<<Style_Atom::Change_Line_Other)
		//	(1<<Style_Atom::Change_fg_fill)
		 |	(1<<Style_Atom::Change_fg_stroke)
		);
	}

	/*!
		request updates that will effect fill drawing
	*/
	void Style_Settings::flush_fill()
	{
		flush_mask( 0
		//	(1<<Style_Atom::Change_Text_Size_Font)
		//	(1<<Style_Atom::Change_Line_Width)
		//	(1<<Style_Atom::Change_Line_dashed)
		//	(1<<Style_Atom::Change_Line_Other)
		 |	(1<<Style_Atom::Change_fg_fill)
		//	(1<<Style_Atom::Change_fg_stroke)
		);
	}

	/*!
		PDF resets everything over a page break, so set curr == NULL.

		want == NULL is just good sense
		except that page break causes next pen use to fail.
	*/
	void Style_Settings::page_break_reset()
	{
		pdf->set_current_pen( NULL );
		int slot = Style_Atom::Change_N;
		while( slot-- > 0 )
		{
			curr[ slot ] = NULL;
			want[ slot ] = NULL;
		}
		/*

			PROVIDED: pdf always resets to sane defaults
			which the caller expects (or requests otherwise)
		*/
		changes_mask = 0;
		/*
			VAGUE: this is not needed provided the output
			functions always do a flush_mask( mask );

			Otherwise dont set want[slot] = NULL
			and re-request the 'old' style
		*/
		// CLEARED - NOOP // requesting_style( this );
	}

	/*!
		obtain the current font (for metrics), which might 
		be requested but not yet established
	*/
	Style_Font_Face_Size * Style_Settings::curr_font()
	{
		flush_text();
		Style_Atom * style;
		style = want[ Style_Atom::Change_Text_Size_Font ];
		if(!style)
			style = curr[ Style_Atom::Change_Text_Size_Font ];
		// FAKE IT ??
		if(!style)
			return (Style_Font_Face_Size *)NULL_dgb_fail( "curr_font()" );
		return (Style_Font_Face_Size *) style; // IT MUST BE
	}

}; // NAMESPACE
