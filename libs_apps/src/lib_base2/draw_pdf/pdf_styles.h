#ifndef pdf_styles_H
#define pdf_styles_H

#include "pdf_base0.h"

// sub style things - allow static initialisers

#ifndef NULL
#define NULL 0
#endif

// use colour structs and findfontnames
#include "pdf_style_parts.h"

namespace PDF_GEN {

class pdf_base;

/*!
	A Style_Atom is a part of a Style_Tray (molecule).

	Each Style_Atom has a bit NUMBER in the change_flag, which is
	used to set a bit in the Style_Tray's bitmask. 

	Each Style_Atom has a virtual pdf_gen() function, which calls a PDF
	function which generates a command in the PDF file (In a
	Postscript it would generate command text).

	Each Style_Atom has a (this) pointer identity, which the
	Style_Tray uses to know that it is corrent (or has to be set).

	Subclasses of Style_Atom specify line-width, line-colour, fill-colour,
	font+size, ...

	LURK - now need to switch back to default (other than page break)
	One method is to set values for default, another is to recode
	switch (from-a-to-b) which would also accomodate other mixtures.
	
*/
class Style_Atom : public pdf_obj_ref
{ public:

	//! Every style can reach the single main PDF channel
	pdf_base0 * pdf;

	/*!
		bitmask numbers, array positions

		Higher bits are processed first.
	*/
	typedef enum {
//		Change_Text_fill,	 // fill,stroke,fill+stroke, clippath
//		Change_Text_lining,      // underline overline strikeout
//		Change_Text_stretch_x,   // unused

		Change_Text_Size_Font,

		Change_Line_Width,
		Change_Line_dashed,
		Change_Line_Other,
		Change_fg_fill,
		Change_fg_stroke,
		Change_N		//!< array size
	} ChangeFlag;

	int change_flag;

	Style_Atom( pdf_base0 * _pdf, ChangeFlag _change_flag );
	virtual ~Style_Atom();
	virtual void pre_setup();
	virtual void pdf_gen();
	virtual void invalidate_delete();
};

/*!
	There is only one single ACTIVE font@point, but PDF_GEN keeps
	a pallette of Style_Font_Face_Size, ready to be selected.

	The combination of a Face and Size is a single Style_Atom.

	Extra functionality comes in the form of calc_string_width() ...
	but that could be moved to another class, and this point to that.

	The findfont should only be done once, when the pallette is setup.
	If you really want to not know the Face, but vary the pointsize,
	you might rewrite this, somehow.
*/
class Style_Font_Face_Size : public Style_Atom 
{
 public:

	Part_Style_Font_Base * font; // shared
	int font_point;

	Style_Font_Face_Size(
		pdf_base0 * _pdf,
		Part_Style_Font_Base * _font,
		int _pt
	);
	Style_Font_Face_Size(
		pdf_base0 * _pdf,
		str0 fontname,
		int _pt
	);

	virtual void pdf_gen();
	float get_descender();
	float get_ascender();
	float get_capheight();
	float calc_string_width( str0 str );

};

/*!
	Foreground colour for lines/text-outline strokes
*/
class Style_fg_stroke : public Style_Atom 
{
 public:

	/*!
		The colour can be shared with other Atoms (eg fill).
	*/
	obj_hold<Part_Style_Colour> colour;

	~Style_fg_stroke();

	Style_fg_stroke(
		pdf_base0 * _pdf,
		Part_Style_Colour * _colour
	);
	virtual void pdf_gen();

};

/*!
	Foreground colour for shape-fill/text-fill
*/
class Style_fg_fill : public Style_Atom 
{
 public:

	/*!
		Shared reference to a colour
	*/
	obj_hold<Part_Style_Colour> colour;

	Style_fg_fill(
		pdf_base0 * _pdf,
		Part_Style_Colour * _colour
	);
	virtual void pdf_gen();
};

//////////////////////////////

/*!
	A Style_Tray is a complete description of a single style, or
	the currently active settings.

	A Style_Tray is a fixed-array-set of orthogonal Style_Atom slots.
	Each Style_Atom (attribute) has a change_flag (slot number).

	Each slot in the tray is a pointer to a selected Style_Atom value,
	or NULL (dont care).

	When this style is applied, each slot is requested, but NULL
	slots means you can choose how styles are mixed.

*/
class Style_Tray : public pdf_obj_ref
{
 public:
	/*!
		The Style_Atoms that describe this style, and will
		be wanted when this style is wanted (and when
		a specific drawing command will use them).
	*/
	obj_hold<Style_Atom> want[ Style_Atom::Change_N ];

	/*!
		individual style constructors need this
	*/
	pdf_base * pdf;

	/*!
		The want[] slots that have changed

		This is flawed: a change can happen other than
		a request here, eg style2
	*/
	int changes_mask;

	Style_Tray( pdf_base * _pdf);
	~Style_Tray();

	void set_NULL();
	void copy_from( Style_Tray * tray2 );
	void requesting_style( Style_Atom * style1 );
	void requesting_style( const Style_Tray & styles );
	void requesting_style( const Style_Tray * styles );

	/*
		maybe these shouldnt be here, they are individual settings
		but it re-centralises focus on the Tray

		init_ means that these are for setup, not for changing,
		but changing is required at init.

		Should also go back to shared font_id's too (later) (HT)
	*/

	void init_fill_rgb( float r, float g, float b );
	void init_line_rgb( float r, float g, float b );
	void init_font( str0 fontname, int pointsize );
};

/*!
	A Style_Settings is used to track the current output mode,
	(i.e. the difference between curr and want), and to
	activate styles which need changing. Not all slots need to
	be set for every drawing command, and flush_mask(mask)
	makes this selective.
*/
class Style_Settings : public Style_Tray
{
 public:
	/*!
		The previous (current) styles
	*/
	obj_hold<Style_Atom> curr[ Style_Atom::Change_N ];

	Style_Settings( pdf_base * _pdf );
	void flush_mask( int mask_used );
	void flush_all();

	void flush_text();
	void flush_line();
	void flush_fill();
	void page_break_reset();
	Style_Font_Face_Size * curr_font();
};


}; // NAMESPACE
#endif
