#ifndef A_point_plus_H
#define A_point_plus_H

#include <X11/Xlib.h>
# include "WAX_i16_hilo.h"
// #include "A_EXPR.h"
#include "A_Obj_IDX_OFFS_SPEC.h"
// #include "WAX/A_Obj_IDX_OFFS_SPEC.h"
// #include <WAX/A_Obj_IDX_OFFS_SPEC.h>
// #include "str0.h" // STR0

namespace WAX {
/*

	W_ WIN32 MINX_
	X_ X11_LIBR
	A_ TOK_LIBR // WORD_t // A_Vertex // V_Vertex // ALIAS CHAIN

	ALIAS_LINK lhs rhs // what it is // PLUS
	ALIAS_LEX lhs rhs // what it is // PLUS // ALIAS is RHS // eval rhs
	// ALIAS_LEX ALIAS LEX // _t from GEN AUTO_GEN_for_CACHE
	// LEX is_EXPR ("_MINX_") // or ("_PSG_") or ("_LIBR_") or ...
	// ALIAS is_Name // not_yet_anystr // _C_IDENT_ // SubLex

	// is a PSG_SUBLEX TOKEN_SPELLING
	// is a TOKEN_SPELLING in PSG_SUBLEX _SEGMENT
	CTXT_MINX_ // is a thread_gloal_PTR to_API_MINX _EXPR _VARS _ETC
	CTXT is cpu_var_CTXT // the ONE reserved cpu_var_Name ("CTXT") //
	// CTXT_t * CTXT = PARAMETER // PRE_SUPPLIED cpu_var_CTXT // u64 //
	// // CTXT is cpu_var_CTXT // EG AUTO_VAR
 #	AUTO_VAR appears no import required, was already there # eg CTXT
 	Shared CTXT offers one STRUCT_FIELD per API
	cpu_var_Name -> API_Name -> MINX_ // lots of callable items // A_Module
	struct MINX_vars : (API_Name == "MINX")
	MINX_WAX MINX_DRAW _GDI _GUI _GL

	There are so many variations on point, rectangle, etc
	
	WAX follows X11, mostly, add some WINE ?
	X_X11.h A_Obj obj EA_item EA_EXPR // both under EXPR _t _get_EA_Item()
	X_Item.h A_Obj obj EA_item EA_DATA // WORD _t // { 
	X_Locn // LOCN TODO
	X_Item
	X_Draw GUI_GDI_code _surface _data // GUI_api _sto _filter
	// EG SAMPLE // NEAR HERE
	// EG_template // Parameter ARGV ITEM_Type OBJ_V[u8]
	// typedef u64_WORD ITEM_TYPE_as_WORD;
	// typedef Word_t   ITEM_TYPE_as_WORD; // SUBLEX TOK WORD_t
	// typedef u64	WORD_t ; // SUBLEX TOK WORD_t
	// typedef WORD_t TOK WORD_t; // TOK_as_int WORD
	// typedef WORD_t TOK API; // CODE_POINT VAR_LIST VARS EXPRS 
	// typedef u64	WORD_t ; // SUBLEX TOK WORD_t
	X_STO
	A_TOK // api gets EXPR WORD CODE SESS TABLES _VARS _TOKENS_ // _NEXT_
	A_OBJ // api gets sess_data _VARS
	A_OBJ // api gets sess_type _TYPES_
	SPEC_WORD // WORD_t // u64 // u3 _3_bit

	WAX has API with local truetype and svg_type_diag
*/

/*!
	A_UNIT is an alias for i16

		typedef i16 A_UNIT_t; // within this namespace

		alias UINT _t XPOS YPOS W H STEP EM MATRIX_XYZ_unit_t

		// unused float16 = i16 << 4; // i16 isof UNIT

	CHOICES WERE:

		i16; // X11 SCREEN PIXEL COUNTER near_screen x10 

		 i16 is plenty for SCREEN +- 18 ft
		 i16 is plenty for SCREEN +- ten_times
		 i16 is plenty for PAGE +- 18 ft
		 i16 is plenty for PIXMAP 600x800 pos u8_INK
		 i16_hilo for stored values on UNIT = PIXEL
 
			TODO SOON: REWRITE i16_hilo hilo;
			TODO SOON: hilo.get_i32() { return BSWAP(val); }
			// (i32) i32_cpu_from_i16_hilo () {
			// val = BSWAP(_val); } 
			// }
			TODO SOON: hilo.set_i32(i32 _val) { val = BSWAP(_val); } 
			TODO SOON: hilo.get_float(i32 _val)
			{ float( BSWAP(_val) ); } 

			TODO SOON: hilo.set_float(float _val)
			{
			 i16 pos = i16_from_float_CLIP_ZONE( _val, CLIP_ZONE );
			 set_i16_cpu( pos );

		i32; // X11 SCREEN PIXEL COUNTER offscreen by 1 km

			TODO SOON: REWRITE i32_hilo

			TODO SOON: hilo.get_i32()



		typedef i32_t UNIT_t; // 

			The advantage is matching register size

		float;
		typedef float UNIT_t; // within this namespace

		i32; // X11 SCREEN PIXEL COUNTER shifted x 1000
		// i32 = i22 << 10 // FILL_AUTO_NONE_ZERO

		Before MATRIX can lay it out like brickwork, all way to roof
		With brick by brick ... gap item gap
		UNIT PIXEL over PIXMAP

		As soon as MATRIX is used it has to be float to fit STO
		double if that what your library wants

		Then XYZ kicks in auto over XY

		 floats instead of i16
		 XYZ instead of XY
		 Z can be added at import XY Z = POOL_Z
		 Z can be added at import XY = POOL[ITEM] POOL_Z[ i16_item ]
		 POOL,Z = POOL_Z[ i16_item ] // export table of XY XYZ
		 // SET POOL.Z = csr_Z_layer // current snapshot

		 local Z value
		 SCRIPTED XY pos



		// STORE 

	A_Point is an alias for XPoint (x,y) (i16,i16)

	CHOICES WERE:

		XPoint (x,y) (i16,i16)	// DONE
		XPoint (x,y) (i32,i32)	// todo
		XPoint (x,y) (float,float) // todo
	
	CHOICES REMAIN: // needed to make it work

		MATRIX of XYZ
		MATRIX of XY
		MATRIX in GPU // LIBR 0XFFFF
	
	A_Point is for an XY POS on XY_SURFACE = SCREEN_PIXMAP // 

		SURFACE = drawable_t drawable; // = SCREEN
		SCREEN dpi = 150 // hotdesk
		PIXMAP depth = u32_ARGB // SVGA-ish += Transparent
		 PIXEL_UNIT {
		 // surrounding code got here using XY
		 u8 AA; // 0xFF Alpha Transparent
		 u8 RR; // 0xFF Red
		 u8 GG; // 0xFF Green
		 u8 BB; // 0xFF blue 
		 // surrounding code might have PALLETTE 
		 // surrounding code might have XY XYWH_ZONE MATRIX PALLETTE 
		 // surrounding code might have LAYOUT
		 // here using XY

		 Basically, any software that 

*/
struct A_Point : public XPoint
{
	/*!
		i16 x; 
		i16 y;
		// UNIT_t UNIT; // i16 // pixels at 150 dpi // 
		// i16_UNIT XPOS // MATCH LIBR of XY stuff // ROM_VIEW_FROM_X11
		// Layout // matches // @expr_in_re2_source
		// LIBR of XY stuff
		// ... // PICK UNIT_t of SCREEN_PIXEL
		// used to be SCROLL_POINT
		// ie actual XY of TWIG or LEAF plus expr
		// expr is bunch of comments imports script_consts pick_vals + 
		//  eg LOCN = ( ZERO + OFFS ) is LOCN
		// with typedef i32 XY_screen_pixel_unit_t;
	*/
	A_Point( short _x, short _y )
	{
		x = _x;
		y = _y;
	}

	STR0 print( buffer1 & buf );
};

/*!
	A_XY is an alias for A_Point and XPoint (i16,i16)
*/
struct A_XY : public A_Point
{
	/*!
	*/
	A_XY( short _x, short _y )
	: A_Point( _x, _y )
	{
	}

	/*!
	*/
	A_XY( A_Point xy )
	: A_Point( xy )
	{
	}
};

/*!
	A_WH (w,h) holds width and height, cannot be negative

	nb A_WH uses i32 when X11 uses u16 sometimes signed
	so should check excess_of_u15 any_signed excess_of_

	WANT A LANG where I can edit the header, setting 

		int & h = y; // CT macro renamer
		int & w = x; // or actually set x = x_offs =  w_offs = w
	
	CHOICE here to change UNITS (eg i16 to i32)

		maybe not here, but just seen,
		OFFS = i32_value_held_in_ROM
		OFFS = i32_value_held_in_STO
		OFFS = i32_value_held_in_FRAME
		OFFS = i32_value_held_SOMEWHERE
		OFFS = u16_from_SOMEWHERE // segment is not from middle
		OFFS = i16_from_SOMEWHERE
		BASE = ZERO + OFFS


*/
struct A_WH
{
	int w;
	int h;

	A_WH()
	: w(0)
	, h(0)
	{
		// CTOR ZERO
	}

	/*!
	*/
	A_WH( int _w, int _h )
	{
		w = _w;
		h = _h;
		// enforce_unsigned();
	}

	/*!
	*/
	void enforce_unsigned()
	{
		if( w < 0 ) w = 0;
		if( h < 0 ) h = 0;
	}

	/*!
	*/
	void reduce( int _w, int _h )
	{
		w -= _w;
		h -= _h;
		enforce_unsigned();
	}

	/*!
	*/
	void reduce( int _w )
	{
		reduce( _w, _w );
	}

	/*!
	*/
	void reduce( const A_WH wh )
	{
		reduce( wh.w, wh.h );
	}

	bool is_zero() const {
		if( w <= 0 ) return true;
		if( h <= 0 ) return true;
		return false;
	}
};

/*!
	A_Rectangle (x,y,width,height) is an alias for XRectangle

	XRectangle uses i16 and u16 so beware!
*/
struct A_Rectangle : XRectangle
{
	/*!
	*/
	A_Rectangle( short _x, short _y, unsigned short _w, unsigned short _h )
	{
		x = _x;
		y = _y;
		width = _w;
		height = _h;
	}

	/*!
	*/
	A_Rectangle( A_XY xy, A_WH wh )
	{
		set_XY( xy );
		set_WH( wh );
	}

	void set_XY_WH( const A_XY & xy, const A_WH & wh )
	{
		set_XY( xy );
		set_WH( wh );
	}

	void get_XY_WH( A_XY & xy, A_WH & wh )
	{
		get_XY( xy );
		get_WH( wh );
	}

	void set_XY( const A_XY xy )
	{
		x = xy.x;	// i32 to i16
		y = xy.y;	// i32 to i16
	}

	void set_WH( const A_WH wh )
	{
		width = wh.w;
		height = wh.h;
	}

	void get_XY( A_XY & xy )
	{
		xy.x = x;
		xy.y = y;
	}

	void get_WH( A_WH & wh )
	{
		wh.w = width;
		wh.h = height;
	}

	A_WH get_WH()
	{
		return A_WH( width, height );
	}

	/*!
		reduce 2 sides by adj, right bottom
	*/
	void reduce2( short adj )
	{
	 /*
		width -= adj ;
		height -= adj ;

		except that width and height are UNSIGNED
		so beware of subtractions
	 */
		if( width > adj )
			width -= adj ;
		else
			width = 0;

		if( height > adj )
			height -= adj ;
		else
			height = 0;
	}
	
	/*!
		reduce all 4 sides by adj each
	*/
	void reduce4( short adj )
	{
		x += adj;
		y += adj;
		reduce2( 2 * adj );
	}

	/*!
		the right most XPOS - inside not out
	*/
	int x_last()
	{
		return x + width -1;
	}

	/*!
		the bottom YPOS - inside not out
	*/
	int y_last()
	{
		return y + height -1;
	}

};

};
#endif

